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
#include "LocationInfo.h"

#include "Command.h"
#include "usb.h"



class BIRL_UWB {
public:
    ///data type
    enum rcmIfType {rcmIfIp, rcmIfSerial, rcmIfUsb};

    enum locationModeType {IDLE = 0, AUTOSURVEY = 1, TRACKING = 2};

    const int OK = 0, ERR = -1, DEFAULT_TIMEOUT_MS = 500;

    ///interface
    BIRL_UWB(char* dev){
        usb_ = usb_->CreateUSB();
        Open(dev);
    }

    ~BIRL_UWB(){
        if(usb_->IsAvailable())usb_->rcmIfClose();
    }

    void SetThisID(int id){this_node_id_ = id;}

    int GetThisID(){return this_node_id_;}

    bool IsAvailable(){return usb_->IsAvailable();}  //true:the node is opened; false:the node is not opened

    bool Open(char* dev){
        return usb_->rcmIfInit(dev)==OK;
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
    Function:     getLocationConfig
    Description:  获取定位模式的配置
    Input:  None
    Output: None
    Return: true:获取成功
            false:获取失败
    Others: None
    *************************************************/
    bool getLocationConfig(LocationConfigInfo&);

//    /*************************************************
//    Function:     getLocationMapConfig
//    Description:  获取定位地图的配置
//    Input:  None
//    Output: None
//    Return: true:获取成功
//            false:获取失败
//    Others: None
//    *************************************************/
//    bool getLocationMapConfig(LocationConfigInfo&);

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

    int radioFd;

    int timeoutMs = DEFAULT_TIMEOUT_MS;

    int this_node_id_;  //ID of this UWB node, -1 for none. Call setThisID to modify it.

    unsigned int messageId = 0;

    std::shared_ptr<USB> usb_;

};

#endif //UWB_BIRL_UWB_H
