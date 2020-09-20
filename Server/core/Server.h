#pragma once

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <sstream>
#include <future>
#include <queue>
    
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <mutex>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>


class Socket{
public:
    Socket();

    void SetPort();         

    void SetDomain();

    bool WaitCommand();   

    bool Accept();

    bool Send(cv::Mat object); // bitmap

    ~Socket();
private:
    unsigned char buf[10];
    std::mutex m_mutex;
    std::vector<uchar> imgBuf;
    struct sockaddr_in serv_addr;
    int listenfd, connfd;

};



class AbstractSender{
public:
    virtual void SetNum(unsigned int new_num) = 0;
   
    virtual void Update() = 0;

    virtual void Send() = 0;            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    virtual bool Die() = 0;

    virtual bool Awake() = 0;

    virtual void SetAwake(bool) = 0;
    
    virtual unsigned int GetNum() const = 0;

    virtual ~AbstractSender() = default;

};


class AbstractVideoReader{
public:
    //Добавить наблюдателя
    virtual void AddObserver(std::shared_ptr<AbstractSender> new_sender) = 0;

    //Удалить наблюдателя по номеру
    // Не забудь, что удалить можно и из середины
    virtual void DeleteObserver(unsigned int num) = 0;

    //Передать картинку
    virtual cv::Mat GetImage() const = 0;

    //Считать новую картинку из видеопотока, обновив текущую
    virtual bool ReadNewImage() = 0;

    //Оповестить очередного наблюдателя
    virtual void Notify() = 0;

    //Проверка потока считывания из видеофайла
    virtual bool Empty() = 0;

    virtual bool Awake() = 0;

    virtual void AwakeAll() = 0;   

    virtual void SetAwake(bool) = 0;

    virtual ~AbstractVideoReader() = default;

};




class Sender: public AbstractSender{
public:
    Sender(std::shared_ptr<AbstractVideoReader> vr, std::shared_ptr<Socket> add_socket);

   void SetNum(unsigned int new_num) override;
   
    void Update() override;

    void Send() override;

    bool Die() override;

    bool Awake() override;

    void SetAwake(bool) override;
    
    unsigned int GetNum() const override;

private:
    std::shared_ptr<AbstractVideoReader> video_reader; 
    cv::Mat image;
    std::shared_ptr<Socket> socket_;
    unsigned int num;
    bool empty;
    bool awake; 
};


class VideoReader : public AbstractVideoReader{ 
public:
//Конструктор
    VideoReader(const std::string& file_name);
    //Добавить наблюдателя
    void AddObserver(std::shared_ptr<AbstractSender> new_sender) override;

    void DeleteObserver(unsigned int num) override;

    cv::Mat GetImage() const override;

    bool ReadNewImage() override;

    void Notify() override;

    bool Awake() override;

    void AwakeAll() override;  

    void SetAwake(bool) override;

    bool Empty() override;

    ~VideoReader();

private:
    cv::VideoCapture source_video;
    std::queue<int> cur_thread;
    std::unordered_map<unsigned int,std::shared_ptr<AbstractSender>> observers;
    cv::Mat image;
    bool awake;    
    bool empty;
};


