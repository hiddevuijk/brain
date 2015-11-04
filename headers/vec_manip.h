#ifndef GUARD_vec_manip_h
#define GUARD_vec_manip_h

template<class V, class D>
void devide_by(V& v, int size, D d)
{
	for(int i=0;i<size;i++)
		v[i] /= d;
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

