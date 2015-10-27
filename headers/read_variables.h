#ifndef GUARD_read_variables_h
#define GUARD_read_variables_h


//#include "ran.h"
#include <string>
#include <fstream>

std::vector<int> find_char(std::string str, char ch) {
	std::vector<int> vec;
	for(int i=0;i<str.size();i++) {
		if(str[i] == ch) {
			vec.push_back(i);
		}
	}
	return vec;
}



void read_FLN(MatDoub &mat, std::string fname){
	ifstream infile;
	infile.open(fname);
	
	std::string line;
	std::vector<int> del_pos;
	for(int i=0;i<mat.nrows();i++) {
		getline(infile,line,'\n');
		line = ';'+line;
		del_pos = find_char(line,';');
		for(int j=0;j<mat.ncols()-1;j++) {
			mat[i][j] = stod(line.substr(del_pos[j]+1,del_pos[j+1]-del_pos[j]-1));
		}
	}
}

const Int seed = 1234567;
const Doub empt = 0.3;
Ran r(seed);
void read_FLN(MatDoub &mat,int a) {
	for(int i=0;i<mat.nrows();i++) {
		for(int j=0;j<mat.ncols();j++) {
			Doub rn = r.doub();
			if(rn>empt) {
				mat[i][j] = 0.01*r.doub();
			}else {
				mat[i][j] =0.0;
			}
		}
	}
}
			
void read_NN_variables(Doub &mu_ee, Doub &mu_ie, Doub &eta,
		Doub &tau_e, Doub &tau_i, Doub &beta_e, Doub &beta_i,
		Doub &w_ee, Doub &w_ei, Doub &w_ie, Doub &w_ii,
		Doub& noise, Int &seed, std::string input_file_name) {

	std::string temp;
	std::ifstream infile(input_file_name);
	
	infile >> temp;infile >> temp; infile >> temp;

	infile >> mu_ee;
	infile >> temp;	infile >> temp;

	infile >> mu_ie;
	infile >> temp;	infile >> temp;

	infile >> eta;
	infile >> temp;	infile >> temp;

	infile >> tau_e;
	infile >> temp;	infile >> temp;

	infile >> tau_i;
	infile >> temp; infile >> temp;

	infile >> beta_e;
	infile >> temp; infile >> temp;

	infile >> beta_i;
	infile >> temp; infile >> temp;

	infile >> w_ee;
	infile >> temp; infile >> temp;

	infile >> w_ei;
	infile >> temp; infile >> temp;

	infile >> w_ie;
	infile >> temp; infile >> temp;

	infile >> w_ii;
	infile >> temp; infile >> temp;

	infile >> noise;
	infile >> temp; infile >> temp;

	infile >> seed;


}

void read_integration_variables(Doub &atol, Doub &rtol, Doub &h1, Doub &hmin,
		Doub &t1, Doub &t2, double &dt, Int &routine, std::string input_file_name) {

	std::string temp;
	std::ifstream infile(input_file_name);

	infile >>temp;

	infile >> atol;
	infile >> temp; infile >> temp;

	infile >> rtol;
	infile >> temp; infile >> temp;

	infile >> h1;
	infile >> temp; infile >> temp;

	infile >> hmin;
	infile >> temp; infile >> temp;

	infile >> t1;
	infile >> temp; infile >> temp;

	infile >> t2;
	infile >> temp; infile >> temp;

	infile >> dt;
	infile >> temp; infile >> temp;

	infile >> routine;
	
}

IS read_input_signal(std::string input_file_name) {

	int area;
	int type;
	double t_start, t_end, period, phase, amplitude, offset;

	std::string temp;
	std::ifstream infile(input_file_name);

	infile >> temp;	

	infile >> area;
	infile >> temp;

	infile >> type;
	infile >> temp;

	infile >> t_start;
	infile >> temp;

	infile >> t_end;
	infile >> temp;

	infile >> period;
	infile >> temp;

	infile >> phase;
	infile >> temp;

	infile >> amplitude;
	infile >> temp;

	infile >> offset;

	IS out(area,type,t_start,t_end,period,phase,amplitude,offset);

	return out;
}

#endif


