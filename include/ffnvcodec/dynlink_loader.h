/*
 * This copyright notice applies to this header file only:
 *
 * Copyright (c) 2016
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the software, and to permit persons to whom the
 * software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef FFNV_CUDA_DYNLINK_LOADER_H
#define FFNV_CUDA_DYNLINK_LOADER_H

#include <stdlib.h>

#include "dynlink_cuda.h"
#include "dynlink_nvcuvid.h"
#include "nvEncodeAPI.h"

#if defined(_WIN32) && (!defined(FFNV_LOAD_FUNC) || !defined(FFNV_SYM_FUNC) || !defined(FFNV_LIB_HANDLE))
# include <windows.h>
#endif

#ifndef FFNV_LIB_HANDLE
# if defined(_WIN32)
#  define FFNV_LIB_HANDLE HMODULE
# else
#  define FFNV_LIB_HANDLE void*
# endif
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
# define CUDA_LIBNAME "nvcuda.dll"
# define NVCUVID_LIBNAME "nvcuvid.dll"
# if defined(_WIN64) || defined(__CYGWIN64__)
#  define NVENC_LIBNAME "nvEncodeAPI64.dll"
# else
#  define NVENC_LIBNAME "nvEncodeAPI.dll"
# endif
#else
# define CUDA_LIBNAME "libcuda.so.1"
# define NVCUVID_LIBNAME "libnvcuvid.so.1"
# define NVENC_LIBNAME "libnvidia-encode.so.1"
#endif

#if !defined(FFNV_LOAD_FUNC) || !defined(FFNV_SYM_FUNC)
# ifdef _WIN32
#  define FFNV_LOAD_FUNC(path) LoadLibrary(TEXT(path))
#  define FFNV_SYM_FUNC(lib, sym) GetProcAddress((lib), (sym))
#  define FFNV_FREE_FUNC(lib) FreeLibrary(lib)
# else
#  include <dlfcn.h>
#  define FFNV_LOAD_FUNC(path) dlopen((path), RTLD_LAZY)
#  define FFNV_SYM_FUNC(lib, sym) dlsym((lib), (sym))
#  define FFNV_FREE_FUNC(lib) dlclose(lib)
# endif
#endif

#if !defined(FFNV_LOG_FUNC) || !defined(FFNV_DEBUG_LOG_FUNC)
# include <stdio.h>
# define FFNV_LOG_FUNC(logctx, msg, ...) fprintf(stderr, (msg), __VA_ARGS__)
# define FFNV_DEBUG_LOG_FUNC(logctx, msg, ...)
#endif

#define STRINGIFY(X) #X

#define LOAD_LIBRARY(l, path)                                  \
    do {                                                       \
        if (!((l) = FFNV_LOAD_FUNC(path))) {                   \
            FFNV_LOG_FUNC(logctx, "Cannot load %s\n", path);   \
            ret = -1;                                          \
            goto error;                                        \
        }                                                      \
        FFNV_DEBUG_LOG_FUNC(logctx, "Loaded lib: %s\n", path); \
    } while (0)

#define LOAD_SYMBOL(fun, tp, symbol)                             \
    do {                                                         \
        if (!((f->fun) = (tp*)FFNV_SYM_FUNC(f->lib, symbol))) {  \
            FFNV_LOG_FUNC(logctx, "Cannot load %s\n", symbol);   \
            ret = -1;                                            \
            goto error;                                          \
        }                                                        \
        FFNV_DEBUG_LOG_FUNC(logctx, "Loaded sym: %s\n", symbol); \
    } while (0)

#define LOAD_SYMBOL_OPT(fun, tp, symbol)                                      \
    do {                                                                      \
        if (!((f->fun) = (tp*)FFNV_SYM_FUNC(f->lib, symbol))) {               \
            FFNV_DEBUG_LOG_FUNC(logctx, "Cannot load optional %s\n", symbol); \
        } else {                                                              \
            FFNV_DEBUG_LOG_FUNC(logctx, "Loaded sym: %s\n", symbol);          \
        }                                                                     \
    } while (0)

#define GET_PROC_EX(name, alias, required)              \
    if (required)                                       \
        LOAD_SYMBOL(alias, t##name, #name);              \
    else                                                \
        LOAD_SYMBOL_OPT(alias, t##name, #name);

#define GET_PROC_EX_V2(name, alias, required)                           \
    if (required)                                                       \
        LOAD_SYMBOL(alias, t##name##_v2, STRINGIFY(name##_v2));              \
    else                                                                \
        LOAD_SYMBOL_OPT(alias, t##name##_v2, STRINGIFY(name##_v2));

#define GET_PROC_REQUIRED(name) GET_PROC_EX(name,name,1)
#define GET_PROC_OPTIONAL(name) GET_PROC_EX(name,name,0)
#define GET_PROC(name)          GET_PROC_REQUIRED(name)
#define GET_PROC_V2(name)       GET_PROC_EX_V2(name,name,1)

#define GENERIC_LOAD_FUNC_PREAMBLE(T, n, N)     \
    T *f;                                       \
    int ret;                                    \
                                                \
    n##_free_functions(functions);              \
                                                \
    f = *functions = (T*)calloc(1, sizeof(*f)); \
    if (!f)                                     \
        return -1;                              \
                                                \
    LOAD_LIBRARY(f->lib, N);

#define GENERIC_LOAD_FUNC_FINALE(n) \
    return 0;                       \
error:                              \
    n##_free_functions(functions);  \
    return ret;

#define GENERIC_FREE_FUNC()                \
    if (!functions)                        \
        return;                            \
    if (*functions && (*functions)->lib)   \
        FFNV_FREE_FUNC((*functions)->lib); \
    free(*functions);                      \
    *functions = NULL;

#ifdef FFNV_DYNLINK_CUDA_H
typedef struct CudaFunctions {
    tcuInit *cuInit;
    tcuDeviceGetCount *cuDeviceGetCount;
    tcuDeviceGet *cuDeviceGet;
    tcuDeviceGetAttribute *cuDeviceGetAttribute;
    tcuDeviceGetName *cuDeviceGetName;
    tcuDeviceGetUuid *cuDeviceGetUuid;
    tcuDeviceComputeCapability *cuDeviceComputeCapability;
    tcuCtxCreate_v2 *cuCtxCreate;
    tcuCtxSetLimit *cuCtxSetLimit;
    tcuCtxPushCurrent_v2 *cuCtxPushCurrent;
    tcuCtxPopCurrent_v2 *cuCtxPopCurrent;
    tcuCtxGetApiVersion *cuCtxGetApiVersion;
    tcuCtxGetCurrent *cuCtxGetCurrent;
    tcuCtxGetDevice *cuCtxGetDevice;
    tcuCtxDestroy_v2 *cuCtxDestroy;
    tcuMemAlloc_v2 *cuMemAlloc;
    tcuMemAllocPitch_v2 *cuMemAllocPitch;
    tcuMemsetD8Async *cuMemsetD8Async;
    tcuMemFree_v2 *cuMemFree;
    tcuMemcpy2D_v2 *cuMemcpy2D;
    tcuMemcpy2DAsync_v2 *cuMemcpy2DAsync;
    tcuGetErrorName *cuGetErrorName;
    tcuGetErrorString *cuGetErrorString;

    tcuStreamCreate *cuStreamCreate;
    tcuStreamQuery *cuStreamQuery;
    tcuStreamSynchronize *cuStreamSynchronize;
    tcuStreamDestroy_v2 *cuStreamDestroy;
    tcuStreamAddCallback *cuStreamAddCallback;
    tcuEventCreate *cuEventCreate;
    tcuEventDestroy_v2 *cuEventDestroy;
    tcuEventSynchronize *cuEventSynchronize;
    tcuEventQuery *cuEventQuery;
    tcuEventRecord *cuEventRecord;

    tcuLaunchKernel *cuLaunchKernel;
    tcuModuleLoad *cuModuleLoad;
    tcuModuleLoadData *cuModuleLoadData;
    tcuModuleLoadDataEx *cuModuleLoadDataEx;
    tcuModuleUnload *cuModuleUnload;
    tcuModuleGetFunction *cuModuleGetFunction;
    tcuModuleGetGlobal_v2 *cuModuleGetGlobal;
    tcuTexObjectCreate *cuTexObjectCreate;
    tcuTexObjectDestroy *cuTexObjectDestroy;
    tcuModuleGetTexRef *cuModuleGetTexRef;

    tcuGLGetDevices_v2 *cuGLGetDevices;
    tcuGraphicsGLRegisterImage *cuGraphicsGLRegisterImage;
    tcuGraphicsUnregisterResource *cuGraphicsUnregisterResource;
    tcuGraphicsMapResources *cuGraphicsMapResources;
    tcuGraphicsUnmapResources *cuGraphicsUnmapResources;
    tcuGraphicsSubResourceGetMappedArray *cuGraphicsSubResourceGetMappedArray;

    tcuTexRefSetArray *cuTexRefSetArray;
    tcuTexRefSetFilterMode *cuTexRefSetFilterMode;

    //more setting fore texref
    tcuTexRefSetAddressMode *cuTexRefSetAddressMode;
    tcuTexRefSetFlags *cuTexRefSetFlags;

    //graphic buffer related
    tcuTexRefSetAddress_v2 *cuTexRefSetAddress;
    tcuTexRefSetAddress2D_v2 *cuTexRefSetAddress2D;
    tcuGraphicsGLRegisterBuffer *cuGraphicsGLRegisterBuffer;
    tcuGraphicsResourceGetMappedPointer_v2 *cuGraphicsResourceGetMappedPointer;

    tcuImportExternalMemory *cuImportExternalMemory;
    tcuDestroyExternalMemory *cuDestroyExternalMemory;
    tcuExternalMemoryGetMappedBuffer *cuExternalMemoryGetMappedBuffer;
    tcuExternalMemoryGetMappedMipmappedArray *cuExternalMemoryGetMappedMipmappedArray;
    tcuMipmappedArrayDestroy *cuMipmappedArrayDestroy;

    tcuMipmappedArrayGetLevel *cuMipmappedArrayGetLevel;

    tcuImportExternalSemaphore *cuImportExternalSemaphore;
    tcuDestroyExternalSemaphore *cuDestroyExternalSemaphore;
    tcuSignalExternalSemaphoresAsync *cuSignalExternalSemaphoresAsync;
    tcuWaitExternalSemaphoresAsync *cuWaitExternalSemaphoresAsync;

    //cuda surface
    tcuSurfObjectCreate *cuSurfObjectCreate;
    tcuSurfObjectDestroy *cuSurfObjectDestroy;
    tcuSurfObjectGetResourceDesc *cuSurfObjectGetResourceDesc;

    //cuda array
    tcuArrayGetDescriptor *cuArrayGetDescriptor;
    tcuArrayDestroy *cuArrayDestroy;
    tcuArray3DGetDescriptor *cuArray3DGetDescriptor;

    FFNV_LIB_HANDLE lib;
} CudaFunctions;
#else
typedef struct CudaFunctions CudaFunctions;
#endif

typedef struct CuvidFunctions {
    tcuvidGetDecoderCaps *cuvidGetDecoderCaps;
    tcuvidCreateDecoder *cuvidCreateDecoder;
    tcuvidDestroyDecoder *cuvidDestroyDecoder;
    tcuvidDecodePicture *cuvidDecodePicture;
    tcuvidGetDecodeStatus *cuvidGetDecodeStatus;
    tcuvidReconfigureDecoder *cuvidReconfigureDecoder;
    tcuvidMapVideoFrame *cuvidMapVideoFrame;
    tcuvidUnmapVideoFrame *cuvidUnmapVideoFrame;
    tcuvidCtxLockCreate *cuvidCtxLockCreate;
    tcuvidCtxLockDestroy *cuvidCtxLockDestroy;
    tcuvidCtxLock *cuvidCtxLock;
    tcuvidCtxUnlock *cuvidCtxUnlock;

    tcuvidCreateVideoSource *cuvidCreateVideoSource;
    tcuvidCreateVideoSourceW *cuvidCreateVideoSourceW;
    tcuvidDestroyVideoSource *cuvidDestroyVideoSource;
    tcuvidSetVideoSourceState *cuvidSetVideoSourceState;
    tcuvidGetVideoSourceState *cuvidGetVideoSourceState;
    tcuvidGetSourceVideoFormat *cuvidGetSourceVideoFormat;
    tcuvidGetSourceAudioFormat *cuvidGetSourceAudioFormat;
    tcuvidCreateVideoParser *cuvidCreateVideoParser;
    tcuvidParseVideoData *cuvidParseVideoData;
    tcuvidDestroyVideoParser *cuvidDestroyVideoParser;

    FFNV_LIB_HANDLE lib;
} CuvidFunctions;

typedef NVENCSTATUS NVENCAPI tNvEncodeAPICreateInstance(NV_ENCODE_API_FUNCTION_LIST *functionList);
typedef NVENCSTATUS NVENCAPI tNvEncodeAPIGetMaxSupportedVersion(uint32_t* version);

typedef struct NvencFunctions {
    tNvEncodeAPICreateInstance *NvEncodeAPICreateInstance;
    tNvEncodeAPIGetMaxSupportedVersion *NvEncodeAPIGetMaxSupportedVersion;

    FFNV_LIB_HANDLE lib;
} NvencFunctions;

#ifdef FFNV_DYNLINK_CUDA_H
static inline void cuda_free_functions(CudaFunctions **functions)
{
    GENERIC_FREE_FUNC();
}
#endif

static inline void cuvid_free_functions(CuvidFunctions **functions)
{
    GENERIC_FREE_FUNC();
}

static inline void nvenc_free_functions(NvencFunctions **functions)
{
    GENERIC_FREE_FUNC();
}

#ifdef FFNV_DYNLINK_CUDA_H
static inline int cuda_load_functions(CudaFunctions **functions, void *logctx)
{
    GENERIC_LOAD_FUNC_PREAMBLE(CudaFunctions, cuda, CUDA_LIBNAME);

    GET_PROC(cuInit);
    GET_PROC(cuDeviceGetCount);
    GET_PROC(cuDeviceGet);
    GET_PROC(cuDeviceGetName);
    GET_PROC(cuDeviceComputeCapability);
    GET_PROC_V2(cuCtxCreate);
    GET_PROC(cuCtxSetLimit);
    GET_PROC_V2(cuCtxPushCurrent);
    GET_PROC_V2(cuCtxPopCurrent);
    GET_PROC(cuCtxGetApiVersion);
    GET_PROC(cuCtxGetCurrent);
    GET_PROC(cuCtxGetDevice);
    GET_PROC_V2(cuCtxDestroy);
    GET_PROC_V2(cuMemAlloc);
    GET_PROC_V2(cuMemFree);
    GET_PROC_V2(cuMemcpy2D);
    GET_PROC_V2(cuMemcpy2DAsync);
    GET_PROC(cuGetErrorName);
    GET_PROC(cuGetErrorString);
    GET_PROC(cuDeviceGetAttribute);
    GET_PROC_V2(cuMemAllocPitch);
    GET_PROC(cuMemsetD8Async);

    GET_PROC(cuStreamCreate);
    GET_PROC(cuStreamQuery);
    GET_PROC(cuStreamSynchronize);
    GET_PROC_V2(cuStreamDestroy);
    GET_PROC(cuStreamAddCallback);
    GET_PROC(cuEventCreate);
    GET_PROC_V2(cuEventDestroy);
    GET_PROC(cuEventSynchronize);
    GET_PROC(cuEventQuery);
    GET_PROC(cuEventRecord);

    GET_PROC_V2(cuGLGetDevices);
    GET_PROC(cuGraphicsGLRegisterImage);
    GET_PROC(cuGraphicsUnregisterResource);
    GET_PROC(cuGraphicsMapResources);
    GET_PROC(cuGraphicsUnmapResources);
    GET_PROC(cuGraphicsSubResourceGetMappedArray);

    GET_PROC(cuModuleLoad);
    GET_PROC(cuModuleLoadData);
    GET_PROC(cuModuleLoadDataEx);
    GET_PROC(cuModuleUnload);
    GET_PROC(cuModuleGetFunction);
    GET_PROC(cuModuleGetGlobal);
    GET_PROC(cuModuleGetTexRef);
    GET_PROC(cuLaunchKernel);

    GET_PROC(cuTexObjectCreate);
    GET_PROC(cuTexObjectDestroy);

    GET_PROC(cuTexRefSetArray);
    GET_PROC(cuTexRefSetFilterMode);

    //more setting fore texref
    GET_PROC(cuTexRefSetAddressMode);
    GET_PROC(cuTexRefSetFlags);

    //graphic buffer related
    GET_PROC_V2(cuTexRefSetAddress);
    GET_PROC_V2(cuTexRefSetAddress2D);
    GET_PROC(cuGraphicsGLRegisterBuffer);
    GET_PROC_V2(cuGraphicsResourceGetMappedPointer);
    //cuda surface
    GET_PROC(cuSurfObjectCreate);
    GET_PROC(cuSurfObjectDestroy);
    GET_PROC(cuSurfObjectGetResourceDesc);
    //cuda array
    //cuda array
    GET_PROC(cuArrayGetDescriptor);
    GET_PROC(cuArrayDestroy);
    GET_PROC(cuArray3DGetDescriptor);

    //more driver info
    GET_PROC(cuDeviceGetAttribute);

    GET_PROC_OPTIONAL(cuDeviceGetUuid);
    GET_PROC_OPTIONAL(cuImportExternalMemory);
    GET_PROC_OPTIONAL(cuDestroyExternalMemory);
    GET_PROC_OPTIONAL(cuExternalMemoryGetMappedBuffer);
    GET_PROC_OPTIONAL(cuExternalMemoryGetMappedMipmappedArray);
    GET_PROC_OPTIONAL(cuMipmappedArrayGetLevel);
    GET_PROC_OPTIONAL(cuMipmappedArrayDestroy);

    GET_PROC_OPTIONAL(cuImportExternalSemaphore);
    GET_PROC_OPTIONAL(cuDestroyExternalSemaphore);
    GET_PROC_OPTIONAL(cuSignalExternalSemaphoresAsync);
    GET_PROC_OPTIONAL(cuWaitExternalSemaphoresAsync);

    GENERIC_LOAD_FUNC_FINALE(cuda);
}
#endif

static inline int cuvid_load_functions(CuvidFunctions **functions, void *logctx)
{
    GENERIC_LOAD_FUNC_PREAMBLE(CuvidFunctions, cuvid, NVCUVID_LIBNAME);

    GET_PROC_OPTIONAL(cuvidGetDecoderCaps);
    GET_PROC(cuvidCreateDecoder);
    GET_PROC(cuvidDestroyDecoder);
    GET_PROC(cuvidDecodePicture);
    GET_PROC(cuvidGetDecodeStatus);
    GET_PROC(cuvidReconfigureDecoder);
#ifdef __CUVID_DEVPTR64
    LOAD_SYMBOL(cuvidMapVideoFrame, tcuvidMapVideoFrame, "cuvidMapVideoFrame64");
    LOAD_SYMBOL(cuvidUnmapVideoFrame, tcuvidUnmapVideoFrame, "cuvidUnmapVideoFrame64");
#else
    GET_PROC(cuvidMapVideoFrame);
    GET_PROC(cuvidUnmapVideoFrame);
#endif
    GET_PROC(cuvidCtxLockCreate);
    GET_PROC(cuvidCtxLockDestroy);
    GET_PROC(cuvidCtxLock);
    GET_PROC(cuvidCtxUnlock);

    GET_PROC(cuvidCreateVideoSource);
    GET_PROC(cuvidCreateVideoSourceW);
    GET_PROC(cuvidDestroyVideoSource);
    GET_PROC(cuvidSetVideoSourceState);
    GET_PROC(cuvidGetVideoSourceState);
    GET_PROC(cuvidGetSourceVideoFormat);
    GET_PROC(cuvidGetSourceAudioFormat);
    GET_PROC(cuvidCreateVideoParser);
    GET_PROC(cuvidParseVideoData);
    GET_PROC(cuvidDestroyVideoParser);

    GENERIC_LOAD_FUNC_FINALE(cuvid);
}

static inline int nvenc_load_functions(NvencFunctions **functions, void *logctx)
{
    GENERIC_LOAD_FUNC_PREAMBLE(NvencFunctions, nvenc, NVENC_LIBNAME);

    GET_PROC(NvEncodeAPICreateInstance);
    GET_PROC(NvEncodeAPIGetMaxSupportedVersion);

    GENERIC_LOAD_FUNC_FINALE(nvenc);
}

#undef GENERIC_LOAD_FUNC_PREAMBLE
#undef LOAD_LIBRARY
#undef GET_PROC
#undef GET_PROC_V2
#undef GET_PROC_OPTIONAL
#undef GET_PROC_REQUIRED
#undef GET_PROC_EX
#undef GET_PROC_EX_V2
#undef STRINGIFY
#undef LOAD_SYMBOL
#undef LOAD_SYMBOL_OPT
#undef GENERIC_LOAD_FUNC_FINALE
#undef GENERIC_FREE_FUNC
#undef CUDA_LIBNAME
#undef NVCUVID_LIBNAME
#undef NVENC_LIBNAME

#endif

