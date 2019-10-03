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
#include "custom_fft_impl.h"

namespace gr {
  namespace nbiot {

    custom_fft::sptr
    custom_fft::make()
    {
      return gnuradio::get_initial_sptr
        (new custom_fft_impl());
    }

    /*
     * The private constructor
     */
    custom_fft_impl::custom_fft_impl()
      : gr::block("custom_fft",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
        found_subframe_start(false),
        max_magnitude(0)
    {
      // Initialize fft object and get reference to buffers
      d_fft = new gr::fft::fft_complex(FFT_LEN);
      buffer_in = d_fft->get_inbuf();
      buffer_out = d_fft->get_outbuf(); 

      // do not propagate TAG as sample rate will change
      set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    custom_fft_impl::~custom_fft_impl()
    {
      delete d_fft;
    }


    // Method used to tell GNU Radio scheduler how many input itemps is needed
    // to generate a given number of output
    // In this case we need input stream need 11 times more samples thant output stream.
    void
    custom_fft_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
       // as we keep 12 samples over 128, it's closed to 11 times more
       ninput_items_required[0] = noutput_items * 11;
    }

    int
    custom_fft_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      // Get in/out stream reference
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // local variable
      int remaining_samples = ninput_items[0];
      int generated_sample = 0;
      std::vector<tag_t> tags;

      // Look for tag in input stream
      get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + remaining_samples);

      // If we don't already fine a frame start, get it from tags
      if (!found_subframe_start){
        if (tags.size() > 0){
          found_subframe_start = true;
          std::cout << "FFT: offset : " << tags[0].offset << " Size : " << tags.size() << " \n";
          subframe_number = pmt::to_long(tags[0].value);
        }
      }


      // execute the FFT until there is more than 128 samples in input stream
      // and that we not reach maximum outpout samples needed
      while (remaining_samples >= FFT_LEN && generated_sample < noutput_items){

        if (remaining_samples >= FFT_LEN){

          // Copy all samples to input buffer
          memcpy(buffer_in, in, FFT_LEN*sizeof(gr_complex));

          // Execut fft
          d_fft->execute();

          // Sliding max magnitude detection (for normalization)
          float local_max_magnitude = 0;
          for ( int i=1; i<=NB_SUBCARRIERS; i++ ) {
            if ( abs(buffer_out[i]) > local_max_magnitude ) {
              local_max_magnitude = abs(buffer_out[i]);
            }
          }

          // Smooth the max magnitude to support idle samples
          if ( local_max_magnitude > max_magnitude ) {
            max_magnitude = local_max_magnitude;
          } else {
            max_magnitude *= 0.98;
          }

          // Copy output to out stream
          for (int i = 1; i <= NB_SUBCARRIERS; ++i)
          {
            *out++ = *(buffer_out+i)/max_magnitude;
          }

          // Add tag to the stream
          if ((nitems_written(0) + generated_sample) % ELEMENTS_PER_SUBFRAME == 0 ){
            if (subframe_number == NPSS_FRAME_INDEX){
            add_item_tag(0,
              nitems_written(0) + generated_sample,
              pmt::string_to_symbol("start_npss"), 
              pmt::from_long(subframe_number),
              pmt::string_to_symbol(this->name() ));  
          }else{
            add_item_tag(0,
              nitems_written(0) + generated_sample,
              pmt::string_to_symbol("start_sub_frame"), 
              pmt::from_long(subframe_number),
              pmt::string_to_symbol(this->name() ));
          }
            // increment subframe number
            subframe_number = (subframe_number+1)%SUBFRAMES_PER_FRAME;
          }

          // Adjust local variable
          in+= FFT_LEN;
          remaining_samples -= FFT_LEN;
          generated_sample += NB_SUBCARRIERS;
        }
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (ninput_items[0] - remaining_samples);

      // Tell runtime system how many output items we produced.
      return generated_sample;
    }

  } /* namespace nbiot */
} /* namespace gr */

