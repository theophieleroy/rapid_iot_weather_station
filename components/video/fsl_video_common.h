/*
 * The Clear BSD License
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_VIDEO_COMMON_H_
#define _FSL_VIDEO_COMMON_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Pixel format FOURCC. */
#define FSL_VIDEO_FOURCC(a, b, c, d) \
    ((uint32_t)(a) | ((uint32_t)(b) << 8U) | ((uint32_t)(c) << 16U) | ((uint32_t)(d) << 24U))

/*! @brief Macro to define resolution. */
#define FSL_VIDEO_RESOLUTION(width, height) ((uint32_t)(width) | ((uint32_t)(height) << 16U))

#define FSL_VIDEO_EXTRACT_WIDTH(resolution) ((uint16_t)((resolution)&0xFFFFU))
#define FSL_VIDEO_EXTRACT_HEIGHT(resolution) ((uint16_t)((resolution) >> 16U))

/*! @brief Pixel format definition. */
typedef enum _video_pixel_format
{
    /* RGB */
    kVIDEO_PixelFormatXRGB8888 = FSL_VIDEO_FOURCC('X', 'R', '2', '4'), /*!< 32-bit XRGB8888. */
    kVIDEO_PixelFormatRGBX8888 = FSL_VIDEO_FOURCC('R', 'X', '2', '4'), /*!< 32-bit RGBX8888. */
    kVIDEO_PixelFormatXBGR8888 = FSL_VIDEO_FOURCC('X', 'B', '2', '4'), /*!< 32-bit XBGR8888. */
    kVIDEO_PixelFormatBGRX8888 = FSL_VIDEO_FOURCC('B', 'X', '2', '4'), /*!< 32-bit BGRX8888. */

    kVIDEO_PixelFormatRGB888 = FSL_VIDEO_FOURCC('R', 'G', '2', '4'), /*!< 24-bit RGB888. */
    kVIDEO_PixelFormatBGR888 = FSL_VIDEO_FOURCC('B', 'G', '2', '4'), /*!< 24-bit BGR888. */

    kVIDEO_PixelFormatRGB565 = FSL_VIDEO_FOURCC('R', 'G', '1', '6'), /*!< 16-bit RGB565. */
    kVIDEO_PixelFormatBGR565 = FSL_VIDEO_FOURCC('B', 'G', '1', '6'), /*!< 16-bit BGR565. */

    kVIDEO_PixelFormatXRGB1555 = FSL_VIDEO_FOURCC('X', 'R', '1', '5'), /*!< 16-bit XRGB1555. */
    kVIDEO_PixelFormatRGBX5551 = FSL_VIDEO_FOURCC('R', 'X', '1', '5'), /*!< 16-bit RGBX5551. */
    kVIDEO_PixelFormatXBGR1555 = FSL_VIDEO_FOURCC('X', 'B', '1', '5'), /*!< 16-bit XBGR1555. */
    kVIDEO_PixelFormatBGRX5551 = FSL_VIDEO_FOURCC('B', 'X', '1', '5'), /*!< 16-bit BGRX5551. */

    kVIDEO_PixelFormatXRGB4444 = FSL_VIDEO_FOURCC('X', 'R', '1', '2'), /*!< 16-bit XRGB4444. */
    kVIDEO_PixelFormatRGBX4444 = FSL_VIDEO_FOURCC('R', 'X', '1', '2'), /*!< 16-bit RGBX4444. */
    kVIDEO_PixelFormatXBGR4444 = FSL_VIDEO_FOURCC('X', 'B', '1', '2'), /*!< 16-bit XBGR4444. */
    kVIDEO_PixelFormatBGRX4444 = FSL_VIDEO_FOURCC('B', 'X', '1', '2'), /*!< 16-bit BGRX4444. */

    /* YUV. */
    kVIDEO_PixelFormatYUYV = FSL_VIDEO_FOURCC('Y', 'U', 'Y', 'V'), /*!< YUV422, Y-U-Y-V. */
    kVIDEO_PixelFormatYVYU = FSL_VIDEO_FOURCC('Y', 'V', 'Y', 'U'), /*!< YUV422, Y-V-Y-U. */
    kVIDEO_PixelFormatUYVY = FSL_VIDEO_FOURCC('U', 'Y', 'V', 'Y'), /*!< YUV422, U-Y-V-Y. */
    kVIDEO_PixelFormatVYUY = FSL_VIDEO_FOURCC('V', 'Y', 'U', 'Y'), /*!< YUV422, V-Y-U-Y. */

    kVIDEO_PixelFormatXYUV = FSL_VIDEO_FOURCC('X', 'Y', 'U', 'V'), /*!< YUV444, X-Y-U-V. */
    kVIDEO_PixelFormatXYVU = FSL_VIDEO_FOURCC('X', 'Y', 'V', 'U'), /*!< YUV444, X-Y-V-U. */
} video_pixel_format_t;

/*! @brief Resolution definition. */
typedef enum _video_resolution
{
    kVIDEO_ResolutionVGA = FSL_VIDEO_RESOLUTION(640, 480),     /*!< VGA, 640 * 480 */
    kVIDEO_ResolutionQVGA = FSL_VIDEO_RESOLUTION(320, 240),    /*!< QVGA, 320 * 240 */
    kVIDEO_ResolutionQQVGA = FSL_VIDEO_RESOLUTION(160, 120),   /*!< QQVGA, 160 * 120 */
    kVIDEO_ResolutionCIF = FSL_VIDEO_RESOLUTION(352, 288),     /*!< CIF, 352 * 288 */
    kVIDEO_ResolutionQCIF = FSL_VIDEO_RESOLUTION(176, 144),    /*!< QCIF, 176 * 144 */
    kVIDEO_ResolutionQQCIF = FSL_VIDEO_RESOLUTION(88, 72),     /*!< QQCIF, 88 * 72 */
    kVIDEO_Resolution720P = FSL_VIDEO_RESOLUTION(1280, 720),   /*!< 720P, 1280 * 720 */
    kVIDEO_Resolution1080P = FSL_VIDEO_RESOLUTION(1920, 1080), /*!< 1080P, 1920 * 1280*/
} video_resolution_t;

/*!
 * @brief Ring buffer structure.
 *
 * There is one empty room reserved in the ring buffer, used to distinguish
 * whether the ring buffer is full or empty. When rear equals front, it is empty;
 * when rear+1 equals front, it is full.
 */
typedef struct
{
    volatile uint32_t rear;  /*!< Pointer to save the incoming item. */
    volatile uint32_t front; /*!< Pointer to read out the item. */
    void *volatile *buf;     /*!< Memory to the ring buffer. */
    uint32_t size;           /*!< Ring buffer total size. */
} video_ringbuf_t;

/*!
 * @brief Memory pool structure.
 */
typedef struct
{
    void *volatile pool;   /*!< Pointer to the pool.         */
    volatile uint32_t cnt; /*!< Count of memory blocks in the pool. */
} video_mempool_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Common
 * @{
 */

/*!
 * @brief Check the pixel format is YUV or not.
 *
 * @param format Pixel format.
 */
bool VIDEO_IsYUV(video_pixel_format_t format);

/*!
 * @brief Delay the specific time.
 *
 * @param ms How many milli-second to delay.
 */
void VIDEO_DelayMs(uint32_t ms);

/* @} */

/*!
 * @name Ring buffer.
 * @{
 */

/*!
 * @brief Initializes ring buffer.
 *
 * @param ringbuf Pointer to the ring buffer handle.
 * @param buf Memory to save the items.
 * @param size Size of the @p buf.
 * @return Returns @ref kStatus_Success if initialize success, otherwise returns
 * error code.
 */
status_t VIDEO_RINGBUF_Init(video_ringbuf_t *ringbuf, void **buf, uint32_t size);

/*!
 * @brief Get one item from the ring buffer.
 *
 * @param ringbuf Pointer to the ring buffer handle.
 * @param item Memory to save the item.
 * @return Returns @ref kStatus_Success if get success, otherwise returns
 * error code.
 */
status_t VIDEO_RINGBUF_Get(video_ringbuf_t *ringbuf, void **item);

/*!
 * @brief Put one item to the ring buffer.
 *
 * @param ringbuf Pointer to the ring buffer handle.
 * @param item The new item to save.
 * @return Returns @ref kStatus_Success if put success, otherwise returns
 * error code.
 */
status_t VIDEO_RINGBUF_Put(video_ringbuf_t *ringbuf, void *item);

/*!
 * @brief Get current count of items in the ring buffer.
 *
 * @param ringbuf Pointer to the ring buffer handle.
 * @return Returns the item count.
 */
uint32_t VIDEO_RINGBUF_GetLength(video_ringbuf_t *ringbuf);

/*!
 * @brief Check whether the ring buffer is empty.
 *
 * @param ringbuf Pointer to the ring buffer handle.
 * @return Returns true if the ring buffer is empty, otherwise returns false.
 */
bool VIDEO_RINGBUF_IsEmpty(video_ringbuf_t *ringbuf);

/*!
 * @brief Check whether the ring buffer is full.
 *
 * @param ringbuf Pointer to the ring buffer handle.
 * @return Returns true if the ring buffer is full, otherwise returns false.
 */
bool VIDEO_RINGBUF_IsFull(video_ringbuf_t *ringbuf);
/* @} */

/*!
 * @name Memory Pool
 *
 * User can put memory block to the pool, or get memory block from the pool.
 * There is no count limitation to put memory block in to the pool. The memory
 * content in the pool might be modified.
 *
 * The memory block should be 4-byte aligned, and the dividable by 4-byte.
 *
 * @{
 */

/*!
 * @brief Initializes memory pool.
 *
 * Initializes memory pool. Initial memory blocks in the memory pool is optional.
 * If initial blocks are used, user should specify the initial block size and count.
 *
 * @param mempool Pointer to the memory pool handle.
 * @param initMem Initial memory blocks to saved in the pool.
 * @param size Every memory block's size (bytes) in the @p initMem.
 * @param count Number of memory blocks @p initMem.
 * @return Returns @ref kStatus_Success if initialize success, otherwise returns
 * error code.
 */
status_t VIDEO_MEMPOOL_Init(video_mempool_t *mempool, void *initMem, uint32_t size, uint32_t count);

/*!
 * @brief Put memory block in the pool.
 *
 * @param mempool Pointer to the memory pool handle.
 * @param mem Pointer to the memory block.
 */
void VIDEO_MEMPOOL_Put(video_mempool_t *mempool, void *mem);

/*!
 * @brief Get memory block in the pool.
 *
 * @param mempool Pointer to the memory pool handle.
 * @return The memory block get from pool. If the pool is empty, returns NULL.
 */
void *VIDEO_MEMPOOL_Get(video_mempool_t *mempool);

/*!
 * @brief How many memory blocks in the pool.
 *
 * @param mempool Pointer to the memory pool handle.
 * @return The memory block count in the pool
 */
uint32_t VIDEO_MEMPOOL_GetCount(video_mempool_t *mempool);

/* @} */

#if defined(__cplusplus)
}
#endif

#endif /* _FSL_VIDEO_COMMON_H_ */
