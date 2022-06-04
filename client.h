#ifndef CLIENT_H
#define CLIENT_H


#include <bits/stdc++.h>


#define MAXLEN 256
#define MAXSIZE 2048
#define MAXSLEN 128


//Htype
#define GET 0         // get items
#define SEND 1        // send item
#define RESPOND 2     // respond


#define DEFAULTITEM 0     // default item
#define TEXTITEM 1        // text item
#define MULTIITEM 2       // mutlimedia item


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



class Item {
public:

    int Itype;   // item type
    int index;   // item index
    int userID;
    Datetime timestamp;

    Item* prev;   //double list
    Item* next;

    Item(int i, int u, Datetime t): Itype(DEFAULTITEM),index(i),userID(u),timestamp(t){

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



#endif // CLIENT_H
