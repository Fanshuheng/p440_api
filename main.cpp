//
// Created by ch on 20-4-23.
//
#include "BIRL_UWB.h"
#include <iostream>
#include <string>
#define DEBUG

void usage(void)
{
    printf("usage: rcmSampleApp -i <IP address> | -s <COM port> | -u <USB COM port>\n");
    printf("\nTo connect to radio at IP address 192.168.1.100 via Ethernet:\n");
    printf("\trcmSampleApp -i 192.168.1.100\n");
    printf("\nTo connect to radio's serial port at /dev/ttyUSB0:\n");
    printf("\trcmSampleApp -s /dev/ttyUSB0\n");
    printf("\nTo connect to radio's USB port at /dev/ttyACM0:\n");
    printf("\trcmSampleApp -u /dev/ttyACM0\n");

    exit(0);
}

int main(int argc, char *argv[]) {
#ifdef DEBUG
    char dev[] = "/dev/ttyACM0";

    std::shared_ptr<BIRL_UWB> uwb(new BIRL_UWB(dev));

    uwb->Open(dev);
#else
    // check command line arguments
    if (argc != 3){
        usage();
    }
    std::shared_ptr<BIRL_UWB> uwb = BIRL_UWB::CreateUWB();
    uwb->Open(argv[2]);
#endif
    if(!uwb->IsAvailable()){
        std::cout << "Fail to open p440!" << std::endl;
        exit(0);
    }
    std::cout << "P440 is opened.\n" << std::endl;
    printf("RCM Sample App\n\n");


    static LocationInfo location_data;
    static LocationConfigInfo location_config_info;
    int command;
    while(command!=-1){

        std::cout << "What to do with P440?:\n";
        std::cout << "1.get range.\n";
        std::cout << "2.get location.\n";
        std::cout << "3.set ranging mode.\n";
        std::cout << "4.set idle mode.\n";
        std::cout << "5.set tracking mode.\n";
        std::cout << "6.get location config.\n";
        std::cout << "-1.quit\n";

        std::cin >> command;
        switch (command){
            case 1:{
                std::cout << "Target id:\n";
                std::cin >> command;
                int ret = uwb->getRange(command);
                if(ret == -1) std::cout << "Fail to get range of node "<< command << "\n";
                else std::cout << "Range between this node and node " << command << " is:" << ret << "\n";
                break;
            }
            case 2:{
                bool ret = uwb->getTrackingInfo(location_data);
                if(!ret) std::cout << "Fail to locate.\n";
                else {
                    std::cout << "x:" << location_data.x<<
                    ", y:" << location_data.y<<
                    ", z:" << location_data.z<< "\n";
                    std::cout << "Time stamp: " << location_data.time_stamp << ", node ID: "<< location_data.node_ID << "\n";

                }
                break;
            }
            case 3:{
                if(!uwb->setRangingMode()){
                    std::cout << "Fail to set range mode.\n";
                }else{
                    std::cout << "Range mode OK.\n";
                }
                break;
            }
            case 4:{
                if(!uwb->setLocationIdleMode()){
                    std::cout << "Fail to set idle mode.\n";
                }else{
                    std::cout << "Idle mode OK.\n";
                }
                break;
            }
            case 5:{
                if(!uwb->setLocationTrackingMode()){
                    std::cout << "Fail to set tracking mode.\n";
                }else{
                    std::cout << "Tracking mode OK.\n";
                }
                break;
            }
            case 6:{
                if(!uwb->getLocationConfig(location_config_info)){
                    std::cout << "Fail to get location mode.\n";
                }else{
                    std::cout << "Status: " << location_config_info.status << "\n";
                    std::cout << "Timestamp: " << location_config_info.timeStamp << "\n";
                    std::cout << "Flag: " << location_config_info.flag << "\n";
                    std::cout << "Boot Mode: " << location_config_info.bootMode << "\n";
                    std::cout << "GDOP Anchor History Depth: " << location_config_info.GDOPAnchorHistoryDepth << "\n";
                }
                break;
            }
            case -1:{
                std::cout << "Quit.";
                break;
            }
            default:{
                std::cout << "Please enter 1~5, or -1 to quit.\n";
            }

        }
    }
    return 0;
}