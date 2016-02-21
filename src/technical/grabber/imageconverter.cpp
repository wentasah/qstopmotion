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

#include "imageconverter.h"

#include <memory.h>   // memset

/******************************************************************************
 * Destination Format is abgr32
 *
 * Memory layout abgr32:
 *   +----+----+----+----+  +----+----+----+----+
 *   | B0 | G0 | R0 | A0 |  | B1 | G1 | R1 | A1 |
 *   +----+----+----+----+  +----+----+----+----+
 *
 ******************************************************************************/

/******************************************************************************
 * Convert RGB 24bit to XBGR 32bit
 ******************************************************************************/

/*
 * Memory layout rgb24:
 *   +----+----+----+  +----+----+----+
 *   | R0 | G0 | B0 |  | R1 | G1 | B1 |
 *   +----+----+----+  +----+----+----+
 */
int convert_rgb24_to_xbgr32_buffer(unsigned char *rgb24, unsigned char *xbgr32, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned int  in = 0;
    unsigned int  inEnd;
    unsigned int  out = 0;
    bool          upsideDown = (stride < 0);
    int           r;
    int           g;
    int           b;

    if (!upsideDown) {
        for (in = 0; in < bufferLength; in += 3) {
            r = rgb24[in + 0];
            g = rgb24[in + 1];
            b = rgb24[in + 2];

            xbgr32[out++] = b;      // B
            xbgr32[out++] = g;      // G
            xbgr32[out++] = r;      // R
            xbgr32[out++] = 0xffU;  // A
        }
    }
    else {
        stride = -stride;
        if (stride != (width * 3)) {
            return 0;
        }
        for (int scanLine = height - 1; scanLine >= 0; --scanLine) {
            in = scanLine * stride;
            inEnd = in + width * 3;
            for (; in < inEnd; in += 3) {
                r = rgb24[in + 0];
                g = rgb24[in + 1];
                b = rgb24[in + 2];

                xbgr32[out++] = b;      // B
                xbgr32[out++] = g;      // G
                xbgr32[out++] = r;      // R
                xbgr32[out++] = 0xffU;  // A
            }
        }
    }

    return 0;
}

/******************************************************************************
 * Convert BGR 24bit to XBGR 32bit
 ******************************************************************************/

/*
 * Memory layout bgr24:
 *   +----+----+----+  +----+----+----+
 *   | B0 | G0 | R0 |  | B1 | G1 | R1 |
 *   +----+----+----+  +----+----+----+
 */
int convert_bgr24_to_xbgr32_buffer(unsigned char *bgr24, unsigned char *xbgr32, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned int  in = 0;
    unsigned int  inEnd;
    unsigned int  out = 0;
    bool          upsideDown = (stride < 0);
    int           r;
    int           g;
    int           b;

    if (!upsideDown) {
        for (in = 0; in < bufferLength; in += 3) {
            b = bgr24[in + 0];
            g = bgr24[in + 1];
            r = bgr24[in + 2];

            xbgr32[out++] = b;      // B
            xbgr32[out++] = g;      // G
            xbgr32[out++] = r;      // R
            xbgr32[out++] = 0xffU;  // A
        }
    }
    else {
        stride = -stride;
        if (stride != (width * 3)) {
            return 0;
        }
        for (int scanLine = height - 1; scanLine >= 0; --scanLine) {
            in = scanLine * stride;
            inEnd = in + width * 3;
            for (; in < inEnd; in += 3) {
                b = bgr24[in + 0];
                g = bgr24[in + 1];
                r = bgr24[in + 2];

                xbgr32[out++] = b;      // B
                xbgr32[out++] = g;      // G
                xbgr32[out++] = r;      // R
                xbgr32[out++] = 0xffU;  // A
            }
        }
    }

    return 0;
}

/******************************************************************************
 * Convert YUV to XBGR 32bit
 ******************************************************************************/

/*
 * Converting YUV to RGB for Full Range
 *
 * Calculation matrix:
 *   -   -   -                     -   -           -
 *   | R |   | 1.000  0.000  1.400 |   | (y -   0) |
 *   | G | = | 1.000 -0.343 -0.711 | * | (u - 128) |
 *   | B |   | 1.000  1.765  0.000 |   | (v - 128) |
 *   -   -   -                     -   -           -
 *
 * Ranges:
 *   Y     [ 0...255]
 *   U/V   [ 0...255]
 *   R/G/B [ 0...255]
 */

static const int FrPrecision     = 32768;
static const int FrCoefficientY  = (int)( 1.000 * FrPrecision + 0.5);
static const int FrCoefficientRV = (int)( 1.400 * FrPrecision + 0.5);
static const int FrCoefficientGU = (int)(-0.343 * FrPrecision + 0.5);
static const int FrCoefficientGV = (int)(-0.711 * FrPrecision + 0.5);
static const int FrCoefficientBU = (int)( 1.765 * FrPrecision + 0.5);

void convert_yuv_to_rgb_FR_pixel(int y, int u, int v, int& r, int& g, int& b)
{
    r = (int)(FrCoefficientY * y + FrCoefficientRV * (v - 128));
    g = (int)(FrCoefficientY * y + FrCoefficientGU * (u - 128) + FrCoefficientGV * (v - 128));
    b = (int)(FrCoefficientY * y + FrCoefficientBU * (u - 128));

    r = (r + FrPrecision / 2) / FrPrecision;
    g = (g + FrPrecision / 2) / FrPrecision;
    b = (b + FrPrecision / 2) / FrPrecision;

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
}


/*
 * Converting YUV to RGB for SDTV
 *
 * Calculation matrix:
 *   -   -   -                     -   -           -
 *   | R |   | 1.164  0.000  1.596 |   | (y -  16) |
 *   | G | = | 1.164 -0.392 -0.813 | * | (u - 128) |
 *   | B |   | 1.164  2.017  0.000 |   | (v - 128) |
 *   -   -   -                     -   -           -
 *
 * Ranges:
 *   Y     [16...235]
 *   u/v   [16...240]
 *   R/G/B [ 0...255]
 */

static const int SdtvPrecision     = 32768;
static const int SdtvCoefficientY  = (int)( 1.164 * SdtvPrecision + 0.5);
static const int SdtvCoefficientRV = (int)( 1.596 * SdtvPrecision + 0.5);
static const int SdtvCoefficientGU = (int)(-0.391 * SdtvPrecision + 0.5);
static const int SdtvCoefficientGV = (int)(-0.813 * SdtvPrecision + 0.5);
static const int SdtvCoefficientBU = (int)( 2.018 * SdtvPrecision + 0.5);

void convert_yuv_to_rgb_SDTV_pixel(int y, int u, int v, int& r, int& g, int& b)
{
    r = (int)(SdtvCoefficientY * (y - 16)                                 + SdtvCoefficientRV * (v - 128));
    g = (int)(SdtvCoefficientY * (y - 16) + SdtvCoefficientGU * (u - 128) + SdtvCoefficientGV * (v - 128));
    b = (int)(SdtvCoefficientY * (y - 16) + SdtvCoefficientBU * (u - 128));

    r = (r + SdtvPrecision / 2) / SdtvPrecision;
    g = (g + SdtvPrecision / 2) / SdtvPrecision;
    b = (b + SdtvPrecision / 2) / SdtvPrecision;

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
}

/*
 * Converting YUV to RGB for HDTV
 *
 * Calculation matrix:
 *   -   -   -                     -   -           -
 *   | R |   | 1.164  0.000  1.793 |   | (y -  16) |
 *   | G | = | 1.164 -0.213 -0.533 | * | (u - 128) |
 *   | B |   | 1.164  2.112  0.000 |   | (v - 128) |
 *   -   -   -                     -   -           -
 *
 * Ranges:
 *   Y     [16...235]
 *   u/v   [16...240]
 *   R/G/B [ 0...255]
 */

static const int HdtvPrecision     = 32768;
static const int HdtvCoefficientY  = (int)( 1.164 * HdtvPrecision + 0.5);
static const int HdtvCoefficientRV = (int)( 1.793 * HdtvPrecision + 0.5);
static const int HdtvCoefficientGU = (int)(-0.213 * HdtvPrecision + 0.5);
static const int HdtvCoefficientGV = (int)(-0.533 * HdtvPrecision + 0.5);
static const int HdtvCoefficientBU = (int)( 2.112 * HdtvPrecision + 0.5);

void convert_yuv_to_rgb_HDTV_pixel(int y, int u, int v, int& r, int& g, int& b)
{
    r = (int)(HdtvCoefficientY * (y - 16) + HdtvCoefficientRV * (v - 128));
    g = (int)(HdtvCoefficientY * (y - 16) + HdtvCoefficientGU * (u - 128) + HdtvCoefficientGV * (v - 128));
    b = (int)(HdtvCoefficientY * (y - 16) + HdtvCoefficientBU * (u - 128));

    r = (r + HdtvPrecision / 2) / HdtvPrecision;
    g = (g + HdtvPrecision / 2) / HdtvPrecision;
    b = (b + HdtvPrecision / 2) / HdtvPrecision;

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
}


/*
 * Memory layout ayuv:
 *   +----+----+----+----+  +----+----+----+----+
 *   | V0 | U0 | Y0 | A0 |  | V1 | U1 | Y1 | A1 |
 *   +----+----+----+----+  +----+----+----+----+
 */
int convert_ayuv_to_abgr32_buffer(unsigned char *ayuv, unsigned char *abgr32, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned int  in = 0;
    unsigned int  out = 0;
    int           v;
    int           u;
    int           y;
    int           a;
    int           r;
    int           g;
    int           b;
    bool          upsideDown = (stride < 0);

    if (!upsideDown) {
        for (in = 0; in < width * height * 2; in += 4) {
            v = ayuv[in + 0];
            u = ayuv[in + 1];
            y = ayuv[in + 2];
            a = ayuv[in + 3];

            convert_yuv_to_rgb_SDTV_pixel(y, u, v, r, g, b);

            abgr32[out++] = b;      // B
            abgr32[out++] = g;      // G
            abgr32[out++] = r;      // R
            abgr32[out++] = 0xffU;  // A
        }
    }
    else {
        // Not supported
        return -1;
    }

    return 0;
}


/*
 * Memory layout yuv2:
 *   +----+----+----+----+  +----+----+----+----+
 *   | Y0 | U0 | Y1 | V0 |  | Y2 | U1 | Y3 | V1 |
 *   +----+----+----+----+  +----+----+----+----+
 */
int convert_yuy2_to_xbgr32_buffer(unsigned char *yuy2, unsigned char *xbgr32, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned int  in = 0;
    unsigned int  out = 0;
    int           y0;
    int           u;
    int           y1;
    int           v;
    int           r;
    int           g;
    int           b;
    bool          upsideDown = (stride < 0);

    if (!upsideDown) {
        for (in = 0; in < width * height * 2; in += 4) {
            y0 = yuy2[in + 0];
            u  = yuy2[in + 1];
            y1 = yuy2[in + 2];
            v  = yuy2[in + 3];

            convert_yuv_to_rgb_SDTV_pixel(y0, u, v, r, g, b);

            xbgr32[out++] = b;      // B
            xbgr32[out++] = g;      // G
            xbgr32[out++] = r;      // R
            xbgr32[out++] = 0xffU;  // A

            convert_yuv_to_rgb_SDTV_pixel(y1, u, v, r, g, b);

            xbgr32[out++] = b;      // B
            xbgr32[out++] = g;      // G
            xbgr32[out++] = r;      // R
            xbgr32[out++] = 0xffU;  // A
        }
    }
    else {
        // Not supported
        return -1;
    }

    return 0;
}


/*
 * Memory layout uyvy:
 *   +----+----+----+----+  +----+----+----+----+
 *   | U0 | Y0 | V0 | Y1 |  | U1 | Y2 | V1 | Y3 |
 *   +----+----+----+----+  +----+----+----+----+
 */
int convert_uyvy_to_xbgr32_buffer(unsigned char *uyvy, unsigned char *xbgr32, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned int  in = 0;
    unsigned int  out = 0;
    int           y0;
    int           u;
    int           y1;
    int           v;
    int           r;
    int           g;
    int           b;
    bool          upsideDown = (stride < 0);

    if (!upsideDown) {
        for (in = 0; in < width * height * 2; in += 4) {
            u  = uyvy[in + 0];
            y0 = uyvy[in + 1];
            v  = uyvy[in + 2];
            y1 = uyvy[in + 3];

            convert_yuv_to_rgb_SDTV_pixel(y0, u, v, r, g, b);

            xbgr32[out++] = b;      // B
            xbgr32[out++] = g;      // G
            xbgr32[out++] = r;      // R
            xbgr32[out++] = 0xffU;  // A

            convert_yuv_to_rgb_SDTV_pixel(y1, u, v, r, g, b);

            xbgr32[out++] = b;      // B
            xbgr32[out++] = g;      // G
            xbgr32[out++] = r;      // R
            xbgr32[out++] = 0xffU;  // A
        }
    }
    else {
        // Not supported
        return -1;
    }

    return 0;
}


/*
 * Memory layout yv12:
 *   +----+----+----+----+----+----+----+----+
 *   | Y0 | Y1 | Y2 | Y3 |
 *   +----+----+----+----+
 *   |
 *   |
 *   |
 *   +----+----+----+----+----+----+----+----+
 *   | V0 | V1 |
 *   +----+----+
 *   |
 *   |
 *   |
 *   +----+----+----+----+----+----+----+----+
 *   | U0 | U1 |
 *   +----+----+
 *   |
 *   |
 *   |
 *   +----+----+----+----+----+----+----+----+
 */
int convert_yv12_to_xbgr32_buffer(unsigned char *yv12, unsigned char *xbgr32, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned int  yPos1, yPos2;
    unsigned int  uPos;
    unsigned int  vPos;
    int           y0, y1, y2, y3;
    int           u;
    int           v;
    int           r;
    int           g;
    int           b;
    int           outPos1;
    int           outPos2;
    bool          upsideDown = (stride < 0);

    if (!upsideDown) {
        for (unsigned int h = 0; h < height; h = h + 2) {
            for (unsigned int w = 0; w < width; w = w + 2) {
                yPos1 = h * width + w;
                yPos2 = (h + 1) * width + w;
                vPos = (height * width) + ((h / 2) * (width / 2) + (w / 2));
                uPos = vPos + (height * width / 4);
                outPos1 = (h * width + w) * 4;
                outPos2 = ((h + 1) * width + w) * 4;

                y0 = yv12[yPos1];
                y1 = yv12[yPos1+1];
                y2 = yv12[yPos2];
                y3 = yv12[yPos2+1];
                u = yv12[uPos];
                v = yv12[vPos];

                convert_yuv_to_rgb_SDTV_pixel(y0, u, v, r, g, b);

                xbgr32[outPos1++] = b;      // B
                xbgr32[outPos1++] = g;      // G
                xbgr32[outPos1++] = r;      // R
                xbgr32[outPos1++] = 0xffU;  // A

                convert_yuv_to_rgb_SDTV_pixel(y1, u, v, r, g, b);

                xbgr32[outPos1++] = b;      // B
                xbgr32[outPos1++] = g;      // G
                xbgr32[outPos1++] = r;      // R
                xbgr32[outPos1++] = 0xffU;  // A

                convert_yuv_to_rgb_SDTV_pixel(y2, u, v, r, g, b);

                xbgr32[outPos2++] = b;      // B
                xbgr32[outPos2++] = g;      // G
                xbgr32[outPos2++] = r;      // R
                xbgr32[outPos2++] = 0xffU;  // A

                convert_yuv_to_rgb_SDTV_pixel(y3, u, v, r, g, b);

                xbgr32[outPos2++] = b;      // B
                xbgr32[outPos2++] = g;      // G
                xbgr32[outPos2++] = r;      // R
                xbgr32[outPos2++] = 0xffU;  // A
            }
        }
    }
    else {
        // Not supported
        return -1;
    }

    return 0;
}


/*
 * Memory layout i420:
 *   +----+----+----+----+----+----+----+----+
 *   | Y0 | Y1 | Y2 | Y3 |
 *   +----+----+----+----+
 *   |
 *   |
 *   |
 *   +----+----+----+----+----+----+----+----+
 *   | U0 | U1 |
 *   +----+----+
 *   |
 *   |
 *   |
 *   +----+----+----+----+----+----+----+----+
 *   | V0 | V1 |
 *   +----+----+
 *   |
 *   |
 *   |
 *   +----+----+----+----+----+----+----+----+
 */
int convert_i420_to_xbgr32_buffer(unsigned char *i420, unsigned char *xbgr32, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned int  yPos1, yPos2;
    unsigned int  uPos;
    unsigned int  vPos;
    int           y0, y1, y2, y3;
    int           u;
    int           v;
    int           r;
    int           g;
    int           b;
    int           outPos1;
    int           outPos2;
    bool          upsideDown = (stride < 0);

    if (!upsideDown) {
        for (unsigned int h = 0; h < height; h = h + 2) {
            for (unsigned int w = 0; w < width; w = w + 2) {
                yPos1 = h * width + w;
                yPos2 = (h + 1) * width + w;
                uPos = (height * width) + ((h / 2) * (width / 2) + (w / 2));
                vPos = uPos + (height * width / 4);
                outPos1 = (h * width + w) * 4;
                outPos2 = ((h + 1) * width + w) * 4;

                y0 = i420[yPos1];
                y1 = i420[yPos1+1];
                y2 = i420[yPos2];
                y3 = i420[yPos2+1];
                u = i420[uPos];
                v = i420[vPos];

                convert_yuv_to_rgb_SDTV_pixel(y0, u, v, r, g, b);

                xbgr32[outPos1++] = b;      // B
                xbgr32[outPos1++] = g;      // G
                xbgr32[outPos1++] = r;      // R
                xbgr32[outPos1++] = 0xffU;  // A

                convert_yuv_to_rgb_SDTV_pixel(y1, u, v, r, g, b);

                xbgr32[outPos1++] = b;      // B
                xbgr32[outPos1++] = g;      // G
                xbgr32[outPos1++] = r;      // R
                xbgr32[outPos1++] = 0xffU;  // A

                convert_yuv_to_rgb_SDTV_pixel(y2, u, v, r, g, b);

                xbgr32[outPos2++] = b;      // B
                xbgr32[outPos2++] = g;      // G
                xbgr32[outPos2++] = r;      // R
                xbgr32[outPos2++] = 0xffU;  // A

                convert_yuv_to_rgb_SDTV_pixel(y3, u, v, r, g, b);

                xbgr32[outPos2++] = b;      // B
                xbgr32[outPos2++] = g;      // G
                xbgr32[outPos2++] = r;      // R
                xbgr32[outPos2++] = 0xffU;  // A
            }
        }
    }
    else {
        // Not supported
        return -1;
    }

    return 0;
}


/*
 * Memory layout nv12:
 *   +----+----+----+----+----+----+----+----+
 *   | Y0 | Y1 | Y2 | Y3 |
 *   +----+----+----+----+
 *   |
 *   |
 *   |
 *   +----+----+----+----+----+----+----+----+
 *   | U0 | V0 | U1 | V1 |
 *   +----+----+----+----+
 *   |
 *   |
 *   |
 *   +----+----+----+----+----+----+----+----+
 */
int convert_nv12_to_xbgr32_buffer(unsigned char *nv12, unsigned char *xbgr32, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned int  yPos1, yPos2;
    unsigned int  uPos;
    unsigned int  vPos;
    int           y0, y1, y2, y3;
    int           u;
    int           v;
    int           r;
    int           g;
    int           b;
    int           outPos1;
    int           outPos2;
    bool          upsideDown = (stride < 0);

    if (!upsideDown) {
        for (unsigned int h = 0; h < height; h = h + 2) {
            for (unsigned int w = 0; w < width; w = w + 2) {
                yPos1 = h * width + w;
                yPos2 = (h + 1) * width + w;
                uPos = (height * width) + ((h / 2) * (width / 2) + w);
                vPos = uPos + 1;
                outPos1 = (h * width + w) * 4;
                outPos2 = ((h + 1) * width + w) * 4;

                y0 = nv12[yPos1];
                y1 = nv12[yPos1+1];
                y2 = nv12[yPos2];
                y3 = nv12[yPos2+1];
                u = nv12[uPos];
                v = nv12[vPos];

                convert_yuv_to_rgb_SDTV_pixel(y0, u, v, r, g, b);

                xbgr32[outPos1++] = b;      // B
                xbgr32[outPos1++] = g;      // G
                xbgr32[outPos1++] = r;      // R
                xbgr32[outPos1++] = 0xffU;  // A

                convert_yuv_to_rgb_SDTV_pixel(y1, u, v, r, g, b);

                xbgr32[outPos1++] = b;      // B
                xbgr32[outPos1++] = g;      // G
                xbgr32[outPos1++] = r;      // R
                xbgr32[outPos1++] = 0xffU;  // A

                convert_yuv_to_rgb_SDTV_pixel(y2, u, v, r, g, b);

                xbgr32[outPos2++] = b;      // B
                xbgr32[outPos2++] = g;      // G
                xbgr32[outPos2++] = r;      // R
                xbgr32[outPos2++] = 0xffU;  // A

                convert_yuv_to_rgb_SDTV_pixel(y3, u, v, r, g, b);

                xbgr32[outPos2++] = b;      // B
                xbgr32[outPos2++] = g;      // G
                xbgr32[outPos2++] = r;      // R
                xbgr32[outPos2++] = 0xffU;  // A
            }
        }
    }
    else {
        // Not supported
        return -1;
    }

    return 0;
}

/******************************************************************************
 * Convert RGB 8bit to YUV
 ******************************************************************************/

int convert_rgb_to_yuv_pixel(int r8, int g8, int b8)
{
    unsigned int pixel_32 = 0;
    unsigned char *pixel = (unsigned char *)&pixel_32;
    int y2, u2, v2;

    y2 = (int)(  0.299 * (r8 - 128) + 0.587 * (g8 - 128) + 0.114 * (b8 - 128) + 128);
    u2 = (int)(- 0.147 * (r8 - 128) - 0.289 * (g8 - 128) + 0.436 * (b8 - 128) + 128);
    v2 = (int)(  0.615 * (r8 - 128) - 0.515 * (g8 - 128) - 0.100 * (b8 - 128) + 128);

    if (y2 > 255) y2 = 255;
    if (u2 > 255) u2 = 255;
    if (v2 > 255) v2 = 255;

    if (y2 < 0) y2 = 0;
    if (u2 < 0) u2 = 0;
    if (v2 < 0) v2 = 0;

    pixel[0] = y2;
    pixel[1] = u2;
    pixel[2] = v2;

    return pixel_32;
}


int convert_rgb24_to_yuy2_buffer(unsigned char *rgb24, unsigned char *yuy2, unsigned int width, unsigned int height)
{
    unsigned int in, out = 0;
    unsigned int pixel_32;
    int y0, u0, v0, y1, u1, v1;

    for(in = 0; in < width * height * 3; in += 6) {
        pixel_32 = convert_rgb_to_yuv_pixel(rgb24[in], rgb24[in + 1], rgb24[in + 2]);
        y0 = (pixel_32 & 0x000000ff);
        u0 = (pixel_32 & 0x0000ff00) >>  8;
        v0 = (pixel_32 & 0x00ff0000) >> 16;

        pixel_32 = convert_rgb_to_yuv_pixel(rgb24[in + 3], rgb24[in + 4], rgb24[in + 5]);
        y1 = (pixel_32 & 0x000000ff);
        u1 = (pixel_32 & 0x0000ff00) >>  8;
        v1 = (pixel_32 & 0x00ff0000) >> 16;

        yuy2[out++] = y0;
        yuy2[out++] = (u0 + u1) / 2;
        yuy2[out++] = y1;
        yuy2[out++] = (v0 + v1) / 2;
    }

    return 0;
}

/******************************************************************************
 * Convert Motion-JPEG to JPEG
 ******************************************************************************/

unsigned char jpeg_dht[JPEG_DHT_LENGTH] = {
    0xff, 0xc4, 0x01, 0xa2,

    0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,

    0x01, 0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,

    0x10, 0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01,
    0x7d, 0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61,
    0x07, 0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1,
    0xf0, 0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
    0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
    0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4,
    0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1,
    0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9, 0xfa, 0x11,

    0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77,
    0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
    0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
    0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
    0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
    0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
    0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
    0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
    0xf9, 0xfa
};

int mjpegToJpeg(unsigned char *inFrame, unsigned char *outFrame, unsigned int bufSize)
{
    unsigned int j = 0, has_dht = 0;

    outFrame[0] = inFrame[0];
    outFrame[1] = inFrame[1];

    if((inFrame[0] != 0xff) && (inFrame[1] != 0xd8))
    {
        // printf("Not a JPEG nor MJPEG file \nExiting.\n");
        return -1;
    }

    unsigned int pos = 2, size = 0; // move into the image's table

    while (has_dht == 0)
    {
        if (inFrame[pos] != 0xff)
        {
            // printf("This isn't a valid JPEG file.\n");
            return -1;
        }

        if (inFrame[pos+1] == 0xc4)
        {
            has_dht = 1;
        }
        else if (inFrame[pos+1] == 0xda) // Add the huffman tables here...
        {
            has_dht = 0;
            break;
        }

        // Skip to the next marker
        size = (inFrame[pos+2] << 8) + inFrame[pos+3];
        for (j = 0; j < size + 2; j++)
        {
            outFrame[pos + j] = inFrame[pos + j];
        }
        pos = pos + size + 2;
    }

    j = 0;

    if (has_dht == 0)
    {
        for ( ; j < 420 ; j++)
        {
            outFrame[pos + j] = jpeg_dht[j];
        }

        for ( ; pos < bufSize ; pos++)
        {
            outFrame[pos + 420] = inFrame[pos];
        }

    }
    else
    {
        for ( ; pos < bufSize ; pos++)
        {
            outFrame[pos] = inFrame[pos];
        }
    }

    return 0;
}
