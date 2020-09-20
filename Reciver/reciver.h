#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>



class Reciver{
public:
    Reciver() = default;

    bool Init(const char* adr);
    //шаблонный метод
    void Start();

    virtual ~Reciver();
protected:

    bool Connect();

    int Recv();

    virtual void InitReading() = 0;

    virtual bool MakeRequest() = 0;

    virtual void Read(int) = 0;

    virtual bool NeedStop() = 0;



    static const int MAX_BUF_SIZE = 2073600; //максимальный размер приемного буфера
    unsigned char buf[MAX_BUF_SIZE]; // Буфер для прима сообщений
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

};


class VideoReciver : public Reciver{
public:
    VideoReciver() = default;
    ~VideoReciver();

protected:

    void InitReading() override;

    bool MakeRequest() override;

    void Read(int) override;

    bool NeedStop() override;

    std::vector<uchar> sendBuf;
    std::vector<uchar> videoBuffer;        // Буфер данных изображения
    cv::Mat jpegimage;                     // Вектор данных изображения

};