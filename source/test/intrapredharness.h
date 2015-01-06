/*****************************************************************************
 * Copyright (C) 2013 x265 project
 *
 * Authors: Min Chen <chenm003@163.com>
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
 * For more information, contact us at license @ x265.com.
 *****************************************************************************/

#ifndef _INTRAPREDHARNESS_H_1
#define _INTRAPREDHARNESS_H_1 1

#include "testharness.h"
#include "primitives.h"

class IntraPredHarness : public TestHarness
{
protected:

    enum { INPUT_SIZE = 4 * 65 * 65 * 100 };
    enum { OUTPUT_SIZE = 64 * FENC_STRIDE };
    enum { OUTPUT_SIZE_33 = 33 * OUTPUT_SIZE };

    ALIGN_VAR_16(pixel, pixel_buff[INPUT_SIZE]);
    pixel pixel_out_c[OUTPUT_SIZE];
    pixel pixel_out_vec[OUTPUT_SIZE];
    pixel pixel_out_33_c[OUTPUT_SIZE_33];
    pixel pixel_out_33_vec[OUTPUT_SIZE_33];

    bool check_dc_primitive(intra_pred_new_t ref, intra_pred_new_t opt, int width);
    bool check_planar_primitive(intra_pred_new_t ref, intra_pred_new_t opt, int width);
    bool check_angular_primitive(const intra_pred_t ref[][NUM_TR_SIZE], const intra_pred_t opt[][NUM_TR_SIZE]);
    bool check_angular_primitive(const intra_pred_new_t ref[][NUM_TR_SIZE], const intra_pred_new_t opt[][NUM_TR_SIZE]);
    bool check_allangs_primitive(const intra_allangs_t ref[], const intra_allangs_t opt[]);
    bool check_allangs_new_primitive(const intra_allangs_new_t ref[], const intra_allangs_new_t opt[]);

public:

    IntraPredHarness();

    const char *getName() const { return "intrapred"; }

    bool testCorrectness(const EncoderPrimitives& ref, const EncoderPrimitives& opt);

    void measureSpeed(const EncoderPrimitives& ref, const EncoderPrimitives& opt);
};

#endif // ifndef _INTRAPREDHARNESS_H_1
