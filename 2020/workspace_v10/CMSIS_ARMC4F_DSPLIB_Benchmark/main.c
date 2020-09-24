//#include "config.h"
#include <math.h>					//we use cos()
#include <stdlib.h>					//we use rand()
#include <stdio.h>
#include <time.h>
#include "fft_float.h"

//hardware configuration
//end hardware configuration

//global defines
double dat_real[FFT_N], dat_imag[FFT_N];
//double lut_sin[FFT_N/2], lut_cos[FFT_N/2];		//look-up table for sin/cos
//double lut_twd[FFT_N];							//cos..sin look-up table: even point/cos, odd point/sin

//global variables
//compx dat[FFT_N];	//data

//generate data for fft
void fft_init(int n, double *real, double *imag) {
	int i;

	for (i=0; i<n; i++) {
		real[i]= 	200+										//dc content
					100 * cos(2.0*M_PI * 1 * i / n) +			//1f content
					10 * cos(2.0*M_PI * 2 * i / n) +			//2f content
					1 * ((double) rand() / RAND_MAX  - 0.5);	//noise
		imag[i]=0;
	}
}

compx xin[FFT_N];

int main(void) {
	int i;

	//srand(time(NULL));								//seed the random generator
	lut_cal();										//initialize cos/sin lut
	while (1) {
		fft_init(FFT_N, dat_real, dat_imag);							//initialize data arrays, 5.6M ticks / 256 points
		//fft_ditr2dj(FFT_LOG2N, dat_real, dat_imag);		//origian douglas jones dit fft radix 2
		//fft_ditr2(FFT_LOG2N, dat_real, dat_imag);			//douglas jones dit fft radix 2 with look-up tables

		//prepare data for fft_float()
		for (i=0; i<FFT_N; i++) {
			xin[i].real=dat_real[i];
			xin[i].imag=dat_imag[i];
		}
		fft_float(FFT_LOG2N, xin);							//brutal force fft
		//return data for dat_real/dat_imag
		for (i=0; i<FFT_N; i++) {
			dat_real[i]=xin[i].real;
			dat_imag[i]=xin[i].imag;
		}

		//print out the results
		i=0; printf("dat_real[%5d]=%10.5f, dat_imag[%5d]=%10.5f\n\r", i, dat_real[i]/(FFT_N/1), i, dat_imag[i]/(FFT_N/1));
		for (i=1; i<FFT_N; i++)
			printf("dat_real[%5d]=%10.5f, dat_imag[%5d]=%10.5f\n\r", i, dat_real[i]/(FFT_N/2), i, dat_imag[i]/(FFT_N/2));
		break;										//only run it once
	}
	return 0;
}

