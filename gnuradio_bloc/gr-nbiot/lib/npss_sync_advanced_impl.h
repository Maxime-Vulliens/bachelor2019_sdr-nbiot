/* -*- c++ -*- */
/* 
 * Copyright 2019 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_NBIOT_NPSS_SYNC_ADVANCED_IMPL_H
#define INCLUDED_NBIOT_NPSS_SYNC_ADVANCED_IMPL_H

#include <nbiot/npss_sync_advanced.h>
#include "global.h"

namespace gr {
  namespace nbiot {

    #define PSS_DECIMATED_SAMPLES_PER_SYMBOL (17)
    #define PSS_OFDM_SYMBOLS (11)
    #define PSS_DECIMATION              (8)
    typedef gr_complex pss_symb_t[PSS_DECIMATED_SAMPLES_PER_SYMBOL];  // 17 samples per PSS symbol
    typedef pss_symb_t pss_seq_t[PSS_OFDM_SYMBOLS];            // 11 PSS symbol per PSS sequence

    class npss_sync_advanced_impl : public npss_sync_advanced
    {
     private:
      // Nothing to declare in this block.

      // Code-cover for Zadoff-Chu sequence
      const int CODE_COVER_NPSS[PSS_OFDM_SYMBOLS]={1,1,1,1,-1,-1,1,1,1,-1,1};
      const float CORR_WEIGHT = 0;  // used to smooth correlation function / o = no smoothing
      const int THRESHOLD_VALUE = 20;


      gr_complex smooth_correlation;
      bool fine_mode;

      gr_complex auto_corelate( int k,const gr_complex* pss_seq, int sample_per_symbol);
      void decimate(const gr_complex *in,  pss_seq_t pss_seq );

     public:
      npss_sync_advanced_impl();
      ~npss_sync_advanced_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace nbiot
} // namespace gr

#endif /* INCLUDED_NBIOT_NPSS_SYNC_ADVANCED_IMPL_H */

