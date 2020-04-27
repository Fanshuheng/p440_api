//
// Created by ch on 20-4-22.
//

#ifndef USB_H
#define USB_H

#include <memory>

#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#include "hostInterfaceCommon.h"
#include "hostInterfaceRCM.h"

class USB {
public:
    ///data type
    const int OK = 0, ERR = -1, DEFAULT_TIMEOUT_MS = 500;

    ///interface
    static std::shared_ptr<USB> CreateUSB(){
        if(instance_ == nullptr)
            instance_ = std::shared_ptr<USB>(new USB());
        return instance_;
    }

    ~USB(){
        if(radioFd != 0)rcmIfClose();
    }

    bool IsAvailable(){ return isAvailable_; }  //true:the node is opened; false:the node is not opened

    bool Open(char* dev){
        return rcmIfInit(dev)==OK;
    }

    int rcmIfInit(char* destAddr);

    void rcmIfClose(void);

    int rcmIfSendPacket(void *pkt, unsigned size);

    int rcmIfGetPacket(void *pkt, unsigned maxSize);

    int rcmIfGetPacketSerial(void *pkt, unsigned maxSize);

    int rcmIfSendPacketSerial(void *pkt, unsigned size);

    void rcmIfFlush(void);

    int serTimedRead(void *buf, int cnt);

    int serWrite(void *buf, int cnt);

    unsigned short crc16(void *ptr, int len);
private:
    USB() = default;

    int radioFd{};

    int timeoutMs = DEFAULT_TIMEOUT_MS;

    bool isAvailable_ = false;  //If this node is available(init successfully)

    static std::shared_ptr<USB> instance_;  //for singleton pattern

    static const unsigned short crc16_tab[32*8];
};

#endif //USB_H