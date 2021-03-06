/* -*- c++ -*- */
/*
 * Copyright 2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "float_to_char_impl.h"
#include <gnuradio/io_signature.h>
#include <volk/volk.h>

namespace gr {
namespace blocks {

float_to_char::sptr float_to_char::make(size_t vlen, float scale)
{
    return gnuradio::make_block_sptr<float_to_char_impl>(vlen, scale);
}

float_to_char_impl::float_to_char_impl(size_t vlen, float scale)
    : sync_block("float_to_char",
                 io_signature::make(1, 1, sizeof(float) * vlen),
                 io_signature::make(1, 1, sizeof(char) * vlen)),
      d_vlen(vlen),
      d_scale(scale)
{
    const int alignment_multiple = volk_get_alignment() / sizeof(char);
    set_alignment(std::max(1, alignment_multiple));
}

int float_to_char_impl::work(int noutput_items,
                             gr_vector_const_void_star& input_items,
                             gr_vector_void_star& output_items)
{
    const float* in = (const float*)input_items[0];
    int8_t* out = (int8_t*)output_items[0];

    volk_32f_s32f_convert_8i(out, in, d_scale, d_vlen * noutput_items);

    return noutput_items;
}

} /* namespace blocks */
} /* namespace gr */
