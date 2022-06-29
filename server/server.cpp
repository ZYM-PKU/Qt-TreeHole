#include <stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <malloc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <bits/stdc++.h>

#include "server.h"

using namespace std;
using namespace THS;

// ip addr & port
#define HOST "127.0.0.1"
#define PORT 9900


// Statement
bool verify(ll uid, ll token);
void detach(int connfd);
void send_msg(State st, int connfd);
void signup(const ll uid, const char* password, int connfd);
void login(const ll uid, const char* password, int connfd);
void logout(const ll uid);
void reply(State st, ll tk, int connfd, int index = 0);

void additemtext(const char* textbuf, int uid, int index);
void additemmulti(const char* textbuf, const char* multibuf, int uid, int index);
void getitem(ll uid, int index, int connfd);
void sendlist(vector<const Item*>& il, int connfd);
void getcomment(ll uid, int index, int connfd);


void printlist();

void *client_thread(void *arg);


// Global data structrue
ItemList IL;
UserTable UT;


inline Datetime get_time(){

    time_t t;
    Datetime tmp; 
	struct tm* linux_tm;

	time(&t);
    linux_tm = localtime(&t);

	tmp.tm_hour = linux_tm->tm_hour;
	tmp.tm_min = linux_tm->tm_min;
	tmp.tm_sec = linux_tm->tm_sec;
	tmp.year = linux_tm->tm_year;
	tmp.tm_mon = linux_tm->tm_mon;
	tmp.tm_mday = linux_tm->tm_mday;
	tmp.tm_wday = linux_tm->tm_wday;
	tmp.tm_isdst = linux_tm->tm_isdst;
	tmp.yday = linux_tm->tm_yday;

    return tmp;

}


void term_handler(int sig){
	UT.saveuser();
	cout<<"bye bye"<<endl;
	exit(0);
}


//多线程 并发服务器
int main(int argc, char const *argv[])
{
	signal(SIGINT, term_handler);

    int ret;
    int sockfd;  //套接字描述符
    int connfd;
    char buf[1024];
    int fp;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //创建通信套接字ipv4协议 tcp通信
    if(sockfd == -1){
        cout<< "socket failed" <<endl;
        exit(-1);
    }
    
    //定义addr存入本机地址信息
    struct sockaddr_in addr;
    addr.sin_family =  AF_INET   ;  //协议
    addr.sin_port =  htons(PORT) ;  //端口
    addr.sin_addr.s_addr = inet_addr("0") ; //ip  0代表本机

    //绑定地址信息(sockfd + addr)
    ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)); 
    if(ret == -1){
        cout<< "bind failed" <<endl;
        exit(-1);
    }
    
    listen(sockfd, 255);  //建立监听队列,并监听状态

	int nodelay = 1;
	int sret = setsockopt(sockfd, IPPROTO_TCP, 1, (char*)&nodelay, sizeof(nodelay));		


    while(1){

        cout<< "Main thread listening on port "<<PORT<<"..."<<endl;
        connfd =  accept(sockfd, NULL, NULL); //建立连接

        // 创建子线程
        pthread_t pid;
        pthread_create(&pid, NULL, client_thread, (void *)&connfd);
        pthread_detach(pid);

    }

    close(sockfd);
    return 0;
}


// 多线程处理函数
void *client_thread(void *arg)
{

    int connfd = *(int *)arg; 
	cout<< "Establish Connection with client "<<connfd<<endl;

	ll uid = 0;
	ll token = 0;

	ssize_t ret;
	char hbuf[sizeof(RequestHeader)+4]; 

    while(1) {

		memset(hbuf, 0 ,sizeof(hbuf));
		if((ret = read(connfd, hbuf, sizeof(hbuf))) <= 0){
			detach(connfd);
		}
    	if(ret != sizeof(hbuf)){// 返回值为实际接收的字节数，ret <= 0 表示客户端断开连接
			cerr<< "header size error" <<endl;
			logout(uid);
			detach(connfd);
			break;
		}	

		RequestHeader* h = reinterpret_cast<RequestHeader*>(hbuf); 

		switch(h->htype){

			case SIGNUP:{

				uid = h->userID;
				char* password = h->password;
				signup(uid, password, connfd);
				break;

			}

			case LOGIN:{

				uid = h->userID;
				char* password = h->password;
				login(uid, password, connfd);
				break;

			}

			case GET:{

				uid = h->userID;
				token = h->token;

				if(!verify(uid, token)){
					cerr<< "illegal user" <<endl;
					reply(ILLEGAL_USER, token, connfd);
				}
				else{
					reply(SUCCESS, token, connfd);
					vector<const Item*> il = IL.getitems(h->starti, h->endi);
					cout<<"user "<<uid << " attempt to get "<<il.size()<<" items "<<endl;
					sendlist(il, connfd);
				}

				break;

			}

			case UPLOAD:{

				uid = h->userID;
				token = h->token;

				if(!verify(uid, token)){
					cerr<< "illegal user" <<endl;
					reply(ILLEGAL_USER, token, connfd);
				}
				else{
					int index = IL.grabindex();
					reply(SUCCESS, token, connfd, index);
					getitem(uid, index, connfd);
				}

				break;

			}

			case SUBSCRIBE: {

				uid = h->userID;
				token = h->token;

				if(!verify(uid, token)){
					cerr<< "illegal user" <<endl;
					reply(ILLEGAL_USER, token, connfd);
				}
				else{
					reply(SUCCESS, token, connfd);
					int index = h->index;
					IL.subscribe(uid, index);
				}

				break;

			}

			case DESUBSCRIBE: {

				uid = h->userID;
				token = h->token;

				if(!verify(uid, token)){
					cerr<< "illegal user" <<endl;
					reply(ILLEGAL_USER, token, connfd);
				}
				else{
					reply(SUCCESS, token, connfd);
					int index = h->index;
					IL.desubscribe(uid, index);
				}

				break;

			}

			case COMMENT: {

				uid = h->userID;
				token = h->token;

				if(!verify(uid, token)){
					cerr<< "illegal user" <<endl;
					reply(ILLEGAL_USER, token, connfd);
				}
				else{
					reply(SUCCESS, token, connfd);
					int index = h->index;
					getcomment(uid, index, connfd);
				}

				break;

			}


			default:{

				cout<<"Illegal header type, ignored"<<endl;

			}

		}

    }

	return NULL;
}





bool verify(ll uid, ll token){

	if(!UT.existtoken(uid))return false;

	ll real = UT.gettoken(uid);
	
	return token == real;

}


void detach(int connfd){

	close(connfd);
	cout<< "Connection "<<connfd<<" detached"<<endl;
	pthread_exit(NULL);  // kill this thread

}


void reply(State st, ll tk, int connfd, int index){

	RespondHeader h(REPLY, st, tk);
	if(index != 0){
		h.setindex(index);
	}

	ssize_t ret;
	char hbuf[sizeof(RespondHeader)+4]; 

	memset(hbuf, 0, sizeof(hbuf));
	memcpy(hbuf, &h, sizeof(RespondHeader));

	if((ret = write(connfd, hbuf, sizeof(hbuf))) != sizeof(hbuf)){
		cerr<<"send msg failed"<<endl;
	}

}


void signup(const ll uid, const char* password, int connfd){

	State res = UT.signup(uid, password);
	reply(res, 0, connfd);

	cout<<"User "<<uid<<" sign up with password "<<"\""<<password<<"\""<<endl;
	
}

void login(const ll uid, const char* password, int connfd){

	State res = UT.login(uid, password);

	ll token = 0;

	if(res == SUCCESS){
		
		token = random();  // generate random token for user
		UT.addtoken(uid, token);

		cout<<"User "<<uid<<" login with token "<<"\""<<token<<"\""<<endl;
	}

	reply(res, token, connfd);
	
}


void logout(const ll uid){

	if(uid == 0)return;
	UT.erasetoken(uid);

}


void additemtext(const char* textbuf, int uid, int index){

	size_t tlen = strlen(textbuf);
	if(tlen >= MAXLEN){
		cerr<< "text too long" <<endl;
		return;
	}

	Datetime timestamp = get_time();
	TextItem* tit = new TextItem(0, uid, timestamp, textbuf);

	Item* it = reinterpret_cast<Item*>(tit);
	IL.append(it, index);

}



void additemmulti(const char* textbuf, const char* multibuf, int uid, int index){

	size_t tlen = strlen(textbuf);
	if(tlen >= MAXLEN){
		cerr<< "text too long" <<endl;
		return;
	}


	Datetime timestamp = get_time();
	MultiItem* mit = new MultiItem(0, uid, timestamp, textbuf, multibuf);

	Item* it = reinterpret_cast<Item*>(mit);
	IL.append(it, index);

}



void getitem(ll uid, int index, int connfd){

	ssize_t dret;
	char dbuf[sizeof(DataHeader)+4]; 

	if((dret = read(connfd, dbuf, sizeof(dbuf))) != sizeof(dbuf)){
		cerr<< "header size error" <<endl;
		return;
	}

	DataHeader* dh = reinterpret_cast<DataHeader*>(dbuf); 
	Itype itype = dh->itype;

	if(itype == TEXTITEM){

		size_t tlen = dh->textlen;
		char textbuf[tlen+4];

		ssize_t pret;
		memset(textbuf, 0, sizeof(textbuf));
		
		if((pret = read(connfd, textbuf, tlen)) != tlen){
			cerr<< "textitem length error" <<endl;
			return;
		}
		else{
			cout<<"user "<<uid << " send text: "<<"\""<< textbuf <<"\""<<endl;
			additemtext(textbuf, uid, index);
		}

	}

	else if(itype == MULTIITEM){

		size_t tlen = dh->textlen;
		size_t msize = dh->multisize;

		char textbuf[tlen+4];
		char* multibuf = new char[msize+4];

		ssize_t pret;
		memset(textbuf, 0, sizeof(textbuf));
		memset(multibuf, 0, sizeof(multibuf));

		if((pret = read(connfd, textbuf, tlen)) != tlen){
			cerr<< "textitem length error" <<endl;
			return;
		}
		else{

			cout<<"user "<<uid << " send text: "<<"\""<< textbuf <<"\""<<endl;
			
			pret = 0;
			while((pret += read(connfd, multibuf+pret, msize-pret)) != msize);
			additemmulti(textbuf, multibuf, uid, index);

			delete multibuf;
			// if((pret = read(connfd, multibuf, msize)) != msize){
			// 	cerr<< "multiitem length error" <<endl;
			// 	return;
			// }
			// else{
			// 	additemmulti(textbuf, multibuf, uid);
			// }
		}

	}
	else{
		cerr<<"Illegal item type"<<endl;
		return;
	}
		

}


void sendlist(vector<const Item*>& il, int connfd){

	int seql = il.size();

	RespondHeader h(SEND, seql);

	ssize_t ret;
	char hbuf[sizeof(RespondHeader)+4]; 

	memset(hbuf, 0, sizeof(hbuf));
	memcpy(hbuf, &h, sizeof(RespondHeader));

	if((ret = write(connfd, hbuf, sizeof(hbuf))) != sizeof(hbuf)){
		cerr<<"send header failed"<<endl;
		return;
	}


	ssize_t dret;
	char dbuf[sizeof(DataHeader)+4]; 

	for(auto it: il){

		Itype itype = it->getitype();

		if(itype == TEXTITEM){
			
			const TextItem* tit = reinterpret_cast<const TextItem*>(it);

			size_t tlen = tit->gettextlen();
			int index = tit->getindex();
			ll uid = tit->getuid();
			Datetime timestamp = tit->gettimestamp();
			int subcnt = tit->getsubcnt();
			int comcnt = tit->getcomcnt();
			int comPersoncnt = tit->getComPerson();

			// send header
			DataHeader dh(TEXTITEM, tlen, index, uid, timestamp, subcnt, comcnt, comPersoncnt);
			memset(dbuf, 0, sizeof(dbuf));
			memcpy(dbuf, &dh, sizeof(DataHeader));
			
			if((dret = write(connfd, dbuf, sizeof(dbuf))) != sizeof(dbuf)){
				cerr<<"send dataheader failed"<<endl;
				return;
			}

			// send text
			const char* textbuf = tit->gettext();
			if((dret = write(connfd, textbuf, tlen)) != tlen){
				cerr<<"send text failed"<<endl;
				return;
			}

			// send comments
			vector<Comment>vs = tit->getcomment();
			char cbuf[sizeof(Comment)+4];
			for(auto com : vs){

				memset(cbuf, 0, sizeof(cbuf));
				memcpy(cbuf, &com, sizeof(Comment));

				if((dret = write(connfd, cbuf, sizeof(cbuf))) != sizeof(cbuf)){
					cerr<<"send comment failed"<<endl;
					return;
				}

			}

		}

		else if(itype == MULTIITEM){

			const MultiItem* mit = reinterpret_cast<const MultiItem*>(it);

			size_t tlen = mit->gettextlen();
			size_t msize = mit->getmultisize();
			int index = mit->getindex();
			ll uid = mit->getuid();
			Datetime timestamp = mit->gettimestamp();
			int subcnt = mit->getsubcnt();
			int comcnt = mit->getcomcnt();
			int comPersoncnt = mit->getComPerson();


			// send header
			DataHeader dh(MULTIITEM, tlen, msize, index, uid, timestamp, subcnt, comcnt, comPersoncnt);
			memset(dbuf, 0, sizeof(dbuf));
			memcpy(dbuf, &dh, sizeof(DataHeader));

			if((dret = write(connfd, dbuf, sizeof(dbuf))) != sizeof(dbuf)){
				cerr<<"send dataheader failed"<<endl;
				return;
			}

			// send text & multi
			const char* textbuf = mit->gettext();
			const char* multibuf = mit->getmulti();
			if((dret = write(connfd, textbuf, tlen)) != tlen){
				cerr<<"textitem length error"<<endl;
				return;
			}
			else{
				if((dret = write(connfd, multibuf, msize)) != msize){
					cerr<<"multiitem length error"<<endl;
					return;
				}
			}

			// send comments
			vector<Comment>vs = mit->getcomment();
			char cbuf[sizeof(Comment)+4];
			for(auto com : vs){

				memset(cbuf, 0, sizeof(cbuf));
				memcpy(cbuf, &com, sizeof(Comment));

				if((dret = write(connfd, cbuf, sizeof(cbuf))) != sizeof(cbuf)){
					cerr<<"send comment failed"<<endl;
					return;
				}

			}

		}
		else{
			cerr<<"Illegal item type, ignored"<<endl;
			return;
		}
	}

}



void printlist(){
	vector<const Item*> il = IL.getitems(1, INT_MAX);

	for(auto it: il){
		if(it->getitype() == TEXTITEM){
			const TextItem* tit = reinterpret_cast<const TextItem*>(it);
			ll uid = tit->getuid();
			const char* text = tit->gettext();
			cout<<"user "<<uid << " send text: "<<"\""<< text <<"\""<<endl;
		}
	}
}


void getcomment(ll uid, int index, int connfd){

	ssize_t dret;
	char dbuf[sizeof(DataHeader)+4]; 


	if((dret = read(connfd, dbuf, sizeof(dbuf))) != sizeof(dbuf)){
		cerr<< "header size error" <<endl;
		return;
	}

	DataHeader* dh = reinterpret_cast<DataHeader*>(dbuf); 

	int tlen = dh->textlen;
	char textbuf[tlen+4];

	ssize_t pret;
	memset(textbuf, 0, sizeof(textbuf));
	
	if((pret = read(connfd, textbuf, tlen)) != tlen){
		cerr<< "comment length error" <<endl;
		return;
	}
	else{
		cout<<"user "<<uid << " comment: "<<"\""<< textbuf <<"\""<<endl;
		Datetime timestamp = get_time();
		Comment c(0, uid, timestamp, textbuf);
		IL.comment(index, c);
	}

	
}