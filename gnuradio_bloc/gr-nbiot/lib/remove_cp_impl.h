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

#ifndef INCLUDED_NAGRASDR_REMOVE_CP_IMPL_H
#define INCLUDED_NAGRASDR_REMOVE_CP_IMPL_H

#include <nbiot/remove_cp.h>
#include <queue>

namespace gr {
  namespace nbiot {

    class remove_cp_impl : public remove_cp
    {
     private:
      // private variables
      bool foud_frame_start;
      int frame_number;
      int counter;
      int subframe_number;

      // queue to keep tags
      std::queue<int> subframes_end;

      // Buffer in
      gr_complex buffer_in[SAMPLES_PER_SUBFRAME];
      int buffer_in_index;

      // buffer out
      gr_complex buffer_out[10*SAMPLES_PER_SUBFRAME_WITHOUT_CP];
      int buffer_out_size;

      long copy_samples_from_in_to_out(gr_complex* out, const gr_complex* in);

     public:
      remove_cp_impl();
      ~remove_cp_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace nagrasdr
} // namespace gr

#endif /* INCLUDED_NAGRASDR_CP_REMOVER_IMPL_H */

