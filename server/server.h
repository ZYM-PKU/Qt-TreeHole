#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <bits/stdc++.h>

using namespace std;

#define ll long long

//Maxlen
#define MAXPLEN 24
#define MAXMLEN 48
#define MAXLEN 1024
#define MAXSIZE 1<<20
#define MAXSLEN 128
#define MAXCOM 128
#define MAXUSER 64
#define NAMEBYTE 10

//Thread
#define INIT pthread_mutex_init
#define P pthread_mutex_lock
#define V pthread_mutex_unlock

//Save
#define USER_PATH "./save/user.txt"
#define ITEM_PATH "./save/item.txt"


namespace THS { //TreeHoleServer

//Itemtype
enum Itype{ DEFAULTITEM = 0, TEXTITEM, MULTIITEM };

//Header type
enum Htype{ SIGNUP = 0, LOGIN, GET, UPLOAD, REPLY, SEND, SUBSCRIBE, COMMENT, DESUBSCRIBE };

//Error type
enum State{ SUCCESS = 0, USER_EXIST, USER_NOT_EXIST, PASSWORD_FAIL, ILLEGAL_USER, UPLOAD_FAIL };

constexpr char names[10][10] = {"Alice", "Bob", "Carol", "Dave", "Eve", "Francis", "Grace", "Hans", "Isabella", "Jason"};

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

    //subscribe & comment
    int index;       // target item index


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

    // index
    int index;

    RespondHeader(){}
    
    // reply
    RespondHeader(Htype ty, State st, ll tk):
        htype(ty),state(st),token(tk){}

    // send
    RespondHeader(Htype ty, int sl):
        htype(ty),seqlen(sl){}

    
    void setindex(int i){
        index = i;
    }

};




struct DataHeader {

    // base info
    Itype itype;
    size_t textlen;
    size_t multisize;
    
    int index;
    ll uid;
    Datetime timestamp;
    
    // count
    int subcnt; 
    int comcnt;
    int comPersonCnt;

    // text data
    DataHeader(Itype ty, int tl):
        itype(ty),textlen(tl){}

    DataHeader(Itype ty, int tl, int i, ll u, Datetime dt, int sc, int cc, int cp):
        itype(ty),textlen(tl),index(i),uid(u),timestamp(dt),subcnt(sc),comcnt(cc),comPersonCnt(cp){}

    // multi data
    DataHeader(Itype ty, int tl, int ms):
        itype(ty),textlen(tl),multisize(ms){}

    DataHeader(Itype ty, int tl, int ms, int i, ll u, Datetime dt, int sc, int cc, int cp):
        itype(ty),textlen(tl),multisize(ms),index(i),uid(u),timestamp(dt),subcnt(sc),comcnt(cc),comPersonCnt(cp){}

};


struct Comment {

    int index;

    // user info
    ll userID;
    char name[NAMEBYTE];

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
        memset(name, 0, sizeof(name));
        memset(text, 0, sizeof(text));
        strncpy(text, textbuf, textlen);
    }
    char* getName() {
        return name;
    }
    void setName(char* s) {
        strncpy(name, s, strlen(s));
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
    ll subusers[MAXUSER];     // uid of users who subscribe
    int comcnt;       // comment count
    int comPersonCnt;  // 评论的人数，用来分配名字
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
        comPersonCnt = 0;
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

    void addsubscribe(ll uid) {
        for(int i=0; i<subcnt; i++){
            if(subusers[i] == uid)return;
        }
        subusers[subcnt++] = uid;
    }

    void desubscribe(ll uid) {
        for(int i=0; i<subcnt; i++){
            if(subusers[i] == uid){
                for(int j=i; j<subcnt-1; j++){
                    subusers[j] = subusers[j+1];
                }
                subcnt--;
                break;
            }
        }
    }

    int getsubcnt() const {
        return subcnt;
    }

    void setsubcnt(int sc) {
        subcnt = sc;
    }

    int getcomcnt() const {
        return comcnt;
    }

    int getComPerson() const {
        return comPersonCnt;
    }

    void setComPerson(int cp){
        comPersonCnt = cp;
    }

    void addcomment(Comment c) {

        c.index = comcnt + 1;

        ll uid = c.userID;

        for(int i=0; i<comcnt; i++){
            if(comments[i].userID == uid){
                strncpy(c.name, comments[i].name, strlen(comments[i].name));
                comments[comcnt++] = c;
                return;
            }
        }

        const char* name;

        if(uid == userID){
            name = "DZ";
        }
        else if(comPersonCnt >= 10){
            name = "You Win";
        }
        else{
            name = names[comPersonCnt++];
        }

        strncpy(c.name, name, strlen(name));
        comments[comcnt++] = c;

    }

    vector<Comment> getcomment() const {

        vector<Comment> res;
        for(int i=0; i<comcnt; i++){
            res.push_back(comments[i]);
        }
        return res;

    }

    int getComPerson() {
        return comPersonCnt;
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
        setmultisize(MAXSIZE);

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


    void subscribe(ll uid, int index){

        P(&stop);   
        P(&rmutex);  // writer first
        P(&rclock); 
        readcount++;
        if(readcount == 1)
            P(&wmutex);
        V(&rclock);
        V(&rmutex);
        V(&stop);


        auto iter = ilist.find(index);
        if(iter == ilist.end()){
            cerr<< "item not found" <<endl;
        }
        else{
            iter->second->addsubscribe(uid);
        }

        P(&rclock);
        readcount--;
        if(readcount == 0)
            V(&wmutex);
        V(&rclock);

    }

    void desubscribe(ll uid, int index){

        P(&stop);   
        P(&rmutex);  // writer first
        P(&rclock); 
        readcount++;
        if(readcount == 1)
            P(&wmutex);
        V(&rclock);
        V(&rmutex);
        V(&stop);


        auto iter = ilist.find(index);
        if(iter == ilist.end()){
            cerr<< "item not found" <<endl;
        }
        else{
            iter->second->desubscribe(uid);
        }

        P(&rclock);
        readcount--;
        if(readcount == 0)
            V(&wmutex);
        V(&rclock);

    }

    void comment(int index, const Comment& c){

        P(&stop);   
        P(&rmutex);  // writer first
        P(&rclock); 
        readcount++;
        if(readcount == 1)
            P(&wmutex);
        V(&rclock);
        V(&rmutex);
        V(&stop);


        auto iter = ilist.find(index);
        if(iter == ilist.end()){
            cerr<< "item not found" <<endl;
        }
        else{
            iter->second->addcomment(c);
        }

        P(&rclock);
        readcount--;
        if(readcount == 0)
            V(&wmutex);
        V(&rclock);

    }

    int grabindex() {

        int index = 0;

        P(&wclock);
        writecount++;
        if(writecount == 1)
            P(&rmutex);
        V(&wclock);

        P(&wmutex);

        index = ++cnt;

        V(&wmutex);

        P(&wclock);
        writecount--;
        if(writecount == 0)
            V(&rmutex);
        V(&wclock);

        return index;

        
    }

    void append(Item* it, int index){

        P(&wclock);
        writecount++;
        if(writecount == 1)
            P(&rmutex);
        V(&wclock);

        P(&wmutex);

        it->setindex(index);
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

        vector<const Item*>ans;

        if(iter_l == ilist.end())ans = {};  // not found

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

        //admin account
        User* admin = new User(100, "0047");
        hash[100] = admin;

        loaduser();

    }


    void saveuser(){

        P(&rmutex);
        if(readcount==0)
            P(&wmutex);
        readcount++;
        V(&rmutex);

        int outfd = open(USER_PATH, O_RDWR | O_CREAT | O_TRUNC, 0666);

        for(auto ha : hash){
            User* user = ha.second;
            char* buf = reinterpret_cast<char*>(user);
            write(outfd, buf, sizeof(User));
        }

        close(outfd);


        P(&rmutex);
        readcount--;
        if(readcount==0)
            V(&wmutex);
        V(&rmutex);

    }


    void loaduser(){

        int infd = open(USER_PATH, O_RDONLY);
        if(infd < 0){
            cerr<<"no save file, load user fail"<<endl;
            return;
        }

        ssize_t ret;
        char buf[sizeof(User)+4];
        while((ret = read(infd, buf, sizeof(User))) == sizeof(User)){
            User* user = reinterpret_cast<User*>(buf);
            User* newuser = new User(*user);
            hash[newuser->userID] = newuser;
        }

        close(infd);
        
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
        memset(buf, 0, sizeof(buf));
        get(id, buf);

        if(strcmp(buf, password) != 0)return PASSWORD_FAIL;

        return SUCCESS;

    }

};



};



#endif // SERVER_H


