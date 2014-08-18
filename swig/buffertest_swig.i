/* -*- c++ -*- */

#define BUFFERTEST_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "buffertest_swig_doc.i"

%{
#include "buffertest/buf1.h"
#include "buffertest/buf2.h"
%}


%include "buffertest/buf1.h"
GR_SWIG_BLOCK_MAGIC2(buffertest, buf1);
%include "buffertest/buf2.h"
GR_SWIG_BLOCK_MAGIC2(buffertest, buf2);
