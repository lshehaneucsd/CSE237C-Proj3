/*
This is DFT computation using matrix vector multiplication form.
INPUT:
	In_R, In_I[]: Real and Imag parts of Complex signal in time domain.
OUTPUT:
	Out_R, Out_I[]: Real and Imag parts of Complex signal in frequency domain.

*/

#include<stdio.h>
#include <stdlib.h>
#include<iostream>
#include <math.h>
#include "dft.h"

struct Rmse
{
	int num_sq;
	float sum_sq;
	float error;

	Rmse(){ num_sq = 0; sum_sq = 0; error = 0; }

	float add_value(float d_n)
	{
		num_sq++;
		sum_sq += (d_n*d_n);
		error = sqrtf(sum_sq / num_sq);
		return error;
	}

};


Rmse rmse_R,  rmse_I;

DTYPE In_R[SIZE], In_I[SIZE],Out_R[SIZE],Out_I[SIZE];   //Modify the testbench while checking for demo. You will have to access the data variable of the structure//
hls::stream<dpkt> in_r_s, in_i_s, out_r_s, out_i_s;
dpkt in_r_pkt, in_i_pkt, out_r_pkt, out_i_pkt;

int main()
{
	int index;
	float gold_R, gold_I;

	FILE * fp = fopen("out.gold.dat","r");

	// getting input data
	for(int i=0; i<SIZE; i++)
	{
		In_R[i] = i;
		In_I[i] = 0.0;

	}
	
	// Write to stream
	for(int i=0; i<SIZE; i++)
	{
		in_r_pkt.data = In_R[i];
		in_i_pkt.data = In_I[i];
		if (i== SIZE-1){
			in_r_pkt.last = 1;
			in_i_pkt.last = 1;
		}
		else{
			in_r_pkt.last = 0;
			in_i_pkt.last = 0;
		}
		in_r_s.write(in_r_pkt);
		in_i_s.write(in_i_pkt);
	}

	// DFT
	dft(in_r_s, in_i_s,out_r_s,out_i_s);


	// comparing with golden output
	for(int i=0; i<SIZE; i++)
	{
		out_r_pkt = out_r_s.read();
		out_i_pkt = out_i_s.read();

		Out_R[i] = out_r_pkt.data;
		Out_I[i] = out_i_pkt.data;

        if ((i < SIZE - 1 && (out_r_pkt.last != 0 || out_i_pkt.last != 0)) ||
            (i == SIZE - 1 && (out_r_pkt.last != 1 || out_i_pkt.last != 1))) {
            printf("FAIL: TLAST mismatch at index %d\n", i);
            return 1;
        }
		
		fscanf(fp, "%d %f %f", &index, &gold_R, &gold_I);
		rmse_R.add_value((float)Out_R[i] - gold_R);
		rmse_I.add_value((float)Out_I[i] - gold_I);
	}
	fclose(fp);


	// printing error results
	printf("----------------------------------------------\n");
	printf("   RMSE(R)           RMSE(I)\n");
	printf("%0.15f %0.15f\n", rmse_R.error, rmse_I.error);
	printf("----------------------------------------------\n");

	if (rmse_R.error > 0.1 || rmse_I.error > 0.1 ) {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
		fprintf(stdout, "*******************************************\n");
	    return 1;
	}else {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "PASS: The output matches the golden output!\n");
		fprintf(stdout, "*******************************************\n");
	    return 0;
	}

}
