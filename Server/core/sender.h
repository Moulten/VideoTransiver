#pragma once

#include <stdio.h>
#include "socket.h"
#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <sstream>
#include <future>
#include "VideoReader.h"

class Sender{
public:
    Sender(cv::VideoReader* vr, Socket* add_socket);

   void SetNum(unsigned int new_num);
   
    void Update();

    void Send();

    bool Die();
    
    unsigned int GetNum() const;

private:
    std::shared_ptr<VideoReader> video_reader; 
    cv::Mat image;
    //std::shared_ptr<Socket> socket_;
    unsigned int num;
    bool empty;
};

