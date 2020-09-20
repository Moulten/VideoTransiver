#include "reciver.h"

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }

    VideoReciver rcv;
    rcv.Init(argv[1]);
    rcv.Start();

    return 0;
}