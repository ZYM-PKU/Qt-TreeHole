#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
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
using namespace std;

#include "server.h"

#define HOST "127.0.0.1"
#define PORT 9999


void *client_thread(void *arg);

extern Datetime gettime();
void additemtext(const char* textbuf, int uid);
void additemmulti(const char* textbuf, const char* multibuf, int uid);
void sendlist(Item* il, int connfd);


ItemList IL;


//多线程 并发服务器
int main(int argc, char const *argv[])
{
    int ret;
    int sockfd;  //套接字描述符
    int connfd;
    char buf[1024];
    int fp;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //创建通信套接字ipv4协议 tcp通信
    if(sockfd == -1){
        printf("socket failed\n");
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
        printf("bind failed\n");
        exit(-1);
    }
    
    listen(sockfd, 255);  //建立监听队列,并监听状态

    while(1){

        printf("wait...\n");
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

	int uid = 0;


	int ret;
	char hbuf[sizeof(RequestHeader)+4]; 

    while(1) {

		memset(hbuf, 0 ,sizeof(hbuf));
    	if((ret = read(connfd, hbuf, sizeof(hbuf))) > 0){// 返回值为实际接收的字节数，ret <= 0 表示客户端断开连接
			
			if(ret != sizeof(RequestHeader))continue;

			RequestHeader* h = (RequestHeader*)hbuf;

			uid = h->userID;

			if(h->Htype == GET){

				Item* il = IL.getitems(h->getnum);
				sendlist(il, connfd);
				break;

			}
			else if(h->Htype == SEND){


				int Itype = h->itemtype;

				if(Itype == TEXTITEM){

					int tlen = h->textlen;
					char textbuf[tlen+4];

					memset(textbuf, 0, sizeof(textbuf));
					if((ret = read(connfd, textbuf, tlen)) != tlen)
						goto detach;

					additemtext(textbuf, uid);

				}
				else if(Itype == MULTIITEM){

					int tlen = h->textlen;
					int mlen = h->multilen;

					char textbuf[tlen+4];
					char multibuf[mlen+4];

					memset(textbuf, 0, sizeof(textbuf));
					if((ret = read(connfd, textbuf, tlen)) != tlen)
						goto detach;

					memset(multibuf, 0, sizeof(multibuf));
					if((ret = read(connfd, multibuf, mlen)) == mlen)
						goto detach;

					additemmulti(textbuf, multibuf, uid);

				}
				else{
					cout<<"Illegal item type, ignored"<<endl;
				}
				
			}
			else{
				cout<<"Illegal header type, ignored"<<endl;
			}

			
		}  
		else{
detach:
			close(connfd);
			cout<< "Connection "<<connfd<<" detached"<<endl;
			return NULL;
		}
    }

	return NULL;
}



void additemtext(const char* textbuf, int uid){

	Datetime timestamp = gettime();

	TextItem* it = new TextItem(0, uid, timestamp, textbuf);
	IL.append(it);

}



void additemmulti(const char* textbuf, const char* multibuf, int uid){


	Datetime timestamp = gettime();

	MuiltItem* it = new MuiltItem(0, uid, timestamp, textbuf, multibuf);
	IL.append(it);

}


void sendlist(Item* il, int connfd){

}