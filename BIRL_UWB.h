//
// Created by ch on 20-4-22.
//

#ifndef UWB_BIRL_UWB_H
#define UWB_BIRL_UWB_H

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

#include "Command.h"

///necessary localization data
struct LocationInfo{

    rcm_uint32_t node_ID;  //ID of node sending this info

    rcm_uint32_t time_stamp;

    rcm_int32_t x;

    rcm_int32_t y;

    rcm_int32_t z;
};

struct LocationInfoEX{

    rcm_uint32_t node_ID;  //ID of node sending this info

    rcm_uint32_t time_stamp;

    rcm_int32_t x;

    rcm_int32_t y;

    rcm_int32_t z;

    rcm_uint16_t x_variance;

    rcm_uint16_t y_variance;

    rcm_uint16_t z_variance;

    rcm_int16_t x_y_cov;

    rcm_int16_t x_z_cov;

    rcm_int16_t y_z_cov;
};

class BIRL_UWB {
public:
    ///data type
    enum rcmIfType {rcmIfIp, rcmIfSerial, rcmIfUsb};

    enum locationModeType {IDLE = 0, AUTOSURVEY = 1, TRACKING = 2};

    const int OK = 0, ERR = -1, DEFAULT_TIMEOUT_MS = 500;

    unsigned int messageId = 0;

    ///interface
    static std::shared_ptr<BIRL_UWB> CreateUWB(){
        if(instance_ == nullptr)
            instance_ = std::shared_ptr<BIRL_UWB>(new BIRL_UWB());
        return instance_;
    }

    ~BIRL_UWB(){
        if(radioFd != 0)rcmIfClose();
    }

    void SetThisID(int id){this_node_id_ = id;}

    int GetThisID(){return this_node_id_;}

    bool IsAvailable(){ return isAvailable_; }  //true:the node is opened; false:the node is not opened

    bool Open(char* dev){
        return rcmIfInit(dev)==OK;
    }

    /*************************************************
    Function:     setRangingMode
    Description:  将P440设置为测距模式
    Input:  None
    Output: None
    Return: true:设置成功
            false:设置失败
    Others: None
    *************************************************/
    bool setRangingMode();

    /*************************************************
    Function:     setLocationTrackingMode
    Description:  将P440设置为定位-追踪模式
    Input:  None
    Output: None
    Return: true:设置成功
            false:设置失败
    Others: None
    *************************************************/
    bool setLocationTrackingMode();

    /*************************************************
    Function:     setLocationIdleMode
    Description:  将P440设置为定位-空闲模式
    Input:  None
    Output: None
    Return: true:设置成功
            false:设置失败
    Others: None
    *************************************************/
    bool setLocationIdleMode();

    /*************************************************
    Function:     setLocationMode
    Description:  将P440设置为定位模式的Idle、Autosurvey或Tracking中的一种
    Input:  modeType：定位模式三种状态中的一种
    Output: None
    Return: true:设置成功
            false:设置失败
    Others: None
    *************************************************/
    bool setLocationMode(locationModeType modeType);

    /*************************************************
    Function:     getTrackingInfo
    Description:  获取基本定位信息（统计学信息）
    Input:  info：定位信息结构体
    Output: None
    Return: true:获取成功
            false:获取失败
    Others: None
    *************************************************/
    bool getTrackingInfo(LocationInfo& info);

    /*************************************************
    Function:     getTrackingInfoEX
    Description:  获取详细定位信息（带统计学信息）
    Input:  info：定位信息结构体
    Output: None
    Return: true:获取成功
            false:获取失败
    Others: None
    *************************************************/
    bool getTrackingInfoEX(LocationInfoEX& info);

    /*************************************************
    Function:     getTrackingInfoEX
    Description:  获取详细定位信息（带统计学信息）
    Input:  info：定位信息结构体
    Output: None
    Return: true:获取成功
            false:获取失败
    Others: None
    *************************************************/
    bool getRangeInfo(unsigned id);

    /*************************************************
    Function:     getRange
    Description:  get range between this node and target node 'id'
    Input:  id: id of target node
    Output: None
    Return: positive value means success of request for range
            -1 means failure
    Others: None
    *************************************************/
    int getRange(unsigned id);

private:
    BIRL_UWB() = default;

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

    int radioFd{};

    int timeoutMs = DEFAULT_TIMEOUT_MS;

    int this_node_id_;  //ID of this UWB node, -1 for none. Call setThisID to modify it.

    bool isAvailable_ = false;  //If this node is available(init successfully)

    static std::shared_ptr<BIRL_UWB> instance_;  //for singleton pattern

    static const unsigned short crc16_tab[32*8];
};

#endif //UWB_BIRL_UWB_H
