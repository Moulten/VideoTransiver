#include <stdio.h>
//#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <sstream>
#include <thread>
#include "Server.h"
using namespace cv;
using namespace std;

condition_variable mcond;
mutex mmutex;

void ImageSender(shared_ptr<AbstractSender> this_sender);

void ImageGetter(shared_ptr<AbstractVideoReader> this_reader);

int main(int argc, char** argv )
{
    if(argc < 2){
        cout<<"Don't forget about the video-file!\n";
        return -1;
    }
    
    cout<<"Create VideoReader\n";
    shared_ptr<AbstractVideoReader> reader = make_shared<VideoReader>(argv[1]);
    cout<<"Create Socket\n";
    auto link = make_shared<Socket>();

    cout<<"Create odd_sender\n";
    shared_ptr<AbstractSender> odd_sender = make_shared<Sender>(reader, link);
    cout<<"Create even_sender\n";
    shared_ptr<AbstractSender> even_sender = make_shared<Sender>(reader, link);

    cout<<"Add odd_sender\n";
    reader->AddObserver(odd_sender);
    cout<<"Add even_sender\n";
    reader->AddObserver(even_sender);
 
    cout<<"Wait for connection\n";
    while(!link->Accept()){
        cout<<"loading\n";
    }
    cout<<"Connection was created\n";

    cout<<"Thread sender1 start\n";
    thread thread_sender1(ImageSender,odd_sender);
    cout<<"Thread sender2 start\n";
    thread thread_sender2(ImageSender,even_sender);
    cout<<"Thread reader start\n";
    thread thread_reader(ImageGetter,reader);

    thread_reader.join();
    thread_sender1.join();
    thread_sender2.join();

    cout<<"Program stoped\n";
    return 0;
}


void ImageSender(shared_ptr<AbstractSender> this_sender){
	while(1){
		{
            //Ожидаем очереди на передачу
			unique_lock<mutex> lck(mmutex);
			mcond.wait(lck,[&]{return this_sender->Awake();});
		}
		{
			if(this_sender->Die()){
                mcond.notify_all();
				break;
			}
			unique_lock<mutex> lck(mmutex);
            //Отправить данные
            this_sender->Send();
			cout<<"Send by "<<this_sender->GetNum()<<" \n";
		}
		mcond.notify_all();
	}
    {
        lock_guard<mutex> locker(mmutex);
        cout<<"Sender's thread "<<this_sender->GetNum()<< " stoped\n";
    }
}


void ImageGetter(shared_ptr<AbstractVideoReader> this_reader){
	while(1){
		{
            unique_lock<mutex> lck(mmutex);
            cout<<"Ready to read\n";
            if(!this_reader->ReadNewImage()){
                break;
            }
			cout<<"New image was read\n";
		}
		mcond.notify_all();
		{
			unique_lock<mutex> lck(mmutex);
			mcond.wait(lck,[&]{return this_reader->Awake();});

		}
	}
    this_reader->AwakeAll();
	mcond.notify_all();
    {
        lock_guard<mutex> locker(mmutex);
        cout<<"\nVideoReader stoped\n";
    }
	
}