/* -*- c++ -*- */

#define NBIOT_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "nbiot_swig_doc.i"

%{
#include "nbiot/remove_cp.h"
#include "nbiot/custom_fft.h"
#include "nbiot/nsss_decode.h"
#include "nbiot/npss_sync_advanced.h"
#include "nbiot/npss_sync.h"
%}

%include "nbiot/remove_cp.h"
GR_SWIG_BLOCK_MAGIC2(nbiot, remove_cp);
%include "nbiot/custom_fft.h"
GR_SWIG_BLOCK_MAGIC2(nbiot, custom_fft);
%include "nbiot/nsss_decode.h"
GR_SWIG_BLOCK_MAGIC2(nbiot, nsss_decode);

%include "nbiot/npss_sync_advanced.h"
GR_SWIG_BLOCK_MAGIC2(nbiot, npss_sync_advanced);
%include "nbiot/npss_sync.h"
GR_SWIG_BLOCK_MAGIC2(nbiot, npss_sync);
