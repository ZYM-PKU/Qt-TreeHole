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
#include <QTextCodec>
#include <QTcpSocket>
#include <QUdpSocket>
#include<bits/stdc++.h>
#include "client.h"

using namespace std;
using namespace THC;


namespace THC { //TreeHoleClient

inline Datetime get_time(){

    time_t t;
    Datetime* tmp;

    time(&t);
    tmp = localtime(&t);

    return *tmp;

}


/* 用户注册API
 * 参数：套接字，账号，密码
 * 返回值：true（成功）/ false（失败）
 * 注：失败时打印相应信息
*/
bool User::signup(){

    // send request
    RequestHeader header(SIGNUP, userID, password);

    ssize_t ret;
    char hbuf[sizeof(RequestHeader)+4];

    memset(hbuf, 0, sizeof(hbuf));
    memcpy(hbuf, &header, sizeof(RequestHeader));

    if((ret = TSocket->write(hbuf, sizeof(hbuf))) != sizeof(hbuf)){
        cerr<< "send request failed" <<endl;
        return false;
    }

    TSocket->flush();
    TSocket->waitForReadyRead();

    // get respond
    char rbuf[sizeof(RespondHeader)+4];
    if((ret = TSocket->read(rbuf, sizeof(rbuf))) != sizeof(rbuf)){
        cerr<< "get respond failed" <<endl;
        return false;
    }

    RespondHeader* res = reinterpret_cast<RespondHeader*>(rbuf);

    State st = res->state;

    switch(st){
        case SUCCESS:
            return true;
        case USER_EXIST:
            cerr<< "User already exists" <<endl;
            break;
        default:
            cerr<< "Unknown error" <<endl;
    }

    return false;

}


/* 用户登录API
 * 参数：套接字，账号，密码
 * 返回值：true（成功）/ false（失败）
 * 注：失败时打印相应信息
*/
bool User::login(){

    // send request
    RequestHeader header(LOGIN, userID, password);

    ssize_t ret;
    char hbuf[sizeof(RequestHeader)+4];

    memset(hbuf, 0, sizeof(hbuf));
    memcpy(hbuf, &header, sizeof(RequestHeader));

    if((ret = TSocket->write(hbuf, sizeof(hbuf))) != sizeof(hbuf)){
        cerr<< "send request failed" <<endl;
        return false;
    }

    TSocket->flush();
    TSocket->waitForReadyRead();

    // get respond
    char rbuf[sizeof(RespondHeader)+4];
    if((ret = TSocket->read(rbuf, sizeof(rbuf))) != sizeof(rbuf)){
        cerr<< "get respond failed" <<endl;
        return false;
    }

    RespondHeader* res = reinterpret_cast<RespondHeader*>(rbuf);

    State st = res->state;

    switch(st){
        case SUCCESS:
            token = res->token;
            return true;
        case USER_NOT_EXIST:
            cerr<< "User not found" <<endl;
            break;
        case PASSWORD_FAIL:
            cerr<< "Wrong password" <<endl;
            break;
        default:
            cerr<< "Unknown error" <<endl;
    }

    return false;
}


/* 发送文本条目API
 * 参数：字符数组
 * 返回值：true（成功）/ false（失败）
 * 注：失败时打印相应信息
*/
bool User::sendtext(const char *tbuf){

    int tlen = strlen(tbuf);
    if(tlen >= MAXLEN){
        cerr<< "text too long" <<endl;
        return false;
    }

    // send request
    RequestHeader header(UPLOAD, userID, token, 1);

    ssize_t ret;
    char hbuf[sizeof(RequestHeader)+4];

    memset(hbuf, 0, sizeof(hbuf));
    memcpy(hbuf, &header, sizeof(RequestHeader));

    if((ret = TSocket->write(hbuf, sizeof(hbuf))) != sizeof(hbuf)){
        cerr<< "send request failed" <<endl;
        return false;
    }

    TSocket->flush();
    TSocket->waitForReadyRead();


    // get respond
    char rbuf[sizeof(RespondHeader)+4];
    if((ret = TSocket->read(rbuf, sizeof(rbuf))) != sizeof(rbuf)){
        cerr<< "get respond failed" <<endl;
        return false;
    }

    RespondHeader* res = reinterpret_cast<RespondHeader*>(rbuf);

    State st = res->state;

    if(st != SUCCESS){
        cerr<< "identification failed" <<endl;
        return false;
    }

    // send list
    DataHeader dh(TEXTITEM, tlen);

    ssize_t dret;
    char dhbuf[sizeof(DataHeader)+4];

    memset(dhbuf, 0, sizeof(dhbuf));
    memcpy(dhbuf, &dh, sizeof(DataHeader));

    if((dret = TSocket->write(dhbuf, sizeof(dhbuf))) != sizeof(dhbuf)){
        cerr<< "data header error" <<endl;
        return false;
    }

    TSocket->flush();

    if((dret = TSocket->write(tbuf, tlen)) != tlen){
        cerr<< "send text fail" <<endl;
        return false;
    }

    TSocket->flush();

    return true;

}

/* 发送多媒体条目API
 * 参数：字符数组, 文件缓存
 * 返回值：true（成功）/ false（失败）
 * 注：失败时打印相应信息
*/
bool User::sendmulti(const char *tbuf, const char* mbuf){

    int tlen = strlen(tbuf);
    if(tlen >= MAXLEN){
        cerr<< "text too long" <<endl;
        return false;
    }

    int msize = sizeof(mbuf);
    if(msize >= MAXSIZE){
        cerr<< "file too large" <<endl;
        return false;
    }

    // send request
    RequestHeader header(UPLOAD, userID, token, 1);

    ssize_t ret;
    char hbuf[sizeof(RequestHeader)+4];

    memset(hbuf, 0, sizeof(hbuf));
    memcpy(hbuf, &header, sizeof(RequestHeader));

    if((ret = TSocket->write(hbuf, sizeof(hbuf))) != sizeof(hbuf)){
        cerr<< "send request failed" <<endl;
        return false;
    }

    TSocket->flush();
    TSocket->waitForReadyRead();


    // get respond
    char rbuf[sizeof(RespondHeader)+4];
    if((ret = TSocket->read(rbuf, sizeof(rbuf))) != sizeof(rbuf)){
        cerr<< "get respond failed" <<endl;
        return false;
    }

    RespondHeader* res = reinterpret_cast<RespondHeader*>(rbuf);

    State st = res->state;

    if(st != SUCCESS){
        cerr<< "identification failed" <<endl;
        return false;
    }

    // send list
    DataHeader dh(MULTIITEM, tlen, msize);

    ssize_t dret;
    char dhbuf[sizeof(DataHeader)+4];

    memset(dhbuf, 0, sizeof(dhbuf));
    memcpy(dhbuf, &dh, sizeof(DataHeader));

    if((dret = TSocket->write(dhbuf, sizeof(dhbuf))) != sizeof(dhbuf)){
        cerr<< "data header error" <<endl;
        return false;
    }

    TSocket->flush();

    if((dret = TSocket->write(tbuf, tlen)) != tlen){
        cerr<< "send text fail" <<endl;
        return false;
    }

    TSocket->flush();

    if((dret = TSocket->write(mbuf, msize)) != msize){
        cerr<< "send file fail" <<endl;
        return false;
    }

    TSocket->flush();

    return true;
}



void User::getcomment(Item* it, int comcnt){

    ssize_t ret;
    char cbuf[sizeof(Comment)+4];

    for(int i=0; i<comcnt; i++){

        //TSocket->waitForReadyRead();

        memset(cbuf, 0, sizeof(cbuf));

        if((ret = TSocket->read(cbuf, sizeof(Comment))) != sizeof(Comment)){
            cerr<< "get comment failed" <<endl;
            return;
        }

        Comment* com = reinterpret_cast<Comment*>(cbuf);

        it->addcomment(*com);
    }

}


/* 获取条目API
 * 参数：起始条目号，终止条目号
 * 返回值：条目指针数组，范围是[起始条目号，终止条目号)
 * 注：失败时打印相应信息
*/
vector<Item*> User::getitems(int starti, int endi){

    // send request
    RequestHeader header(GET, userID, token, starti, endi);

    ssize_t ret;
    char hbuf[sizeof(RequestHeader)+4];

    memset(hbuf, 0, sizeof(hbuf));
    memcpy(hbuf, &header, sizeof(RequestHeader));

    if((ret = TSocket->write(hbuf, sizeof(hbuf))) != sizeof(hbuf)){
        cerr<< "send request failed" <<endl;
        return {};
    }

    TSocket->flush();
    TSocket->waitForReadyRead();


    // get respond
    char rbuf[sizeof(RespondHeader)+4];
    if((ret = TSocket->read(rbuf, sizeof(rbuf))) != sizeof(rbuf)){
        cerr<< "get respond failed" <<endl;
        return {};
    }

    RespondHeader* res = reinterpret_cast<RespondHeader*>(rbuf);

    State st = res->state;

    if(st != SUCCESS){
        cerr<< "identification failed" <<endl;
        return {};
    }


    vector<Item*> ans;

    // get list
    TSocket->waitForReadyRead();

    if((ret = TSocket->read(rbuf, sizeof(rbuf))) != sizeof(rbuf)){
        cerr<< "get header failed" <<endl;
        return {};
    }

    res = reinterpret_cast<RespondHeader*>(rbuf);
    int seqlen = res->seqlen;

    ssize_t dret;
    char dbuf[sizeof(DataHeader)+4];

    for(int curr = 0; curr < seqlen; curr++){

        //TSocket->waitForReadyRead();

        if((dret = TSocket->read(dbuf, sizeof(dbuf))) != sizeof(dbuf)){
            cerr<< "header size error" <<endl;
            return {};
        }

        DataHeader* dh = reinterpret_cast<DataHeader*>(dbuf);

        Itype itype = dh->itype;
        int index = dh->index;
        ll uid = dh->uid;
        Datetime timestamp = dh->timestamp;
        int subcnt = dh->subcnt;
        int comcnt = dh->comcnt;


        if(itype == TEXTITEM){

            int tlen = dh->textlen;
            char textbuf[tlen+4];

            ssize_t pret;
            memset(textbuf, 0, sizeof(textbuf));

            //TSocket->waitForReadyRead();

            if((pret = TSocket->read(textbuf, tlen)) != tlen){
                cerr<< "textitem length error" <<endl;
                return {};
            }
            else{
                TextItem* tit = new TextItem(index, uid, timestamp, textbuf);
                Item* it = reinterpret_cast<Item*>(tit);
                it->setsubcnt(subcnt);
                getcomment(it, comcnt);
                ans.push_back(it);
            }

        }
        else if(itype == MULTIITEM){

            int tlen = dh->textlen;
            int msize = dh->multisize;

            char textbuf[tlen+4];
            char multibuf[msize+4];

            ssize_t pret;
            memset(textbuf, 0, sizeof(textbuf));
            memset(multibuf, 0, sizeof(multibuf));

            //TSocket->waitForReadyRead();

            if((pret = TSocket->read(textbuf, tlen)) != tlen){
                cerr<< "textitem length error" <<endl;
                return {};
            }
            else{

                //TSocket->waitForReadyRead();

                if((pret = TSocket->read(multibuf, msize)) != msize){
                    cerr<< "multiitem length error" <<endl;
                    return {};
                }
                else{
                    MultiItem* mit = new MultiItem(index, uid, timestamp, textbuf, multibuf);
                    Item* it = reinterpret_cast<Item*>(mit);
                    it->setsubcnt(subcnt);
                    getcomment(it, comcnt);
                    ans.push_back(it);
                }
            }

        }
        else{
            cerr<<"Illegal item type"<<endl;
            return {};
        }

    }

    return ans;

}


bool User::subscribe(int index){


    // send request
    RequestHeader header(SUBSCRIBE, userID, token, index, true);

    ssize_t ret;
    char hbuf[sizeof(RequestHeader)+4];

    memset(hbuf, 0, sizeof(hbuf));
    memcpy(hbuf, &header, sizeof(RequestHeader));

    if((ret = TSocket->write(hbuf, sizeof(hbuf))) != sizeof(hbuf)){
        cerr<< "send request failed" <<endl;
        return false;
    }

    TSocket->flush();
    TSocket->waitForReadyRead();


    // get respond
    char rbuf[sizeof(RespondHeader)+4];
    if((ret = TSocket->read(rbuf, sizeof(rbuf))) != sizeof(rbuf)){
        cerr<< "get respond failed" <<endl;
        return false;
    }

    RespondHeader* res = reinterpret_cast<RespondHeader*>(rbuf);

    State st = res->state;

    if(st != SUCCESS){
        cerr<< "identification failed" <<endl;
        return false;
    }
    else{
        return true;
    }

}


bool User::comment(int index, const char *tbuf){

    int tlen = strlen(tbuf);
    if(tlen >= MAXLEN){
        cerr<< "text too long" <<endl;
        return false;
    }

    // send request
    RequestHeader header(COMMENT, userID, token, index, true);

    ssize_t ret;
    char hbuf[sizeof(RequestHeader)+4];

    memset(hbuf, 0, sizeof(hbuf));
    memcpy(hbuf, &header, sizeof(RequestHeader));

    if((ret = TSocket->write(hbuf, sizeof(hbuf))) != sizeof(hbuf)){
        cerr<< "send request failed" <<endl;
        return false;
    }

    TSocket->flush();
    TSocket->waitForReadyRead();


    // get respond
    char rbuf[sizeof(RespondHeader)+4];
    if((ret = TSocket->read(rbuf, sizeof(rbuf))) != sizeof(rbuf)){
        cerr<< "get respond failed" <<endl;
        return false;
    }

    RespondHeader* res = reinterpret_cast<RespondHeader*>(rbuf);

    State st = res->state;

    if(st != SUCCESS){
        cerr<< "identification failed" <<endl;
        return false;
    }

    // send list
    DataHeader dh(TEXTITEM, tlen);

    ssize_t dret;
    char dhbuf[sizeof(DataHeader)+4];

    memset(dhbuf, 0, sizeof(dhbuf));
    memcpy(dhbuf, &dh, sizeof(DataHeader));

    if((dret = TSocket->write(dhbuf, sizeof(dhbuf))) != sizeof(dhbuf)){
        cerr<< "data header error" <<endl;
        return false;
    }

    TSocket->flush();

    if((dret = TSocket->write(tbuf, tlen)) != tlen){
        cerr<< "send comment fail" <<endl;
        return false;
    }

    TSocket->flush();

    return true;

}

};

