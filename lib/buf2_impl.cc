/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "buf2_impl.h"

#include <stdio.h>
#include <gnuradio/buffer.h>

namespace gr {
  namespace buffertest {

    buf2::sptr
    buf2::make()
    {
      return gnuradio::get_initial_sptr
        (new buf2_impl());
    }

    /*
     * The private constructor
     */
    buf2_impl::buf2_impl()
      : gr::sync_block("buf2",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
		this->set_accel(true);
	}

    /*
     * Our virtual destructor.
     */
    buf2_impl::~buf2_impl()
    {
    }

    int
    buf2_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];

		/*
		 * Just a check to make sure that buf1 and buf2 aren't the same
		 * buffer by checking their base address
		 */
        buffer_sptr buffer = get_buffer();
        printf("ptr_base2: %p size: %d\n", buffer->base(), buffer->bufsize());
		printf("writ_ptr2: %p\n", &out[0]);
		
        // Do <+signal processing+>
		for(int i = 0; i < noutput_items; i++) {
			out[i] = in[i];
		}

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace buffertest */
} /* namespace gr */

