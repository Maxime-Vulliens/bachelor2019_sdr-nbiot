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

#ifndef INCLUDED_NAGRASDR_CUSTOM_FFT_IMPL_H
#define INCLUDED_NAGRASDR_CUSTOM_FFT_IMPL_H


#include <gnuradio/fft/fft.h>
#include <gnuradio/fft/window.h>
#include <nbiot/custom_fft.h>

#include "global.h"

namespace gr {
  namespace nbiot {

    class custom_fft_impl : public custom_fft
    {
     private:

      // Variable to execute an fft
      gr::fft::fft_complex  *d_fft;
      // Need to reference in and out buffer of fft
      gr_complex        *buffer_in;
      gr_complex        *buffer_out;

      // max_magnitude is used for nomralize
      float max_magnitude;

      // Kreep subframe information
      bool found_subframe_start;
      int subframe_number;

     public:
      custom_fft_impl();
      ~custom_fft_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace nagrasdr
} // namespace gr

#endif /* INCLUDED_NAGRASDR_CUSTOM_FFT_IMPL_H */

