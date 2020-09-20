#include "reciver.h"

using namespace cv;
using namespace std;

bool Reciver::Init(const char* adr){

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout<<"Error : Could not create socket \n";
        return false;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, adr, &serv_addr.sin_addr)<=0)
    {
        cout<<"\n inet_pton error occured\n";
        return false;
    }

    return true;
}

void Reciver::Start(){
    int count = 0;
    if (!Connect()){
        cout<<"Connection failed\n";
    }
    else{

        int iResult = 0;
        while(1){
            ++count;   
            //запрос данных у сервера  
            if(MakeRequest()){
                cout<<"Request done\n";
                iResult = Recv();
                cout << "Recieved new massage "<< count <<"\n";
                if(iResult>0){
                    Read(iResult);
                    iResult = 0;
                }
                if(NeedStop() || waitKey(40) >= 0 ){
                    break;
                }                
            }            

        }
    }
}

bool Reciver::Connect(){
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout<<"Error : Connect Failed \n";
        return false;
    }
    return true;
}

int Reciver::Recv(){
    return  recv(sockfd, (char *)&buf[0], MAX_BUF_SIZE, 0);
}


Reciver::~Reciver(){
    close(sockfd);
}

//----------------------------------Доделай---------------------------------------//
bool VideoReciver::MakeRequest(){
    sendBuf = {u'G', u'e', u't'};
    int len = static_cast<int>(sendBuf.size());
    int result = send(sockfd, (const char*)&sendBuf[0],len, 0);
        return result == len;
}

void VideoReciver::InitReading(){
    namedWindow("Display Image", WINDOW_AUTOSIZE );
}

void VideoReciver::Read(int iResult){
        videoBuffer.resize(iResult);
        memcpy((char*)(&videoBuffer[0]), buf, iResult);
        // Декодируем данные
        jpegimage = imdecode(Mat(videoBuffer), 1);
        if(jpegimage.empty()){
            cout<<"Coonection lost!\n";
        }
        else{
            imshow("Display Image", jpegimage);
        }
}

bool VideoReciver::NeedStop(){
    if (waitKey(10) == 27){
        cout << "Esc key is pressed by user. Stoppig the video\n";
        return true;
    }
    return false;
}

VideoReciver::~VideoReciver(){
    destroyAllWindows();
}


