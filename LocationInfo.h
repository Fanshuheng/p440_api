//
// Created by ch on 20-4-28.
//

#ifndef P440_API_LOCATIONINFO_H
#define P440_API_LOCATIONINFO_H
///necessary localization data

struct LocationConfigInfo{

    rcm_uint16_t flag;

    rcm_uint16_t bootMode;

    rcm_uint16_t solverMaxREE;

    rcm_uint16_t solverGDOP;

    rcm_uint8_t GDOPAnchorHistoryDepth;

    rcm_uint8_t solverNLStoKalmanUpdates;

    rcm_uint16_t kalmanSigmaAccel;

    rcm_uint32_t timeStamp;

    rcm_uint32_t status;

};

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

#endif //P440_API_LOCATIONINFO_H
