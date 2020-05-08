//
// Created by ch on 20-5-8.
//

#ifndef P440_API_RANGINGINFO_H
#define P440_API_RANGINGINFO_H

#include <vector>

struct EchoedRangingInfo{

    int id_beacon = 0;

    int id_tag = 0;

    long range = 0;//mm
};

struct EchoedRangingInfos{
    std::vector<EchoedRangingInfo> infos;
};

#endif //P440_API_RANGINGINFO_H
