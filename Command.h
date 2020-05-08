//
// Created by ch on 20-4-22.
// This file defines some command or info structure.
// definition of range info is not in this file.
//

#ifndef UWB_COMMAND_H
#define UWB_COMMAND_H

///define data type
typedef	__signed char			rcm_int8_t;
typedef	unsigned char			rcm_uint8_t;
typedef	short					rcm_int16_t;
typedef	unsigned short			rcm_uint16_t;
typedef	int						rcm_int32_t;
typedef	unsigned int			rcm_uint32_t;
typedef	long long				rcm_int64_t;
typedef	unsigned long long		rcm_uint64_t;

///command type
#define RCM_ECHOED_RANGE_INFO        (0x0204)

#define LOC_SET_CONFIG_REQUEST       (0x5001)
#define LOC_SET_CONFIG_CONFIRM       (0x5101)
#define LOC_GET_CONFIG_REQUEST       (0x5002)
#define LOC_GET_CONFIG_CONFIRM       (0x5102)
#define LOC_SET_MODE_REQUEST         (0x5003)
#define LOC_SET_MODE_CONFIRM         (0x5103)
#define LOC_GET_MODE_REQUEST         (0x5004)
#define LOC_GET_MODE_CONFIRM         (0x5104)
#define LOC_ECHOED_LOCATION_INFO     (0x5202)
#define LOC_ECHOED_LOCATION_EX_INFO  (0x5203)
#define LOC_SET_LOCATION_MAP_REQUEST (0x5005)
#define LOC_SET_LOCATION_MAP_CONFIRM (0x5105)
#define LOC_GET_LOCATION_MAP_REQUEST (0x5006)
#define LOC_GET_LOCATION_MAP_CONFIRM (0x5106)
#define LOC_LOCATION_INFO            (0x5201)

///others
#define RCM_MAX_SCAN_SAMPLES	    (350)

typedef union
{
    rcmMsg_FullRangeInfo rangeInfo;
    rcmMsg_DataInfo dataInfo;
    rcmMsg_ScanInfo scanInfo;
    rcmMsg_FullScanInfo fullScanInfo;
} infoMsgs_t;
/***************  commands  ***************/
namespace UWBcommands{

    ///location part

    struct InfoELR{

        rcm_uint16_t msgType = RCM_ECHOED_RANGE_INFO;

        rcm_uint16_t msgId;

        rcm_uint32_t requesterID;

        rcm_uint32_t responderID;

        rcm_uint32_t PRM;

        rcm_uint16_t PRMErrorEstimate;

        rcm_uint16_t LEDFlag;

        rcm_uint32_t timeStamp;
    };

    struct RequestLocationSetConfig{

        rcm_uint16_t msgType = LOC_SET_CONFIG_REQUEST;

        rcm_uint16_t msgId;

        rcm_uint16_t flag;

        rcm_uint8_t bootMode;

        rcm_uint8_t reserve4 = 0;

        rcm_uint16_t reserve5 = 0;

        rcm_uint16_t solverMaxREE = 100;

        rcm_uint16_t solverMaxGDOP = 400;

        rcm_uint8_t GDOPAnchorHistoryDepth = 4;

        rcm_uint8_t solverNLSToKalmanUpdates = 4;

        rcm_uint16_t kalmanSigmaAccel = 0;

        rcm_uint8_t NLSSolverOutputBoxcarFilterDepth = 4;

        rcm_uint8_t reserve12 = 0;

        rcm_uint8_t persistFlag = 0;

        rcm_uint8_t reserve14 = 0;

        rcm_uint16_t reserve15 = 0;

    };

    struct ConfirmLocationSetConfig{

        rcm_uint16_t msgType = LOC_SET_CONFIG_CONFIRM;

        rcm_uint16_t msgId;

        rcm_uint32_t status;
    };

    struct RequestLocationGetConfig{

        rcm_uint16_t msgType = LOC_GET_CONFIG_REQUEST;

        rcm_uint16_t msgId;

    };

    struct ConfirmLocationGetConfig{

        rcm_uint16_t msgType = LOC_GET_CONFIG_CONFIRM;

        rcm_uint16_t msgId;

        rcm_uint16_t flag;

        rcm_uint8_t bootMode;

        rcm_uint8_t reserve4 = 0;

        rcm_uint16_t reserve5 = 0;

        rcm_uint16_t solverMaxREE = 100;

        rcm_uint16_t solverMaxGDOP = 400;

        rcm_uint8_t GDOPAnchorHistoryDepth = 4;

        rcm_uint8_t solverNLSToKalmanUpdates = 4;

        rcm_uint16_t kalmanSigmaAccel = 0;

        rcm_uint8_t NLSSolverOutputBoxcarFilterDepth = 4;

        rcm_uint8_t reserve12 = 0;

        rcm_uint32_t timeStamp;

        rcm_uint32_t status;

    };


    struct RequestLocationSetMode{

        rcm_uint16_t msgType = LOC_SET_MODE_REQUEST;

        rcm_uint16_t msgId;

        rcm_uint8_t mode;

        rcm_uint8_t broadcastFlag;

        rcm_uint16_t reserved5 = 0;
    };

    struct ConfirmLocationSetMode{

        rcm_uint16_t msgType = LOC_SET_MODE_CONFIRM;

        rcm_uint16_t msgId;

        rcm_uint8_t mode;

        rcm_uint8_t reserved3 = 0;

        rcm_uint16_t reserved4 = 0;

        rcm_uint32_t status;
    };

    struct RequestLocationGetMode{

        rcm_uint16_t msgType = LOC_GET_MODE_REQUEST;

        rcm_uint16_t msgId;
    };

    struct ConfirmLocationGetMode{

        rcm_uint16_t msgType = LOC_GET_MODE_CONFIRM;

        rcm_uint16_t msgId;

        rcm_uint8_t mode;

        rcm_uint8_t reserved3 = 0;

        rcm_uint16_t reserved4 = 0;
    };

    struct LocationMapEntries{

        rcm_uint32_t nodeID;

        rcm_uint8_t nodeType;

        rcm_uint8_t reserved2 = 0;

        rcm_uint8_t flags;

        rcm_uint8_t reserved4 = 0;

        rcm_uint16_t reserverd5 = 0;

        rcm_uint16_t beaconInterval;

        rcm_int32_t x;

        rcm_int32_t y;

        rcm_int32_t z;

    };

    struct RequestSetLocationMap{

        rcm_uint16_t msgType = LOC_SET_LOCATION_MAP_REQUEST;

        rcm_uint16_t messageId;

        rcm_uint8_t broadcastFlag;

        rcm_uint8_t numberOfEntries;

        rcm_uint8_t persistFlag;

    };

    struct ConfirmSetLocationMap{

        rcm_uint16_t msgType = LOC_GET_LOCATION_MAP_CONFIRM;

        rcm_uint16_t messageID;

        rcm_uint32_t status;

    };

    struct RequestGetLocationMap{

        rcm_uint16_t msgType = LOC_GET_LOCATION_MAP_REQUEST;

        rcm_uint16_t messageId;

    };

    struct ConfirmGetLocationMap{

        rcm_uint16_t msgType = LOC_GET_LOCATION_MAP_CONFIRM;

        rcm_uint16_t messageId;

        rcm_uint8_t numberLocations;

        rcm_uint8_t reserverd3 = 0;

        rcm_uint16_t reserved4 = 0;

        rcm_uint32_t status;

    };

    struct InfoLocationEchoed{

        rcm_uint16_t msgType = LOC_ECHOED_LOCATION_INFO;

        rcm_uint16_t msgId;

        rcm_uint32_t nodeID;

        rcm_uint32_t remoteTimestamp;

        rcm_int32_t x;

        rcm_int32_t y;

        rcm_int32_t z;

        rcm_uint16_t GDOP;

        rcm_uint8_t solverStage;

        rcm_uint8_t solverErrorCode;
    };

    struct InfoLocation{

        rcm_uint16_t msgType = LOC_LOCATION_INFO;

        rcm_uint16_t msgId;

        rcm_uint32_t nodeID;

        rcm_uint8_t nodeTYpe;

        rcm_uint8_t solverStage;

        rcm_uint8_t solverErrorCode;

        rcm_uint8_t reserved6;

        rcm_uint16_t reserved7;

        rcm_uint16_t GDOP;

        rcm_uint32_t timeStamp;

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

    struct InfoLocationEchoedEx{

        rcm_uint16_t msgType = LOC_ECHOED_LOCATION_EX_INFO;

        rcm_uint16_t msgId;

        rcm_uint32_t nodeID;

        rcm_uint8_t nodeTYpe;

        rcm_uint8_t solverStage;

        rcm_uint8_t solverErrorCode;

        rcm_uint8_t reserved6;

        rcm_uint16_t reserved7;

        rcm_uint16_t GDOP;

        rcm_uint32_t timeStamp;

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

    ///RCM part
    struct RequestSetOpmode {

        RequestSetOpmode():msgType(RCM_SET_OPMODE_REQUEST){}

        rcm_uint16_t msgType;

        rcm_uint16_t msgId;

        //0:ranging
        //4: RangeNet (Networking)
        //6: Location
        rcm_uint32_t Mode;
    };

    struct ComfirmSetOpmode {

        ComfirmSetOpmode():msgType(RCM_SET_OPMODE_CONFIRM){}

        rcm_uint16_t msgType;

        rcm_uint16_t msgId;

        //New Operational Mode
        //0:ranging
        //4: RangeNet (Networking)
        //6: Location
        rcm_uint32_t newMode;

        rcm_uint32_t status;
    };

    struct RequestGetOpmode {

        rcm_uint16_t msgType = RCM_GET_OPMODE_REQUEST;

        rcm_uint16_t msgId;
    };

    struct ComfirmGetOpmode {

       rcm_uint16_t msgType = RCM_GET_OPMODE_CONFIRM;

        rcm_uint16_t msgId;

        //0:ranging
        //4: RangeNet (Networking)
        //6: Location
        rcm_uint32_t Mode;
    };

    struct InfoEchoedRange {
        // set to RCM_ECHOED_RANGE_INFO
        rcm_uint16_t msgType = RCM_ECHOED_RANGE_INFO;
        // identifier to correlate range requests with info messages
        rcm_uint16_t msgId;

        // ID of the requesting radio
        rcm_uint32_t requesterId;

        // ID of the responding radio
        rcm_uint32_t responderId;

        // Reported range in millimeters
        rcm_uint32_t precisionRangeMm;

        // Reported estimated range error in millimeters
        rcm_uint16_t precisionRangeErrEst;

        // Requester and responder LED flags, or'd together.
        rcm_uint16_t ledFlags;

        // Milliseconds since the requesting radio boot at the time of the range conversation.
        // Used to identify duplicate ELR reports.
        rcm_uint32_t timestamp;
    };

    struct InfoSmallRange {
        // set to RCM_SMALL_RANGE_INFO
        rcm_uint16_t msgType = RCM_SMALL_RANGE_INFO;
        // identifier to correlate requests with confirms
        rcm_uint16_t msgId;

        rcm_uint32_t responderId;

        rcm_uint16_t rangeCm;
        rcm_uint8_t rangeErrEst;

        rcm_uint8_t rangeType;

        rcm_uint8_t rangeStatus;

        rcm_uint8_t flags;
    };

    struct InfoRCMScan {
        // set to RCM_SCAN_INFO
        rcm_uint16_t msgType = RCM_SCAN_INFO;
        // identifier to correlate range requests with info messages
        rcm_uint16_t msgId;

        // ID of the transmitting radio
        rcm_uint32_t sourceId;

        // Antenna the UWB packet was received on
        rcm_uint8_t antennaId;

        rcm_uint8_t reserved;    // alignment

        rcm_uint16_t LEDflags;

        // channel quality (LED rise time; lower (faster rise) is better)
        rcm_uint16_t noise;
        // Peak voltage, normalized.
        rcm_uint16_t vPeak;

        // milliseconds since radio boot at the time the data packet was received
        rcm_uint32_t timestamp;

        rcm_int32_t ledIndex;
        rcm_int32_t lockspotOffset;

        // Number of samples provided in the scan
        rcm_uint32_t numSamples;
        // Scan samples. Note that only the first numSamples samples are sent in the UDP packet.
        rcm_int32_t samples[RCM_MAX_SCAN_SAMPLES];
    };
}
#endif //UWB_COMMAND_H
