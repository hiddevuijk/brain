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
#include "headers/vec_manip.h"
#include "headers/corr2.h"

using namespace::std;

int main()
{

	// number of brain areas
	Int nareas =29;

	// paremeters of the network
	Doub mu_ee, mu_ie, eta,
		 tau_e, tau_i, beta_e, beta_i,
		 w_ee, w_ei, w_ie, w_ii,
		 noise;

	// integration parameters
	Doub atol, rtol, h1, hmin, t1, t2;
	Int npoints;		// number of time points
	Int routine;		// Which integratoin routine is used: 
	double dt;			// integration time step in ms
	Int seed;			// seed for random generator

	// initialize NR3 random generator
	Ran r(seed);

	// number of points in Welch segments for coherence
	// only multiples of SN points are used
	int SN = 256;		

	// read info of inputsignal in insig
	IS insig  = read_input_signal("input/input_signal.txt"); 

	// read network variables
	read_NN_variables(mu_ee,mu_ie,eta,tau_e,tau_i,beta_e,beta_i,w_ee,w_ei,w_ie,w_ii,noise,seed,"input/NN_variables.txt");

	// read integration variables
	read_integration_variables(atol,rtol,h1,hmin,t1,t2, dt,routine,"input/integration_variables.txt");

	// determine number of time steps
	int t_steps;
	if(fmod((t2-t1)/dt,1)==0){
		t_steps = (t2-t1)/dt + 1;
	} else {
		t_steps = (t2-t1)/dt + 2;
	}

	// smalles number larger than 2*t_steps that is a power of 2
	int t_steps2 = pow2(t_steps);
	insig.t_end=t_steps2;
	t2 = t_steps2;


	// FLN: matrix containing the FLN values. (a measure of the connection strength)
	// etah:  a vector containing the values: 1+eta*hi
	// v_start: starting values of the firing rates, this is updated with an Odeint routine
	MatDoub FLN(nareas,nareas,0.0);
	VecDoub etah(nareas,1.0);
	VecDoub v_start(2*nareas,0.0);

	// frates: matrix with firing rates
	vector<VecDoub> frates(2*nareas,VecDoub(t_steps2,0.0));

	// acorr: a matrix containing the auto correlatoins of the firing rates of each area
	//corr_V1:  matrix with correlatoins with area V1, length is 2*t_step2 for zero padding in correl(see NR)
	//corr_in:  matrix with correlations with input vector
	// corr_coeff_v1: vector with the pearson correlation coefficient of each area with v1
	vector<VecDoub> acorr(2*nareas,VecDoub(2*t_steps2,0.0));
	vector<VecDoub> corr_V1(2*nareas,VecDoub(2*t_steps2,0.0));
	vector<VecDoub> corr_in(2*nareas,VecDoub(2*t_steps2,0.0));
	VecDoub corr_coeff_v1(2*nareas,0.0);

	// empty_VecDoub: An empty NR3 vector used for initialization of some matrices
	// coh_in: matrix with coherence w.r.t inputsignal, initialized with an empty NR3 vector
	// coh_V1: matrix with coherence w.r.t. area V1, initialized with an empty NR3 vector
	VecDoub empty_VecDoub;
	vector<VecDoub> coh_in(2*nareas,empty_VecDoub);
	vector<VecDoub> coh_V1(2*nareas,empty_VecDoub);

	// noise_vec: noise input, this is updates each integration step
	// input_vec: if input signal is white noise, values are stored in input_noise
	VecDoub noise_vec (2*nareas,0.0);
	VecDoub input_vec(t_steps2,0.0);


	// input hierarchy (linear estimation)
	for(int i=0;i<etah.size();i++){
		etah[i] += eta*(1 - (nareas -1 - i)/(nareas-1.));
	}

	// copy the starting values in frates[i][0]
	for(int i=0;i<v_start.size();i++) frates[i][0] = v_start[i];	
	// save the t values (saved in xvalues.csv)
	ofstream x_points("xvalues.csv");
	double tt = t1;
	while(tt<=t2){
		x_points << tt << '\n';
		tt += dt;
	}

	// read the FLN values from the FLN.csv file
	read_FLN(FLN,"FLN.csv");

	// the object handeling the output of the Odeint routines
	Output NNout;

	// each iteration of the loop the time is advanced from t to t+dt
	// and noise is added after saving the rates in frates
	double t = t1;		// current time, each iteration it get incremented by dt
	int ti=1;			// time index location for frates matrix

	while(t<(t2-1)) {

		//update noise_vec
		for( int i=0;i<noise_vec.size();i++) noise_vec[i] = bm_transform(r)*noise;

		// update input_vec
		if( t > insig.t_start && t< insig.t_end){
			if(insig.type == 0) {
				noise_vec[insig.area] += bm_transform(r)*insig.A+insig.offset;
				input_vec[ti] = noise_vec[insig.area];
			} if(insig.type == 1) {
				input_vec[ti] = insig.offset;
			} if(insig.type == 2) {
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

		// copy updated v_start in frates
		for(int i=0;i<v_start.size();i++) frates[i][ti] = v_start[i];
		t += dt;	// increment start time of integration
		ti += 1;	// increment ti
	}


	// copy (frates[V1e]-mean)/std in a vector with zero padding (these are used in correl)
	// same for frates[V1i] and input_vec
	VecDoub temp_V1e(2*t_steps2,0.0);
	VecDoub temp_V1i(2*t_steps2,0.0);
	VecDoub temp_in(2*t_steps2,0.0);
	double mV1e = mean(frates[0],frates[0].size());
	double mV1i = mean(frates[nareas],frates[nareas].size());
	double min = mean(input_vec,input_vec.size());
	double sV1e = stdev(frates[0],frates[0].size());
	double sV1i = stdev(frates[nareas],frates[nareas].size());
	double sin = stdev(input_vec,input_vec.size());

	for(int i=0;i<t_steps2;i++) {
		temp_V1e[i] = (frates[0][i]-mV1e)/sV1e;
		temp_V1i[i] = (frates[nareas][i]-mV1i)/sV1i;
		temp_in[i] = (input_vec[i]-min)/sin;
	}

	// calculate the coherence and correlations
	for(int a=0;a<2*nareas;a++){
		// copy (frates[a]-mean)/std in a vector temp, with zero padding
		VecDoub temp(2*t_steps2,0.0);
		double ma = mean(frates[a],frates[a].size());
		double sa = stdev(frates[a],frates[a].size());
		for(int i=0;i<t_steps2;i++) temp[i] = (frates[a][i]-ma)/sa;

		// calculate auto-correltaion
		correl(temp,temp,acorr[a]);
		// devide by  N=0.5*size for normalization
		devide_by(acorr[a],acorr[a].size(),0.5*acorr[a].size());

		// claculate correlation between area a and the input signal
		correl(temp,temp_in,corr_in[a]);
		// devide by N=0.5*size for normalization
		devide_by(corr_in[a],corr_in[a].size(),0.5*corr_in[a].size());
			
		// calculate correlation between V1 and area a (excitatory area with excitatory V1)
		if(a<nareas) correl(temp_V1e,temp,corr_V1[a]);
		if(a>=nareas) correl(temp_V1i,temp,corr_V1[a]);

		// devide the correlation by N=0.5*size for normalization
		devide_by(corr_V1[a],corr_V1[a].size(),0.5*corr_V1[a].size());

		// calculate coherence between the input signal and area a
		coh_in[a] = coherence(frates[a],input_vec,frates[a].size(),SN);

		// calculate coherence of area a with area V1(excitatory a with excitatory V1, same for inhibitory)
		if(a<nareas) coh_V1[a] = coherence(frates[a],frates[0],frates[0].size(),SN);	
		if(a>=nareas) coh_V1[a] = coherence(frates[a],frates[nareas],frates[nareas].size(),SN);
	}

	// calculate Pearson Correlation Coefficients
	pcc_v1(frates,corr_coeff_v1);

	// caculate frequencies
	// dt is in ms so a factor 1000 to get Hz
	VecDoub freq(SN/2,0.0);
	for(int i=0;i<SN/2;i++) freq[i] = 1000*i/(SN*dt); 


	// save results
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
	write_matrix(corr_V1,2*t_steps2,2*nareas,corr_V1_out);

	ofstream corr_in_out("corr_in.csv");
	corr_in_out << setprecision(16);
	write_matrix(corr_in,2*t_steps2,2*nareas,corr_in_out);

	ofstream coh_V1_out("coh_V1.csv");
	coh_V1_out << setprecision(16);
	write_matrix(coh_V1,coh_V1[0].size(),2*nareas,coh_V1_out);
	
	ofstream coh_in_out("coh_in.csv");
	coh_in_out << setprecision(16);
	write_matrix(coh_in,coh_in[0].size(),2*nareas,coh_in_out);

	ofstream f_out("f.csv");
	f_out << setprecision(16);
	write_matrix(freq,freq.size(),f_out);

	return 0;
}


