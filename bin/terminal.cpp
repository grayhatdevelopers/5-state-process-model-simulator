
#include <iostream>
#include <string.h>
#include <queue>
#include <unistd.h>
#include <signal.h>
#include <vector>
using namespace std;

int main(){
	char * input;
	input = new char [20];
	do {
		cin.getline(input, 20);
		vector <char *> arguments;
		arguments.push_back( strtok (input," ") );
		int argument_count = 0;
		  while (arguments[argument_count] != NULL)
		  {
		    arguments[argument_count] = strtok (NULL, " ,.-");
		    argument_count++;
		  }
		if (fork()){

		}
		else{
			path = arguments[0];
			char ** argv  = &arguments[1];
//			arg1 = "./" + [0];
			execv(path.c_str(), argv);
			cout<<"Exec failed "<<path<<" "<<arg1<<endl;
			exit(1);
		}
	} while(1);
    return 0;
}
