#ifndef PIPE_H_
#define PIPE_H_

#include <unistd.h>
#include <fcntl.h>

#include <vector>
#include <iostream>
#include <string>
using namespace std;
struct Pipe {
	string name;
	int input_fd;
	int output_fd;

	// Pipe() : input_fd (-1), output_fd (-1){}

	Pipe (string Name): name(Name), input_fd (-1), output_fd (-1) {}

	void setInputFileDescriptor(int ifd){
		input_fd = ifd;
	}
	void setOutputFileDescriptor(int ofd){
		output_fd = ofd;
	}
};

int closeAllPipesExcept(vector<int> pipes, int __MAX_PIPE){
	bool foundIt = false;

	for (int i=3; i<=__MAX_PIPE; i++){
		for (int j=0; (unsigned)j<pipes.size(); j++){
			if (i == pipes[j]){
				foundIt = true;
				break;
			}
		}
		if (!foundIt)
			close(i);
		foundIt = false;
	}
	return 1;
}

int closeAllPipesExcept(Pipe * & pipes, int __MAX_PIPE, int __TOTAL_NUMBER_OF_PIPES){
	bool foundIt = false;

	for (int i=3; i<=__MAX_PIPE; i++){		
		for (int j=0; j<__TOTAL_NUMBER_OF_PIPES; j++){
			// cout<<"pipe name "<<pipes[j].name<<endl;
			if (i == pipes[j].input_fd or i == pipes[j].output_fd){
				// cout<<"Found i..."<< i<<endl;
				foundIt = true;
				break;
			}
		}
		if (!foundIt)
			close(i);
		foundIt = false;
	}
	return 1;
}



#endif /* PIPE_H_ */
