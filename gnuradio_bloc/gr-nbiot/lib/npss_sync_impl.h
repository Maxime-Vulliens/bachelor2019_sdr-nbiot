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

#ifndef INCLUDED_NAGRASDR_NPSS_SYNC_H
#define INCLUDED_NAGRASDR_NPSS_SYNC_H

#include <nbiot/npss_sync.h>
#include <list>
#include "global.h"

namespace gr {
  namespace nbiot {

    #define PSS_SYMBOLS (11)

    class npss_sync_impl : public npss_sync
    {
     private:
      #define DUMP_FILE "dump_npss_sync.log"

      // Code-cover for Zadoff-Chu sequence
      const int CODE_COVER_NPSS[PSS_SYMBOLS]={1,1,1,1,-1,-1,1,1,1,-1,1};
      const float CORR_WEIGHT = 0;  // used to smooth correlation function / o = no smoothing
      const int THRESHOLD_VALUE = 20;


      float max_abs_correlation;
      float average_abs_correlation;

      int first_loop;
      int nb_items;
      int fine_mode;
      int step;
      int last_nb_items;
      int last_diff_nb_items;
      int nb_sample_treshold_detect;
      int npss_start_samples;
      int counter;
      int set_tag;
      int alert_generator;

      float max_magnitude;
      float arg_at_peak;
      int offset_at_peak;

      float base_freq;
      float freq_sent;

      gr_complex smooth_correlation;

      FILE *fdump;

      gr_complex auto_corelate( int k,const gr_complex* pss_seq, int sample_per_symbol);

     public:
      npss_sync_impl();
      ~npss_sync_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace nbiot
} // namespace gr

#endif /* INCLUDED_NAGRASDR_NPSS_FINE_MODE_IMPL_H */



