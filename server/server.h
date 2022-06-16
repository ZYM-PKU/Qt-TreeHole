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



namespace THS { //TreeHoleServer

//Itemtype
enum Itype{ DEFAULTITEM = 0, TEXTITEM, MULTIITEM };

//Header type
enum Htype{ SIGNUP = 0, LOGIN, GET, UPLOAD, REPLY, SEND };

//Error type
enum State{ SUCCESS = 0, USER_EXIST, USER_NOT_EXIST, PASSWORD_FAIL, ILLEGAL_USER, UPLOAD_FAIL };


struct Datetime {

    int tm_hour;
    int tm_isdst;
    int tm_mday;
    int tm_min;
    int tm_mon;
    int tm_sec;
    int tm_wday;
    int yday;
    int year;

};

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
        memset(password, 0, sizeof(password));
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

    // state
    State state;

    // user token
    ll token;

    // content
    int seqlen;   // sequence length

    RespondHeader(){}
    
    // reply
    RespondHeader(Htype ty, State st, ll tk):
        htype(ty),state(st),token(tk){}

    // send
    RespondHeader(Htype ty, int sl):
        htype(ty),seqlen(sl){}

};



struct DataHeader {

    Itype itype;
    size_t textlen;
    size_t multisize;
    
    int index;
    ll uid;
    Datetime timestamp;
    
    int comcnt;  // comment count

    // text data
    DataHeader(Itype ty, int tl):
        itype(ty),textlen(tl){}

    DataHeader(Itype ty, int tl, int i, ll u, Datetime dt, int cc):
        itype(ty),textlen(tl),index(i),uid(u),timestamp(dt),comcnt(cc){}

    // multi data
    DataHeader(Itype ty, int tl, int ms):
        itype(ty),textlen(tl),multisize(ms){}

    DataHeader(Itype ty, int tl, int ms, int i, ll u, Datetime dt, int cc):
        itype(ty),textlen(tl),multisize(ms),index(i),uid(u),timestamp(dt),comcnt(cc){}

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
    int subcnt;       // subscribe count
    int comcnt;       // comment count
    Comment comments[MAXCOM];     // short comments

    // content info
    size_t textlen;
    size_t multisize;

protected:
    // base item 声明保护，防止基类初始化
    Item(Itype it, int i, int u, Datetime t):
        itype(it),index(i),userID(u),timestamp(t)
    {
        subcnt = 0;
        comcnt = 0;
        textlen = 0;
        multisize = 0;
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

    void setindex(int i) {
        index = i;
    }

    size_t gettextlen() const {
        return textlen;
    }

    void settextlen(size_t l){
        textlen = l;
    }

    void setmultisize(size_t s){
        multisize = s;
    }

    size_t getmultisize() const {
        return multisize;
    }

    int getsubcnt() const {
        return subcnt;
    }

    int getcomcnt() const {
        return comcnt;
    }

    void addcomment(const Comment* c) {
        comments[comcnt++] = *c;
    }

    vector<Comment> getcomment() const {

        vector<Comment> res;
        for(int i=0; i<comcnt; i++){
            res.push_back(comments[i]);
        }
        return res;

    }


    friend bool operator< (const Item& it1, const Item& it2){
        return it1.index < it2.index;
    }

};



class TextItem : public Item{

    // content
    char text[MAXLEN];    // text

public:

    TextItem(int i, int u, Datetime t, const char* textbuf):
        Item(TEXTITEM, i, u, t)
    {
        settextlen(strlen(textbuf));
        memset(text, 0, MAXLEN);
        strncpy(text, textbuf, strlen(textbuf));
    }

    const char* gettext() const {
        return text;
    }

};

class MultiItem : public Item{

    // content
    char text[MAXLEN];    // text
    char multi[MAXSIZE];  // multi file

public:

    MultiItem(int i, int u, Datetime t, const char* textbuf, const char* multibuf):
        Item(MULTIITEM, i, u, t)
    {
        settextlen(strlen(textbuf));
        setmultisize(sizeof(multibuf));

        memset(text, 0, MAXLEN);
        memset(multi, 0, MAXSIZE);

        strncpy(text, textbuf, strlen(textbuf));
        memcpy(multi, multibuf, MAXSIZE);
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
    int cnt;                // item count  

public:

    ItemList(){
        
        INIT(&rmutex, NULL);
        INIT(&wmutex, NULL);
        INIT(&rclock, NULL);
        INIT(&wclock, NULL);
        INIT(&stop, NULL);

        readcount = writecount = 0;

        ilist.clear();
        cnt = 0;

    }

    void append(Item* it){

        P(&wclock);
        writecount++;
        if(writecount == 1)
            P(&rmutex);
        V(&wclock);

        P(&wmutex);

        it->setindex(++cnt);
        ilist[cnt] = it;

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


