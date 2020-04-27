//
// Created by ch on 20-4-22.
//

#include "BIRL_UWB.h"

bool BIRL_UWB::setRangingMode() {
    static UWBcommands::RequestSetOpmode request;
    static UWBcommands::ComfirmSetOpmode confirm;
    bool isOK = false;
    int numBytes;

    request.msgType = htons(request.msgType);
    request.msgId = htons(messageId++);
    request.Mode = htonl(0);

    usb_->rcmIfFlush();

    int ret = usb_->rcmIfSendPacket(&request, sizeof(request));
    numBytes = usb_->rcmIfGetPacket(&confirm, sizeof(confirm));

    if (numBytes == sizeof(confirm))
    {
        // Handle byte ordering
        confirm.msgType = ntohs(confirm.msgType);
        confirm.status = ntohl(confirm.status);

        // is this the correct message type and is status good?
        if (confirm.msgType == RCM_SET_OPMODE_CONFIRM &&
            confirm.status == OK)
            isOK = true;
    }
    return isOK;
}

bool BIRL_UWB::setLocationIdleMode() {
    return setLocationMode(IDLE);
}

bool BIRL_UWB::setLocationTrackingMode(){
    return setLocationMode(TRACKING);
}

bool BIRL_UWB::setLocationMode(locationModeType modeType) {
    static UWBcommands::RequestLocationSetMode request;
    static UWBcommands::ConfirmLocationSetMode confirm;
    bool isOK = false;
    int numBytes;

    request.msgType = htons(request.msgType);
    request.msgId = htons(messageId++);
    request.mode = modeType;
    //@todo:0 means that only this node is modified. If you want to broadcast to other nodes, change to 1.
    //@todo: When you change it to 1, you should also think about another confirm package(LOC_OTA_MODE_CHANGE_INFO)
    request.broadcastFlag = 0;


    usb_->rcmIfFlush();

    usb_->rcmIfSendPacket(&request, sizeof(request));
    numBytes = usb_->rcmIfGetPacket(&confirm, sizeof(confirm));

    if (numBytes == sizeof(confirm))
    {
        // Handle byte ordering
        confirm.msgType = ntohs(confirm.msgType);
        confirm.status = ntohl(confirm.status);

        // is this the correct message type and is status good?
        if (confirm.msgType == LOC_SET_MODE_CONFIRM &&
            confirm.status == OK)
            isOK = true;
    }
    return isOK;
}

bool BIRL_UWB::getTrackingInfo(LocationInfo &info) {

//    //clear pending message
//    /*@todo:I don't know whether it would clear the latest location info and just wait for next info ,
//      @todo:it might cause some unexceptable results if call this function here
//    */
//    rcmIfFlush();

    static UWBcommands::InfoLocationEchoed echoed_info;
    bool isOK = false;

    //get echoed info
    int numBytes = usb_->rcmIfGetPacket(&echoed_info, sizeof(echoed_info));

    //check if what i get is what i want to get
    if (numBytes == sizeof(info))
    {
        // Handle byte ordering
        echoed_info.msgType = ntohs(echoed_info.msgType);
        echoed_info.remoteTimestamp = ntohl(echoed_info.remoteTimestamp);
        echoed_info.x = ntohl(echoed_info.x);
        echoed_info.y = ntohl(echoed_info.y);
        echoed_info.z = ntohl(echoed_info.z);
        echoed_info.nodeID = ntohl(echoed_info.nodeID);

        // is this the correct message type and is status good?
        bool t1 = echoed_info.msgType == LOC_ECHOED_LOCATION_INFO;
        bool t2 = echoed_info.solverErrorCode == OK;
        if (echoed_info.msgType == LOC_ECHOED_LOCATION_INFO &&
            echoed_info.solverErrorCode == OK){
            isOK = true;
            info.x = echoed_info.x;
            info.y = echoed_info.y;
            info.z = echoed_info.z;
            info.node_ID = echoed_info.nodeID;
            info.time_stamp = echoed_info.remoteTimestamp;
        }

    }
    return isOK;
}

bool BIRL_UWB::getTrackingInfoEX(LocationInfoEX &info) {

//    //clear pending message
//    /*@todo:I don't know whether it would clear the latest location info and just wait for next info ,
//      @todo:it might cause some unexceptable results if call this function here
//    */
//    rcmIfFlush();

    static UWBcommands::InfoLocationEchoedEx echoed_info;
    bool isOK = false;

    //get echoed info
    int numBytes = usb_->rcmIfGetPacket(&echoed_info, sizeof(echoed_info));

    //check if what i get is what i want to get
    if (numBytes == sizeof(info))
    {
        // Handle byte ordering
        echoed_info.msgType = ntohs(echoed_info.msgType);
        echoed_info.timeStamp = ntohl(echoed_info.timeStamp);
        echoed_info.x = ntohl(echoed_info.x);
        echoed_info.y = ntohl(echoed_info.y);
        echoed_info.z = ntohl(echoed_info.z);
        echoed_info.x_variance = ntohs(echoed_info.x_variance);
        echoed_info.y_variance = ntohs(echoed_info.y_variance);
        echoed_info.z_variance = ntohs(echoed_info.z_variance);
        echoed_info.x_y_cov = ntohs(echoed_info.x_y_cov);
        echoed_info.x_z_cov = ntohs(echoed_info.x_z_cov);
        echoed_info.y_z_cov = ntohs(echoed_info.y_z_cov);
        echoed_info.nodeID = ntohl(echoed_info.nodeID);

        // is this the correct message type and is status good?
        if (echoed_info.msgType == LOC_ECHOED_LOCATION_EX_INFO &&
            echoed_info.solverErrorCode == OK){
            isOK = true;
            info.x = echoed_info.x;
            info.y = echoed_info.y;
            info.z = echoed_info.z;
            info.x_variance = echoed_info.x_variance;
            info.y_variance = echoed_info.y_variance;
            info.z_variance = echoed_info.z_variance;
            info.x_y_cov = echoed_info.x_y_cov;
            info.x_z_cov = echoed_info.x_z_cov;
            info.y_z_cov = echoed_info.y_z_cov;
            info.time_stamp = echoed_info.timeStamp;
            info.node_ID = echoed_info.nodeID;
            isOK = true;
        }

    }
    return isOK;
}

int BIRL_UWB::getRange(unsigned id) {
    static rcmMsg_SendRangeRequest request;
    static rcmMsg_SendRangeRequestConfirm confirm;
    static rcmMsg_FullRangeInfo rangeInfo;
    static rcmMsg_DataInfo dataInfo;
    static rcmMsg_ScanInfo scanInfo;
    static rcmMsg_FullScanInfo fullScanInfo;
    static infoMsgs_t infoMsgs;
    int retVal = ERR, numBytes;
    unsigned i;

    // create request message
    request.msgType = htons(RCM_SEND_RANGE_REQUEST);
    request.msgId = htons(messageId);
    request.responderId = htonl(id);
    request.antennaMode = RCM_ANTENNAMODE_TXA_RXA; //@todo: default setting, you should modify it according to the hardware
    request.dataSize = htons(0);

    // make sure no pending messages
    usb_->rcmIfFlush();

    // send message to RCM
    numBytes = sizeof(request);
    usb_->rcmIfSendPacket(&request, numBytes);

    // wait for response
    numBytes = usb_->rcmIfGetPacket(&confirm, sizeof(confirm));

    if (numBytes == sizeof(confirm))
    {
        // Handle byte ordering
        confirm.msgType = ntohs(confirm.msgType);
        confirm.msgId = ntohs(confirm.msgId);

        // is this the correct message type?
        if (confirm.msgType == RCM_SEND_RANGE_REQUEST_CONFIRM)
        {
            // check status code
            confirm.status = ntohl(confirm.status);
            if (confirm.status == OK)
            {
                retVal = OK;

                // clear out caller's info structs
                rangeInfo.rangeStatus = RCM_RANGE_STATUS_TIMEOUT;

                // Collect any info messages
                // We will always get a rangeInfo, maybe a dataInfo and/or scanInfo also
                while ((numBytes = usb_->rcmIfGetPacket(&infoMsgs, sizeof(infoMsgs))) > 0)
                {
                    // make sure this info message has the same msgId as the request
                    // the msgId is in the same place in all structs
                    if (ntohs(infoMsgs.rangeInfo.msgId) == messageId)
                    {
                        switch(ntohs(infoMsgs.rangeInfo.msgType))
                        {
                            case RCM_FULL_RANGE_INFO:
                                // copy message to caller's struct
                                memcpy(&rangeInfo, &infoMsgs.rangeInfo, sizeof(rangeInfo));
                                // handle byte ordering
                                rangeInfo.msgType = ntohs(rangeInfo.msgType);
                                rangeInfo.msgId = ntohs(rangeInfo.msgId);
                                rangeInfo.responderId = ntohl(rangeInfo.responderId);
                                rangeInfo.stopwatchTime = ntohs(rangeInfo.stopwatchTime);
                                rangeInfo.precisionRangeMm = ntohl(rangeInfo.precisionRangeMm);
                                rangeInfo.coarseRangeMm = ntohl(rangeInfo.coarseRangeMm);
                                rangeInfo.filteredRangeMm = ntohl(rangeInfo.filteredRangeMm);
                                rangeInfo.precisionRangeErrEst = ntohs(rangeInfo.precisionRangeErrEst);
                                rangeInfo.coarseRangeErrEst = ntohs(rangeInfo.coarseRangeErrEst);
                                rangeInfo.filteredRangeErrEst = ntohs(rangeInfo.filteredRangeErrEst);
                                rangeInfo.filteredRangeVel = ntohs(rangeInfo.filteredRangeVel);
                                rangeInfo.filteredRangeVelErrEst = ntohs(rangeInfo.filteredRangeVelErrEst);
                                rangeInfo.reqLEDFlags = ntohs(rangeInfo.reqLEDFlags);
                                rangeInfo.respLEDFlags = ntohs(rangeInfo.respLEDFlags);
                                rangeInfo.noise = ntohs(rangeInfo.noise);
                                rangeInfo.vPeak = ntohs(rangeInfo.vPeak);
                                rangeInfo.coarseTOFInBins = ntohl(rangeInfo.coarseTOFInBins);
                                rangeInfo.timestamp = ntohl(rangeInfo.timestamp);
                                break;
                            case RCM_DATA_INFO:
                                // copy message to caller's struct
                                memcpy(&dataInfo, &infoMsgs.dataInfo, sizeof(dataInfo));
                                // handle byte ordering
                                dataInfo.msgType = ntohs(dataInfo.msgType);
                                dataInfo.msgId = ntohs(dataInfo.msgId);
                                dataInfo.sourceId = ntohl(dataInfo.sourceId);
                                dataInfo.noise = ntohs(dataInfo.noise);
                                dataInfo.vPeak = ntohs(dataInfo.vPeak);
                                dataInfo.timestamp = ntohl(dataInfo.timestamp);
                                dataInfo.dataSize = ntohs(dataInfo.dataSize);
                                break;
                            case RCM_SCAN_INFO:
                                // copy message to caller's struct
                                memcpy(&scanInfo, &infoMsgs.scanInfo, sizeof(scanInfo));
                                // handle byte ordering
                                scanInfo.msgType = ntohs(scanInfo.msgType);
                                scanInfo.msgId = ntohs(scanInfo.msgId);
                                scanInfo.sourceId = ntohl(scanInfo.sourceId);
                                scanInfo.LEDflags = ntohs(scanInfo.LEDflags);
                                scanInfo.noise = ntohs(scanInfo.noise);
                                scanInfo.vPeak = ntohs(scanInfo.vPeak);
                                scanInfo.timestamp = ntohl(scanInfo.timestamp);
                                scanInfo.ledIndex = ntohl(scanInfo.ledIndex);
                                scanInfo.lockspotOffset = ntohl(scanInfo.lockspotOffset);
                                scanInfo.numSamples = ntohl(scanInfo.numSamples);
                                for (i = 0; i < scanInfo.numSamples; i++)
                                    scanInfo.samples[i] = ntohl(scanInfo.samples[i]);
                                break;
                            case RCM_FULL_SCAN_INFO:
                                //
                                // NOTE: this only returns the last RCM_FULL_SCAN_INFO packet
                                //
                                // copy message to caller's struct
                                memcpy(&fullScanInfo, &infoMsgs.fullScanInfo, sizeof(fullScanInfo));
                                // handle byte ordering
                                fullScanInfo.msgType = ntohs(fullScanInfo.msgType);
                                fullScanInfo.msgId = ntohs(fullScanInfo.msgId);
                                fullScanInfo.sourceId = ntohl(fullScanInfo.sourceId);
                                fullScanInfo.timestamp = ntohl(fullScanInfo.timestamp);
                                fullScanInfo.noise = ntohs(fullScanInfo.noise);
                                fullScanInfo.vPeak = ntohs(fullScanInfo.vPeak);
                                fullScanInfo.ledIndex = ntohl(fullScanInfo.ledIndex);
                                fullScanInfo.lockspotOffset = ntohl(fullScanInfo.lockspotOffset);
                                fullScanInfo.scanStartPs = ntohl(fullScanInfo.scanStartPs);
                                fullScanInfo.scanStopPs = ntohl(fullScanInfo.scanStopPs);
                                fullScanInfo.scanStepBins = ntohs(fullScanInfo.scanStepBins);
                                fullScanInfo.numSamplesInMessage = ntohs(fullScanInfo.numSamplesInMessage);
                                fullScanInfo.numSamplesTotal = ntohl(fullScanInfo.numSamplesTotal);
                                fullScanInfo.messageIndex = ntohs(fullScanInfo.messageIndex);
                                fullScanInfo.numMessagesTotal = ntohs(fullScanInfo.numMessagesTotal);
                                for (i = 0; i < fullScanInfo.numSamplesInMessage; i++)
                                    fullScanInfo.scan[i] = ntohl(fullScanInfo.scan[i]);
                                break;
                        }

                        // We get RANGE_INFO last.
                        if(ntohs(infoMsgs.rangeInfo.msgType) == RCM_FULL_RANGE_INFO)
                            break;
                    }
                }
                return rangeInfo.precisionRangeMm;
            }
        }
    }
    messageId++;
    return -1;
}