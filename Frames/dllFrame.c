//
// Created by munk on 5/26/18.
//

#include <string.h>
#include <util/crc16.h>
#include "dllFrame.h"
#include "../external_libs/md5.h"

const uint8_t preambleSize = 1;
const uint8_t lengthSize = 2;
const uint8_t versionSize = 1;
const uint8_t hashSize = 16;

uint16_t getTotalSizeOfDllFrame(uint16_t appFrameSize) {
    return (preambleSize + lengthSize + versionSize + appFrameSize + hashSize);
}


/**
 * This function will return the size of the dllframe counted from the version byte of the frame.
 * This is in order for getting that field and not for the total size of the dll frame.
 *
 * @param appFrameSize  The size of the AppFrame
 * @return              The size field of the DLL frame
 */
uint16_t getSizeOfDllFrame(uint16_t appFrameSize) {
    return (versionSize + appFrameSize + hashSize);
}

/**
 * Creating MD5 hash in the dllFrameNoMD5 input, so it should be 16B larger than dllFrameSizeNoMD5
 *
 * @param dllFrameNoMD5     The DLL frame with allocated 16 bytes at the end for MD5 checksum
 * @param dllFrameSizeNoMD5 The size of the total dll frame -16 Bytes
 */
void createHash(uint8_t* dllFrameNoMD5, uint16_t dllFrameSizeNoMD5) {
    // Following md5 usage is inspired from https://stackoverflow.com/a/8389763
    MD5_CTX md5_ctx;
    MD5Init(&md5_ctx);
    MD5Update(&md5_ctx, dllFrameNoMD5, dllFrameSizeNoMD5);
    uint8_t hashLen = 16;
    uint8_t hash[hashLen];
    MD5Final(hash, &md5_ctx);

    memcpy(&dllFrameNoMD5[dllFrameSizeNoMD5], hash, hashLen);

}

void createDllFrame(uint8_t* appFrame, uint8_t* dllFrame, uint16_t appFrameSize) {
    dllFrame[0] = PREAMBLE;    // preamble
    uint16_t frameSize = getSizeOfDllFrame(appFrameSize); // version appFrameSize and md5 hash
    dllFrame[1] = ((uint8_t)((frameSize & 0xFF00) >> 8));
    dllFrame[2] = (uint8_t)(frameSize & 0xFF);
    dllFrame[3] = VERSION;
    memcpy(&dllFrame[4], appFrame, appFrameSize);
    createHash(dllFrame, 4+appFrameSize);
}