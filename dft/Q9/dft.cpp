#include "dft.h"

#include<math.h>
#include"coefficients1024.h"

#include<iostream>

const DTYPE TWO_PI = 6.283185482025146484375;
const DTYPE TWO_PI_DIV_SIZE = TWO_PI / (DTYPE)SIZE;

void dft(
	hls::stream<dpkt> real_sample, hls::stream<dpkt> imag_sample,
	hls::stream<dpkt> real_out, hls::stream<dpkt> imag_out)
{
	DTYPE real_cache, imag_cache;
	uint i, j; // loop iterators
	DTYPE c, s; // cos & sin

	DTYPE real_out_buf[SIZE];
	DTYPE imag_out_buf[SIZE];
	
initialization_loop:
	for (i=0; i<SIZE; i++){
		#pragma HLS PIPELINE II=1
		#pragma HLS UNROLL factor=32
		real_out_buf[i] = 0;
		imag_out_buf[i] = 0;
	}

data_loop:
	for (i=0; i<SIZE; i++){
		#pragma HLS PIPELINE II=1
dot_prod_loop:
		for (j=0; j<SIZE; j++){
			#pragma HLS PIPELINE II=1
			// #pragma HLS UNROLL factor=32
			// since streaming in, we dont unroll

			// 1D trig lookup
			uint t = (i * j) % SIZE;
			c = cos_coefficients_table[t];
			s = sin_coefficients_table[t];

			real_cache = real_sample.read()
			imag_cache = imag_sample.read()

			real_out[j] += real_cache * c - imag_cache * s;
			imag_out[j] += real_cache * s + imag_cache * c;
		}
	}

output_loop:
	for (i=0; i<SIZE; i++){
		// maybe there is some sort of casting that needs to happen here.
		real_out.write(real_out_buf[i])
		imag_out.write(imag_out_buf[i]);
	}

}