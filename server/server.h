#ifndef SERVER_H
#include <pthread.h>
#include <bits/stdc++.h>

#define SERVER_H

#define MAXLEN 256
#define MAXSIZE 2048
#define MAXSLEN 128

#define MAXCOM 100

//Htype
#define GET 0         // get items
#define SEND 1        // send item

#define SEQRESPOND 0     // sequence respond
#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <bits/stdc++.h>

using namespace std;

#define ll long long

//Maxlen
#define MAXPLEN 24
#define MAXMLEN 48
#define MAXLEN 256
#define MAXSIZE 2048
#define MAXSLEN 128
#define MAXCOM 128

//Thread
#define INIT pthread_mutex_init
#define P pthread_mutex_lock
#define V pthread_mutex_unlock

//Time
#define Datetime struct tm


namespace THS { //TreeHoleServer

//Itemtype
enum Itype{ DEFAULTITEM = 0, TEXTITEM, MULTIITEM };

//Header type
enum Htype{ SIGNUP = 0, LOGIN, GET, UPLOAD, REPLY, SEND };

//Error type
enum State{ SUCCESS = 0, USER_EXIST, USER_NOT_EXIST, PASSWORD_FAIL };


struct RequestHeader {

    // header type
    Htype htype;    

    // login info
    ll userID;
    ll token;

    char password[MAXPLEN];

    // content
    int seqlen;      // length of upload sequence

    // request
    int starti;      // start index of items requested
    int endi;        // end index of items requested


    // signup & login
    RequestHeader(Htype ty, ll uid, const char* pbuf):
        htype(ty),userID(uid)
    {
        strncpy(password, pbuf, strlen(pbuf));
    }

    // get
    RequestHeader(Htype ty, ll uid, ll to, int si, int ei):
        htype(ty),userID(uid),token(to),starti(si),endi(ei){}

    // upload
    RequestHeader(Htype ty, ll uid, ll to, int sl):
        htype(ty),userID(uid),token(to),seqlen(sl){}


};



struct RespondHeader {

    // header type
    Htype htype;   

    // msg
    char msg[MAXMLEN];

    // content
    int seqlen;   // sequence length


    // reply
    RespondHeader(Htype ty, const char* mbuf):
        htype(ty)
    {
        strncpy(msg, mbuf, strlen(mbuf));
    }

    // send
    RespondHeader(Htype ty, int sl):
        htype(ty),seqlen(sl){}

};



struct DataHeader {

    Itype itype;
    size_t textlen;
    size_t multisize;

    // text data
    DataHeader(Itype ty, int tl):
        itype(ty),textlen(tl){}

    // multi data
    DataHeader(Itype ty, int tl, int ms):
        itype(ty),textlen(tl),multisize(ms){}

};


struct Comment {

    int index;  

    // user info
    ll userID;

    // time
    Datetime timestamp;

    // length
    size_t textlen;

    // content 
    char text[MAXLEN];   


    Comment(){}
    Comment(int i, int u, Datetime t, const char* textbuf):
        index(i),userID(u),timestamp(t)
    {
        textlen = strlen(textbuf);
        strncpy(text, textbuf, textlen);
    }

};


class Item {

    // item type
    Itype itype; 
    int index; 

    // user info
    ll userID;

    // time info
    Datetime timestamp;

    // comments
    int subscribe_num;     // num of subscribes
    int comment_num;       // num of comments
    Comment comments[MAXCOM];     // short comments

    // content info
    size_t textlen;
    size_t multisize;

protected:
    // base item 声明保护，防止基类初始化
    Item(Itype it, int i, int u, Datetime t): 
        itype(it),index(i),userID(u),timestamp(t)
    {
        comment_num = 0;
        subscribe_num = 0;
    }

public:

    Itype getitype() const {
        return itype;
    }

    ll getuid() const {
        return userID;
    }

    Datetime gettimestamp() const {
        return timestamp;
    }

    int getindex() const {
        return index;
    }

    size_t gettextlen() const {
        return textlen;
    }

    size_t getmultisize() const {
        return multisize;
    }


    friend bool operator< (const Item& it1, const Item& it2){
        return it1.index < it2.index;
    }

};


class TextItem : public Item{

    // length
    size_t textlen;

    // content
    char text[MAXLEN];    // text

public:

    TextItem(int i, int u, Datetime t, const char* textbuf):
        Item(TEXTITEM, i, u, t)
    {
        textlen = strlen(textbuf);
        strncpy(text, textbuf, textlen);
    }

    const char* gettext() const {
        return text;
    }

};

class MultiItem : public Item{

    // length
    size_t textlen;
    size_t multisize;

    // content
    char text[MAXLEN];    // text
    char multi[MAXSIZE];  // multi file

public:

    MultiItem(int i, int u, Datetime t, const char* textbuf, const char* multibuf):
        Item(MULTIITEM, i, u, t)
    {
        textlen = strlen(textbuf);
        multisize = sizeof(multibuf);

        strncpy(text, textbuf, strlen(textbuf));
        memcpy(multi, multibuf, multisize);
    }

    const char* gettext() const {
        return text;
    }

    const char* getmulti() const {
        return multi;
    }

};



class ItemList {//线程安全的内容项结构，采用写者优先解法

    pthread_mutex_t rmutex;  
	pthread_mutex_t wmutex;

    int readcount;
    int writecount;

    pthread_mutex_t rclock;  // lock on readcount
	pthread_mutex_t wclock;  // lock on writecount

    pthread_mutex_t stop;   // mutex on writers

    map<int, Item*>ilist;   // Red-Black Tree 
    int listlen;  

public:

    ItemList(){
        
        INIT(&rmutex, NULL);
        INIT(&wmutex, NULL);
        INIT(&rclock, NULL);
        INIT(&wclock, NULL);
        INIT(&stop, NULL);

        readcount = writecount = 0;

        ilist.clear();
        listlen = 0;

    }

    void append(Item* it){

        P(&wclock);
        writecount++;
        if(writecount == 1)
            P(&rmutex);
        V(&wclock);

        P(&wmutex);

        int index = it->getindex();
        ilist[index] = it;

        V(&wmutex);

        P(&wclock);
        writecount--;
        if(writecount == 0)
            V(&rmutex);
        V(&wclock);

    }

    vector<const Item*> getitems(int starti, int endi){

        P(&stop);   
        P(&rmutex);  // writer first
        P(&rclock); 
        readcount++;
        if(readcount == 1)
            P(&wmutex);
        V(&rclock);
        V(&rmutex);
        V(&stop);


        auto iter_l = ilist.lower_bound(starti); 
        auto iter_r = ilist.upper_bound(endi); 


        if(iter_l == ilist.end())return {};  // not found

        vector<const Item*>ans;

        for(auto iter = iter_l; iter != iter_r; ++iter){

            Item* temp = iter->second;
            ans.push_back(temp);

        }


        P(&rclock);
        readcount--;
        if(readcount == 0)
            V(&wmutex);
        V(&rclock);

        return ans;

    }

};


struct User {

    ll userID;
    char password[MAXPLEN];

    User(ll id, const char* pbuf):
        userID(id)
    {
        strncpy(password, pbuf, strlen(pbuf));
    }

};


class UserTable {//线程安全的用户表结构，采用读者优先解法

    pthread_mutex_t rmutex;  
	pthread_mutex_t wmutex;

    int readcount;

    unordered_map<ll,User*> hash;
    unordered_map<ll,ll> tokens;

public:

    UserTable(){

        INIT(&rmutex, NULL);
        INIT(&wmutex, NULL);
        readcount = 0;

        hash.clear();
        tokens.clear();

    }

    bool exist(const ll key){

        bool is_exist;

        P(&rmutex);
        if(readcount==0)
            P(&wmutex);
        readcount++;
        V(&rmutex);

        is_exist = (hash.count(key) != 0);

        P(&rmutex);
        readcount--;
        if(readcount==0)
            V(&wmutex);
        V(&rmutex);

        return is_exist;

    }


    void get(const ll key, char* buf){

        P(&rmutex);
        if(readcount==0)
            P(&wmutex);
        readcount++;
        V(&rmutex);

        User* temp = hash[key];
        strncpy(buf, temp->password, strlen(temp->password));

        P(&rmutex);
        readcount--;
        if(readcount==0)
            V(&wmutex);
        V(&rmutex);


    }

    void insert(const ll key, const char* value){

        User* temp = new User(key, value);

        P(&wmutex);
        hash[key] = temp;
        V(&wmutex);

    }



    bool existtoken(const ll id){

        bool is_exist;

        P(&rmutex);
        if(readcount==0)
            P(&wmutex);
        readcount++;
        V(&rmutex);

        is_exist = (tokens.count(id) != 0);

        P(&rmutex);
        readcount--;
        if(readcount==0)
            V(&wmutex);
        V(&rmutex);

        return is_exist;

    }

    ll gettoken(const ll id){
        
        ll token;

        P(&rmutex);
        if(readcount==0)
            P(&wmutex);
        readcount++;
        V(&rmutex);

        token = tokens[id];

        P(&rmutex);
        readcount--;
        if(readcount==0)
            V(&wmutex);
        V(&rmutex);


        return token;

    }

    void addtoken(const ll id, const ll token){

        P(&wmutex);
        tokens[id] = token;
        V(&wmutex);

    }


    void erasetoken(const ll id){

        P(&wmutex);
        tokens.erase(id);
        V(&wmutex);

    }


    State signup(const ll id, const char* password){

        bool is_exist = exist(id);
        if(is_exist)return USER_EXIST;

        insert(id, password);
        return SUCCESS;

    }


    State login(const ll id, const char* password){

        bool is_exist = exist(id);
        if(!is_exist)return USER_NOT_EXIST;

        char buf[MAXPLEN];
        get(id, buf);

        if(strcmp(buf, password) != 0)return PASSWORD_FAIL;

        return SUCCESS;

    }

};



};



#endif // SERVER_H

#define DEFAULTITEM 0     // default item
#define TEXTITEM 1        // text item
#define MULTIITEM 2       // mutlimedia item

#define INIT pthread_mutex_init
#define P pthread_mutex_lock
#define V pthread_mutex_unlock

#define Datetime struct tm


Datetime gettime(){

    time_t t;
    Datetime* tmp; 

	time(&t);
    tmp = localtime(&t);
	
    return *tmp;

}

struct RequestHeader {

    int Htype;    // header type
    int userID;

    int itemtype;        // type of item
    int textlen;         // length of text
    int multilen;        // length of multimedia

    int getnum;   // item num requested

    RequestHeader(int t, int u, int it, int g=0):Htype(t),userID(u),itemtype(it),getnum(g){
        textlen = multilen = 0;
    }
};

struct RespondHeader {

    int Htype;    // header type
    int userID;

    int seqlen;   // sequence length
    int itemt[MAXSLEN];        // type array of items
    int textiteml[MAXSLEN];    //length array of text items
    int multiiteml[MAXSLEN];   //length array of multimedia items

    RespondHeader(int t, int u, int s):Htype(t),userID(u),seqlen(s){
        memset(itemt, 0 ,sizeof(itemt));
        memset(textiteml, 0 ,sizeof(textiteml));
        memset(multiiteml, 0 ,sizeof(multiiteml));
    }
};


struct Comment {

    int index;  // comment index
    int userID;
    Datetime timestamp;
    char text[MAXLEN];    // text

    Comment(){}
    Comment(int i, int u, Datetime t, const char* textbuf):index(i),userID(u),timestamp(t){
        strncpy(text, textbuf, sizeof(char)*strlen(textbuf));
    }

};


class Item {
public:

    int Itype;   // item type
    int index;   // item index
    int userID;
    Datetime timestamp;

    int comment_num;    // num of comments
    int subscribe_num;    // num of subscribes
    Comment comments[MAXCOM];

    Item* prev;   //double list
    Item* next;

    Item(int i, int u, Datetime t): Itype(DEFAULTITEM),index(i),userID(u),timestamp(t){

        comment_num = 0;
        subscribe_num = 0;
        prev = next = NULL;

    }

    virtual const char* gettext(){return NULL;}
    virtual const char* getmulti(){return NULL;}
};


class TextItem : public Item{
public:
    char text[MAXLEN];    // text
    TextItem(int i, int u, Datetime t, const char* textbuf):Item(i,u,t){
        Itype = TEXTITEM;
        strncpy(text, textbuf, sizeof(char)*strlen(textbuf));
    }
    virtual const char* gettext(){
        return text;
    }
};

class MuiltItem : public Item{
public:
    char text[MAXLEN];    // text
    char multifile[MAXSIZE];  // file
    MuiltItem(int i, int u, Datetime t, const char* textbuf, const char* multibuf):Item(i,u,t){
        Itype = MULTIITEM;
        strncpy(text, textbuf, sizeof(char)*strlen(textbuf));
        memcpy(multifile, multibuf, sizeof(char)*strlen(multibuf));
    }
    virtual const char* gettext(){
        return text;
    }
    virtual const char* getmulti(){
        return  multifile;
    }
};



class ItemList{

    pthread_mutex_t rmutex;  
	pthread_mutex_t wmutex;

    int readcount;
    int writecount;

    pthread_mutex_t rclock;  // lock on readcount
	pthread_mutex_t wclock;  // lock on writecount

    pthread_mutex_t stop;  // mutex on writers

    Item* itemlist;
    int listlen;  

public:

    ItemList(){
        
        INIT(&rmutex, NULL);
        INIT(&wmutex, NULL);
        INIT(&rclock, NULL);
        INIT(&wclock, NULL);
        INIT(&stop, NULL);

        readcount = writecount = 0;


        itemlist = new Item(0, 0, gettime());
        listlen = 0;

    }

    void append(Item* it){

        P(&wclock);
        writecount++;
        if(writecount == 1)
            P(&rmutex);
        V(&wclock);

        P(&wmutex);

        it->index = ++listlen;
        it->next = itemlist;
        itemlist->prev = it;
        itemlist = it;  // append at the head of list


        V(&wmutex);

        P(&wclock);
        writecount--;
        if(writecount == 0)
            V(&rmutex);
        V(&wclock);

    }

    Item* getitems(int num){

        P(&stop);   
        P(&rmutex);  // writer first
        P(&rclock); 
        readcount++;
        if(readcount == 1)
            P(&wmutex);
        V(&rclock);
        V(&rmutex);
        V(&stop);

        Item* temp = itemlist;
        Item* newlist = NULL;
        Item* tail = NULL;
        for(int i=0; i<num; i++){

            if(!temp)break;
            if(temp->Itype == TEXTITEM){

                TextItem* curr = new TextItem(temp->index, temp->userID, temp->timestamp, temp->gettext());
                Item* icurr = (Item*)curr;
                if(i==0){
                    newlist = icurr;
                    tail = newlist;
                }
                else{
                    tail->next = icurr;
                    icurr->prev = tail;
                    tail = icurr;
                }

            }
            else if(temp->Itype == MULTIITEM){

                MuiltItem* curr = new MuiltItem(temp->index, temp->userID, temp->timestamp, temp->gettext(), temp->getmulti());
                Item* icurr = (Item*)curr;
                if(i==0){
                    newlist = icurr;
                    tail = newlist;
                }
                else{
                    tail->next = icurr;
                    icurr->prev = tail;
                    tail = icurr;
                }

            }
            temp = temp->next;

        }


        P(&rclock);
        readcount--;
        if(readcount == 0)
            V(&wmutex);
        V(&rclock);

        return newlist;

    }

};



#endif // SERVER_H