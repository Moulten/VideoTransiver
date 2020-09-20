#include "Server.h"

using namespace cv;
using namespace std;


VideoReader::VideoReader(const string& file_name)
    : source_video(file_name), empty(false) {
    if (!source_video.isOpened()){
        stringstream ss;
        ss << "File " << file_name << " not found\n";
        throw runtime_error(ss.str());
    }
}

void VideoReader::AddObserver(shared_ptr<AbstractSender> new_sender){
    new_sender->SetNum(observers.size() + 1);
    observers[new_sender->GetNum()] = new_sender;
    cur_thread.push(new_sender->GetNum());
}

void VideoReader::DeleteObserver(unsigned int num){
    if(!observers.count(num)){
        stringstream ss;
        ss<<"Sender with num <"<< num <<"> doesn't exist.\n";
        throw runtime_error(ss.str());
    }  
    observers.erase(num);
}

Mat VideoReader::GetImage() const{
    return image;
}

bool VideoReader::ReadNewImage(){  
    if(source_video.read(image)){
        SetAwake(false);
        Notify();  
        return true;
    }
    else{
        empty = true;
        return false;
    }
    
}

void VideoReader::Notify(){
    observers[cur_thread.front()]->SetAwake(true);
    observers[cur_thread.front()]->Update();
    cur_thread.push(cur_thread.front());
    cur_thread.pop();
}

bool VideoReader::Empty(){
    return empty;
}

bool VideoReader::Awake(){
    return awake;
}

void VideoReader::AwakeAll(){
    for(auto& [key, value]: observers){
        value->SetAwake(true);
    }
}

void VideoReader::SetAwake(bool set){
    awake = set;
}

VideoReader::~VideoReader(){
    source_video.release();
}
