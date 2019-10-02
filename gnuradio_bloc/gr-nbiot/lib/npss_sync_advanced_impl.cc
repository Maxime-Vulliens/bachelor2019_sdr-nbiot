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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "npss_sync_advanced_impl.h"
#include <volk/volk.h>
#include <gnuradio/fft/fft.h>

namespace gr {
  namespace nbiot {

    npss_sync_advanced::sptr
    npss_sync_advanced::make()
    {
      return gnuradio::get_initial_sptr
        (new npss_sync_advanced_impl());
    }

    /*
     * The private constructor
     */
    npss_sync_advanced_impl::npss_sync_advanced_impl()
      : gr::sync_block("npss_sync_advanced",
              gr::io_signature::make( 1 , 1 , sizeof(gr_complex)),
              gr::io_signature::make( 2 , 2 , sizeof(gr_complex))),
        smooth_correlation(0),
        fine_mode(true)
    {}

    /*
     * Our virtual destructor.
     */
    npss_sync_advanced_impl::~npss_sync_advanced_impl()
    {
    }

    /*
      This method is use to decimate input stream
      param [in] in       : vector of minimum 1500 samples
      param [out] pss_seq : vector of 11 symbols of 17 samples
    */
    void npss_sync_advanced_impl::decimate(const gr_complex *in,  pss_seq_t pss_seq )
    {
      /*
        Need to decimate stream from 1.92MHz to 240KHz 

        1 PSS symbol @ 1.92 MHz = 137 samples 
        1 PSS symbol @ ~240 KHz =  17 samples

        output stream is obtain as follow :
        we take sample : 0, 9, 17, ...(+8)..., 129
        R1[0:9:17:...:129]  R2[137:...:266] ... R11[1370::1499]
      */
      int index_in = 0;

      for (int symbol = 0; symbol < PSS_OFDM_SYMBOLS; symbol++){
        for(int sample = 0; sample < PSS_DECIMATED_SAMPLES_PER_SYMBOL; sample++, index_in+=PSS_DECIMATION){

          // add sample to decimated stream
          pss_seq[symbol][sample] = in[index_in];

          // add 1 if first sample -> first jump is +9
          // as the decimation is not an integer
          if(sample == 0)
            index_in += 1;
        }
      }

    }


    /*
      This method is use to decimate input stream
      param [in] k        : lag-constant parameter
      param [in] pss_seq  : vector of gr_complex values
      param [in] sample_per_symbol : tell how many samples there is for a symbol (17 for decimate / 137 for fine mode)
      return gr_complex   : current correlation value Ak(t)
    */
    gr_complex npss_sync_advanced_impl::auto_corelate( int k,const gr_complex* pss_seq, int sample_per_symbol)
    {
      gr_complex current_correlation(0,0);
      gr_complex conjugate_res;

      for(int m = 0; m < PSS_OFDM_SYMBOLS-k; m++){
        // compute conjugate
        volk_32fc_x2_conjugate_dot_prod_32fc(&conjugate_res,&pss_seq[m*sample_per_symbol],&pss_seq[(m+k)*sample_per_symbol],sample_per_symbol);

        // apply code coverage
        if (CODE_COVER_NPSS[m] != CODE_COVER_NPSS[m+k]){
          current_correlation -= conjugate_res;
        }else{
          current_correlation += conjugate_res;
        }

      }
      current_correlation /= (PSS_OFDM_SYMBOLS-k);
      
      return current_correlation;
    }



    int
    npss_sync_advanced_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = output_items.size()?( gr_complex*) output_items[0]:nullptr;
      gr_complex *out_clean = output_items.size()>1?( gr_complex*) output_items[1]:nullptr;

      const int k = 4; // lag-parameter
      int step = 1;
      int nb_generated_out = 0;

      gr_complex current_correlation;
      pss_seq_t decimate_stream;
      int remaining_samples = noutput_items;




      while (remaining_samples > 1500){


        //if (npss_start_offset + SAMPLES_PER_PSS_SYMBOL - nitems_read(0)+nb_generated_out <)
        if (fine_mode){

          current_correlation = auto_corelate(k, in, SAMPLES_PER_PSS_SYMBOL);
          
        }else{

          decimate(in,decimate_stream);
          current_correlation = auto_corelate(k,(gr_complex*)decimate_stream,PSS_DECIMATED_SAMPLES_PER_SYMBOL);
          
        }
        
        smooth_correlation = (CORR_WEIGHT*smooth_correlation) + ((1-CORR_WEIGHT)*current_correlation);

        //if (abs(smooth_correlation) > max_correlation){
          //max_offset = nitems_read(0) + nb_generated_out;
          //max_correlation = abs(smooth_correlation);
          //arg_at_peak = arg(smooth_correlation);
        //}

        // update out stream with new correalaiton value
        // On 2nd output, the stream is keep as the input stream with some tags added
        for (int i = 0; i < step; ++i)
        {
          *out++ = smooth_correlation;
          *out_clean++ = *(in + i);
        }

        // update loop variables
        remaining_samples -= step;
        nb_generated_out += step;
        in += step;


        // When we process a full frame, chosse a maximum
        //if (first_frame && nitems_read(0)+nb_generated_out >= SAMPLES_PER_FRAME){
        //  first_frame = false;
        //  npss_start_offset = max_offset;
        //}else{

        //}


        
      }



      // Tell runtime system how many output items we produced.
      return nb_generated_out;
    }

  } /* namespace nbiot */
} /* namespace gr */

