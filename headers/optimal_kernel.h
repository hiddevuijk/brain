#ifndef GUARD_optimal_kernel_h
#define GUARD_optimal_kernel_h


template<class V>
void optimal_kernel(V s,V r, V& D, int size)
{
	realft(s,1);
	realft(r,1);

	for(int i=0;i<size;i+=2) {
		double r_re = r[i];
		double r_im = r[i+1];
		double s_re = s[i];
		double s_im = s[i+1];

		D[i] = (r_re*s_re + r_im*s_im)/(s_re*s_re + s_im*s_im);
		D[i+1] = (r_im*s_re-  r_re*s_im)/(s_re*s_re + s_im*s_im);
	}
	D[0] = r[0]/s[0];
	D[1] = r[1]/s[1];

	realft(D,-1);
	for(int i=0;i<size;i++) D[i] *= 2./size;

}




#endif

