#include "dft.h"

#include<math.h>
#include"coefficients1024.h"

#include<iostream>

const DTYPE TWO_PI = 6.283185482025146484375;
const DTYPE TWO_PI_DIV_SIZE = TWO_PI / (DTYPE)SIZE;

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_out[SIZE], DTYPE imag_out[SIZE])
{
	uint i, j; // loop iterators
	DTYPE c, s; // cos & sin
	
initialization_loop:
	for (i=0; i<SIZE; i++){
		real_out[i] = 0;
		imag_out[i] = 0;
	}

data_loop:
	for (i=0; i<SIZE; i++){

dot_prod_loop:
		for (j=0; j<SIZE; j++){
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