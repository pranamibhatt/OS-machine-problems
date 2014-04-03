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

// 2048 37 8
#include <math.h>
#include <stdio.h>

#define HIST_LEN 	40 // history length 37
#define NUM_WTS		2048 // number of weights per table
#define WT_BITS		8 // number of bits per weight

class my_predictor : public branch_predictor {
public:
	// perceptron parameters
	int outcome; // outcome of the 	
	int wt_tab[NUM_WTS][HIST_LEN]; //table of weights
	unsigned int hist_reg[HIST_LEN]; // global branch hist
	bool prediction; // prediction of the branch
	bool t, nt;

	my_predictor()
	{
	//	wt_tab = new int [NUM_WTS][HIST_LEN];
		t = true;
		nt = false;
	// fill the perceptron entries with 0
	for (int i = 0; i < NUM_WTS; i++)
	{
		for (int j = 0; j < HIST_LEN; j++)
			wt_tab[i][j] = 0;
	}
	// initialize hist reg to NT initially 
	for (int k = 0; k < HIST_LEN; k++)
		hist_reg[k] = -1;
	}

	class my_update : public branch_update {
		public:
			unsigned int index[HIST_LEN]; // index to the hist reg and weight tab

			my_update() 
			{
				//index = new int[HIST_LEN];
			}
	};

	my_update u;
	/* u.direction_prediction(), u.target_prediction() */
	branch_info bi; // bi.BA, bi.opcode, bi.flags

	// initialize parameters
	// gshare parameters
	unsigned int history;
	//unsigned char tab[1<<TABLE_BITS];
	int perc_out;

	// predict the branch at the branch address
	branch_update *predict (branch_info & b) {
		bi = b;

		// branch flags set and its a conditional branch
		if (b.br_flags & BR_CONDITIONAL) 
		{
			// hashed perceptron algo

			// the first index
			u.index[0] = b.address % NUM_WTS; // branch addr % number of weights per table

			perc_out = wt_tab[0][u.index[0]];
			printf ("%d \n", perc_out);
			//	perceptron.hist_reg[0] = 1; // bias input? to be confirmed
			int i;

			for (i = 1; i < HIST_LEN; i++)	
			{
				// output = hist register XOR pc_addr % weights
			//	u.index[i] = (b.address) % NUM_WTS;
				u.index[i] = (hist_reg[i-1] ^ (b.address)) % NUM_WTS;
				perc_out += wt_tab[i][u.index[i]];

			}
			
			// prediction direction 
			if (perc_out >= 0)
			{
				prediction = t;
				u.direction_prediction (prediction);
			}
			//		u.direction_prediction (perceptron.wt_tab[i][u.index[i]]);
			else
			{	
				prediction = nt;
				u.direction_prediction (prediction);
			}

		}
		else
			u.direction_prediction (true); // not a conditional branch or branch flags not set, check!
		u.target_prediction (0); // not predicting the target address
		return &u;

	}
	// update the prediction based on the output returned by u and the actual outcome
	void update (branch_update *u, bool taken, unsigned int target) {
#if 1
		if (bi.br_flags & BR_CONDITIONAL)
		{
			float theta;	
			// compute theta
			theta = (1.93 * HIST_LEN) + (HIST_LEN/2);
			int max_wt, min_wt;
			int i;
			// direction mismatch or out <= theta
			if (u->direction_prediction() != taken || abs(perc_out) <= theta)
			{

				for (i = 0; i < HIST_LEN; i++)
				{

				// saturating arithmetic
				// compute max weight
				max_wt = pow (2, (WT_BITS-1)) -1 ;
				// compute min weight
				min_wt = -(max_wt +1);
				int c = wt_tab[i][((my_update *)u)->index[i]];
				if (taken)
				{
					if (c < max_wt)
						c++;
					else
						c = max_wt;
				}
				else
				{
					if (c > min_wt)
						c--;
					else		
						c = min_wt;
				}
				}
			}
			// update the hist reg
			for (int j = 0; j < HIST_LEN; j++ )
				hist_reg[i-1] = hist_reg[i];
			hist_reg[HIST_LEN-1] = taken;
			
			
#endif
		}
	}	
};
