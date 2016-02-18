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
 * Convert Motion-JPEG to XBGR 32bit Variant 2
 ******************************************************************************/

/*
 * linux/drivers/video/fbcon-jpegdec.c - a tiny jpeg decoder.
 *
 * (w) August 2001 by Michael Schroeder, <mls@suse.de>
 *
 * I severly gutted this beast and hardcoded it to the palette and subset
 * of jpeg needed for the spca50x driver. Also converted it from K&R style
 * C to a more modern form ;). Michael can't be blamed for what is left.
 * All nice features are his, all bugs are mine. - till
 *
 * Change color space converter for YUVP and RGB -
 * Rework the IDCT implementation for best speed, cut test in the loop but instead
 * more copy and paste code :)
 * For more details about idct look at :
 * http://rnvs.informatik.tu-chemnitz.de/~jan/MPEG/HTML/IDCT.html
 * 12/12/2003 mxhaard@magic.fr
 *
 * add make jpeg from header (mxhaard 20/09/2004)
 * add jpeg_decode for 422 stream (mxhaard 01/10/2004)
 */

#define ISHIFT 11
#define M_RST0  0xd0
#define M_BADHUFF   -1

#define IFIX(a) ((long)((a) * (1 << ISHIFT) + .5))
#define CLIP(color) (unsigned char)(((color)>0xFF)?0xff:(((color)<0)?0:(color)))
#define IMULT(a, b) (((a) * (b)) >> ISHIFT)
#define ITOINT(a) ((a) >> ISHIFT)
#define LEBI_GET(in)    (le = in->left, bi = in->bits)
#define LEBI_PUT(in)    (in->left = le, in->bits = bi)

static int fillbits(struct in *in, int le, unsigned int bi)
{
    int b;
    int m;

    if (in->marker) {
        if (le <= 16) {
            in->bits = bi << 16, le += 16;
        }

        return le;
    }
    while (le <= 24) {
        b = *in->p++;
        if (in->omitescape) {
            if (b == 0xff && (m = *in->p++) != 0) {
                in->marker = m;
                if (le <= 16) {
                    bi = bi << 16, le += 16;
                }
                break;
            }
        }
        bi = bi << 8 | b;
        le += 8;
    }
    in->bits = bi;      // tmp... 2 return values needed

    return le;
}

#define GETBITS(in, n) (                                        \
  (le < (n) ? le = fillbits(in, le, bi), bi = in->bits : 0),    \
  (le -= (n)),                                                  \
  bi >> le & ((1 << (n)) - 1)                                   \
)

#define UNGETBITS(in, n) (   \
  le += (n)                  \
)

static int
dec_rec2(struct in *in, struct dec_hufftbl *hu, int *runp, int c, int i)
{
    int le, bi;

    le = in->left;
    bi = in->bits;
    if (i) {
        UNGETBITS(in, i & 127);
        *runp = i >> 8 & 15;
        i >>= 16;
    }
    else {
        for (i = DECBITS; (c = ((c << 1) | GETBITS(in, 1))) >= (hu->maxcode[i]); i++);
        if (i >= 16) {
            in->marker = M_BADHUFF;
            return 0;
        }
        i = hu->vals[hu->valptr[i] + c - hu->maxcode[i - 1] * 2];
        *runp = i >> 4;
        i &= 15;
    }
    if (i == 0) {       // sigh, 0xf0 is 11 bit
        LEBI_PUT(in);
        return 0;
    }
    // receive part
    c = GETBITS(in, i);
    if (c < (1 << (i - 1))) {
        c += (-1 << i) + 1;
    }
    LEBI_PUT(in);

    return c;
}

#define DEC_REC(in, hu, r, i) (        \
  r = GETBITS(in, DECBITS),            \
  i = hu->llvals[r],                   \
  i & 128 ?                            \
    (                                  \
      UNGETBITS(in, i & 127),          \
      r = i >> 8 & 15,                 \
      i >> 16                          \
    )                                  \
  :                                    \
    (                                  \
      LEBI_PUT(in),                    \
      i = dec_rec2(in, hu, &r, r, i),  \
      LEBI_GET(in),                    \
      i                                \
    )                                  \
)

static int dec_readmarker(struct in *in)
{
    int m;

    in->left = fillbits(in, in->left, in->bits);
    if ((m = in->marker) == 0) {
        return 0;
    }
    in->left = 0;
    in->marker = 0;

    return m;
}

static int dec_checkmarker(struct dec_data *decode)
{
    struct jpginfo *info = &decode->info;
    struct scan *dscans = decode->dscans;
    struct in *in = &decode->in;
    int i;

    if (dec_readmarker(in) != info->rm) {
        return -1;
    }
    info->nm = info->dri;
    info->rm = (info->rm + 1) & ~0x08;
    for (i = 0; i < info->ns; i++) {
        dscans[i].dc = 0;
    }

    return 0;
}

static void jpeg_reset_input_context(struct dec_data *decode, unsigned char *buf, int oescap)
{
    // set input context

    struct in *in = &decode->in;
    in->p = buf;
    in->omitescape = oescap;
    in->left = 0;
    in->bits = 0;
    in->marker = 0;
}

static void dec_initscans(struct dec_data *decode)
{
    jpginfo *info = &decode->info;
    struct scan *dscans = decode->dscans;
    int i;

    info->ns = 3;               // HARDCODED  here
    info->nm = info->dri + 1;   // macroblock count
    info->rm = M_RST0;
    for (i = 0; i < info->ns; i++) {
        dscans[i].dc = 0;
    }
}

inline static void
decode_mcus(struct in *in, int *dct, int n, struct scan *sc, int *maxp)
{
    struct dec_hufftbl *hu;
    int i, r, t;
    int le, bi;

    memset(dct, 0, n * 64 * sizeof(*dct));
    le = in->left;
    bi = in->bits;

    while (n-- > 0) {
        hu = sc->hudc.dhuff;
        *dct++ = (sc->dc += DEC_REC(in, hu, r, t));

        hu = sc->huac.dhuff;
        i = 63;
        while (i > 0) {
            t = DEC_REC(in, hu, r, t);
            if (t == 0 && r == 0) {
                dct += i;
                break;
            }
            dct += r;
            *dct++ = t;
            i -= r + 1;
        }
        *maxp++ = 64 - i;
        if (n == sc->next) {
            sc++;
        }
    }
    LEBI_PUT(in);
}

/****************************************************************/
/**************       huffman decoder             ***************/
/****************************************************************/

/*need to be on init jpeg */
static struct comp comp_template[MAXCOMP] = {
    {0x01, 0x22, 0x00},
    {0x02, 0x11, 0x01},
    {0x03, 0x11, 0x01},
    {0x00, 0x00, 0x00}
};

#define GSMART_JPG_HUFFMAN_TABLE_LENGTH 0x1A0

const unsigned char GsmartJPEGHuffmanTable[GSMART_JPG_HUFFMAN_TABLE_LENGTH] =
{
    0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x0A, 0x0B, 0x01, 0x00, 0x03,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x10,
    0x00, 0x02, 0x01, 0x03, 0x03,
    0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7D,
    0x01, 0x02, 0x03, 0x00, 0x04,
    0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
    0x22, 0x71, 0x14, 0x32, 0x81,
    0x91, 0xA1, 0x08, 0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0,
    0x24, 0x33, 0x62, 0x72, 0x82,
    0x09, 0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4A, 0x53, 0x54, 0x55, 0x56,
    0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6A, 0x73, 0x74, 0x75, 0x76,
    0x77, 0x78, 0x79, 0x7A, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8A, 0x92, 0x93, 0x94, 0x95,
    0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xAA, 0xB2, 0xB3,
    0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5,
    0xC6, 0xC7, 0xC8, 0xC9, 0xCA,
    0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2,
    0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
    0xE8, 0xE9, 0xEA, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
    0xF9, 0xFA, 0x11, 0x00, 0x02,
    0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00,
    0x01, 0x02, 0x77, 0x00, 0x01,
    0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51,
    0x07, 0x61, 0x71, 0x13, 0x22,
    0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xA1, 0xB1, 0xC1, 0x09, 0x23,
    0x33, 0x52, 0xF0, 0x15, 0x62,
    0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34, 0xE1, 0x25, 0xF1, 0x17, 0x18,
    0x19, 0x1A, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45,
    0x46, 0x47, 0x48, 0x49, 0x4A,
    0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65,
    0x66, 0x67, 0x68, 0x69, 0x6A,
    0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x82, 0x83, 0x84,
    0x85, 0x86, 0x87, 0x88, 0x89,
    0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2,
    0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9,
    0xBA, 0xC2, 0xC3, 0xC4, 0xC5,
    0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
    0xD8, 0xD9, 0xDA, 0xE2, 0xE3,
    0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF2, 0xF3, 0xF4, 0xF5,
    0xF6, 0xF7, 0xF8, 0xF9, 0xFA
};

const unsigned char GsmartJPEGScanTable[6] =
{
    0x01, 0x00, 0x02, 0x11, 0x03, 0x11
};

/* table de Huffman global for all */
static struct dec_hufftbl dhuff[4];
#define dec_huffdc (dhuff + 0)
#define dec_huffac (dhuff + 2)

static void
dec_makehuff(struct dec_hufftbl *hu, int *hufflen, unsigned char *huffvals)
{
    int code, k, i, j, d, x, c, v;

    for (i = 0; i < (1 << DECBITS); i++) {
        hu->llvals[i] = 0;
    }

    // llvals layout:
    //
    // value v already known, run r, backup u bits:
    //  vvvvvvvvvvvvvvvv 0000 rrrr 1 uuuuuuu
    // value unknown, size b bits, run r, backup u bits:
    //  000000000000bbbb 0000 rrrr 0 uuuuuuu
    // value and size unknown:
    //  0000000000000000 0000 0000 0 0000000
    //
    code = 0;
    k = 0;
    for (i = 0; i < 16; i++, code <<= 1) {            // sizes
        hu->valptr[i] = k;
        for (j = 0; j < hufflen[i]; j++) {
            hu->vals[k] = *huffvals++;
            if (i < DECBITS) {
                c = code << (DECBITS - 1 - i);
                v = hu->vals[k] & 0x0f; /* size */
                for (d = 1 << (DECBITS - 1 - i); --d >= 0;) {
                    if (v + i < DECBITS) {            // both fit in table
                        x = d >> (DECBITS - 1 - v - i);
                        if (v && x < (1 << (v - 1))) {
                            x += (-1 << v) + 1;
                        }
                        x = x << 16 | (hu->vals[k] & 0xf0) << 4 | (DECBITS - (i + 1 + v)) | 128;
                    }
                    else {
                        x = v << 16 | (hu->vals[k] & 0xf0) << 4 | (DECBITS - (i + 1));
                    }
                    hu->llvals[c | d] = x;
                }
            }
            code++;
            k++;
        }
        hu->maxcode[i] = code;
    }
    hu->maxcode[16] = 0x20000;  /* always terminate decode */
}

void init_jpeg_decoder(struct dec_data &maindecode)
{
    unsigned int i, j, k, l;
    int tc, th, tt, tac, tdc;
    const unsigned char *ptr;
    // unsigned int qIndex = spca50x->qindex;

    memcpy(maindecode.comps, comp_template, MAXCOMP * sizeof(struct comp));

    /* set up the huffman table */
    ptr = (const unsigned char *) GsmartJPEGHuffmanTable;
    l = GSMART_JPG_HUFFMAN_TABLE_LENGTH;
    while (l > 0) {
        int hufflen[16];
        unsigned char huffvals[256];
    
        tc = *ptr++;
        th = tc & 15;
        tc >>= 4;
        tt = tc * 2 + th;
        if (tc > 1 || th > 1) {
            //printf("died whilst setting up huffman table.\n");
            //abort();
        }
        for (i = 0; i < 16; i++) {
            hufflen[i] = *ptr++;
        }
        l -= 1 + 16;
        k = 0;
        for (i = 0; i < 16; i++) {
            for (j = 0; j < (unsigned int) hufflen[i]; j++) {
                huffvals[k++] = *ptr++;
            }
            l -= hufflen[i];
        }
        dec_makehuff(dhuff + tt, hufflen, huffvals);
    }

    /* set up the scan table */
    ptr = (const unsigned char *) GsmartJPEGScanTable;
    for (i = 0; i < 3; i++) {
        maindecode.dscans[i].cid = *ptr++;
        tdc = *ptr++;
        tac = tdc & 15;
        tdc >>= 4;
        if (tdc > 1 || tac > 1) {
            //printf("died whilst setting up scan table.\n");
            //abort();
        }
        /* for each component */
        for (j = 0; j < 3; j++) {
            if (maindecode.comps[j].cid == maindecode.dscans[i].cid) {
                break;
            }
        }
        maindecode.dscans[i].hv = maindecode.comps[j].hv;
        maindecode.dscans[i].tq = maindecode.comps[j].tq;
        maindecode.dscans[i].hudc.dhuff = dec_huffdc + tdc;
        maindecode.dscans[i].huac.dhuff = dec_huffac + tac;
    }

    if (maindecode.dscans[0].cid != 1 ||
        maindecode.dscans[1].cid != 2 ||
        maindecode.dscans[2].cid != 3) {
        //printf("invalid cid found.\n");
        //abort();
    }

    if (maindecode.dscans[0].hv != 0x22 ||
        maindecode.dscans[1].hv != 0x11 ||
        maindecode.dscans[2].hv != 0x11) {
        //printf("invalid hv found.\n");
        //abort();
    }
    maindecode.dscans[0].next = 6 - 4;
    maindecode.dscans[1].next = 6 - 4 - 1;
    maindecode.dscans[2].next = 6 - 4 - 1 - 1;  /* 411 encoding */

    /* set up a quantization table */
    // init_qTable(spca50x, qIndex);
}

// ***********************************************************
// ************             idct                  ************
// ***********************************************************

#define S22 ((long)IFIX(2 * 0.382683432))
#define C22 ((long)IFIX(2 * 0.923879532))
#define IC4 ((long)IFIX(1 / 0.707106781))

static unsigned char zig2[64] =
{
    0, 2, 3, 9, 10, 20, 21, 35,
    14, 16, 25, 31, 39, 46, 50, 57,
    5, 7, 12, 18, 23, 33, 37, 48,
    27, 29, 41, 44, 52, 55, 59, 62,
    15, 26, 30, 40, 45, 51, 56, 58,
    1, 4, 8, 11, 19, 22, 34, 36,
    28, 42, 43, 53, 54, 60, 61, 63,
    6, 13, 17, 24, 32, 38, 47, 49
};

static void idct(int *in, int *out, int *quant, long off, int max)
{
    long  t0, t1, t2, t3, t4, t5, t6, t7;
    long  tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;
    long  tmp[64];
    long* tmpp;
    int   i, j;
    int   te;
    unsigned char *zig2p;

    t0 = off;
    if (max == 1) {
        t0 += in[0] * quant[0];
        for (i = 0; i < 64; i++) {
            out[i] = ITOINT(t0);
        }
        return;
    }
    zig2p = zig2;
    tmpp = tmp;
    for (i = 0; i < 8; i++) {
        j = *zig2p++;
        t0 += in[j] * (long) quant[j];
        j = *zig2p++;
        t5 = in[j] * (long) quant[j];
        j = *zig2p++;
        t2 = in[j] * (long) quant[j];
        j = *zig2p++;
        t7 = in[j] * (long) quant[j];
        j = *zig2p++;
        t1 = in[j] * (long) quant[j];
        j = *zig2p++;
        t4 = in[j] * (long) quant[j];
        j = *zig2p++;
        t3 = in[j] * (long) quant[j];
        j = *zig2p++;
        t6 = in[j] * (long) quant[j];

        if ((t1 | t2 | t3 | t4 | t5 | t6 | t7) == 0) {
            tmpp[0 * 8] = t0;
            tmpp[1 * 8] = t0;
            tmpp[2 * 8] = t0;
            tmpp[3 * 8] = t0;
            tmpp[4 * 8] = t0;
            tmpp[5 * 8] = t0;
            tmpp[6 * 8] = t0;
            tmpp[7 * 8] = t0;

            tmpp++;
            t0 = 0;

            continue;
        }
        //IDCT;
        tmp0 = t0 + t1;
        t1 = t0 - t1;
        tmp2 = t2 - t3;
        t3 = t2 + t3;
        tmp2 = IMULT(tmp2, IC4) - t3;
        tmp3 = tmp0 + t3;
        t3 = tmp0 - t3;
        tmp1 = t1 + tmp2;
        tmp2 = t1 - tmp2;
        tmp4 = t4 - t7;
        t7 = t4 + t7;
        tmp5 = t5 + t6;
        t6 = t5 - t6;
        tmp6 = tmp5 - t7;
        t7 = tmp5 + t7;
        tmp5 = IMULT(tmp6, IC4);
        tmp6 = IMULT((tmp4 + t6), S22);
        tmp4 = IMULT(tmp4, (C22 - S22)) + tmp6;
        t6 = IMULT(t6, (C22 + S22)) - tmp6;
        t6 = t6 - t7;
        t5 = tmp5 - t6;
        t4 = tmp4 - t5;

        tmpp[0 * 8] = tmp3 + t7;    //t0;
        tmpp[1 * 8] = tmp1 + t6;    //t1;
        tmpp[2 * 8] = tmp2 + t5;    //t2;
        tmpp[3 * 8] = t3 + t4;  //t3;
        tmpp[4 * 8] = t3 - t4;  //t4;
        tmpp[5 * 8] = tmp2 - t5;    //t5;
        tmpp[6 * 8] = tmp1 - t6;    //t6;
        tmpp[7 * 8] = tmp3 - t7;    //t7;

        tmpp++;
        t0 = 0;
    }
    for (i = 0, j = 0; i < 8; i++) {
        t0 = tmp[j + 0];
        t1 = tmp[j + 1];
        t2 = tmp[j + 2];
        t3 = tmp[j + 3];
        t4 = tmp[j + 4];
        t5 = tmp[j + 5];
        t6 = tmp[j + 6];
        t7 = tmp[j + 7];
        if ((t1 | t2 | t3 | t4 | t5 | t6 | t7) == 0) {
            te = ITOINT(t0);
            out[j + 0] = te;
            out[j + 1] = te;
            out[j + 2] = te;
            out[j + 3] = te;
            out[j + 4] = te;
            out[j + 5] = te;
            out[j + 6] = te;
            out[j + 7] = te;
            j += 8;

            continue;
        }
        //IDCT;
        tmp0 = t0 + t1;
        t1 = t0 - t1;
        tmp2 = t2 - t3;
        t3 = t2 + t3;
        tmp2 = IMULT(tmp2, IC4) - t3;
        tmp3 = tmp0 + t3;
        t3 = tmp0 - t3;
        tmp1 = t1 + tmp2;
        tmp2 = t1 - tmp2;
        tmp4 = t4 - t7;
        t7 = t4 + t7;
        tmp5 = t5 + t6;
        t6 = t5 - t6;
        tmp6 = tmp5 - t7;
        t7 = tmp5 + t7;
        tmp5 = IMULT(tmp6, IC4);
        tmp6 = IMULT((tmp4 + t6), S22);
        tmp4 = IMULT(tmp4, (C22 - S22)) + tmp6;
        t6 = IMULT(t6, (C22 + S22)) - tmp6;
        t6 = t6 - t7;
        t5 = tmp5 - t6;
        t4 = tmp4 - t5;

        out[j + 0] = ITOINT(tmp3 + t7);
        out[j + 1] = ITOINT(tmp1 + t6);
        out[j + 2] = ITOINT(tmp2 + t5);
        out[j + 3] = ITOINT(t3 + t4);
        out[j + 4] = ITOINT(t3 - t4);
        out[j + 5] = ITOINT(tmp2 - t5);
        out[j + 6] = ITOINT(tmp1 - t6);
        out[j + 7] = ITOINT(tmp3 - t7);

        j += 8;
    }

}

int convert_mjpeg411_to_xbgr32_buffer(unsigned char *mjpeg411, unsigned char *xbgr32, unsigned int width, unsigned int height /*, unsigned long bufferLength, long stride */)
{
    int mcusx, mcusy, mx, my;
    int dcts[6 * 64 + 16];
    int out[6 * 64];
    int max[6];
    int bpp;
    int framesize, frameUsize;
    int k, j;
    int nextline, nextblk, nextnewline;
    unsigned char *pic0, *pic1;
    int picy, picx;
    unsigned char *U, *V;
    int *outy, *inv, *inu;
    int outy1, outy2;
    int v, u, y1, v1, u1, u2;
    int r_offset, g_offset, b_offset;

    unsigned char red[256];
    unsigned char green[256];
    unsigned char blue[256];
    struct dec_data decoder;

    init_jpeg_decoder(decoder);

    struct dec_data *decode = &decoder;

    if ((height & 15) || (width & 15)) {
        return 1;
    }

    mcusx = width >> 4;
    mcusy = height >> 4;
    framesize = width * height;
    frameUsize = framesize >> 2;
    jpeg_reset_input_context(decode, mjpeg411, 0);

    // Reset dc values.
    dec_initscans(decode);

    /* create rgb
    U = xbgr32 + framesize;
    V = U + frameUsize;
    r_offset = 2;
    g_offset = 1;
    b_offset = 0; */
    // crate bgr
    V = xbgr32 + framesize;
    U = V + frameUsize;
    r_offset = 0;
    g_offset = 1;
    b_offset = 2;

    // Decode to the correct format.
    // case VIDEO_PALETTE_RGB32:
    // case VIDEO_PALETTE_RGB24:
    bpp = 4;    // (format == VIDEO_PALETTE_RGB32) ? 4 : 3;
    nextline = bpp * ((width << 1) - 16);
    nextblk = bpp * (width << 4);
    nextnewline = bpp * width;
    for (my = 0, picy = 0; my < mcusy; my++) {
        for (mx = 0, picx = 0; mx < mcusx; mx++) {
            decode_mcus(&decode->in, dcts, 6, decode->dscans, max);

            idct(dcts,       out,       decode->dquant[0], IFIX(128.5), max[0]);
            idct(dcts + 64,  out + 64,  decode->dquant[0], IFIX(128.5), max[1]);
            idct(dcts + 128, out + 128, decode->dquant[0], IFIX(128.5), max[2]);
            idct(dcts + 192, out + 192, decode->dquant[0], IFIX(128.5), max[3]);
            idct(dcts + 256, out + 256, decode->dquant[1], IFIX(0.5),   max[4]);
            idct(dcts + 320, out + 320, decode->dquant[2], IFIX(0.5),   max[5]);

            pic0 = xbgr32 + picx + picy;
            pic1 = pic0 + nextnewline;
            outy = out;
            outy1 = 0;
            outy2 = 8;
            inv = out + 64 * 4;
            inu = out + 64 * 5;

            for (j = 0; j < 8; j++) {
                for (k = 0; k < 8; k++) {
                    if (k == 4) {
                        outy1 += 56;
                        outy2 += 56;
                    }
                    // outup 4 pixels
                    // get the UV colors need to change UV order for force rgb?
                    v = *inv++;
                    u = *inu++;
                    // MX color space why not?
                    v1 = ((v << 10)
                          + (v << 9))
                        >> 10;
                    u1 = ((u <<
                           8) + (u << 7) + (v << 9) + (v << 4))
                        >> 10;
                    u2 = ((u << 11)
                          + (u << 4))
                        >> 10;
                    // top pixel Right
                    y1 = outy[outy1++];
                    pic0[r_offset] = red[CLIP((y1 + v1))];
                    pic0[g_offset] = green[CLIP((y1 - u1))];
                    pic0[b_offset] = blue[CLIP((y1 + u2))];
                    pic0 += bpp;
                    // top pixel Left
                    y1 = outy[outy1++];
                    pic0[r_offset] = red[CLIP((y1 + v1))];
                    pic0[g_offset] = green[CLIP((y1 - u1))];
                    pic0[b_offset] = blue[CLIP((y1 + u2))];
                    pic0 += bpp;
                    // bottom pixel Right
                    y1 = outy[outy2++];
                    pic1[r_offset] = red[CLIP((y1 + v1))];
                    pic1[g_offset] = green[CLIP((y1 - u1))];
                    pic1[b_offset] = blue[CLIP((y1 + u2))];
                    pic1 += bpp;
                    // bottom pixel Left
                    y1 = outy[outy2++];
                    pic1[r_offset] = red[CLIP((y1 + v1))];
                    pic1[g_offset] = green[CLIP((y1 - u1))];
                    pic1[b_offset] = blue[CLIP((y1 + u2))];
                    pic1 += bpp;
                }
                if (j == 3) {
                    outy = out + 128;
                }
                else {
                    outy += 16;
                }
                outy1 = 0;
                outy2 = 8;
                pic0 += nextline;
                pic1 += nextline;
            }
            picx += 16 * bpp;
        }
        picy += nextblk;
    }

    return 0;
}

int convert_mjpeg422_to_xbgr32_buffer(unsigned char *mjpeg422, unsigned char *xbgr32, unsigned int width, unsigned int height /*, unsigned long bufferLength, long stride */)
{
    int mcusx, mcusy, mx, my;
    int dcts[6 * 64 + 16];
    int out[6 * 64];
    int max[6];
    int bpp;
    int framesize, frameUsize;
    int k, j;
    int nextline, nextblk, nextnewline;
    unsigned char *pic0, *pic1;
    int picy, picx;
    unsigned char *U, *V;
    int *outy, *inv, *inu;
    int outy1, outy2;
    int v, u, y1, v1, u1, u2;
    int r_offset, g_offset, b_offset;

    unsigned char red[256];
    unsigned char green[256];
    unsigned char blue[256];
    struct dec_data decoder;

    init_jpeg_decoder(decoder);

    struct dec_data *decode = &decoder;

    if ((height & 7) || (width & 7)) {
        return 1;
    }

    mcusx = width >> 4;
    mcusy = height >> 3;
    framesize = width * height;
    frameUsize = framesize >> 2;
    jpeg_reset_input_context(decode, mjpeg422, 1);

    // for each component. Reset dc values.
    dec_initscans(decode);

    /* create rgb
    U = xbgr32 + framesize;
    V = U + frameUsize;
    r_offset = 2;
    g_offset = 1;
    b_offset = 0; */
    // crate bgr
    V = xbgr32 + framesize;
    U = V + frameUsize;
    r_offset = 0;
    g_offset = 1;
    b_offset = 2;

    // Decode to the correct format.
    // case VIDEO_PALETTE_RGB32:
    // case VIDEO_PALETTE_RGB24:
    bpp = 4;  // (format == VIDEO_PALETTE_RGB32) ? 4 : 3;
    nextline = bpp * ((width << 1) - 16);
    nextblk = bpp * (width << 3);
    nextnewline = bpp * width;

    for (my = 0, picy = 0; my < mcusy; my++) {
        for (mx = 0, picx = 0; mx < mcusx; mx++) {
            if (decode->info.dri && !--decode->info.nm) {
                if (dec_checkmarker(decode)) {
                    return -1;   // ERR_WRONG_MARKER;
                }
            }
            decode_mcus(&decode->in, dcts, 4, decode->dscans, max);

            idct(dcts,       out,       decode->dquant[0], IFIX(128.5), max[0]);
            idct(dcts + 64,  out + 64,  decode->dquant[0], IFIX(128.5), max[1]);
            idct(dcts + 128, out + 256, decode->dquant[1], IFIX(0.5),   max[2]);
            idct(dcts + 192, out + 320, decode->dquant[2], IFIX(0.5),   max[3]);

            pic0 = xbgr32 + picx + picy;
            pic1 = pic0 + nextnewline;
            outy = out;
            outy1 = 0;
            outy2 = 8;
            inv = out + 64 * 4;
            inu = out + 64 * 5;

            for (j = 0; j < 4; j++) {
                for (k = 0; k < 8; k++) {
                    if (k == 4) {
                        outy1 += 56;
                        outy2 += 56;
                    }
                    // outup 4 pixels Colors are treated as 411

                    v = *inv++;
                    u = *inu++;

                    // MX color space why not?
                    v1 = ((v << 10)
                    + (v << 9))
                    >> 10;
                    u1 = ((u <<
                    8) + (u << 7) + (v << 9) + (v << 4))
                    >> 10;
                    u2 = ((u << 11)
                    + (u << 4))
                    >> 10;
                    // top pixel Right
                    y1 = outy[outy1++];
                    pic0[r_offset] = red[CLIP((y1 + v1))];
                    pic0[g_offset] = green[CLIP((y1 - u1))];
                    pic0[b_offset] = blue[CLIP((y1 + u2))];
                    pic0 += bpp;
                    // top pixel Left
                    y1 = outy[outy1++];
                    pic0[r_offset] = red[CLIP((y1 + v1))];
                    pic0[g_offset] = green[CLIP((y1 - u1))];
                    pic0[b_offset] = blue[CLIP((y1 + u2))];
                    pic0 += bpp;
                    // bottom pixel Right
                    y1 = outy[outy2++];
                    pic1[r_offset] = red[CLIP((y1 + v1))];
                    pic1[g_offset] = green[CLIP((y1 - u1))];
                    pic1[b_offset] = blue[CLIP((y1 + u2))];
                    pic1 += bpp;
                    // bottom pixel Left
                    y1 = outy[outy2++];
                    pic1[r_offset] = red[CLIP((y1 + v1))];
                    pic1[g_offset] = green[CLIP((y1 - u1))];
                    pic1[b_offset] = blue[CLIP((y1 + u2))];
                    pic1 += bpp;
                }
                outy += 16;
                outy1 = 0;
                outy2 = 8;
                pic0 += nextline;
                pic1 += nextline;
            }
            picx += 16 * bpp;
        }
        picy += nextblk;
    }

    return 0;
}
