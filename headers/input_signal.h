#ifndef GUARD_input_signal_h
#define GUARD_input_signal_h



struct IS {

	int area;			// input to area
	int type;			// 0=white noise, 1=block, 2=sine
	int t_start, t_end;	// start and end time of the signal
	double T;			// period, only used if sinefunc == true
	double phi;			// phase, only used if sinefunc == true
	double A;			// amplitude (both of sine and block function),std for white noise
	double offset;		// hight og block, ofset for sine, and mean for white noise
	// default constructor
	IS ();
	// constructor
	IS ( int areaa, int typee, int t_startt, int t_endd, double TT, double phii, double AA, double offsett) :
		area(areaa), type(typee), t_start(t_startt), t_end(t_endd), T(TT), phi(phii), A(AA), offset(offsett) {}
	
};



#endif

