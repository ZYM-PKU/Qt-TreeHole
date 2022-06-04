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