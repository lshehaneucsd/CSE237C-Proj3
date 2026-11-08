#include <ap_int.h>
#include <hls_stream.h>

// typedef float DTYPE;
typedef hls::axis<float, 0,0,0> dpkt;
#define SIZE 1024 		/* SIZE OF DFT */
// hls::stream<DTYPE> dtype_stream;

void dft(
    hls::stream<dpkt> real_sample,
    hls::stream<dpkt> imag_sample,
    hls::stream<dpkt> real_op,
    hls::stream<dpkt> imag_op
);
// Use streaming interface for demo
