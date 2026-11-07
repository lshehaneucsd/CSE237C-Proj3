#include "dft.h"

#include<math.h>
#include"coefficients32.h"
#include"coefficients32_2D.h"

#include<iostream>

const DTYPE TWO_PI = 6.283185482025146484375;
const DTYPE TWO_PI_DIV_SIZE = TWO_PI / (DTYPE)SIZE;

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_out[SIZE], DTYPE imag_out[SIZE])
{
	// #pragma HLS ARRAY_PARTITION variable=real_sample block factor=2 dim=1
	// #pragma HLS ARRAY_PARTITION variable=imag_sample block factor=2 dim=1
	// #pragma HLS ARRAY_PARTITION variable=real_out block factor=2 dim=1
	// #pragma HLS ARRAY_PARTITION variable=imag_out block factor=2 dim=1

	uint i, j; // loop iterators
	DTYPE w; // omega
	DTYPE c, s; // cos & sin
	
data_loop:
	for (i=0; i<SIZE; i++){
		// #pragma HLS PIPELINE II=1
		real_out[i] = 0;
		imag_out[i] = 0;
		w = TWO_PI_DIV_SIZE * (DTYPE)i;

dot_prod_loop:
		for (j=0; j<SIZE; j++){
			// #pragma HLS PIPELINE II=1
			// #pragma HLS UNROLL factor=2
			// Math.h trig
			// c = std::cos(j * w);
			// s = -std::sin(j * w);

			// 2D trig lookups
			c = cos_coeff_table[i][j];
			s = sin_coeff_table[i][j];

			// 1D trig lookup
			// uint t = (i * j) % SIZE;
			// c = cos_coefficients_table[t];
			// s = sin_coefficients_table[t];

			real_out[i] += real_sample[j] * c - imag_sample[j] * s;
			imag_out[i] += real_sample[j] * s - imag_sample[j] * c;
		}
		std::cout << i << ": " << real_out[i] << " + " << imag_out[i] << "i\n";
	}
	
	// Outputs to the real_out and imag_out
}
