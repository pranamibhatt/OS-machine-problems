#include <iostream>
using namespace std;
#include "disp.h"


class one
{
	int onevar;
	public :
		void disp ()
		{
			cout << "One disp" << endl;
		}
};
 
class two : public one
{
	public :
		void show()
		{
			cout << "two show" << endl;
		}
};

int main ()
{
	two t;
	t.disp();
	t.show();
	hello();
	return 0;
}
