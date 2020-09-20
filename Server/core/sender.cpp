#include "Server.h"
using namespace cv;
using namespace std;


Sender::Sender(shared_ptr<AbstractVideoReader> vr, shared_ptr<Socket> add_socket): awake(false){
    video_reader = vr;
    socket_ = add_socket;
}

void Sender::SetNum(unsigned int new_num){
    num = new_num;
}
void Sender::Update(){
    image = video_reader->GetImage();
}
void Sender::Send(){
    if(socket_->WaitCommand()){
        socket_->Send(image);
    }
    SetAwake(false);
    video_reader->SetAwake(true);
}
bool Sender::Die(){
    return video_reader->Empty();
}

bool Sender::Awake(){
    return awake;
}

void Sender::SetAwake(bool set){
    awake = set;
}

unsigned int Sender::GetNum() const{
    return num;
}

