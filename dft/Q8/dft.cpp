#include "dft.h"

#include<math.h>
#include"coefficients1024.h"

#include<iostream>

#define UNROLL_FACTOR 8

const DTYPE TWO_PI = 6.283185482025146484375;
const DTYPE TWO_PI_DIV_SIZE = TWO_PI / (DTYPE)SIZE;

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_out[SIZE], DTYPE imag_out[SIZE])
{
	#pragma HLS ARRAY_PARTITION variable=real_sample cyclic factor=UNROLL_FACTOR dim=1
	#pragma HLS ARRAY_PARTITION variable=imag_sample cyclic factor=UNROLL_FACTOR dim=1
	#pragma HLS ARRAY_PARTITION variable=real_out cyclic factor=UNROLL_FACTOR dim=1
	#pragma HLS ARRAY_PARTITION variable=imag_out cyclic factor=UNROLL_FACTOR dim=1
	uint i, j; // loop iterators
	DTYPE c, s; // cos & sin
	
initialization_loop:
	for (i=0; i<SIZE; i++){
		#pragma HLS PIPELINE II=1
		#pragma HLS UNROLL factor=UNROLL_FACTOR
		real_out[i] = 0;
		imag_out[i] = 0;
	}

data_loop:
	for (i=0; i<SIZE; i++){
		#pragma HLS PIPELINE off
dot_prod_loop:
		for (j=0; j<SIZE; j++){
			#pragma HLS PIPELINE II=1
			#pragma HLS UNROLL factor=UNROLL_FACTOR
			// 1D trig lookup
			uint t = (i * j) % SIZE;
			c = cos_coefficients_table[t];
			s = sin_coefficients_table[t];

			real_out[j] += real_sample[i] * c - imag_sample[i] * s;
			imag_out[j] += real_sample[i] * s + imag_sample[i] * c;
		}
	}

// debug_print_loop:
// 	for (i=0; i<SIZE; i++){
// 		std::cout << i << ": " << real_out[i] << " + " << imag_out[i] << "i\n";
// 	}

}