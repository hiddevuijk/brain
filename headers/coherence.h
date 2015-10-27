#ifndef GUARD_coherence_h
#define GUARD_coherence_h

#include <math.h>

// The fourier transformation routine comes from NR3
// Needed inlcudes: nr3.h and fourier.h 


// namespace with constants and helper functions used in coherence.h
namespace coh {
	const double PI = acos(-1);

	template<class V>
	void remove_complex(V& x,int size) {
		V temp(size/2,0.0);
		for(int j=0;j<size/2-1;j++) temp[j] = x[j*2];
		temp[size/2 - 1] = x[1];
		x = temp;
	}

	template<class V>
	void abs_sq_vec(V& x, int size) {
		V temp(size/2,0.0);
		temp[0] = x[0]*x[0];
		for(int i=1;i<size/2;i++) {
			temp[i] = x[2*i]*x[2*i] + x[2*i+1]*x[2*i+1];
		}
		temp[size/2-1] = x[1]*x[1];
		x = temp;
	}


	// hanning filter (time domain)
	// i is the time index, N is the total number of points
	double hann(int i,int N)
	{
		return 0.5*(1-cos(2*coh::PI*i/N));
	}



}


template<class V>
V coherence(const V& x, const V& y, int N, int SN, double (*filter)(int,int)=coh::hann)
{
	V Sxx(SN,0.0);
	V Syy(SN,0.0);
	V Sxy(SN,0.0);

	for(int i=0i;i*SN<N;i++) {
		VecDoub xtemp(SN,0.0);
		VecDoub ytemp(SN,0.0);
	
		for(int j=0;j<SN;j++) {
			xtemp[j] = x[j+i*SN]*filter(j,SN);
			ytemp[j] = y[j+i*SN]*filter(j,SN);
		}

		realft(xtemp,1);
		realft(ytemp,1);

		for(int k=0;k<SN;k+=2) {
			double xr = xtemp[k];
			double xi = xtemp[k+1];
			double yr = ytemp[k];
			double yi = ytemp[k+1];

			if(k==0) {
				Sxx[k] += xr*xr;
				Syy[k] += yr*yr;
				Sxy[k] += xr*yr;

				Sxx[k+1] += xi*xi;
				Syy[k+1] += yi*yi;
				Sxy[k+1] += xi*yi;
			} else {
				Sxx[k] += xr*xr + xi*xi;
				Syy[k] += yr*yr + yi*yi;
				Sxy[k] += xr*yr + xi*yi;
				Sxy[k+1] += xi*yr - xr*yi;
			}
		}
	}

	coh::remove_complex(Sxx,SN);
	coh::remove_complex(Syy,SN);
	coh::abs_sq_vec(Sxy,SN);

	V c(SN/2,0.0);
	for(int i=0;i<SN/2;i++) 
		c[i] = Sxy[i]/(Sxx[i]*Syy[i]);

	return c;
} 



// frequencies modifies the values of freq to the frequencies of the coherence function output
// freq should be half the size of the input x and y to coherence
// N is the size of freq, dt is the timestepsize of x and y.
template<class V>
void frequencies(V& freq, int N, double dt)
{
	for(int i=0;i<N;i++) freq[i] = 0.5*i/(N*dt);
}


#endif

