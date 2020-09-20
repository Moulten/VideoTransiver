#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <vector>

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int InitServer(){
        int listenfd = 0, connfd = 0;

        struct sockaddr_in serv_addr;
        char sendBuff[1025];
        listenfd = socket(AF_INET, SOCK_STREAM, 0);
        memset(&serv_addr, '0', sizeof(serv_addr));
        //memset(sendBuff, '0', sizeof(sendBuff));
    
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(5000);
        bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        listen(listenfd, 10);


        //namedWindow("Display Image", WINDOW_AUTOSIZE );

        return listenfd;
}

int main(int argc, char *argv[]) {

    std::vector<uchar> imgBuf;
    int listenfd = InitServer();
    int connfd = -1;
    time_t ticks;

    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }
    Mat image, to_send;

    VideoCapture capt(argv[1]);
     if (!capt.isOpened()){
        printf("No video data\n");
    }

    //namedWindow("Video Player", WINDOW_AUTOSIZE );

    while(connfd < 0){
            connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
            ticks = time(NULL);        
        }


    while(capt.grab()){
    static int count_ = 0;
    ++count_;
        capt >> image;
        //imshow("Video Player", image);
        if ( !image.data ){
            printf("No image data \n");
            return -1;
        }
        vector<int> params = {1,20};
        imencode(".jpg", image, imgBuf, params);
        send(connfd, (const char*)&imgBuf[0],
                        static_cast<int>(imgBuf.size()), 0);
        cout<<"Send "<<count_<<endl;

         if(int count = waitKey(50); count >= 0){
             cout << count;
            break;
        }
    }
    close(connfd);

    capt.release();
     
}
