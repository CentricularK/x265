/*****************************************************************************
 * Copyright (C) 2013 x265 project
 *
 * Authors: Steve Borho <steve@borho.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at licensing@multicorewareinc.com.
 *****************************************************************************/

#ifndef X265_PIXEL_UTIL_H
#define X265_PIXEL_UTIL_H

void x265_calcRecons4_sse2(pixel* pred, int16_t* residual, pixel* recon, int16_t* reconqt, pixel *reconipred, int stride, int strideqt, int strideipred);
void x265_calcRecons8_sse2(pixel* pred, int16_t* residual, pixel* recon, int16_t* reconqt, pixel *reconipred, int stride, int strideqt, int strideipred);
void x265_calcRecons16_sse4(pixel* pred, int16_t* residual, pixel* recon, int16_t* reconqt, pixel *reconipred, int stride, int strideqt, int strideipred);
void x265_calcRecons32_sse4(pixel* pred, int16_t* residual, pixel* recon, int16_t* reconqt, pixel *reconipred, int stride, int strideqt, int strideipred);

void x265_getResidual4_sse2(pixel *fenc, pixel *pred, int16_t *residual, intptr_t stride);
void x265_getResidual8_sse2(pixel *fenc, pixel *pred, int16_t *residual, intptr_t stride);
void x265_getResidual16_sse4(pixel *fenc, pixel *pred, int16_t *residual, intptr_t stride);
void x265_getResidual32_sse4(pixel *fenc, pixel *pred, int16_t *residual, intptr_t stride);

void x265_transpose4_sse2(pixel *dest, pixel *src, intptr_t stride);
void x265_transpose8_sse2(pixel *dest, pixel *src, intptr_t stride);
void x265_transpose16_sse2(pixel *dest, pixel *src, intptr_t stride);
void x265_transpose32_sse2(pixel *dest, pixel *src, intptr_t stride);
void x265_transpose64_sse2(pixel *dest, pixel *src, intptr_t stride);

uint32_t x265_quant_sse4(int32_t *coef, int32_t *quantCoeff, int32_t *deltaU, int32_t *qCoef, int qBits, int add, int numCoeff, int32_t* lastPos);
void x265_dequant_normal_sse4(const int32_t* quantCoef, int32_t* coef, int num, int scale, int shift);

void x265_weight_pp_sse4(pixel *src, pixel *dst, intptr_t srcStride, intptr_t dstStride, int width, int height, int w0, int round, int shift, int offset);
void x265_weight_sp_sse4(int16_t *src, pixel *dst, intptr_t srcStride, intptr_t dstStride, int width, int height, int w0, int round, int shift, int offset);

#endif // ifndef X265_PIXEL_UTIL_H
