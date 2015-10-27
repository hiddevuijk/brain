#ifndef GUARD_pcc_h
#define GUARD_pcc_h

#include <cmath>


void pcc_v1(const vector<VecDoub> &m, VecDoub &corr)
{
	int areas = m.size()/2;
	int t_steps = m[0].size();

	double sum_v1e = 0.0;
	double sum_v1i = 0.0;
	double sum_v1e_sq = 0.0;
	double sum_v1i_sq = 0.0;

	for(int i=0;i<t_steps;i++){
		sum_v1e += m[0][i];
		sum_v1i += m[areas][i];
		sum_v1e_sq += m[0][i]*m[0][i];
		sum_v1i_sq += m[areas][i]*m[areas][i];
	}


	for(int i=0;i<areas;i++) {
		double sum_12e = 0.0;
		double sum_12i = 0.0;
		double sum_v2e = 0.0;
		double sum_v2i = 0.0;
		double sum_v2e_sq = 0.0;
		double sum_v2i_sq = 0.0;

		for(int t=0;t<t_steps;t++) {
			sum_12e += m[0][t]*m[i][t];
			sum_12i += m[areas][t]*m[i+areas][t];
			sum_v2e += m[i][t];
			sum_v2i += m[i+areas][t];
			sum_v2e_sq += m[i][t]*m[i][t];
			sum_v2i_sq += m[i+areas][t]*m[i+areas][t];
		}
		double tempe = sum_12e - sum_v1e*sum_v2e/t_steps;
		double tempi = sum_12i - sum_v1i*sum_v2i/t_steps;

		tempe /= sqrt( (sum_v1e_sq - sum_v1e*sum_v1e/t_steps)*(sum_v2e_sq - sum_v2e*sum_v2e/t_steps) );
		tempi /= sqrt( (sum_v1i_sq - sum_v1i*sum_v1i/t_steps)*(sum_v2i_sq - sum_v2i*sum_v2i/t_steps) );

		corr[i] = tempe;
		corr[i+areas] = tempi;
	}
}




#endif

