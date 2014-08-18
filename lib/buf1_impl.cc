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
#include "buf1_impl.h"

#include <stdio.h>
#include <gnuradio/buffer.h>

#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

// MAGIC is from the magic number list
#define GNURADIO_MAGIC 'G'
#define READ_IOCTL _IOR(GNURADIO_MAGIC, 0, int)
#define WRITE_IOCTL _IOW(GNURADIO_MAGIC, 1, int)

struct buffer_struct {
    void *addr;
    size_t size;
};

namespace gr {
  namespace buffertest {

    buf1::sptr
    buf1::make()
    {
      return gnuradio::get_initial_sptr
        (new buf1_impl());
    }

    /*
     * The private constructor
     */
    buf1_impl::buf1_impl()
      : gr::sync_block("buf1",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
		this->set_accel(true);
	}

    /*
     * Our virtual destructor.
     */
    buf1_impl::~buf1_impl()
    {
    }

    int
    buf1_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];
		struct buffer_struct buf;
		
		/* 
		 * get_buffer() get the buffer object
		 * buffer->base() start address of buffer
		 * buffer->bufsize() buffer size in items
		 * buffer->get_sizeof_item() item size in bytes
		 */
        buffer_sptr buffer = get_buffer();
		
		printf("ptr_base1: %p size: %d\n", buffer->base(), buffer->bufsize());
        printf("sizeofitem: %ld\n", buffer->get_sizeof_item());
		printf("writ_ptr1: %p\n", &out[0]);
		
	    buf.addr = buffer->base();
		buf.size = (buffer->bufsize())*(buffer->get_sizeof_item());
		
		/*
		 * Open up the char device and ioctl the gnuradio buffer into it
		 * seems to have trouble allocating all the pages. I can do 6
		 * just fine but not more. Expected 16 since PAGE_SIZE = 4k
		 */ 
		int fd = -2;
		if ((fd = open("/dev/gnuradio", O_RDWR)) < 0) {
			perror("open");
			return -1;
		}
		int num;
		if(ioctl(fd, WRITE_IOCTL, &buf) < 0)
			perror("first ioctl");
		if(ioctl(fd, READ_IOCTL, &num) < 0)
			perror("second ioctl");

		printf("message: %d\n", num);
		
		for(int i = 0; i < noutput_items; i++) {
			out[i] = in[i];
		}
		
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace buffertest */
} /* namespace gr */

