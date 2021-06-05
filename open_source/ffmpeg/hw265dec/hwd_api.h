/*
* Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
* Description:
* Author: Huawei
* Create: 2019-7-15
* Notes:
*/
#ifndef HW265DEC_API_H /* Macro sentry to avoid redundant including */
#define HW265DEC_API_H

#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

// Log level
typedef enum TAG_HW265VIDEO_ALG_LOG_LEVEL {
    HW265VIDEO_ALG_LOG_ERROR = 0,  // log for error
    HW265VIDEO_ALG_LOG_WARNING,    // log for waring
    HW265VIDEO_ALG_LOG_INFO,       // log for help
    HW265VIDEO_ALG_LOG_DEBUG       // print debug info, used for developer debug
} HW265VIDEO_ALG_LOG_LEVEL;
typedef void *HWD_Handle;  // Decoder handle

/*
 * Instruction : memory create callback func type
 *
 * Param : channelID - [in] channel ID
 *         size      - [in] memory size
 *
 * Return Value : Success return memory address
 *                Failed return NULL
 */
typedef void *(*HW265D_VIDEO_ALG_MALLOC_FXN)(uint32_t channelID, uint32_t size);

/*
 * Instruction   :  memory free callback func type
 *
 * Param   :  channelID - [in] channel ID
 *            ptr       - [in] memory address
 *
 * Return Value : Null
 */
typedef void (*HW265D_VIDEO_ALG_FREE_FXN)(uint32_t channelID, void *ptr);

/*
 * Instruction   :  log callback func type
 *
 * Param  :  channelID - [in] channel ID
 *           level     - [in] set log level
 *           msg       - [in] log info(string)
 *           ...       - [in] changeable param
 *
 * Return Value : Null
 */
typedef void (*HW265D_VIDEO_ALG_LOG_FXN)(uint32_t channelID, HW265VIDEO_ALG_LOG_LEVEL level, int8_t *msg, ...);

// Supported slice type
typedef enum {
    IH265D_B_SLICE = 0,
    IH265D_P_SLICE = 1,
    IH265D_I_SLICE = 2
} SLICE_TYPE;

typedef enum TAG_HW265D_FRAMETYPE {
    IH265D_FRAME_I = 0,
    IH265D_FRAME_P,
    IH265D_FRAME_B,
    IH265D_FRAME_UNKNOWN
} HW265D_FRAMETYPE;

typedef enum TAG_HW265D_DECODEMODE {
    IH265D_DECODE = 0,  // Safe decode
    IH265D_DECODE_END   // Decode over and decoder output the residual picture
} HW265D_DECODEMODE;

typedef enum TAG_HW265D_DECODESTATUS {
    IH265D_GETDISPLAY = 0,
    IH265D_NEED_MORE_BITS,
    IH265D_NO_PICTURE,
    IH265D_ERR_HANDLE
} HW265D_DECODESTATUS;

// Thread type
typedef enum TAG_HW265D_THREADTYPE {
    IH265D_SINGLE_THREAD = 0,
    IH265D_MULTI_THREAD
} HW265D_THREADTYPE;

// Output order
typedef enum TAG_HW265D_OUTPUTORDER {
    IH265D_DECODE_ORDER = 0,  // Output by decoder order
    IH265D_DISPLAY_ORDER      // Output by display order
} HW265D_OUTPUTORDER;

// Version
#define HW265VIDEO_ALG_VERSION_LENGTH 48 // version array lenth
#define HW265VIDEO_ALG_TIME_LENGTH    28 // module compile time array lenth
typedef struct TagHWDVersion {
    int8_t versionChar[HW265VIDEO_ALG_VERSION_LENGTH];  // library version
    int8_t releaseTime[HW265VIDEO_ALG_TIME_LENGTH];     // compiled time
    uint32_t compileVersion;                            // compiler version
} HWDVersion;

typedef struct TagHW265DUserData {
    uint32_t userDataType;  // User data type
    uint32_t userDataSize;  // User data size
    uint8_t *userData;      // User data buffer

    struct TagHW265DUserData *next;  // Direct next user data
} HW265DUserData;

typedef struct TagHWDInitParam {
    uint32_t channelID;  // [in] channel ID, used for channel info
    int32_t maxWidth;     // [in] maximum width,[8,16384]
    int32_t maxHeight;    // [in] maximum height,[8,16384]
    int32_t maxRefNum;    // [in] maximum reference num,[0,15]
    int32_t maxVPSNum;    // [in] maximum vps num,[1,16]
    int32_t maxSPSNum;    // [in] maximum sps num,[1,16]
    int32_t maxPPSNum;    // [in] maximum pps num,[1,64]

    uint32_t bitDepth;   // [in]bitdepth,8 or 10

    HW265D_THREADTYPE threadType;    // thread type,0: singal thread; 1: multi thread
    HW265D_OUTPUTORDER outputOrder;  // output type only used for DecodeFrame mode

    HW265D_VIDEO_ALG_MALLOC_FXN mallocFxn;  // different with SCCENC
    HW265D_VIDEO_ALG_FREE_FXN freeFxn;
    HW265D_VIDEO_ALG_LOG_FXN logFxn;  // log output callback function
} HWDCreateParams;

typedef struct TagHWDInputParams  {
    uint8_t *streamData; // 解码的码流
    uint32_t streamLen; // iNaluLen NAL长度
    uint64_t timeStamp;
    HW265D_DECODEMODE decodeMode;
} HWDInputParams;

typedef struct TagCuOutputInfo  {
    uint32_t cuNumIntra4;
    uint32_t cuNumIntra8;
    uint32_t cuNumIntra16;
    uint32_t cuNumIntra32;
    uint32_t cuNumIntra64;
    uint32_t cuNumPcm4;
    uint32_t cuNumPcm8;
    uint32_t cuNumPcm16;
    uint32_t cuNumPcm32;
    uint32_t cuNumPcm64;
    uint32_t cuNumInter8;
    uint32_t cuNumInter16;
    uint32_t cuNumInter32;
    uint32_t cuNumInter64;
    uint32_t cuNumSkip8;
    uint32_t cuNumSkip16;
    uint32_t cuNumSkip32;
    uint32_t cuNumSkip64;
} CuOutputInfo;

typedef struct TagHWDOutputParams {
    uint32_t channelID;  // [out] channel ID, used for identify channel info
    uint32_t bytsConsumed; // 已经解码的长度

    uint64_t timeStamp;
    HW265D_FRAMETYPE eFrameType;
    HW265D_DECODESTATUS eDecodeStatus;

    uint32_t decWidth;
    uint32_t decHeight;
    uint32_t yStride;
    uint32_t uvStride;

    uint32_t bitDepthY;              // luma bitdepth
    uint32_t bitDepthC;              // chroma bitdepth
    int32_t displayPrimariesX0;       // hdr10 luma x coordinate
    int32_t displayPrimariesX1;       // hdr10  Cb  x coordinate
    int32_t displayPrimariesX2;       // hdr10  Cr  x coordinate
    int32_t displayPrimariesY0;       // hdr10 luma y coordinate
    int32_t displayPrimariesY1;       // hdr10  Cb  y coordinate
    int32_t displayPrimariesY2;       // hdr10  Cr  y coordinate
    int32_t whitePointX;              // hdr10 white point x coordinate
    int32_t whitePointY;              // hdr10 white point y coordinate
    int32_t maxDisplayMasteringLuma;  // max hdr10 luma level
    int32_t minDisplayMasteringLuma;  // min hdr10 luma level
    int32_t maxContentLightLevel;     // max hdr10 content light level
    int32_t maxPicAverageLightLevel;  // min hdr10 content light level
    uint8_t *outYUVData[3];  // YUV address, store YUV in order, 3: means YUV

    // bitlen
    uint32_t codingBytesOfCurFrm;
    // sei
    HW265DUserData userData;
    // vui
    uint32_t aspectRatioIdc;
    uint32_t sarWidth;
    uint32_t sarHeight;

    // vps
    uint32_t vpsNumUnitsInTick;
    uint32_t vpsTimeScale;
    // cuinfo
    CuOutputInfo cuOutInfo;
    // errorinfo
    int32_t isError;

    uint32_t layerIdx;
    uint32_t poc;
} HWDOutputParams;

typedef enum TAG_HWD_RETURNVAL {
    HW265D_FAILED = -1,        // Call Failed
    HW265D_OK = 0,             // Call ok
    HW265D_NEED_MORE_BITS = 1, // Call ok, but need more bit for a frame

    // General error
    HW265D_INVALID_ARGUMENT = 0xF0401000, // Input parameter is wrong
    HW265D_UNSUPPORTED_PLATFORM,          // unsupported platforms
    HW265D_DECODER_NOT_CREATE,            // Decoder not creted
    HW265D_MALLOC_FAIL,                   // memory malloc failed
    HW265D_MEMSET_FAIL,                   // memset memset failed
    HW265D_MEMCPY_FAIL,                   // memcpy memcpy failed
    HW265D_THREAD_ERROR,                  // multi thread error

    // Init error
    HW265D_INVALID_MAX_WIDTH = 0xF0402000,  // maximum width exceed limit
    HW265D_INVALID_MAX_HEIGHT,              // maximum height exceed limit
    HW265D_INVALID_MAX_REF_PIC,             // maximum reference num exceed limit
    HW265D_INVALID_MAX_VPS_NUM,             // maximum vps num exceed limit
    HW265D_INVALID_MAX_SPS_NUM,             // maximum sps num exceed limit
    HW265D_INVALID_MAX_PPS_NUM,             // maximum pps num exceed limit
    HW265D_INVALID_THREAD_CONTROL,          // error thread control
    HW265D_INVALID_MALLOC_FXN,              // malloc callback function pointer invalid
    HW265D_INVALID_FREE_FXN,                // free callback function pointer invalid
    HW265D_INVALID_LOG_FXN,                 // log callback function pointer invalid
    HW265D_STREAMBUF_NULL,                  // decoder input stram buf is empty
    HW265D_INVALID_STREAMBUF_LENGTH,        // decoder input stream lenth error
    HW265D_BITDEPTH_ERROR,                  // bitdepth error

    // Decode error
    HW265D_NAL_HEADER_ERR = 0xF0403000,     // NAL decode error
    HW265D_VPS_ERR,                         // vps decode error
    HW265D_SPS_ERR,                         // sps decode error
    HW265D_PPS_ERR,                         // pps decode error
    HW265D_SLICEHEADER_ERR,                 // sliceheader decode error
    HW265D_SLICEDATA_ERR,                   // slicedata decode error
    HW265D_FRAME_DECODE_WARN                // frame Decode warning
} HWD_RETURNVAL;

#define HWD_API

/*
 * Instruction : create decoder handle
 *
 * Param : decoderHandle     - [out] decoder handle pointer
 *         createParams      - [in]  decoder init config parameter set address
 *
 * Return Value : Success return HW265D_OK
 *                Failed return err code
 */
HWD_API HWD_RETURNVAL HWD_Create(HWD_Handle *decoderHandle, HWDCreateParams *createParams);

/*
 * Instruction : decode a frame data
 *
 * Param : decoderHandle     - [in]  decoder handle
 *         inputParams       - [in]  input parameter struct pointer
 *         outputParams      - [out] output parameter struct pointer
 *
 * Return Value : Success return HW265D_OK
 *                Failed return err code
 */
HWD_API HWD_RETURNVAL HWD_DecodeAU(HWD_Handle decoderHandle, HWDInputParams *inputParams,
    HWDOutputParams *outputParams);

/*
 * Instruction : decode stream data
 *
 * Param : decoderHandle     - [in]  decoder handle
 *         inputParams       - [in]  input parameter struct pointer
 *         outputParams      - [out] output parameter struct pointer
 *
 * Return Value : Success return HW265D_OK / HW265D_NEED_MORE_BITS
 *                Failed return err code
 */
HWD_API HWD_RETURNVAL HWD_DecodeStream(HWD_Handle decoderHandle, HWDInputParams *inputParams,
    HWDOutputParams *outputParams);

/*
 * Instruction : delete decoder
 *
 * Param : decoderHandle     - [in] decoder handle
 *
 * Return Value : Success return HW265D_OK
 *                Failed return err code
 */
HWD_API HWD_RETURNVAL HWD_Delete(HWD_Handle decoderHandle);

/*
 * Instruction : get decoder version
 *
 * Param : version           - [out] version number struct pointer
 *
 * Return Value : Success return version information
 *                Failed return err code
 */
HWD_API HWD_RETURNVAL HWD_GetVersion(HWDVersion *version);

#ifdef __cplusplus
}
#endif
#endif /* __HW265D_API_H__ */
