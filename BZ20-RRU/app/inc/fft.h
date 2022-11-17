#ifndef FFT_H
#define FFT_H

#include <valarray>
#include <complex>

typedef std::complex<double>   Complex;
typedef std::valarray<Complex> CArray;

void fft(CArray& x);

#endif // FFT_H
