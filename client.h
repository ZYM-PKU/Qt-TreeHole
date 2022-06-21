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

#include <QDialog>
#include <QTextCodec>
#include <QTcpSocket>
#include <QUdpSocket>
#include <bits/stdc++.h>

using namespace std;

#define ll long long

#define IP "162.105.101.249"
#define PORT 9999

//Maxlen
#define MAXPLEN 24
#define MAXMLEN 48
#define MAXLEN 256
#define MAXSIZE 2048
#define MAXSLEN 128
#define MAXCOM 128

//Time
#define Datetime struct tm

namespace THC { //TreeHoleClient

//Itemtype
enum Itype{ DEFAULTITEM = 0, TEXTITEM, MULTIITEM };

//Header type
enum Htype{ SIGNUP = 0, LOGIN, GET, UPLOAD, REPLY, SEND };

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
    RespondHeader(Htype ty, State st, ll tk = 0):
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
    User(QDialog* qd){
        token = 0;
        //初始化套接字对象
        TSocket = new QTcpSocket(qd);
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
    bool sendtext(const char* tbuf);
    bool sendmulti(const char* tbuf, const char* mbuf);
    vector<Item*> getitems(int starti, int endi);
    void getcomment(Item* it, int comcnt);


};


};




#endif // CLIENT_H
