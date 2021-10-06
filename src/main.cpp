#include "system.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){

	System::init();

	int res = userMain(argc, argv);

	System::restore();

	return res;
}
