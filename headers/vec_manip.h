#ifndef GUARD_vec_manip_h
#define GUARD_vec_manip_h

template<class V, class D>
void devide_by(V& v, int size, D d)
{
	for(int i=0;i<size;i++)
		v[i] /= d;
}



#endif

