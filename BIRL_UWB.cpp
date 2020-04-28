//
// Created by ch on 20-4-22.
//

#include "BIRL_UWB.h"

std::shared_ptr<BIRL_UWB> BIRL_UWB::instance_ = nullptr;

const unsigned short BIRL_UWB::crc16_tab[32*8] = {
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
        0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
        0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
        0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
        0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
        0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
        0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
        0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
        0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
        0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
        0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
        0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
        0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
        0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
        0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
        0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
        0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
        0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
        0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
        0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
        0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
        0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
        0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
        0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
        0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
        0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
        0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
        0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
        0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
        0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};


int BIRL_UWB::rcmIfInit(char* destAddr){
    unsigned radioIpAddr;
    struct termios term;
    radioFd = open(destAddr, O_RDWR|O_NOCTTY|O_NONBLOCK);
    if (radioFd < 0)
    {
        printf("Can't open serial port\n");
        return ERR;
    }
    tcgetattr(radioFd, &term);
    memset(&term, 0, sizeof(term));
    cfmakeraw(&term);
    term.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    term.c_iflag = IGNPAR;
    tcflush(radioFd, TCIFLUSH);
    tcsetattr(radioFd, TCSANOW, &term);
    isAvailable_ = true;
    return OK;
}

void BIRL_UWB::rcmIfClose(void)
{
    close(radioFd);
    isAvailable_ = false;
}

int BIRL_UWB::rcmIfGetPacket(void *pkt, unsigned maxSize)
{
    return rcmIfGetPacketSerial(pkt, maxSize);
}

int BIRL_UWB::rcmIfSendPacket(void *pkt, unsigned size)
{

    return rcmIfSendPacketSerial(pkt, size);
}

int BIRL_UWB::rcmIfGetPacketSerial(void *pkt, unsigned maxSize)
{
    int c=0, i, crc;
    unsigned short val;

    while (1)
    {
        // read first sync byte
        if (serTimedRead(&c, 1) <= 0)
            return ERR;
        if (c != 0xa5)
            continue;

        // read second sync byte
        if (serTimedRead(&c, 1) <= 0)
            return ERR;
        if (c != 0xa5)
            continue;

        break;
    }

    // read size
    if (serTimedRead(&val, sizeof(val)) <= 0)
        return ERR;
    val = ntohs(val);

    // read packet
    if (val > maxSize)
        val = maxSize;
    i = serTimedRead((char *)pkt, val);

    return i;
}

int BIRL_UWB::rcmIfSendPacketSerial(void *pkt, unsigned size)
{
    int i;
    unsigned short val;

    // send sync bytes
    i = serWrite((void *) "\xa5\xa5", 2);
    if (i < 0)
        return ERR;

    // send size bytes
    val = htons(size);
    i = serWrite(&val, sizeof(val));
    if (i < 0)
        return ERR;

    // send packet
    i = serWrite(pkt, size);
    if (i < 0)
        return ERR;

    // send CRC for serial port
    val = crc16(pkt, size);
    val = htons(val);
    if (serWrite(&val, sizeof(val)) < 0)
        return ERR;
    return OK;
}

void BIRL_UWB::rcmIfFlush(void)
{
    int tmp, i = timeoutMs;

    timeoutMs = 0;
    while (rcmIfGetPacket(&tmp, sizeof(tmp)) > 0)
        ;
    timeoutMs = i;
}

int BIRL_UWB::serTimedRead(void *buf, int cnt)
{
    fd_set fds;
    struct timeval tv;
    int total = 0, i;
    char *ptr = (char *)buf;

    while (cnt)
    {
        // basic select call setup
        FD_ZERO(&fds);
        FD_SET(radioFd, &fds);

        // Set up timeout
        tv.tv_sec = timeoutMs / 1000;
        tv.tv_usec = (timeoutMs * 1000) % 1000000;

        if (select(radioFd + 1, &fds, NULL, NULL, &tv) > 0)
        {
            i = read(radioFd, ptr, cnt);
            if (i > 0)
            {
                cnt -= i;
                ptr += i;
                total += i;
            }
        }
        else
        {
            total = -1;
            break;
        }
    }

    return total;
}

int BIRL_UWB::serWrite(void *buf, int cnt)
{
    return write(radioFd, buf, cnt);
}

unsigned short BIRL_UWB::crc16(void *ptr, int len)
{
    int i;
    unsigned short cksum;
    unsigned char *buf = (unsigned char *)ptr;

    cksum = 0;
    for (i = 0;  i < len;  i++) {
        cksum = crc16_tab[((cksum>>8) ^ *buf++) & 0xFF] ^ (cksum << 8);
    }
    return cksum;
}

bool BIRL_UWB::setRangingMode() {
    static UWBcommands::RequestSetOpmode request;
    static UWBcommands::ComfirmSetOpmode confirm;
    bool isOK = false;
    int numBytes;

    request.msgType = htons(request.msgType);
    request.msgId = htons(messageId++);
    request.Mode = htonl(0);

    rcmIfFlush();

    int ret = rcmIfSendPacket(&request, sizeof(request));
    numBytes = rcmIfGetPacket(&confirm, sizeof(confirm));

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


    rcmIfFlush();

    rcmIfSendPacket(&request, sizeof(request));
    numBytes = rcmIfGetPacket(&confirm, sizeof(confirm));

    if (numBytes == sizeof(confirm))
    {
        // Handle byte ordering
        confirm.msgType = ntohs(confirm.msgType);
        confirm.status = ntohl(confirm.status);

        // is this the correct message type and is status good?
        bool t1 = confirm.msgType == LOC_SET_MODE_CONFIRM;
        bool t2 = confirm.status == OK;
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
    int numBytes = rcmIfGetPacket(&echoed_info, sizeof(echoed_info));

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
    int numBytes = rcmIfGetPacket(&echoed_info, sizeof(echoed_info));

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
    rcmIfFlush();

    // send message to RCM
    numBytes = sizeof(request);
    rcmIfSendPacket(&request, numBytes);

    // wait for response
    numBytes = rcmIfGetPacket(&confirm, sizeof(confirm));

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
                while ((numBytes = rcmIfGetPacket(&infoMsgs, sizeof(infoMsgs))) > 0)
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