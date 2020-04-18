/*
 * New.cpp
 *
 *  Created on: Mar 22, 2020
 *      Author: saad
 */

#include "New.h"

int main(int argc, char ** argv){

	cout<<"I am "<<setStateName(argv[0], STATE_NAME)<<"!"<<endl;

	for (int i=0; i<STATE_NAME.length(); i++){
		STATE_NAME[i] = toupper (STATE_NAME[i]);
	}
	pipes[0].input_fd = atoi(argv[1]);


	MAX_PIPE = atoi(argv[argc-1]);	


	//real work starts here


	// ------------------ TO OPEN A DIFFERENT FILE, JUST CHANGE THIS STRING -------------------
	string fileToOpen = fileNames[2];
	// ---------------- ^^^^^^^^^^^^ ------------------


	cout<<"Opening "<<fileToOpen<<endl;

	auto fd = open(fileToOpen.c_str(), O_RDONLY);
    if(fd < 0){
		cout <<STATE_NAME<<": File could not be opened"<<endl;
        return 1;
	}


	Proc * procedures;
	int NUMBER_OF_PROCS;
	unpackFile(fd, procedures, NUMBER_OF_PROCS);


	queue <Proc> NewQueue;

	for (int i=0; i<NUMBER_OF_PROCS; i++){
		procedures[i].print();
		NewQueue.push(procedures[i]);
	}


	string data;
	int data_bytes;

	data = "";
	data_bytes = 0;


	for (int i=0; i<NUMBER_OF_PROCS; i++){

		sleep (S_PILL);

		if(packPipe(pipes[0].input_fd, NewQueue.front(), 1, data, data_bytes) != 0)
		{
			// cout <<"NEW: There was an error writing to pipe\n"<<endl;
			return 1;
		}
		else{
			cout<<STATE_NAME<<" "<<pipes[0].name<<" <=== ";
			NewQueue.front().print();
			NewQueue.pop();
		}

	}

	cout<<"Exiting New.........."<<endl;

}
