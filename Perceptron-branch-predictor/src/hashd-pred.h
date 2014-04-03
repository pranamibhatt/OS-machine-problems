/**
	Name: my_predictor.h
	Author: Pranami Bhattacharya
	Desc: this is a simple implementaion of the path based neural branch predictor
	// to be modified for hashed perceptron
	
	Reference: 
	1. http://taco.cse.tamu.edu/public-bp/fpbnp.java
	2. http://www.simplescalar.com/

**/

/* basic organization of the perceptron predictor
   branch addr-> table of perceptron-> selected perceptron
   |
   hist register-> computed o/p
   |	|
   ------prediction
 */
#include <math.h>
#include <stdio.h>

#define HISTORY_LENGTH	15
#define TABLE_BITS	15
// new params
#define NUM_PERCEPTRONS	1024
#define HIST_LEN 	40 // in the paper range given as 12-62
#define NUM_WTS		15 // number of weights per table
#define WT_BITS		15 // number of bits per weight

// perceptron parameters
typedef struct perceptron_params
{
	int histlength; // history length to be maintained
	int outcome; // store the outcome of each computation
	int wt_tab[NUM_WTS][HIST_LEN]; // table of weights
	unsigned int hist_reg[HIST_LEN]; // global branch history register

} perc;

perc perceptron;

class my_predictor : public branch_predictor {
public:
	// perceptron parameters
	int outcome; // outcome of the perceptron	
	class my_update : public branch_update {
	public:
		unsigned int index[HIST_LEN]; // index to the hist reg and weight tab
	};
	my_update u; 
	/* u.direction_prediction(), u.target_prediction() */
	branch_info bi; // bi.BA, bi.opcode, bi.flags

	// gshare parameters
	unsigned int history;
	unsigned char tab[1<<TABLE_BITS];
	int *perc_out;

	my_predictor (void) : history(0) { 
		memset (tab, 0, sizeof (tab));
	}

	// predict the branch
	branch_update *predict (branch_info & b) {
		bi = b;
		
		// branch flags set and its a conditional branch
		if (b.br_flags & BR_CONDITIONAL) 
		{
			// hashed perceptron algo
			// fill the perceptron entries with 0
			for (int i = 0; i < NUM_WTS; i++)
			{
				for (int j = 0; j < HIST_LEN; j++)
					perceptron.wt_tab[i][j] = 0;
			}
			// initialize hist reg to NT initially 
			for (int k = 0; k < HIST_LEN; k++)
				perceptron.hist_reg[k] = -1;
			// the first index
			u.index[0] = b.address % NUM_WTS; // branch addr % number of weights per table

			perc_out = &perceptron.wt_tab[0][u.index[0]];
			//	perceptron.hist_reg[0] = 1; // bias input? to be confirmed
			int i;

			for (i = 1; i < HIST_LEN; i++)	
			{
				// output = hist register * pc_addr % weights
				u.index[i] = (perceptron.hist_reg[i-1] ^ (b.address)) % NUM_WTS;
				perc_out += perceptron.wt_tab[i][u.index[i]];

			}
			//	perceptron.outcome = &perc_out;
			
			// prediction direction 
			if (perc_out >= 0)
				u.direction_prediction (false);
	//		u.direction_prediction (perceptron.wt_tab[i][u.index[i]]);
			else	
				u.direction_prediction (true);
			
		}
		else
			u.direction_prediction (true); // not a conditional branch or branch flags not set, check!
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
			int out[200];
			// compute theta
			theta = (1.93 * HIST_LEN) + (HIST_LEN/2);
			int outcome = perceptron.outcome;
			if (outcome < 0)
				outcome = (-1) * outcome;
			signed int t = -1; // by defualt assume not taken
			int max_wt, min_wt;
			if (taken)
				t = 1;
			else
				t = -1;
			int i;
			// if sign of output doesnt match with t or output < theta
			if ((t>0 & outcome<0) || (t<0 & outcome>=0) || outcome<=theta)
			{
			
				for (i = 0; i < HIST_LEN; i++)
				if (perceptron.hist_reg[i] == 0)
					out[i] = -1;
				else
					out[i] = 1;
				// if outcome is taken
				//if (t == out[i])

				// saturating arithmetic
				// compute max weight
				max_wt = pow (2, (WT_BITS-1)) -1 ;
				// compute min weight
				min_wt = -(max_wt +1);
				int *c = &perceptron.wt_tab[i][((my_update *)u)->index[i]];
				if (taken)
				{
					if (*c < max_wt)
						*c++;
					else
						*c = max_wt;
				}
				else
				{
					if (*c > min_wt)
						*c--;
					else		
						*c = min_wt;
				}
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
	}	
};
