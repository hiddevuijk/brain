#ifndef GUARD_correlation_h
#define GUARD_correlation_h



//calculates the auto corelation of the rows in the matrix yt
void auto_correlation( const MatDoub &yt, MatDoub &corr){
	int nareas = yt.nrows();	// number of areas 
	int nt = yt.ncols(); 		// number of time steps

	for(int n=0;n<nareas;n++){
		for(int i=0;i<nt;i++){
			for(int j=0;j<(nt-i);j++){
				corr[n][i] += yt[n][j]*yt[n][j+i];
			}
	//		corr[n][i] /= nt-i;
		}
	}
	// normalize correlation w.r.t corr[n][0]
	for(int n=0;n<nareas;n++) {
		double corr0 = corr[n][0];
		for(int i=0;i<nt;i++) {
			corr[n][i] /= corr0;
		}
	}

}

MatDoub acorr(const MatDoub &yt)
{
	int areas = yt.nrows();
	int t_steps = yt.ncols();
	int t_steps2 = 2*t_steps;
	--t_steps2;
	t_steps2 |= t_steps2 >> 1;
	t_steps2 |= t_steps2 >> 2;
	t_steps2 |= t_steps2 >> 4;
	t_steps2 |= t_steps2 >> 8;
	t_steps2 |= t_steps2 >> 16;
	++t_steps2;

	MatDoub out(areas,t_steps2,0.0);
	for(int a=0;a<areas;a++) {
		VecDoub temp(t_steps2,0.0);
		VecDoub temp_out(t_steps2,0.0);
		for(int i=0;i<t_steps;i++) temp[i] = yt[a][i];
		correl(temp,temp,temp_out);
		for(int i=0;i<t_steps2;i++) out[a][i] = temp_out[i];
	}

	return out;
}


MatDoub corr(const MatDoub &yt, const VecDoub &v)
{
	int areas = yt.nrows();
	int t_steps = yt.ncols();
	int t_steps2 = 2*t_steps;
	--t_steps2;
	t_steps2 |= t_steps2 >> 1;
	t_steps2 |= t_steps2 >> 2;
	t_steps2 |= t_steps2 >> 4;
	t_steps2 |= t_steps2 >> 8;
	t_steps2 |= t_steps2 >> 16;
	++t_steps2;
	
	MatDoub out(areas,t_steps2,0.0);
	for(int a=0;a<areas;a++) {
		VecDoub temp1(t_steps2,0.0);
		VecDoub temp2(t_steps2,0.0);
		VecDoub temp_out(t_steps2,0.0);
		for(int i=0;i<t_steps;i++) {
			temp1[i] = yt[a][i];
			temp2[i] = v[i];
		}
		correl(temp1,temp2,temp_out);
		for(int i=0;i<t_steps2;i++) out[a][i] = temp_out[i];
	}

	return out;
}

MatDoub corr(const MatDoub &yt, int cw)
{
	int areas = yt.nrows();
	int t_steps = yt.ncols();
	int t_steps2 = 2*t_steps;
	--t_steps2;
	t_steps2 |= t_steps2 >> 1;
	t_steps2 |= t_steps2 >> 2;
	t_steps2 |= t_steps2 >> 4;
	t_steps2 |= t_steps2 >> 8;
	t_steps2 |= t_steps2 >> 16;
	++t_steps2;
	
	MatDoub out(areas,t_steps2,0.0);
	for(int a=0;a<areas;a++) {
		VecDoub temp1(t_steps2,0.0);
		VecDoub temp2(t_steps2,0.0);
		VecDoub temp_out(t_steps2,0.0);
		for(int i=0;i<t_steps;i++) {
			temp1[i] = yt[cw][i];
			temp2[i] = yt[a][i];
		}
		correl(temp1,temp2,temp_out);
		for(int i=0;i<t_steps2;i++) out[a][i] = temp_out[i];
	}

	return out;
}

#endif

