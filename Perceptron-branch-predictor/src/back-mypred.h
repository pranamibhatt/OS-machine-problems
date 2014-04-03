/**
	Name: my_predictor.h
	Author: Pranami Bhattacharya
	Desc: this is a simple implementaion of the path based neural branch predictor
	// to be modified for hashed perceptron
	
	Reference: 
	1. http://taco.cse.tamu.edu/public-bp/fpbnp.java
	2. http://www.simplescalar.com/

**/

#define HISTORY_LENGTH	15
#define TABLE_BITS	15
#define NUM_PERCEPTRONS	1024
#define HIST_LEN 	40 // in the paper range given as 12-62
#define PERCEPTRON_WT	100 	
#define WEIGHT_BITS	12		
#define NUM_WTS		12
#define N		1024	// based on design parameter

// perceptron parameters
typedef struct perceptron_params
{
	int num_wt; // number of weight vectors
	int wt_bits; // number of bits per weight
	int outcome; // outcome of the branch predictor
	// latest modified 
	int wt_tab[N][HIST_LEN+1]; // perceptron table
	bool SG[HIST_LEN]; // speculative global history
	bool G[HIST_LEN]; // non speculative global history
	int SR[HIST_LEN+1]; // running total to compute the jth branch, udated specuatively
	int R[HIST_LEN+1]; // holds the most current non speculative version of SR

} perc;

class my_update : public branch_update {
public:
	unsigned int index;
};

class my_predictor : public branch_predictor {
public:

	my_update u;
	branch_info bi;
	
	perc perceptron;

	// gshare parameters
	unsigned int history;
	unsigned char tab[1<<TABLE_BITS];

	my_predictor (void) : history(0) { 
		memset (tab, 0, sizeof (tab));
	}

	branch_update *predict (branch_info & b) {
		bi = b;

		// fill the perceptron entries with 0
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < HIST_LEN+1; j++)
				perceptron.wt_tab[i][j] = 0;
		}
		// initialize registers
		for (int k = 0; k < HIST_LEN+1; k++)
		{
			perceptron.G[k] = false;
			perceptron.SG[k] = false;	
			SR[k] = -1;
			R[k] = -1;
		}
		
		// index to the weight table from the branch addr
		u.index = (history << (TABLE_BITS - HISTORY_LENGTH) ^ b.address) % N; 
		
	
		perceptron.hist_reg[0] = 1; // bias input
		int perc_out = 0; // perceptron output
		int i;
		
		for (i = 0; i < HIST_LEN; i++)
			// output = hist register * perceptron_tab
			perc_out += perceptron.tab[u.index][i] * perceptron.hist_reg[i];

		perceptron.outcome = perc_out;
		if (b.br_flags & BR_CONDITIONAL)
		{
		// prediction direction true/false?
		 u.direction_prediction (perceptron.tab[u.index][i]);
		}
		else
			u.direction_prediction (true);
		u.target_prediction (0);
		return &u;
	
		// gshare algo	
		#if 0
		if (b.br_flags & BR_CONDITIONAL) {
			u.index = 
				  (history << (TABLE_BITS - HISTORY_LENGTH)) 
				^ (b.address & ((1<<TABLE_BITS)-1));
			u.direction_prediction (tab[u.index] >> 1);
		} else {
			u.direction_prediction (true);
		}
		
		u.target_prediction (0);
		return &u;
		#endif
	}
	
	void update (branch_update *u, bool taken, unsigned int target) {
		// update and training algorithm
		#if 1
		if (bi.br_flags & BR_CONDITIONAL)
		{
			float theta;	
			signed int t = -1; // assume not taken
			if (taken)
				t = 1;
			else 
				t = -1;
			
			// compute theta
			theta = (1.93 * HIST_LEN) + 14;
			int outcome = perceptron.outcome;
			// if sign of output doesnt match with t or output < theta
			if ((t>0 & outcome<0) || (t<0 & outcome>=0) || abs(outcome)<=theta)
			{
				for (int i = 0; i < HIST_LEN; i++)
					
			}		
			// if x and t agree, inc weight, else dec weight	
			
		}
		#endif
		#if 0
		if (bi.br_flags & BR_CONDITIONAL) {
			unsigned char *c = &tab[((my_update*)u)->index];
			if (taken) {
				// increment counter or dec counter
				if (*c < 3) (*c)++;
			} else {
				if (*c > 0) (*c)--;
			}
			history <<= 1;
			history |= taken;
			history &= (1<<HISTORY_LENGTH)-1;
		}
		#endif
	}
};
