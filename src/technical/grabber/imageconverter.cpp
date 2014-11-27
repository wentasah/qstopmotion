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

#include <QtCore/QtDebug>

/******************************************************************************
 * Convert YUV2 to RGB 8bit
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

int convert_yuv_to_rgb8_FR_pixel(int y, int u, int v)
{
    unsigned int pixel_32 = 0;
    unsigned char *pixel = (unsigned char *)&pixel_32;
    int r8, g8, b8;

    r8 = (int)(FrCoefficientY * y + FrCoefficientRV * (v - 128));
    g8 = (int)(FrCoefficientY * y + FrCoefficientGU * (u - 128) + FrCoefficientGV * (v - 128));
    b8 = (int)(FrCoefficientY * y + FrCoefficientBU * (u - 128));

    r8 = (r8 + FrPrecision / 2) / FrPrecision;
    g8 = (g8 + FrPrecision / 2) / FrPrecision;
    b8 = (b8 + FrPrecision / 2) / FrPrecision;

    if (r8 > 255) r8 = 255;
    if (g8 > 255) g8 = 255;
    if (b8 > 255) b8 = 255;

    if (r8 < 0) r8 = 0;
    if (g8 < 0) g8 = 0;
    if (b8 < 0) b8 = 0;

    pixel[0] = r8;
    pixel[1] = g8;
    pixel[2] = b8;

    return pixel_32;
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

int convert_yuv_to_rgb8_SDTV_pixel(int y, int u, int v)
{
    unsigned int pixel_32 = 0;
    unsigned char *pixel = (unsigned char *)&pixel_32;
    int r8, g8, b8;

    r8 = (int)(SdtvCoefficientY * (y - 16)                                 + SdtvCoefficientRV * (v - 128));
    g8 = (int)(SdtvCoefficientY * (y - 16) + SdtvCoefficientGU * (u - 128) + SdtvCoefficientGV * (v - 128));
    b8 = (int)(SdtvCoefficientY * (y - 16) + SdtvCoefficientBU * (u - 128));

    r8 = (r8 + SdtvPrecision / 2) / SdtvPrecision;
    g8 = (g8 + SdtvPrecision / 2) / SdtvPrecision;
    b8 = (b8 + SdtvPrecision / 2) / SdtvPrecision;

    if (r8 > 255) r8 = 255;
    if (g8 > 255) g8 = 255;
    if (b8 > 255) b8 = 255;

    if (r8 < 0) r8 = 0;
    if (g8 < 0) g8 = 0;
    if (b8 < 0) b8 = 0;

    pixel[0] = r8;
    pixel[1] = g8;
    pixel[2] = b8;

    return pixel_32;
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

int convert_yuv_to_rgb8_HDTV_pixel(int y, int u, int v)
{
    unsigned int pixel_32 = 0;
    unsigned char *pixel = (unsigned char *)&pixel_32;
    int r8, g8, b8;

    r8 = (int)(HdtvCoefficientY * (y - 16) + HdtvCoefficientRV * (v - 128));
    g8 = (int)(HdtvCoefficientY * (y - 16) + HdtvCoefficientGU * (u - 128) + HdtvCoefficientGV * (v - 128));
    b8 = (int)(HdtvCoefficientY * (y - 16) + HdtvCoefficientBU * (u - 128));

    r8 = (r8 + HdtvPrecision / 2) / HdtvPrecision;
    g8 = (g8 + HdtvPrecision / 2) / HdtvPrecision;
    b8 = (b8 + HdtvPrecision / 2) / HdtvPrecision;

    if (r8 > 255) r8 = 255;
    if (g8 > 255) g8 = 255;
    if (b8 > 255) b8 = 255;

    if (r8 < 0) r8 = 0;
    if (g8 < 0) g8 = 0;
    if (b8 < 0) b8 = 0;

    pixel[0] = r8;
    pixel[1] = g8;
    pixel[2] = b8;

    return pixel_32;
}


/*
 * Memory layout ayuv:
 *   +----+----+----+----+  +----+----+----+----+
 *   | V0 | U0 | Y0 | A0 |  | V1 | U1 | Y1 | A1 |
 *   +----+----+----+----+  +----+----+----+----+
 */
int convert_ayuv_to_argb8_buffer(unsigned char *ayuv, unsigned char *argb8, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned int  in = 0;
    unsigned int  out = 0;
    unsigned int  pixel_16;
    unsigned char pixel_24[3];
    unsigned int  pixel_32;
    int           v;
    int           u;
    int           y;
    int           a;
    bool          upsideDown = (stride < 0);

    if (!upsideDown) {
        for (in = 0; in < width * height * 2; in += 4) {
            pixel_16 =
                ayuv[in + 3] << 24 |
                ayuv[in + 2] << 16 |
                ayuv[in + 1] <<  8 |
                ayuv[in + 0];

            v = (pixel_16 & 0x000000ff);
            u = (pixel_16 & 0x0000ff00) >>  8;
            y = (pixel_16 & 0x00ff0000) >> 16;
            a = (pixel_16 & 0xff000000) >> 24;

            pixel_32 = convert_yuv_to_rgb8_SDTV_pixel(y, u, v);
            pixel_24[0] = (pixel_32 & 0x000000ff);
            pixel_24[1] = (pixel_32 & 0x0000ff00) >> 8;
            pixel_24[2] = (pixel_32 & 0x00ff0000) >> 16;

            argb8[out++] = a;            // A
            argb8[out++] = pixel_24[0];  // R
            argb8[out++] = pixel_24[1];  // G
            argb8[out++] = pixel_24[2];  // B
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
int convert_yuv2_to_argb8_buffer(unsigned char *yuv2, unsigned char *argb8, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned int  in = 0;
    unsigned int  out = 0;
    unsigned int  pixel_16;
    unsigned char pixel_24[3];
    unsigned int  pixel_32;
    int           y0;
    int           u;
    int           y1;
    int           v;
    bool          upsideDown = (stride < 0);

    if (!upsideDown) {
        for (in = 0; in < width * height * 2; in += 4) {
            pixel_16 =
                yuv2[in + 3] << 24 |
                yuv2[in + 2] << 16 |
                yuv2[in + 1] <<  8 |
                yuv2[in + 0];

            y0 = (pixel_16 & 0x000000ff);
            u  = (pixel_16 & 0x0000ff00) >>  8;
            y1 = (pixel_16 & 0x00ff0000) >> 16;
            v  = (pixel_16 & 0xff000000) >> 24;

            pixel_32 = convert_yuv_to_rgb8_SDTV_pixel(y0, u, v);
            pixel_24[0] = (pixel_32 & 0x000000ff);           // R
            pixel_24[1] = (pixel_32 & 0x0000ff00) >> 8;      // G
            pixel_24[2] = (pixel_32 & 0x00ff0000) >> 16;     // B

            argb8[out++] = 0xffU;        // A
            argb8[out++] = pixel_24[0];  // R
            argb8[out++] = pixel_24[1];  // G
            argb8[out++] = pixel_24[2];  // B

            pixel_32 = convert_yuv_to_rgb8_SDTV_pixel(y1, u, v);
            pixel_24[0] = (pixel_32 & 0x000000ff);
            pixel_24[1] = (pixel_32 & 0x0000ff00) >> 8;
            pixel_24[2] = (pixel_32 & 0x00ff0000) >> 16;

            argb8[out++] = 0xffU;        // A
            argb8[out++] = pixel_24[0];  // R
            argb8[out++] = pixel_24[1];  // G
            argb8[out++] = pixel_24[2];  // B
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
int convert_uyvy_to_argb8_buffer(unsigned char *uyvy, unsigned char *argb8, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned int  in = 0;
    unsigned int  out = 0;
    unsigned int  pixel_16;
    unsigned char pixel_24[3];
    unsigned int  pixel_32;
    int           y0;
    int           u;
    int           y1;
    int           v;
    bool          upsideDown = (stride < 0);

    if (!upsideDown) {
        for (in = 0; in < width * height * 2; in += 4) {
            pixel_16 =
                uyvy[in + 3] << 24 |
                uyvy[in + 2] << 16 |
                uyvy[in + 1] <<  8 |
                uyvy[in + 0];

            u = (pixel_16 & 0x000000ff);
            y0  = (pixel_16 & 0x0000ff00) >>  8;
            v = (pixel_16 & 0x00ff0000) >> 16;
            y1  = (pixel_16 & 0xff000000) >> 24;

            pixel_32 = convert_yuv_to_rgb8_SDTV_pixel(y0, u, v);
            pixel_24[0] = (pixel_32 & 0x000000ff);
            pixel_24[1] = (pixel_32 & 0x0000ff00) >> 8;
            pixel_24[2] = (pixel_32 & 0x00ff0000) >> 16;

            argb8[out++] = 0xffU;        // A
            argb8[out++] = pixel_24[0];  // R
            argb8[out++] = pixel_24[1];  // G
            argb8[out++] = pixel_24[2];  // B

            pixel_32 = convert_yuv_to_rgb8_SDTV_pixel(y1, u, v);
            pixel_24[0] = (pixel_32 & 0x000000ff);
            pixel_24[1] = (pixel_32 & 0x0000ff00) >> 8;
            pixel_24[2] = (pixel_32 & 0x00ff0000) >> 16;

            argb8[out++] = 0xffU;        // A
            argb8[out++] = pixel_24[0];  // R
            argb8[out++] = pixel_24[1];  // G
            argb8[out++] = pixel_24[2];  // B
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
int convert_yv12_to_argb8_buffer(unsigned char *y12, unsigned char *v12, unsigned char *u12, unsigned char *argb8, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned int  k;
    unsigned int  i;
    unsigned char pixel_24[3];
    unsigned int  pixel_32;
    int           y;
    int           u;
    int           v;
    bool          upsideDown = (stride < 0);

    if (!upsideDown) {
        for (unsigned int h = 0; h < height; h++) {
            for (unsigned int w = 0; w < width; w++) {
                k = h * width + w;
                i = (h / 2) * (width / 2) + (w / 2);

                y = y12[k];
                u = u12[i];
                v = v12[i];

                pixel_32 = convert_yuv_to_rgb8_SDTV_pixel(y, u, v);
                pixel_24[0] = (pixel_32 & 0x000000ff);
                pixel_24[1] = (pixel_32 & 0x0000ff00) >> 8;
                pixel_24[2] = (pixel_32 & 0x00ff0000) >> 16;

                argb8[k + 0] = pixel_24[0];
                argb8[k + 1] = pixel_24[1];
                argb8[k + 2] = pixel_24[2];
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
int convert_yuv2_to_rgb8_file(char *yuv2file, char *rgb8file, unsigned int width, unsigned int height)
{
    FILE *in, *out;
    unsigned char *yuv2, *rgb8;

    in = fopen(yuv2file, "rb");
    if (!in) {
        return 1;
    }

    out = fopen(rgb8file, "wb");
    if (!out) {
        return 1;
    }

    yuv2 = malloc(width * height * 2);
    if (yuv2 == NULL) {
        return 2;
    }

    rgb8 = malloc(width * height * 3);
    if (rgb8 == NULL) {
        return 2;
    }

    fread(yuv2, width * height * 2, 1, in);
    fclose(in);

    if (convert_yuv2_to_rgb8_buffer(yuv2, rgb8, width, height)) {
        return 3;
    }

    fwrite(rgb8, width * height * 3, 1, out);
    fclose(out);

    return 0;
}
*/
/******************************************************************************
 * Convert RGB 8bit to YUV2
 ******************************************************************************/

int convert_rgb8_to_yuv2_pixel(int r8, int g8, int b8)
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


int convert_rgb8_to_yuv2_buffer(unsigned char *rgb8, unsigned char *yuv2, unsigned int width, unsigned int height)
{
    unsigned int in, out = 0;
    unsigned int pixel_32;
    int y0, u0, v0, y1, u1, v1;

    for(in = 0; in < width * height * 3; in += 6) {
        pixel_32 = convert_rgb8_to_yuv2_pixel(rgb8[in], rgb8[in + 1], rgb8[in + 2]);
        y0 = (pixel_32 & 0x000000ff);
        u0 = (pixel_32 & 0x0000ff00) >>  8;
        v0 = (pixel_32 & 0x00ff0000) >> 16;

        pixel_32 = convert_rgb8_to_yuv2_pixel(rgb8[in + 3], rgb8[in + 4], rgb8[in + 5]);
        y1 = (pixel_32 & 0x000000ff);
        u1 = (pixel_32 & 0x0000ff00) >>  8;
        v1 = (pixel_32 & 0x00ff0000) >> 16;

        yuv2[out++] = y0;
        yuv2[out++] = (u0 + u1) / 2;
        yuv2[out++] = y1;
        yuv2[out++] = (v0 + v1) / 2;
    }

    return 0;
}

/*
int convert_rgb8_to_yuv2_file(char *rgb8file, char *yuv2file, unsigned int width, unsigned int height)
{
    FILE *in, *out;
    unsigned char *yuv2, *rgb8;

    in = fopen(rgb8file, "rb");
    if (!in) {
        return 1;
    }

    out = fopen(yuv2file, "wb");
    if (!out) {
        return 1;
    }

    rgb8 = malloc(width * height * 3);
    if (rgb8 == NULL) {
        return 2;
    }

    yuv2 = malloc(width * height * 2);
    if (yuv2 == NULL) {
        return 2;
    }

    fread(rgb8, width * height * 3, 1, in);

    fclose(in);

    if (convert_rgb8_to_yuv2_buffer(rgb8, yuv2, width, height)) {
        return 3;
    }

    fwrite(yuv2, width * height * 2, 1, out);

    fclose(out);

    return 0;
}
*/
/******************************************************************************
 * Convert RGB 24bit to RGB 8bit
 ******************************************************************************/
/*
int convert_rgb24_to_rgb8_pixel(int r24, int g24, int b24, bool upsideDown)
{
    unsigned int pixel_32 = 0;
    unsigned char *pixel = (unsigned char *)&pixel_32;
    int r8, g8, b8;

    r8 = (int)(y + (1.370705 * (v-128)));
    g8 = (int)(y - (0.698001 * (v-128)) - (0.337633 * (u-128)));
    b8 = (int)(y + (1.732446 * (u-128)));

    if (r8 > 255) r8 = 255;
    if (g8 > 255) g8 = 255;
    if (b8 > 255) b8 = 255;

    if (r8 < 0) r8 = 0;
    if (g8 < 0) g8 = 0;
    if (b8 < 0) b8 = 0;

    pixel[0] = r8 * 220 / 256;
    pixel[1] = g8 * 220 / 256;
    pixel[2] = b8 * 220 / 256;

    return pixel_32;
}
*/

/*
 * Memory layout rgb24:
 *   +----+----+----+  +----+----+----+
 *   | R0 | G0 | B0 |  | R1 | G1 | B1 |
 *   +----+----+----+  +----+----+----+
 */
int convert_rgb24_to_argb8_buffer(unsigned char *rgb24, unsigned char *argb8, unsigned int width, unsigned int height, unsigned long bufferLength, long stride)
{
    unsigned char* dst = const_cast<unsigned char*>(argb8);
    const unsigned char* const dstEnd = dst + bufferLength;
    bool upsideDown = (stride < 0);

    if (!upsideDown) {
        const unsigned char* src = rgb24;
        while (dst < dstEnd) {
            *dst++ = 0xffU;        // A
            *dst++ = *src++;       // R
            *dst++ = *src++;       // G
            *dst++ = *src++;       // B
        }
    }
    else {
        stride = -stride;
        if (stride != (width * 3)) {
            return 0;
        }
        for (int scanLine = height - 1; scanLine >= 0; --scanLine) {
            const unsigned char* src = rgb24 + scanLine * stride;
            const unsigned char* const srcEnd = src + width * 3;
            while (src < srcEnd) {
                *dst++ = 0xffU;        // A
                *dst++ = *src++;       // R
                *dst++ = *src++;       // G
                *dst++ = *src++;       // B
            }
        }
    }

    return 0;
}

/*
int convert_rgb24_to_rgb8_file(char *rgb24file, char *rgb8file, unsigned int width, unsigned int height)
{
    FILE *in, *out;
    unsigned char *rgb24, *rgb8;

    in = fopen(rgb24file, "rb");
    if (!in) {
        return 1;
    }

    out = fopen(rgb8file, "wb");
    if (!out) {
        return 1;
    }

    rgb24 = malloc(width * height * 2);
    if (rgb24 == NULL) {
        return 2;
    }

    rgb8 = malloc(width * height * 3);
    if (rgb8 == NULL) {
        return 2;
    }

    fread(rgb24, width * height * 2, 1, in);
    fclose(in);

    if (convert_rgb24_to_rgb8_buffer(rgb24, rgb8, width, height)) {
        return 3;
    }

    fwrite(rgb8, width * height * 3, 1, out);
    fclose(out);

    return 0;
}
*/
/******************************************************************************
 * Example
 ******************************************************************************/
/*
int main(int argc, char **argv)
{
    int ret = 0;
    char *infile, *outfile;
    unsigned int width, height;

    if (argc < 5) {
#ifdef RGB2YUV
        fprintf(stderr, "Convert RGB images to YUY2 (YUYV).\n\n");
#else
        fprintf(stderr, "Convert YUY2 (YUYV) images to RGB.\n\n");
#endif
        fprintf(stderr, "Syntax: %s width height input output\n", argv[0]);
        return 1;
    }

    width = atoi(argv[1]);
    height = atoi(argv[2]);
    infile = argv[3];
    outfile = argv[4];
#ifdef RGB2YUV
    printf("RGB2YUY: %s => %s\n", infile, outfile);
    ret = convert_rgb8_to_yuv2_file(infile, outfile, width, height);
#else
    printf("YUV2RGB: %s => %s\n", infile, outfile);
    ret = convert_yuv2_to_rgb8_file(infile, outfile, width, height);
#endif
    if (ret == 0) {
        printf("Done.\n");
    } else {
        printf("Failed (error = %d). Aborting.\n", ret);
        return 1;
    }

    return 0;
}

void MainWindow::paintEvent ( QPaintEvent * event )
{
    QPainter Painter(this) ;
    read_frame();
    convert_yuv2_to_rgb8_buffer((unsigned char *)buffers[JPEGindex].start,bufrgb8,320,240);
    QImage img(bufrgb8,320,240,QImage::Format_RGB888);
    Painter.drawImage(0,0,img) ;

    update();
}
*/
