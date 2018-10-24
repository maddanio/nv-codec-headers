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

#if !defined(FFNV_DYNLINK_CUDA_H) && !defined(CUDA_VERSION)
#define FFNV_DYNLINK_CUDA_H

#include <stddef.h>

#define CUDA_VERSION 7050

#if defined(_WIN32) || defined(__CYGWIN__)
#define CUDAAPI __stdcall
#else
#define CUDAAPI
#endif

#define CU_CTX_SCHED_BLOCKING_SYNC 4

typedef int CUdevice;
typedef void* CUarray;
typedef void* CUcontext;
typedef void* CUstream;
typedef void* CUevent;
typedef void* CUmodule;
typedef void* CUfunction;
typedef void* CUtexref;
#if defined(__x86_64) || defined(AMD64) || defined(_M_AMD64)
typedef unsigned long long CUdeviceptr;
#else
typedef unsigned int CUdeviceptr;
#endif

typedef enum cudaError_enum {
    CUDA_SUCCESS = 0,
    CUDA_ERROR_NOT_READY = 600
} CUresult;

typedef enum CUmemorytype_enum {
    CU_MEMORYTYPE_HOST = 1,
    CU_MEMORYTYPE_DEVICE = 2,
    CU_MEMORYTYPE_ARRAY = 3
} CUmemorytype;

typedef enum CUlimit_enum {
    CU_LIMIT_STACK_SIZE = 0,
    CU_LIMIT_PRINTF_FIFO_SIZE = 1,
    CU_LIMIT_MALLOC_HEAP_SIZE = 2,
    CU_LIMIT_DEV_RUNTIME_SYNC_DEPTH = 3,
    CU_LIMIT_DEV_RUNTIME_PENDING_LAUNCH_COUNT = 4
} CUlimit;

typedef enum CUgraphicsRegisterFlags_enum {
    CU_GRAPHICS_REGISTER_FLAGS_NONE = 0,
    CU_GRAPHICS_REGISTER_FLAGS_READ_ONLY = 1,
    CU_GRAPHICS_REGISTER_FLAGS_WRITE_DISCARD = 2,
    CU_GRAPHICS_REGISTER_FLAGS_SURFACE_LDST = 4,
    CU_GRAPHICS_REGISTER_FLAGS_TEXTURE_GATHER = 8
} CUgraphicsRegisterFlags;

typedef struct CUDA_MEMCPY2D_st {
    size_t srcXInBytes;
    size_t srcY;
    CUmemorytype srcMemoryType;
    const void *srcHost;
    CUdeviceptr srcDevice;
    CUarray srcArray;
    size_t srcPitch;

    size_t dstXInBytes;
    size_t dstY;
    CUmemorytype dstMemoryType;
    void *dstHost;
    CUdeviceptr dstDevice;
    CUarray dstArray;
    size_t dstPitch;

    size_t WidthInBytes;
    size_t Height;
} CUDA_MEMCPY2D;

typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef struct CUgraphicsResource_st *CUgraphicsResource;

typedef enum CUGLDeviceList_enum {
    CU_GL_DEVICE_LIST_ALL = 1,
    CU_GL_DEVICE_LIST_CURRENT_FRAME = 2,
    CU_GL_DEVICE_LIST_NEXT_FRAME = 3,
} CUGLDeviceList;

typedef enum CUjit_option_enum {
    CU_JIT_MAX_REGISTERS = 0,
    CU_JIT_THREADS_PER_BLOCK,
    CU_JIT_WALL_TIME,
    CU_JIT_INFO_LOG_BUFFER,
    CU_JIT_INFO_LOG_BUFFER_SIZE_BYTES,
    CU_JIT_ERROR_LOG_BUFFER,
    CU_JIT_ERROR_LOG_BUFFER_SIZE_BYTES,
    CU_JIT_OPTIMIZATION_LEVEL,
    CU_JIT_TARGET_FROM_CUCONTEXT,
    CU_JIT_TARGET,
    CU_JIT_FALLBACK_STRATEGY,
    CU_JIT_GENERATE_DEBUG_INFO,
    CU_JIT_LOG_VERBOSE,
    CU_JIT_GENERATE_LINE_INFO,
    CU_JIT_CACHE_MODE,
    CU_JIT_NEW_SM3X_OPT,
    CU_JIT_FAST_COMPILE,
    CU_JIT_NUM_OPTIONS
} CUjit_option;

typedef enum CUfilter_mode_enum {
    CU_TR_FILTER_MODE_POINT = 0,
    CU_TR_FILTER_MODE_LINEAR = 1
} CUfilter_mode;

/**
 * Texture reference addressing modes
 */
typedef enum CUaddress_mode_enum {
    CU_TR_ADDRESS_MODE_WRAP   = 0, /**< Wrapping address mode */
    CU_TR_ADDRESS_MODE_CLAMP  = 1, /**< Clamp to edge address mode */
    CU_TR_ADDRESS_MODE_MIRROR = 2, /**< Mirror address mode */
    CU_TR_ADDRESS_MODE_BORDER = 3  /**< Border address mode */
} CUaddress_mode;

/**
 * Array formats
 */
typedef enum CUarray_format_enum {
    CU_AD_FORMAT_UNSIGNED_INT8  = 0x01, /**< Unsigned 8-bit integers */
    CU_AD_FORMAT_UNSIGNED_INT16 = 0x02, /**< Unsigned 16-bit integers */
    CU_AD_FORMAT_UNSIGNED_INT32 = 0x03, /**< Unsigned 32-bit integers */
    CU_AD_FORMAT_SIGNED_INT8    = 0x08, /**< Signed 8-bit integers */
    CU_AD_FORMAT_SIGNED_INT16   = 0x09, /**< Signed 16-bit integers */
    CU_AD_FORMAT_SIGNED_INT32   = 0x0a, /**< Signed 32-bit integers */
    CU_AD_FORMAT_HALF           = 0x10, /**< 16-bit floating point */
    CU_AD_FORMAT_FLOAT          = 0x20  /**< 32-bit floating point */
} CUarray_format;

/**
 * Array descriptor
 */
typedef struct CUDA_ARRAY_DESCRIPTOR_st
{
    size_t Width;             /**< Width of array */
    size_t Height;            /**< Height of array */

    CUarray_format Format;    /**< Array format */
    unsigned int NumChannels; /**< Channels per array element */
} CUDA_ARRAY_DESCRIPTOR;

/**
 * Device properties
 */
typedef enum CUdevice_attribute_enum {
    CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_BLOCK = 1,              /**< Maximum number of threads per block */
    CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_X = 2,                    /**< Maximum block dimension X */
    CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Y = 3,                    /**< Maximum block dimension Y */
    CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Z = 4,                    /**< Maximum block dimension Z */
    CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_X = 5,                     /**< Maximum grid dimension X */
    CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Y = 6,                     /**< Maximum grid dimension Y */
    CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Z = 7,                     /**< Maximum grid dimension Z */
    CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK = 8,        /**< Maximum shared memory available per block in bytes */
    CU_DEVICE_ATTRIBUTE_SHARED_MEMORY_PER_BLOCK = 8,            /**< Deprecated, use CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK */
    CU_DEVICE_ATTRIBUTE_TOTAL_CONSTANT_MEMORY = 9,              /**< Memory available on device for __constant__ variables in a CUDA C kernel in bytes */
    CU_DEVICE_ATTRIBUTE_WARP_SIZE = 10,                         /**< Warp size in threads */
    CU_DEVICE_ATTRIBUTE_MAX_PITCH = 11,                         /**< Maximum pitch in bytes allowed by memory copies */
    CU_DEVICE_ATTRIBUTE_MAX_REGISTERS_PER_BLOCK = 12,           /**< Maximum number of 32-bit registers available per block */
    CU_DEVICE_ATTRIBUTE_REGISTERS_PER_BLOCK = 12,               /**< Deprecated, use CU_DEVICE_ATTRIBUTE_MAX_REGISTERS_PER_BLOCK */
    CU_DEVICE_ATTRIBUTE_CLOCK_RATE = 13,                        /**< Typical clock frequency in kilohertz */
    CU_DEVICE_ATTRIBUTE_TEXTURE_ALIGNMENT = 14,                 /**< Alignment requirement for textures */
    CU_DEVICE_ATTRIBUTE_GPU_OVERLAP = 15,                       /**< Device can possibly copy memory and execute a kernel concurrently. Deprecated. Use instead CU_DEVICE_ATTRIBUTE_ASYNC_ENGINE_COUNT. */
    CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT = 16,              /**< Number of multiprocessors on device */
    CU_DEVICE_ATTRIBUTE_KERNEL_EXEC_TIMEOUT = 17,               /**< Specifies whether there is a run time limit on kernels */
    CU_DEVICE_ATTRIBUTE_INTEGRATED = 18,                        /**< Device is integrated with host memory */
    CU_DEVICE_ATTRIBUTE_CAN_MAP_HOST_MEMORY = 19,               /**< Device can map host memory into CUDA address space */
    CU_DEVICE_ATTRIBUTE_COMPUTE_MODE = 20,                      /**< Compute mode (See ::CUcomputemode for details) */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE1D_WIDTH = 21,           /**< Maximum 1D texture width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_WIDTH = 22,           /**< Maximum 2D texture width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_HEIGHT = 23,          /**< Maximum 2D texture height */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_WIDTH = 24,           /**< Maximum 3D texture width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_HEIGHT = 25,          /**< Maximum 3D texture height */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_DEPTH = 26,           /**< Maximum 3D texture depth */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LAYERED_WIDTH = 27,   /**< Maximum 2D layered texture width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LAYERED_HEIGHT = 28,  /**< Maximum 2D layered texture height */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LAYERED_LAYERS = 29,  /**< Maximum layers in a 2D layered texture */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_ARRAY_WIDTH = 27,     /**< Deprecated, use CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LAYERED_WIDTH */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_ARRAY_HEIGHT = 28,    /**< Deprecated, use CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LAYERED_HEIGHT */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_ARRAY_NUMSLICES = 29, /**< Deprecated, use CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LAYERED_LAYERS */
    CU_DEVICE_ATTRIBUTE_SURFACE_ALIGNMENT = 30,                 /**< Alignment requirement for surfaces */
    CU_DEVICE_ATTRIBUTE_CONCURRENT_KERNELS = 31,                /**< Device can possibly execute multiple kernels concurrently */
    CU_DEVICE_ATTRIBUTE_ECC_ENABLED = 32,                       /**< Device has ECC support enabled */
    CU_DEVICE_ATTRIBUTE_PCI_BUS_ID = 33,                        /**< PCI bus ID of the device */
    CU_DEVICE_ATTRIBUTE_PCI_DEVICE_ID = 34,                     /**< PCI device ID of the device */
    CU_DEVICE_ATTRIBUTE_TCC_DRIVER = 35,                        /**< Device is using TCC driver model */
    CU_DEVICE_ATTRIBUTE_MEMORY_CLOCK_RATE = 36,                 /**< Peak memory clock frequency in kilohertz */
    CU_DEVICE_ATTRIBUTE_GLOBAL_MEMORY_BUS_WIDTH = 37,           /**< Global memory bus width in bits */
    CU_DEVICE_ATTRIBUTE_L2_CACHE_SIZE = 38,                     /**< Size of L2 cache in bytes */
    CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_MULTIPROCESSOR = 39,    /**< Maximum resident threads per multiprocessor */
    CU_DEVICE_ATTRIBUTE_ASYNC_ENGINE_COUNT = 40,                /**< Number of asynchronous engines */
    CU_DEVICE_ATTRIBUTE_UNIFIED_ADDRESSING = 41,                /**< Device shares a unified address space with the host */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE1D_LAYERED_WIDTH = 42,   /**< Maximum 1D layered texture width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE1D_LAYERED_LAYERS = 43,  /**< Maximum layers in a 1D layered texture */
    CU_DEVICE_ATTRIBUTE_CAN_TEX2D_GATHER = 44,                  /**< Deprecated, do not use. */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_GATHER_WIDTH = 45,    /**< Maximum 2D texture width if CUDA_ARRAY3D_TEXTURE_GATHER is set */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_GATHER_HEIGHT = 46,   /**< Maximum 2D texture height if CUDA_ARRAY3D_TEXTURE_GATHER is set */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_WIDTH_ALTERNATE = 47, /**< Alternate maximum 3D texture width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_HEIGHT_ALTERNATE = 48,/**< Alternate maximum 3D texture height */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE3D_DEPTH_ALTERNATE = 49, /**< Alternate maximum 3D texture depth */
    CU_DEVICE_ATTRIBUTE_PCI_DOMAIN_ID = 50,                     /**< PCI domain ID of the device */
    CU_DEVICE_ATTRIBUTE_TEXTURE_PITCH_ALIGNMENT = 51,           /**< Pitch alignment requirement for textures */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURECUBEMAP_WIDTH = 52,      /**< Maximum cubemap texture width/height */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURECUBEMAP_LAYERED_WIDTH = 53,  /**< Maximum cubemap layered texture width/height */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURECUBEMAP_LAYERED_LAYERS = 54, /**< Maximum layers in a cubemap layered texture */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE1D_WIDTH = 55,           /**< Maximum 1D surface width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE2D_WIDTH = 56,           /**< Maximum 2D surface width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE2D_HEIGHT = 57,          /**< Maximum 2D surface height */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE3D_WIDTH = 58,           /**< Maximum 3D surface width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE3D_HEIGHT = 59,          /**< Maximum 3D surface height */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE3D_DEPTH = 60,           /**< Maximum 3D surface depth */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE1D_LAYERED_WIDTH = 61,   /**< Maximum 1D layered surface width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE1D_LAYERED_LAYERS = 62,  /**< Maximum layers in a 1D layered surface */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE2D_LAYERED_WIDTH = 63,   /**< Maximum 2D layered surface width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE2D_LAYERED_HEIGHT = 64,  /**< Maximum 2D layered surface height */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACE2D_LAYERED_LAYERS = 65,  /**< Maximum layers in a 2D layered surface */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACECUBEMAP_WIDTH = 66,      /**< Maximum cubemap surface width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACECUBEMAP_LAYERED_WIDTH = 67,  /**< Maximum cubemap layered surface width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_SURFACECUBEMAP_LAYERED_LAYERS = 68, /**< Maximum layers in a cubemap layered surface */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE1D_LINEAR_WIDTH = 69,    /**< Maximum 1D linear texture width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LINEAR_WIDTH = 70,    /**< Maximum 2D linear texture width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LINEAR_HEIGHT = 71,   /**< Maximum 2D linear texture height */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_LINEAR_PITCH = 72,    /**< Maximum 2D linear texture pitch in bytes */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_MIPMAPPED_WIDTH = 73, /**< Maximum mipmapped 2D texture width */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_MIPMAPPED_HEIGHT = 74,/**< Maximum mipmapped 2D texture height */
    CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR = 75,          /**< Major compute capability version number */
    CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR = 76,          /**< Minor compute capability version number */
    CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE1D_MIPMAPPED_WIDTH = 77, /**< Maximum mipmapped 1D texture width */
    CU_DEVICE_ATTRIBUTE_STREAM_PRIORITIES_SUPPORTED = 78,       /**< Device supports stream priorities */
    CU_DEVICE_ATTRIBUTE_GLOBAL_L1_CACHE_SUPPORTED = 79,         /**< Device supports caching globals in L1 */
    CU_DEVICE_ATTRIBUTE_LOCAL_L1_CACHE_SUPPORTED = 80,          /**< Device supports caching locals in L1 */
    CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_MULTIPROCESSOR = 81,  /**< Maximum shared memory available per multiprocessor in bytes */
    CU_DEVICE_ATTRIBUTE_MAX_REGISTERS_PER_MULTIPROCESSOR = 82,  /**< Maximum number of 32-bit registers available per multiprocessor */
    CU_DEVICE_ATTRIBUTE_MANAGED_MEMORY = 83,                    /**< Device can allocate managed memory on this system */
    CU_DEVICE_ATTRIBUTE_MULTI_GPU_BOARD = 84,                    /**< Device is on a multi-GPU board */
    CU_DEVICE_ATTRIBUTE_MULTI_GPU_BOARD_GROUP_ID = 85,           /**< Unique id for a group of devices on the same multi-GPU board */
    CU_DEVICE_ATTRIBUTE_HOST_NATIVE_ATOMIC_SUPPORTED = 86,       /**< Link between the device and the host supports native atomic operations (this is a placeholder attribute, and is not supported on any current hardware)*/
    CU_DEVICE_ATTRIBUTE_SINGLE_TO_DOUBLE_PRECISION_PERF_RATIO = 87,  /**< Ratio of single precision performance (in floating-point operations per second) to double precision performance */
    CU_DEVICE_ATTRIBUTE_PAGEABLE_MEMORY_ACCESS = 88,            /**< Device supports coherently accessing pageable memory without calling cudaHostRegister on it */
    CU_DEVICE_ATTRIBUTE_CONCURRENT_MANAGED_ACCESS = 89,         /**< Device can coherently access managed memory concurrently with the CPU */
    CU_DEVICE_ATTRIBUTE_COMPUTE_PREEMPTION_SUPPORTED = 90,      /**< Device supports compute preemption. */
    CU_DEVICE_ATTRIBUTE_CAN_USE_HOST_POINTER_FOR_REGISTERED_MEM = 91, /**< Device can access host registered memory at the same virtual address as the CPU */
    CU_DEVICE_ATTRIBUTE_CAN_USE_STREAM_MEM_OPS = 92,            /**< ::cuStreamBatchMemOp and related APIs are supported. */
    CU_DEVICE_ATTRIBUTE_CAN_USE_64_BIT_STREAM_MEM_OPS = 93,     /**< 64-bit operations are supported in ::cuStreamBatchMemOp and related APIs. */
    CU_DEVICE_ATTRIBUTE_CAN_USE_STREAM_WAIT_VALUE_NOR = 94,     /**< ::CU_STREAM_WAIT_VALUE_NOR is supported. */
    CU_DEVICE_ATTRIBUTE_COOPERATIVE_LAUNCH = 95,                /**< Device supports launching cooperative kernels via ::cuLaunchCooperativeKernel */
    CU_DEVICE_ATTRIBUTE_COOPERATIVE_MULTI_DEVICE_LAUNCH = 96,   /**< Device can participate in cooperative kernels launched via ::cuLaunchCooperativeKernelMultiDevice */
    CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK_OPTIN = 97, /**< Maximum optin shared memory per block */
    CU_DEVICE_ATTRIBUTE_CAN_FLUSH_REMOTE_WRITES = 98,           /**< Both the ::CU_STREAM_WAIT_VALUE_FLUSH flag and the ::CU_STREAM_MEM_OP_FLUSH_REMOTE_WRITES MemOp are supported on the device. See \ref CUDA_MEMOP for additional details. */
    CU_DEVICE_ATTRIBUTE_HOST_REGISTER_SUPPORTED = 99,           /**< Device supports host memory registration via ::cudaHostRegister. */
    CU_DEVICE_ATTRIBUTE_PAGEABLE_MEMORY_ACCESS_USES_HOST_PAGE_TABLES = 100, /**< Device accesses pageable memory via the host's page tables. */
    CU_DEVICE_ATTRIBUTE_DIRECT_MANAGED_MEM_ACCESS_FROM_HOST = 101, /**< The host can directly access managed memory on the device without migration. */
    CU_DEVICE_ATTRIBUTE_MAX
} CUdevice_attribute;

#define CU_STREAM_NON_BLOCKING 1
#define CU_EVENT_BLOCKING_SYNC 1
#define CU_EVENT_DISABLE_TIMING 2
#define CU_TRSA_OVERRIDE_FORMAT 0x01

typedef void CUDAAPI CUstreamCallback(CUstream hStream, CUresult status, void *userdata);

typedef CUresult CUDAAPI tcuInit(unsigned int Flags);
typedef CUresult CUDAAPI tcuDeviceGetCount(int *count);
typedef CUresult CUDAAPI tcuDeviceGet(CUdevice *device, int ordinal);
typedef CUresult CUDAAPI tcuDeviceGetName(char *name, int len, CUdevice dev);
typedef CUresult CUDAAPI tcuDeviceComputeCapability(int *major, int *minor, CUdevice dev);
typedef CUresult CUDAAPI tcuCtxCreate_v2(CUcontext *pctx, unsigned int flags, CUdevice dev);
typedef CUresult CUDAAPI tcuCtxSetLimit(CUlimit limit, size_t value);
typedef CUresult CUDAAPI tcuCtxPushCurrent_v2(CUcontext pctx);
typedef CUresult CUDAAPI tcuCtxPopCurrent_v2(CUcontext *pctx);
typedef CUresult CUDAAPI tcuCtxDestroy_v2(CUcontext ctx);
typedef CUresult CUDAAPI tcuMemAlloc_v2(CUdeviceptr *dptr, size_t bytesize);
typedef CUresult CUDAAPI tcuMemFree_v2(CUdeviceptr dptr);
typedef CUresult CUDAAPI tcuMemcpy2D_v2(const CUDA_MEMCPY2D *pcopy);
typedef CUresult CUDAAPI tcuMemcpy2DAsync_v2(const CUDA_MEMCPY2D *pcopy, CUstream hStream);
typedef CUresult CUDAAPI tcuGetErrorName(CUresult error, const char** pstr);
typedef CUresult CUDAAPI tcuGetErrorString(CUresult error, const char** pstr);

typedef CUresult CUDAAPI tcuStreamCreate(CUstream *phStream, unsigned int flags);
typedef CUresult CUDAAPI tcuStreamQuery(CUstream hStream);
typedef CUresult CUDAAPI tcuStreamSynchronize(CUstream hStream);
typedef CUresult CUDAAPI tcuStreamDestroy_v2(CUstream hStream);
typedef CUresult CUDAAPI tcuStreamAddCallback(CUstream hStream, CUstreamCallback *callback, void *userdata, unsigned int flags);
typedef CUresult CUDAAPI tcuEventCreate(CUevent *phEvent, unsigned int flags);
typedef CUresult CUDAAPI tcuEventDestroy_v2(CUevent hEvent);
typedef CUresult CUDAAPI tcuEventSynchronize(CUevent hEvent);
typedef CUresult CUDAAPI tcuEventQuery(CUevent hEvent);
typedef CUresult CUDAAPI tcuEventRecord(CUevent hEvent, CUstream hStream);

typedef CUresult CUDAAPI tcuGLGetDevices_v2(unsigned int* pCudaDeviceCount, CUdevice* pCudaDevices, unsigned int cudaDeviceCount, CUGLDeviceList deviceList);
typedef CUresult CUDAAPI tcuGraphicsGLRegisterImage(CUgraphicsResource* pCudaResource, GLuint image, GLenum target, unsigned int Flags);
typedef CUresult CUDAAPI tcuGraphicsUnregisterResource(CUgraphicsResource resource);
typedef CUresult CUDAAPI tcuGraphicsMapResources(unsigned int count, CUgraphicsResource* resources, CUstream hStream);
typedef CUresult CUDAAPI tcuGraphicsUnmapResources(unsigned int count, CUgraphicsResource* resources, CUstream hStream);
typedef CUresult CUDAAPI tcuGraphicsSubResourceGetMappedArray(CUarray* pArray, CUgraphicsResource resource, unsigned int arrayIndex, unsigned int mipLevel);

typedef CUresult CUDAAPI tcuGLRegisterBufferObject(GLuint buffer);
typedef CUresult CUDAAPI tcuGLUnregisterBufferObject(GLuint buffer);
typedef CUresult CUDAAPI tcuGLMapBufferObject(CUdeviceptr* dptr, size_t* size, GLuint buffer);
typedef CUresult CUDAAPI tcuGLMapBufferObjectAsync(CUdeviceptr* dptr, size_t* size, GLuint buffer, CUstream hStream);
typedef CUresult CUDAAPI tcuGLMapBufferObject_v2(CUdeviceptr *dptr, size_t *size,  GLuint buffer);
typedef CUresult CUDAAPI tcuGLMapBufferObjectAsync_v2(CUdeviceptr *dptr, size_t *size,  GLuint buffer, CUstream hStream);
typedef CUresult CUDAAPI tcuGLUnmapBufferObject(GLuint buffer);
typedef CUresult CUDAAPI tcuGLUnmapBufferObjectAsync(GLuint buffer, CUstream hStream);

typedef CUresult CUDAAPI tcuModuleLoadDataEx(CUmodule* module, const void* image, unsigned int numOptions, CUjit_option* options, void** optionValues);
typedef CUresult CUDAAPI tcuModuleUnload(CUmodule module);
typedef CUresult CUDAAPI tcuModuleGetFunction(CUfunction* hfunc, CUmodule hmod, const char* name);
typedef CUresult CUDAAPI tcuModuleGetGlobal(CUdeviceptr* dptr, size_t* bytes, CUmodule hmod, const char* name);
typedef CUresult CUDAAPI tcuModuleGetGlobal_v2(CUdeviceptr* dptr, size_t* bytes, CUmodule hmod, const char* name);
typedef CUresult CUDAAPI tcuModuleGetTexRef(CUtexref* pTexRef, CUmodule hmod, const char* name);
typedef CUresult CUDAAPI tcuLaunchKernel(CUfunction f, unsigned int  gridDimX, unsigned int  gridDimY, unsigned int  gridDimZ, unsigned int  blockDimX, unsigned int  blockDimY, unsigned int  blockDimZ, unsigned int  sharedMemBytes, CUstream hStream, void** kernelParams, void** extra);

typedef CUresult CUDAAPI tcuTexRefSetArray(CUtexref hTexRef, CUarray hArray, unsigned int Flags);
typedef CUresult CUDAAPI tcuTexRefSetFilterMode(CUtexref hTexRef, CUfilter_mode fm);

//more setting fore texref
typedef CUresult  CUDAAPI tcuTexRefSetAddressMode(CUtexref hTexRef, int dim, CUaddress_mode am);
typedef CUresult  CUDAAPI tcuTexRefSetFlags(CUtexref hTexRef, unsigned int Flags);

//graphic buffer related
typedef CUresult  CUDAAPI tcuTexRefSetAddress_v2(size_t *ByteOffset, CUtexref hTexRef, CUdeviceptr dptr, size_t bytes);
typedef CUresult  CUDAAPI tcuTexRefSetAddress2D_v2(CUtexref hTexRef, const CUDA_ARRAY_DESCRIPTOR *desc, CUdeviceptr dptr, size_t Pitch);
typedef CUresult CUDAAPI tcuGraphicsGLRegisterBuffer(CUgraphicsResource *pCudaResource, GLuint buffer, unsigned int Flags);
typedef CUresult CUDAAPI tcuGraphicsResourceGetMappedPointer_v2(CUdeviceptr *pDevPtr, size_t *pSize, CUgraphicsResource resource);

//more driver info
typedef CUresult  CUDAAPI tcuDeviceGetAttribute(int *pi, CUdevice_attribute attrib, CUdevice dev);

#endif
