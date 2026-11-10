#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef float DTYPE;
typedef hls::axis<float, 0,0,0> dpkt;
#define SIZE 1024 		/* SIZE OF DFT */

void dft(
    hls::stream<dpkt> &real_sample,
    hls::stream<dpkt> &imag_sample,
    hls::stream<dpkt> &real_out,
    hls::stream<dpkt> &imag_out
);
// Use streaming interface for demo
