//
// Created by ch on 20-4-23.
//
#include "BIRL_UWB.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
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
    //location of anchor(location map is optional)
    std::vector<int> x;
    std::vector<int> y;
    std::vector<int> z;
#ifdef DEBUG
    char dev[] = "/dev/ttyACM0";

    std::shared_ptr<BIRL_UWB> uwb(new BIRL_UWB(dev));

    uwb->Open(dev);

    ///location map
    std::ifstream in("map.csv");
    if(!in.good()) std::cout << "Fail to open the csv file." << "\n" << "\n";
    else std::cout << "Success to open the csv file." << "\n" << "\n";
    std::string line, word;
    bool isFirstLine = true;
    while(std::getline(in, line)){
        if(isFirstLine){
            isFirstLine = false;
            continue;
        }
        std::string::size_type pos_l = 0, pos_r = line.size();
        for(int count = 0; count < 6; count ++){
            while(line[pos_l++] != ',');
        }
        pos_r = pos_l;
        while(line[pos_r++] != ',');
        x.push_back(std::stoi(line.substr(pos_l, pos_r-pos_l - 1)));
        pos_l = pos_r;
        while(line[pos_r++] != ',');
        y.push_back(std::stoi(line.substr(pos_l, pos_r-pos_l - 1)));
        pos_l = pos_r;
        while(line[pos_r++] != ',');
        z.push_back(std::stoi(line.substr(pos_l, pos_r-pos_l - 1)));
    }
#else
    // check command line arguments
    if (argc < 3){
        usage();
    }
    std::shared_ptr<BIRL_UWB> uwb = BIRL_UWB::CreateUWB();
    uwb->Open(argv[2]);
        if(argc == 2){
        std::ifstream in(argv[1]);
        std::string line, word;
        while(std::getline(in, line)){
            std::string::size_type pos_l = 0, pos_r = line.size();
            for(int count = 0; count < 5; count ++){
                pos_l = line.find(",", pos_l, pos_r - pos_l);
            }
            pos_r = line.find(",", pos_l, line.size() - pos_l);
            x.push_back(std::stoi(line.substr(pos_l + 1, pos_r-pos_l - 1)));
            pos_l = pos_r;
            y.push_back(std::stoi(line.substr(pos_l + 1, pos_r-pos_l - 1)));
            pos_l = pos_r;
            y.push_back(std::stoi(line.substr(pos_l + 1, pos_r-pos_l - 1)));
            }
        }
    }
#endif
    if(!uwb->IsAvailable()){
        std::cout << "Fail to open p440!" << std::endl;
        exit(0);
    }
    std::cout << "P440 is opened.\n" << std::endl;
    printf("RCM Sample App\n\n");


    static LocationInfo location_data;
    static LocationConfigInfo location_config_info;
    static std::vector<UWBcommands::LocationMapEntries> location_map_config;
    int command;
    while(command!=-1){

        std::cout << "What to do with P440?:\n";
        std::cout << "1.get range.\n";
        std::cout << "2.get location.\n";
        std::cout << "3.set ranging mode.\n";
        std::cout << "4.set idle mode.\n";
        std::cout << "5.set tracking mode.\n";
        std::cout << "6.get location config.\n";
        std::cout << "7.get location map config.\n";
        std::cout << "8.set location map config(from the csv file).\n";
        std::cout << "9.set location mode.\n";
        std::cout << "10.get location mode.\n";
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
                if(!uwb->setMode(BIRL_UWB::RANGING)){
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
            case 7:{
                if(!uwb->getLocationMap(location_map_config)){
                    std::cout << "Fail to get location map config.\n";
                }else{
                    for(const auto& e : location_map_config){
                        std::cout << "Node ID: " << e.nodeID << "\n";
                        std::cout << "Node type: " << long(e.nodeType) << "\n";
                        std::cout << "Flag: " << long(e.flags) << "\n";
                        std::cout << "Beacon interval: " << e.beaconInterval << "\n";
                        std::cout << "X: " << e.x << "\n";
                        std::cout << "Y: " << e.y << "\n";
                        std::cout << "Z: " << e.z << "\n" << "\n";
                    }
                }
                break;
            }
            case 8:{
                if(!uwb->setLocationMap(x, y, z)){
                    std::cout << "Fail to set location map config.\n";
                }else std::cout << "Set location mao config OK.\n";
                break;
            }
            case 9:{
                if( (!uwb->setMode(BIRL_UWB::LOCATION)) && (!uwb->setLocationConfig())){
                    std::cout << "Fail to set location mode.\n";
                }else std::cout << "Set location mode OK.\n";
                break;
            }
            case 10:{
                int mode = uwb->getLocationMode();
                if( mode > -1){
                    std::cout << "Location mode is : " << mode << "\n";
                }else std::cout << "Fail to get location mode.\n";
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