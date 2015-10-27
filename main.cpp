//nr_headers
#include "nr_headers/nr3.h"
#include "nr_headers/ran.h"
#include "nr_headers/fourier.h"
#include "nr_headers/correl.h"
#include "nr_headers/stepper.h"
#include "nr_headers/stepperbs.h"
#include "nr_headers/odeint.h"
#include "nr_headers/stepperdopr853.h"
#include "nr_headers/stepperdopr5.h"
#include "nr_headers/stepperstoerm.h"

//std headers
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <string>

// other headers
#include "headers/other.h"
#include "headers/box_muller.h"
#include "headers/input_signal.h"
#include "headers/derivatives.h"
#include "headers/read_variables.h"
#include "headers/randomvec.h"
#include "headers/corr.h"
#include "headers/write_matrix.h"
#include "headers/pcc.h"
#include "headers/coherence.h"

using namespace::std;

int main()
{

	Int nareas =29;
	Doub mu_ee, mu_ie, eta,
		 tau_e, tau_i, beta_e, beta_i,
		 w_ee, w_ei, w_ie, w_ii,
		 noise;

	Doub atol, rtol, h1, hmin, t1, t2;
	Int npoints;
	Int routine;
	double dt;
	Int seed;
	
	SN = 256;		// number of points in Welch segments for coherence

	IS insig  = read_input_signal("input/input_signal.txt");
	read_NN_variables(mu_ee,mu_ie,eta,tau_e,tau_i,beta_e,beta_i,w_ee,w_ei,w_ie,w_ii,noise,seed,"input/NN_variables.txt");
	read_integration_variables(atol,rtol,h1,hmin,t1,t2, dt,routine,"input/integration_variables.txt");
	Ran r(seed);

	
	int t_steps;
	if(fmod((t2-t1)/dt,1)==0){
		t_steps = (t2-t1)/dt + 1;
	} else {
		t_steps = (t2-t1)/dt + 2;
	}
	int t_steps2 = pow2(t_steps);				// smalles number larger than 2*t_steps that is a power of 2
	insig.t_end=t_steps2;
	t2 = t_steps2;
	MatDoub FLN(nareas,nareas,0.0);								// matrix containing the FLN values. (a measure of the connection strength)
	VecDoub etah(nareas,1.0);									// a vector containing the values: 1+eta*hi
	VecDoub v_start(2*nareas,0.0);								// starting values of the firing rates, this is updated with an Odeint routine

	vector<VecDoub> frates(2*nareas,VecDoub(t_steps2,0.0));		// matrix with firing rates
	vector<VecDoub> acorr(2*nareas,VecDoub(2*t_steps2,0.0));	// a matrix containing the auto correlatoins of the firing rates of each area
	vector<VecDoub> corr_V1(2*nareas,VecDoub(2*t_steps2,0.0));	// matrix with correlatoins with area V1, length is 2*t_step2 for zero padding in correl(see NR)
	vector<VecDoub> corr_in(2*nareas,VecDoub(2*t_steps2,0.0));	// matrix with correlations with input vector
	vector<VecDoub> coh_in(2*nareas,VecDoub(t_steps2,0.0));		// matrix with coherence w.r.t inputsignal
	vector<VecDoub> coh_V1(2*nareas,VecDoub(t_steps2,0.0));		// matrix with coherence w.r.t. area V1

	VecDoub noise_vec (2*nareas,0.0);							// noise input, this is updates each integration step
	VecDoub input_vec(t_steps2,0.0);							// if input signal is white noise, values are stored in input_noise
	VecDoub corr_coeff_v1(2*nareas,0.0);						// vector with the pearson correlation coefficient of each area with v1



//	for(int i=0;i<etah.size();i++){
//		etah[i] += eta*(1 - (nareas -1 - i)/(nareas-1.));
//	}

	for(int i=0;i<v_start.size();i++) frates[i][0] = v_start[i];	

	// save the t values, and input_signal
	ofstream x_points("xvalues.csv");
	double tt = t1;
	while(tt<=t2){
		x_points << tt << '\n';
		tt += dt;
	}	

	read_FLN(FLN,"FLN.csv");					// read the FLN values from the FLN.csv file

	Output NNout;	// the object handeling the output of the Odeint routines


	// each iteration of the loop the time is advanced from t to t+dt
	// and noise is added after saving the rates in frates
	double t = t1; // current time, each iteration it get incremented by dt
	int ti=1;		// time index location for frates matrix

	while(t<t2) {

		//update noise_vec
		for( int i=0;i<noise_vec.size();i++) noise_vec[i] = bm_transform(r)*noise;

		// update input_vec
		if( t > insig.t_start && t< insig.t_end){
			if(insig.type == 0) {
				noise_vec[insig.area] += bm_transform(r)*insig.A+insig.offset;
				input_vec[ti] = noise_vec[insig.area];
			}
			if(insig.type == 1) {
				input_vec[ti] = insig.offset;
			}
			if(insig.type == 2) {
				input_vec[ti] = insig.offset+insig.A*sin(t*2*acos(-1)/insig.T - insig.phi);
			}
		}


		// brain is the object(def in derivatives) containing the information of the differential equations
		NN brain(FLN,etah, mu_ee,mu_ie,eta,tau_e,tau_i,beta_e,beta_i,w_ee,w_ei,w_ie,w_ii,insig,noise_vec);

		// integrate, different routines possible.
		if(routine==1){
			// a RK routine
			Odeint<StepperDopr5<NN> > ode(v_start,t,t+dt,atol,rtol,h1,hmin,NNout,brain);
			ode.integrate();
		}
		if(routine==2) {
			// a RK routine
			Odeint<StepperDopr853<NN> > ode(v_start,t,t+dt,atol,rtol,h1,hmin,NNout,brain);
			ode.integrate();
		}
		if(routine==3) {
			// a BS ruotine
			Odeint<StepperBS<NN> > ode(v_start,t,t+dt,atol,rtol,h1,hmin,NNout,brain);
			ode.integrate();
		}
		if(routine==4) {
			// a BS routine
			Odeint<StepperStoerm<NN> > ode(v_start,t,dt+t,atol,rtol,h1,hmin,NNout,brain);
			ode.integrate();
		}

		
		for(int i=0;i<v_start.size();i++) frates[i][ti] = v_start[i];		// copy updated v_start in frates

		t += dt;	// increment start time of integration
		ti += 1;	// increment ti
	}

	VecDoub temp_V1e(2*t_steps2,0.0);
	VecDoub temp_V1i(2*t_steps2,0.0);
	VecDoub temp_in(2*t_steps2,0.0);
	for(int i=0;i<t_steps2;i++) {
		temp_V1e[i] = frates[0][i];
		temp_V1i[i] = frates[nareas][i];
		temp_in[i] = input_vec[i];
	}

	for(int a=0;a<2*nareas;a++){
		VecDoub temp(2*t_steps2,0.0);
		for(int i=0;i<t_steps2;i++) temp[i] = frates[a][i];

		correl(temp,temp,acorr[a]);
		correl(temp,temp_in,corr_in[a]);

		if(a<nareas) correl(temp_V1e,temp,corr_V1[a]);
		if(a>=nareas) correl(temp_V1i,temp,corr_V1[a]);

		coh_in[a] = coherence(frates[a],input_vec,frates[a].size(),SN);
		if(a<nareas) coh_V1[a] = coherence(frates[a],frates[0],frates[0].size(),SN);	
		if(a>=nareas) coh_V1[nareas] = coherence(frates[a],frates[nareas],frates[nareas].size(),SN);
	}

	

	pcc_v1(frates,corr_coeff_v1);

	// save frates and auto_corr
	ofstream rates_out("firing_rates.csv");
	rates_out << setprecision(16);
	write_matrix(frates,t_steps2,2*nareas,rates_out);

	ofstream acorr_out("acorr.csv");
	acorr_out << setprecision(16);
	write_matrix(acorr,t_steps2,2*nareas,acorr_out);	
	
	ofstream pcc_out("pcc.csv");
	pcc_out << setprecision(16);
	write_matrix(corr_coeff_v1,corr_coeff_v1.size(),pcc_out);

	ofstream corr_V1_out("corr_V1.csv");
	corr_V1_out << setprecision(16);
	write_matrix(corr_V1,t_steps2,2*nareas,corr_V1_out);

	ofstream corr_in_out("corr_in.csv");
	corr_in_out << setprecision(16);
	write_matrix(corr_in,t_steps2,2*nareas,corr_in_out);

	ofstream coh_V1_out("coh_V1.csv");
	coh_V1_out << setprecision(16);
	write_matrix(coh_V1,t_steps2,2*nareas,coh_V1_out);

	ofstream coh_in_out("coh_in.csv");
	coh_in_out << setprecision(16);
	write_matrix(coh_in,t_steps2,2*nareas,coh_in_out);


	return 0;
}

