#ifndef GUARD_corr2_h
#define GUARD_corr2_h

#include <math.h>


template<class V>
double mean(const V&, int);

template<class V>
double stdev(const V&, int);

template<class V>
void crrl(const V& x,const V& y,  V& c, int size, bool norm = false)
{
	double mx = mean(x,size);
	double sx = stdev(x,size);
	double my = mean(y,size);
	double sy = stdev(y,size);

	for(int i=0;i<size;i++)
		for(int j=0;j<(size-i);j++)
			c[i] += (x[j]-mx)*(y[j+i]-my)/(sx*sy);	

	if(norm) {
		double c0 = c[0];
		for(int i=0;i<size;i++) c[i] /= c0;
	}

}




template<class V>
double mean(const V& v, int size)
{
	double m = 0.0;
	for(int i=0;i<size;i++) m += v[i]/size;
	return m;
}

template<class V>
double stdev(const V& v, int size)
{
	double s = 0.0;
	double m = mean(v,size);
	for(int i=0;i<size;i++) s += (v[i]-m)*(v[i]-m)/size;
	return sqrt(s);
}
	

#endif

