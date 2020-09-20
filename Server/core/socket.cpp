#include "Server.h"

using namespace std;
using namespace cv;


Socket::Socket(): connfd(0){
        listenfd = socket(AF_INET, SOCK_STREAM, 0);
        memset(&serv_addr, '0', sizeof(serv_addr));
    
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(5000);
        bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        listen(listenfd, 10);
}

bool Socket::WaitCommand(){
    lock_guard<mutex> locker(m_mutex);
    cout<<"wait client's command\n";
    int iResult = recv(connfd, (char *)&buf[0], 10, 0);
    cout<< "Recived command: <";
    for (int i = 0; i<10; i++){
        cout<<buf[i];
    }
    cout<<">\n";
    return iResult > 0;
}

bool Socket::Accept(){
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    return connfd > 0;
}

bool Socket::Send(Mat object){
    lock_guard<mutex> locker(m_mutex);
    if ( !object.data ){
        printf("No image data \n");
        return false;
    }
    int result;

    imencode(".jpg", object, imgBuf, {1,20});
    int len = static_cast<int>(imgBuf.size());
    result = send(connfd, (const char*)&imgBuf[0],
                     len, 0);

    if(waitKey(10) >= 0){
        return false;
    }
    return result == len;
}

Socket::~Socket(){
    close(connfd);
}