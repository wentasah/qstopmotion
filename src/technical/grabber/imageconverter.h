/******************************************************************************
 *  Copyright (C) 2010-2014 by                                                *
 *    Ralf Lange (ralf.lange@longsoft.de)                                     *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation; either version 2 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the                             *
 *  Free Software Foundation, Inc.,                                           *
 *  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
 *                                                                            *
 *  Based on:                                                                 *
 *  Convert between YUY2 (YUYV) and RGB.                                      *
 *  Copyright 2007 (c) Logitech. All Rights Reserved.                         *
 ******************************************************************************/

#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

/******************************************************************************
 * Convert YUV to RGB 8bit
 ******************************************************************************/

/**
 * Convert one pixel off YUV to RGB 8bit for full range.
 *
 * @brief convert_yuv_to_rgb8_FR_pixel
 * @param y
 * @param u
 * @param v
 * @return
 */
int convert_yuv_to_rgb8_FR_pixel(int y, int u, int v);

/**
 * Convert one pixel off YUV to RGB 8bit for SDTV.
 *
 * @brief convert_yuv_to_rgb8_SDTV_pixel
 * @param y
 * @param u
 * @param v
 * @return
 */
int convert_yuv_to_rgb8_SDTV_pixel(int y, int u, int v);

/**
 * Convert one pixel off YUV to RGB 8bit for HDTV.
 *
 * @brief convert_yuv_to_rgb8_HDTV_pixel
 * @param y
 * @param u
 * @param v
 * @return
 */
int convert_yuv_to_rgb8_HDTV_pixel(int y, int u, int v);

/**
 * Convert a buffer with YUV 4:4:4 (Microsoft AYUV) pixels to RGB 8bit.
 *
 * @brief convert_ayuv_to_argb8_buffer
 * @param ayuv
 * @param argb8
 * @param width
 * @param height
 * @param bufferLength
 * @param stride
 * @return
 */
int convert_ayuv_to_argb8_buffer(unsigned char *ayuv,
                                 unsigned char *argb8,
                                 unsigned int   width,
                                 unsigned int   height,
                                 unsigned long  bufferLength,
                                 long           stride);

/**
 * Convert a buffer with YUV 4:2:2 (Microsoft YUY2) pixels to RGB 8bit.
 *
 * @brief convert_yuy2_to_argb8_buffer
 * @param yuy2
 * @param argb8
 * @param width
 * @param height
 * @param bufferLength
 * @param stride
 * @return
 */
int convert_yuy2_to_argb8_buffer(unsigned char *yuy2,
                                 unsigned char *argb8,
                                 unsigned int   width,
                                 unsigned int   height,
                                 unsigned long  bufferLength,
                                 long           stride);

/**
 * Convert a buffer with YUV 4:2:2 (Microsoft UYVY) pixels to RGB 8bit.
 *
 * @brief convert_uyvy_to_argb8_buffer
 * @param uyvy
 * @param argb8
 * @param width
 * @param height
 * @param bufferLength
 * @param stride
 * @return
 */
int convert_uyvy_to_argb8_buffer(unsigned char *uyvy,
                                 unsigned char *argb8,
                                 unsigned int   width,
                                 unsigned int   height,
                                 unsigned long  bufferLength,
                                 long           stride);

/**
 * Convert a buffer with YUV 4:2:0 (Microsoft YV12) pixels to RGB 8bit.
 *
 * @brief convert_yv12_to_argb8_buffer
 * @param yv12
 * @param argb8
 * @param width
 * @param height
 * @param bufferLength
 * @param stride
 * @return
 */
int convert_yv12_to_argb8_buffer(unsigned char *yv12,
                                 unsigned char *argb8,
                                 unsigned int   width,
                                 unsigned int   height,
                                 unsigned long  bufferLength,
                                 long           stride);

/**
 * Convert a buffer with YUV 4:2:0 (Microsoft NV12) pixels to RGB 8bit.
 *
 * @brief convert_nv12_to_argb8_buffer
 * @param nv12
 * @param argb8
 * @param width
 * @param height
 * @param bufferLength
 * @param stride
 * @return
 */
int convert_nv12_to_argb8_buffer(unsigned char *nv12,
                                 unsigned char *argb8,
                                 unsigned int   width,
                                 unsigned int   height,
                                 unsigned long  bufferLength,
                                 long           stride);

/**
 * Convert a buffer with YUV 4:2:0 (Microsoft I420) pixels to RGB 8bit.
 *
 * @brief convert_i420_to_argb8_buffer
 * @param i420
 * @param argb8
 * @param width
 * @param height
 * @param bufferLength
 * @param stride
 * @return
 */
int convert_i420_to_argb8_buffer(unsigned char *i420,
                                 unsigned char *argb8,
                                 unsigned int   width,
                                 unsigned int   height,
                                 unsigned long  bufferLength,
                                 long           stride);

// int convert_yuy2_to_rgb8_file(char *yuy2file, char *rgb8file, unsigned int width, unsigned int height);

/******************************************************************************
 * Convert RGB 8bit to YUV
 ******************************************************************************/

int convert_rgb8_to_yuv_pixel(int r8, int g8, int b8);
int convert_rgb8_to_yuy2_buffer(unsigned char *rgb8,
                                unsigned char *yuy2,
                                unsigned int width,
                                unsigned int height);
// int convert_rgb8_to_yuy2_file(char *rgb8file, char *yuy2file, unsigned int width, unsigned int height);

/******************************************************************************
 * Convert RGB 24bit to RGB 8bit
 ******************************************************************************/

int convert_rgb24_to_rgb8_pixel(int r24, int g24, int b24);
int convert_rgb24_to_argb8_buffer(unsigned char *rgb24,
                                  unsigned char *rgb8,
                                  unsigned int   width,
                                  unsigned int   height,
                                  unsigned long  bufferLength,
                                  long           stride);
// int convert_rgb24_to_rgb8_file(char *rgb24file, char *rgb8file, unsigned int width, unsigned int height);

#endif // IMAGECONVERTER_H
