#ifndef GUARD_derivatives_h
#define GUARD_derivatives_h


VecDoub mat_vec_head(const MatDoub&, const VecDoub&);
VecDoub mat_vec_tail(const MatDoub&, const VecDoub&);


struct NN {

	MatDoub FLN;		// Matrix with FLN connections
	VecDoub etah;		// etah is a vector with 1+eta*h_i, 
	Doub mu_ee;	
	Doub mu_ie;
	Doub eta;			
	
	Doub tau_e;
	Doub tau_i;
	Doub beta_e;
	Doub beta_i;
	
	Doub w_ee;
	Doub w_ei;
	Doub w_ie;
	Doub w_ii;

	Int nodes;		// number of areas in the network

	IS insig;		// insig contains the info of th input signal (IS defined in headers/iput_signal.h)

	VecDoub noise;	// vector contianing the noise
	
	// The constructor
	NN (MatDoub FLN0, VecDoub etah0, Doub mu_ee0, Doub mu_ie0, Doub eta0,
		Doub tau_e0, Doub tau_i0, Doub beta_e0, Doub beta_i0, Doub w_ee0,
		Doub w_ei0, Doub w_ie0, Doub w_ii0, IS insigg, VecDoub noisee) :
		FLN(FLN0), etah(etah0), mu_ee(mu_ee0), mu_ie(mu_ie0), eta(eta0),
		tau_e(tau_e0), tau_i(tau_i0), beta_e(beta_e0), beta_i(beta_i0),
		w_ee(w_ee0), w_ei(w_ei0), w_ie(w_ie0), w_ii(w_ii0), nodes(etah.size()),
		insig(insigg), noise(noisee) {}

	// the activation function: act_func(x) = max(x,0)
	void act_func(Doub &x) { if(x<0) x=0; }

	// external input to an area
	Doub input(Doub t);

	void operator() (const Doub t, VecDoub_I &v, VecDoub_O &dvdt) {
		VecDoub I_lr_e = mat_vec_head(FLN,v);
		VecDoub I_lr_i = mat_vec_tail(FLN,v);

		for(int i=0;i<nodes;i++) {
			dvdt[i] = etah[i]*(w_ee*v[i] + mu_ee*I_lr_e[i]) - w_ei*v[i+nodes];
			if(i!=0) dvdt[i] += noise[i];
			if(i==0) dvdt[i] += input(t);
			act_func(dvdt[i]);
			dvdt[i] *= beta_e;
			dvdt[i] -= v[i];
			dvdt[i] /= tau_e;
		}

		for(int i=0;i<nodes;i++) {
			dvdt[i+nodes] = etah[i]*(w_ie*v[i]+mu_ie*I_lr_i[i]) - w_ii*v[i+nodes] + noise[i+nodes];
			act_func(dvdt[i+nodes]);
			dvdt[i+nodes] *= beta_i;
			dvdt[i+nodes] -= v[i+nodes];
			dvdt[i+nodes] /= tau_i;
		}
	}

};


Doub NN::input(Doub t){ 
	if(insig.type==2) {
		if( t > insig.t_start && t < insig.t_end) {
			return insig.offset+insig.A*sin(t*2*acos(-1)/insig.T - insig.phi);
		} else {
			return 0.0;
		}
	} else if(insig.type==1) { // block function
		if( t > insig.t_start && t < insig.t_end )	{
			return insig.offset;
		} else {
			return 0.0;
		}
	} else { // white noise is added in main
		return 0.0;
	}

}	

VecDoub mat_vec_head(const MatDoub &mat, const VecDoub &v) {
	
	VecDoub out(v.size()/2);
	for(int i=0;i<out.size();i++) {
		out[i] = 0.0;
		for(int j=0;j<out.size();j++) {
			out[i] += mat[i][j]*v[j];
		}
	}
	return out;	
}

VecDoub mat_vec_tail(const MatDoub &mat, const VecDoub &v) {
	VecDoub out(v.size()/2);
	for(int i=0;i<out.size();i++) {
		out[i]=0.0;
		for(int j=0;j<out.size();j++) {
			out[i] += mat[i][j]*v[j];
		}
	}
	return out;
}



#endif


