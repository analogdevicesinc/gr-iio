/* -*- c++ -*- */

/* The adrv9371 block is in a separate file, because Swig chokes on it
 * and produces incorrect Python code if it is present in iio_swig.i... */

#define IIO_API

%include "gnuradio.i"

%{
#include "gnuradio/iio/adrv9371_source.h"
%}

%include "gnuradio/iio/adrv9371_source.h"

GR_SWIG_BLOCK_MAGIC2(iio, adrv9371_source);
GR_SWIG_BLOCK_MAGIC2(iio, adrv9371_source_f32c);
