#include "dft.h"

#include<math.h>
#include"coefficients32.h"
#include"coefficients32_2D.h"

#include<iostream>

const DTYPE TWO_PI = 6.283185482025146484375;
const DTYPE TWO_PI_DIV_SIZE = TWO_PI / (DTYPE)SIZE;

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE])
{
	uint i, j; // loop iterators
	DTYPE w; // omega
	DTYPE c, s; // cos & sin
	DTYPE temp_real[SIZE];
	DTYPE temp_imag[SIZE];
	
data_loop:
	for (i=0; i<SIZE; i++){
		temp_real[i] = 0;
		temp_imag[i] = 0;
		w = TWO_PI_DIV_SIZE * (DTYPE)i;

dot_prod_loop:
		for (j=0; j<SIZE; j++){
			// Math.h trig
			c = std::cos(j * w);
			s = -std::sin(j * w);

			// 2D trig lookups
			// c = cos_coeff_table[i][j];
			// s = sin_coeff_table[i][j];

			// 1D trig lookup
			// uint t = (i * j) % SIZE;
			// c = cos_coefficients_table[t];
			// s = sin_coefficients_table[t];

			temp_real[i] += real_sample[j] * c - imag_sample[j] * s;
			temp_imag[i] += real_sample[j] * s + imag_sample[j] * c;
		}
		std::cout << i << ": " << temp_real[i] << " + " << temp_imag[i] << "i\n";
	}

	for (i=0; i<SIZE; i++){
		real_sample[i] = temp_real[i];
		imag_sample[i] = temp_imag[i];
	}
}
