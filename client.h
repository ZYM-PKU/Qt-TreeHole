/*-------------------------------------------------
#
# Project created by QtCreator
# Author: 赵一鸣
# CreateTime: 2022-5-9
# UpdateTime: 2022-5-9
# Info: Qt树洞
# Github:https://github.com/ZYM-PKU/Qt-TreeHole
#
#-------------------------------------------------*/
#ifndef CLIENT_H
#define CLIENT_H

#include <QTextCodec>
#include <QTcpSocket>
#include <QUdpSocket>
#include <bits/stdc++.h>
#include <QString>

using namespace std;

#define ll long long

#define IP "162.105.101.249"
#define PORT 9900

//Maxlen
#define MAXPLEN 24
#define MAXMLEN 48
#define MAXLEN 1024
#define MAXSIZE 1048576  // 1Mb
#define MAXSLEN 128
#define MAXCOM 128
#define MAXSUB 128
#define MAXUSER 64
#define NAMEBYTE 10


//Time
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

namespace THC { //TreeHoleClient

//Itemtype
enum Itype{ DEFAULTITEM = 0, TEXTITEM, MULTIITEM };

//Header type
enum Htype{ SIGNUP = 0, LOGIN, GET, UPLOAD, REPLY, SEND, SUBSCRIBE, COMMENT, DESUBSCRIBE };

//Error type
enum State{ SUCCESS = 0, USER_EXIST, USER_NOT_EXIST, PASSWORD_FAIL, ILLEGAL_USER, UPLOAD_FAIL };


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
    // subscribe & comment  (pass pass to get compiler quiet)
    RequestHeader(Htype ty, ll uid, ll to, int in, bool pass):
        htype(ty),userID(uid),token(to),index(in){pass = true;}

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
    RespondHeader(Htype ty, State st, ll tk = 0):
        htype(ty),state(st),token(tk){}

    // send
    RespondHeader(Htype ty, int sl):
        htype(ty),seqlen(sl){}

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

    void appendcomment(Comment c){
        comments[comcnt++] = c;
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

    // 排序
    friend bool orderByTime(const Item* t1,const Item* t2);
    friend bool orderByCmtcnt(const Item* t1,const Item* t2);
    friend bool orderBySubcnt(const Item* t1,const Item* t2);
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


class User {

    // user info
    ll userID;
    ll token;
    char password[MAXPLEN];

    // UDP通信
    QUdpSocket *USocket;
    // TCP通信
    QTcpSocket *TSocket;

public:

    // init user
    User(){
        token = 0;
        //初始化套接字对象
        TSocket = new QTcpSocket;
        //链接服务器
        TSocket->connectToHost(QHostAddress(IP), PORT);
        if(!TSocket->waitForConnected(30000))
             qDebug() << "Connection failed";
         else
            qDebug() << "Connect successfully";
    }

    void init(int uid, const char* pbuf){
        userID = uid;
        memset(password, 0, sizeof(password));
        strncpy(password, pbuf, strlen(pbuf));
    }

    ll gettoken(){
        return token;
    }

    void bindsocket(QTcpSocket* sk){
        TSocket = sk;
    }

    bool signup();
    bool login();
    int sendtext(const char* tbuf);
    int sendmulti(const char* tbuf, const char* mbuf);
    vector<Item*> getitems(int starti, int endi);
    void getcomment(Item* it, int comcnt);
    ll getUid() {
        return userID;
    }

    bool subscribe(int index);
    bool desubscribe(int index);
    bool comment(int index, const char* tbuf);
};
// 排序
bool orderByTime(const Item* t1,const Item* t2);
bool orderByCmtcnt(const Item* t1,const Item* t2);
bool orderBySubcnt(const Item* t1,const Item* t2);
};




#endif // CLIENT_H
