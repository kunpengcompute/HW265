/*
 * copyright (c) Huawei Technologies Co., Ltd.2020-2021, All rights reserved
 * Description: API header for libhw265dec
 * Auther: Huawei
 * Create: 2019-7-15
 */

#ifndef IHW265DEC_API_H
#define IHW265DEC_API_H

#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TAG_IHW265VIDEO_ALG_LOG_LEVEL {
    IHW265VIDEO_ALG_LOG_ERROR = 0,
    IHW265VIDEO_ALG_LOG_WARNING,
    IHW265VIDEO_ALG_LOG_INFO,
    IHW265VIDEO_ALG_LOG_DEBUG
} IHW265VIDEO_ALG_LOG_LEVEL;

typedef void *HWD_Handle;

typedef void *(*IHW265D_VIDEO_ALG_MALLOC_FXN)(uint32_t channelID, uint32_t size);
typedef void (*IHW265D_VIDEO_ALG_FREE_FXN)(uint32_t channelID, void *ptr);
typedef void (*IHW265D_VIDEO_ALG_LOG_FXN)(uint32_t channelID, IHW265VIDEO_ALG_LOG_LEVEL level, int8_t *msg, ...);

#if defined(_MSC_VER)
#if defined(HW_VIDEO_ALG_EXPORTS_DLL)
#define HWD_API extern __declspec(dllexport)
#elif defined(HW_VIDEO_ALG_EXPORTS_LIB)
#define HWD_API extern __declspec(dllimport)
#else
#define HWD_API
#endif
#define inline __inline
#elif defined(__GNUC__)
#define HWD_API
#else
#define HWD_API
#endif

typedef enum {
    IH265D_B_SLICE = 0,
    IH265D_P_SLICE = 1,
    IH265D_I_SLICE = 2,
} SLICE_TYPE;

typedef enum {
    IH265D_FRAME_I = 0,
    IH265D_FRAME_P,
    IH265D_FRAME_B,
    IH265D_FRAME_UNKNOWN,
} HW265D_FRAMETYPE;

typedef enum {
    IH265D_DECODE = 0,
    IH265D_DECODE_END
} HW265D_DECODEMODE;

typedef enum {
    IH265D_GETDISPLAY = 0,
    IH265D_NEED_MORE_BITS,
    IH265D_NO_PICTURE,
    IH265D_ERR_HANDLE
} HW265D_DECODESTATUS;

typedef enum {
    IH265D_SINGLE_THREAD = 0,
    IH265D_MULTI_THREAD
} HW265D_THREADTYPE;

typedef enum {
    IH265D_DECODE_ORDER = 0,
    IH265D_DISPLAY_ORDER
} HW265D_OUTPUTORDER;

#define IHW265VIDEO_ALG_VERSION_LENGTH 48
#define IHW265VIDEO_ALG_TIME_LENGTH 28

typedef struct TagHWDVersion {
    int8_t versionChar[IHW265VIDEO_ALG_VERSION_LENGTH];
    int8_t releaseTime[IHW265VIDEO_ALG_TIME_LENGTH];
    uint32_t compileVersion;
} HWDVersion;

typedef struct TagHW265DUserData {
    uint32_t userDataType;
    uint32_t userDataSize;
    uint8_t *userData;

    struct TagHW265DUserData *next;
} HW265DUserData;

typedef struct TagHWDInitParam {
    uint32_t channelID;
    int32_t maxWidth;
    int32_t maxHeight;
    int32_t maxRefNum;
    int32_t maxVPSNum;
    int32_t maxSPSNum;
    int32_t maxPPSNum;

    uint32_t bitDepth;

    HW265D_THREADTYPE threadType;
    HW265D_OUTPUTORDER outputOrder;

    IHW265D_VIDEO_ALG_MALLOC_FXN mallocFxn;
    IHW265D_VIDEO_ALG_FREE_FXN freeFxn;
    IHW265D_VIDEO_ALG_LOG_FXN logFxn;
} HWDCreateParams;

typedef struct TagHWDInputParams {
    uint8_t *streamData;
    uint32_t streamLen;
    uint64_t timeStamp;
    HW265D_DECODEMODE decodeMode;
} HWDInputParams;

typedef struct TagCuOutputInfo {
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
    uint32_t channelID;
    uint32_t bytsConsumed;

    uint64_t timeStamp;
    HW265D_FRAMETYPE eFrameType;
    HW265D_DECODESTATUS eDecodeStatus;

    uint32_t decWidth;
    uint32_t decHeight;
    uint32_t yStride;
    uint32_t uvStride;

    uint32_t bitDepthY;
    uint32_t bitDepthC;
    int32_t displayPrimariesX0;
    int32_t displayPrimariesX1;
    int32_t displayPrimariesX2;
    int32_t displayPrimariesY0;
    int32_t displayPrimariesY1;
    int32_t displayPrimariesY2;
    int32_t whitePointX;
    int32_t whitePointY;
    int32_t maxDisplayMasteringLuma;
    int32_t minDisplayMasteringLuma;
    int32_t maxContentLightLevel;
    int32_t maxPicAverageLightLevel;
    uint8_t *outYUVData[3];

    uint32_t codingBytesOfCurFrm;

    HW265DUserData userData;

    uint32_t aspectRatioIdc;
    uint32_t sarWidth;
    uint32_t sarHeight;
    uint32_t vpsNumUnitsInTick;
    uint32_t vpsTimeScale;

    CuOutputInfo cuOutInfo;

    int32_t isError;
    uint32_t layerIdx;
    uint32_t poc;
    int32_t isScc;
} HWDOutputParams;

typedef enum TAG_HWD_RETURNVAL {
    IHW265D_FAILED = -1,
    IHW265D_OK = 0,
    IHW265D_NEED_MORE_BITS = 1,
    IHW265D_FIND_NEW_PIC = 2,

    IHW265D_NAL_HEADER_WARNING = 0x00000004,
    IHW265D_VPS_WARNING = 0x00000008,
    IHW265D_SPS_WARNING = 0x00000010,
    IHW265D_PPS_WARNING = 0x00000020,
    IHW265D_SLICEHEADER_WARNING = 0x00000040,
    IHW265D_SLICEDATA_WARNING = 0x00000080,

    IHW265D_INVALID_ARGUMENT = 0xF0401000,
    IHW265D_DECODER_NOT_CREATE,

    IHW265D_MALLOC_FAIL = 0xF0402000,
    IHW265D_INVALID_MAX_WIDTH,
    IHW265D_INVALID_MAX_HEIGHT,
    IHW265D_INVALID_MAX_REF_PIC,
    IHW265D_INVALID_MAX_VPS_NUM,
    IHW265D_INVALID_MAX_SPS_NUM,
    IHW265D_INVALID_MAX_PPS_NUM,
    IHW265D_INVALID_THREAD_CONTROL,
    IHW265D_INVALID_MALLOC_FXN,
    IHW265D_INVALID_FREE_FXN,
    IHW265D_INVALID_LOG_FXN,
    IHW265D_STREAMBUF_NULL,
    IHW265D_INVALID_STREAMBUF_LENGTH,
    IHW265D_YUVBUF_NULL,
    IHW265D_YUVBUF_ADDR_NOT_ALIGN_16,
    IHW265D_POSTPROCESS_ERR,
    IHW265D_ERRCONCEAL_ERR,

    IHW265D_NAL_HEADER_ERR = 0xF0404001,
    IHW265D_VPS_ERR = 0xF0404002,
    IHW265D_SPS_ERR = 0xF0404003,
    IHW265D_PPS_ERR = 0xF0404004,
    IHW265D_SLICEHEADER_ERR = 0xF0404005,
    IHW265D_SLICEDATA_ERR = 0xF0404006,

    IHW265D_FRAME_DECODE_WARN = 0xE0404007,
    IHW265D_THREAD_ERROR = 0xE0404008,
    IHW265D_BITDEPTH_ERROR = 0xE0404009
} HWD_RETURNVAL;

HWD_API HWD_RETURNVAL HWD_Create(HWD_Handle *decoderHandle, HWDCreateParams *createParams);
HWD_API HWD_RETURNVAL HWD_DecodeAU(HWD_Handle decoderHandle, HWDInputParams *inputParams, HWDOutputParams *outputParams);
HWD_API HWD_RETURNVAL HWD_DecodeStream(HWD_Handle decoderHandle, HWDInputParams *inputParams, HWDOutputParams *outputParams);
HWD_API HWD_RETURNVAL HWD_Delete(HWD_Handle decoderHandle);
HWD_API HWD_RETURNVAL HWD_GetVersion(HWDVersion *version);

#ifdef __cplusplus
}
#endif
#endif