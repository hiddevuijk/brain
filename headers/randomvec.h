#ifndef GUARD_randomvec_h
#define GUARD_randomvec_h


void randvec(VecDoub &vec, double mean, double std, Ran &r )
{
	double stdi = sqrt(std*std/vec.size());
	for(int i=0;i<vec.size();i++) {
		vec[i] = bm_transform(r)*stdi + mean;
	}

}



#endif


