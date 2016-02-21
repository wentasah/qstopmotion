/******************************************************************************
 *  Copyright (C) 2010-2016 by                                                *
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
 ******************************************************************************/

#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

/******************************************************************************
 * Convert RGB 24bit to XBGR 32bit
 ******************************************************************************/

int convert_rgb24_to_xbgr32_buffer(unsigned char *rgb24,
                                   unsigned char *xbgr32,
                                   unsigned int   width,
                                   unsigned int   height,
                                   unsigned long  bufferLength,
                                   long           stride);

/******************************************************************************
 * Convert BGR 24bit to XBGR 32bit
 ******************************************************************************/

int convert_bgr24_to_xbgr32_buffer(unsigned char *bgr24,
                                   unsigned char *xbgr32,
                                   unsigned int   width,
                                   unsigned int   height,
                                   unsigned long  bufferLength,
                                   long           stride);

/******************************************************************************
 * Convert YUV to XBGR 32bit
 ******************************************************************************/

/**
 * Convert one pixel off YUV to RGB 8bit for full range.
 *
 * @brief convert_yuv_to_rgb_FR_pixel
 * @param y
 * @param u
 * @param v
 * @param r
 * @param g
 * @param b
 * @return
 */
void convert_yuv_to_rgb_FR_pixel(int y, int u, int v, int& r, int& g, int& b);

/**
 * Convert one pixel off YUV to RGB 8bit for SDTV.
 *
 * @brief convert_yuv_to_rgb8_SDTV_pixel
 * @param y
 * @param u
 * @param v
 * @param r
 * @param g
 * @param b
 * @return
 */
void convert_yuv_to_rgb_SDTV_pixel(int y, int u, int v, int& r, int& g, int& b);

/**
 * Convert one pixel off YUV to RGB 8bit for HDTV.
 *
 * @brief convert_yuv_to_rgb8_HDTV_pixel
 * @param y
 * @param u
 * @param v
 * @param r
 * @param g
 * @param b
 * @return
 */
int convert_yuv_to_rgb8_HDTV_pixel(int y, int u, int v);

/**
 * Convert a buffer with YUV 4:4:4 (Microsoft AYUV) pixels to ABGR 32bit.
 *
 * @brief convert_ayuv_to_abgr32_buffer
 * @param ayuv
 * @param abgr32
 * @param width
 * @param height
 * @param bufferLength
 * @param stride
 * @return
 */
int convert_ayuv_to_abgr32_buffer(unsigned char *ayuv,
                                  unsigned char *abgr32,
                                  unsigned int   width,
                                  unsigned int   height,
                                  unsigned long  bufferLength,
                                  long           stride);

/**
 * Convert a buffer with YUV 4:2:2 (Microsoft YUY2) pixels to XBGR 32bit.
 *
 * @brief convert_yuy2_to_xbgr32_buffer
 * @param yuy2
 * @param xbgr32
 * @param width
 * @param height
 * @param bufferLength
 * @param stride
 * @return
 */
int convert_yuy2_to_xbgr32_buffer(unsigned char *yuy2,
                                  unsigned char *xbgr32,
                                  unsigned int   width,
                                  unsigned int   height,
                                  unsigned long  bufferLength,
                                  long           stride);

/**
 * Convert a buffer with YUV 4:2:2 (Microsoft UYVY) pixels to XBGR 32bit.
 *
 * @brief convert_uyvy_to_xbgr32_buffer
 * @param uyvy
 * @param xbgr32
 * @param width
 * @param height
 * @param bufferLength
 * @param stride
 * @return
 */
int convert_uyvy_to_xbgr32_buffer(unsigned char *uyvy,
                                  unsigned char *xbgr32,
                                  unsigned int   width,
                                  unsigned int   height,
                                  unsigned long  bufferLength,
                                  long           stride);

/**
 * Convert a buffer with YUV 4:2:0 (Microsoft YV12) pixels to XBGR 32bit.
 *
 * @brief convert_yv12_to_xbgr32_buffer
 * @param yv12
 * @param xbgr32
 * @param width
 * @param height
 * @param bufferLength
 * @param stride
 * @return
 */
int convert_yv12_to_xbgr32_buffer(unsigned char *yv12,
                                  unsigned char *xbgr32,
                                  unsigned int   width,
                                  unsigned int   height,
                                  unsigned long  bufferLength,
                                  long           stride);

/**
 * Convert a buffer with YUV 4:2:0 (Microsoft NV12) pixels to XBGR 32bit.
 *
 * @brief convert_nv12_to_xbgr32_buffer
 * @param nv12
 * @param xbgr32
 * @param width
 * @param height
 * @param bufferLength
 * @param stride
 * @return
 */
int convert_nv12_to_xbgr32_buffer(unsigned char *nv12,
                                  unsigned char *xbgr32,
                                  unsigned int   width,
                                  unsigned int   height,
                                  unsigned long  bufferLength,
                                  long           stride);

/**
 * Convert a buffer with YUV 4:2:0 (Microsoft I420) pixels to XBGR 32bit.
 *
 * @brief convert_i420_to_xbgr32_buffer
 * @param i420
 * @param xbgr32
 * @param width
 * @param height
 * @param bufferLength
 * @param stride
 * @return
 */
int convert_i420_to_xbgr32_buffer(unsigned char *i420,
                                  unsigned char *rgb32,
                                  unsigned int   width,
                                  unsigned int   height,
                                  unsigned long  bufferLength,
                                  long           stride);

/******************************************************************************
 * Convert RGB 24bit to YUV
 ******************************************************************************/

int convert_rgb_to_yuv_pixel(int r8, int g8, int b8);
int convert_rgb24_to_yuy2_buffer(unsigned char *rgb24,
                                 unsigned char *yuy2,
                                 unsigned int   width,
                                 unsigned int   height);

/******************************************************************************
 * Convert Motion-JPEG to XBGR 32bit
 ******************************************************************************/

struct in {
    unsigned char *p;
    unsigned int bits;
    int omitescape;
    int left;
    int marker;
};

struct jpginfo {
    int nc;        // number of components
    int ns;        // number of scans
    int dri;       // restart interval
    int nm;        // mcus til next marker
    int rm;        // next restart marker
};

#define DECBITS 10      // seems to be the optimum
struct dec_hufftbl {
    int maxcode[17];
    int valptr[16];
    unsigned char vals[256];
    unsigned int llvals[1 << DECBITS];
};

struct enc_hufftbl;

union hufftblp {
    struct dec_hufftbl *dhuff;
    struct enc_hufftbl *ehuff;
};

struct scan {
    int dc;                  // old dc value
    union hufftblp hudc;     // pointer to huffman table dc
    union hufftblp huac;     // pointer to huffman table ac
    int next;                // when to switch to next scan
    int cid;                 // component id
    int hv;                  // horiz/vert, copied from comp
    int tq;                  // quant tbl, copied from comp
};

#define MAXCOMP 4
struct comp {
    int cid;
    int hv;
    int tq;
};

struct dec_data {
    struct in in;
    struct jpginfo info;
    struct comp comps[MAXCOMP];
    struct scan dscans[MAXCOMP];
    unsigned char quant[3][64];
    int dquant[3][64];
    // struct code_table_t table[256];
};

int convert_mjpeg411_to_xbgr32_buffer(unsigned char *mjpeg411,
                                      unsigned char *xbgr32,
                                      unsigned int width,
                                      unsigned int height /*,
                                      unsigned long bufferLength,
                                      long stride */);

int convert_mjpeg422_to_xbgr32_buffer(unsigned char *mjpeg422,
                                      unsigned char *xbgr32,
                                      unsigned int width,
                                      unsigned int height /*,
                                      unsigned long bufferLength,
                                      long stride */);

#define JPEG_DHT_LENGTH 0x1A4

int mjpegToJpeg(unsigned char *inFrame, unsigned char *outFrame, unsigned int bufSize);

#endif // IMAGECONVERTER_H
