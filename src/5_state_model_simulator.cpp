//============================================================================
// Name        : 5_state_model_simulator.cpp
// Author      : Saad Ahmed Bazaz
// Version     : 3.0
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include <pthread>
#include "../include/color_modder.hpp"
#include "../include/Pipe.h"
#include "../include/init.h"
#include "../include/State.h"

#include <queue>
#include <vector>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/wait.h>
#include <sys/stat.h>

using namespace std;


/*
 *
 * An imaginary program control block of this master (or init) program, with arrays of the following names
 *
*/

// ---------PROGRAM CONTROL BLOCK ENDS HERE---------
#define STATES_COUNT 5
#define MAX_PROCESS_COUNT 200

#define PROCESS_NAMES "process_names"
#define PROCESS_IDS "process_identifiers"


State* states = new State [STATES_COUNT] {
	{"New"}, 			//					0
	{"Ready"}, 			//					1
	{"Running"}, 		//					2
	{"Blocked"},		//					3
	{"Exit"}			//					4
};

//static const string process_names [MAX_PROGRAM_COUNT] = {""};
//static pid_t process_ids [MAX_PROCESS_COUNT];


#define PIPES_COUNT 9
Pipe* pipes = new Pipe [PIPES_COUNT] {
	{"Admit"}, 			//New To Ready						0
	{"Dispatch"}, 		//Ready To Running					1
	{"TimeOut"}, 		//Running To Ready					2
	{"EventWait"},		//Running To Blocked				3
	{"EventOccurs"},	//Blocked To Ready					4
	{"Release"},		//Running To Exit					5
	{"sendProc"},		//Signalling b/w Ready & Running 	6
	{"sendRemaining"},	//Sending the remaining burst time	7
	{"outputFromStates"} //Console statements from States  	8
};


//int* timerRef;

// ---------PROGRAM CONTROL BLOCK ENDS HERE---------






// ---------ASSISTIVE TERMINAL FUNCTIONS START HERE---------
int timer_producer_fd;
void* timer_producer_ptr;
bool detachGlobalTimer = false;

// to be handled by thread
void* globalTimer ( void* arg ) {
	/* open the named shared memory object, allow everyone to access */
	timer_producer_fd = shm_open(nameOfTimer, O_CREAT | O_TRUNC | O_RDWR, 0666);

	/* configure the size of the shared memory object */
	ftruncate(timer_producer_fd, sizeof(int));

	/* memory map the shared memory object */
	timer_producer_ptr = mmap(0, sizeof(int), PROT_WRITE, MAP_SHARED, timer_producer_fd, 0);

	/* set the detach bool, so the program can continue initialization */
	detachGlobalTimer = true;

    int COUNTER = 0;
	while (1){
		sleep (S_PILL);
		COUNTER ++;
		sprintf((char*)timer_producer_ptr, "%s", to_string(COUNTER).c_str());
//		cout<<"(in driver) Timer is "<<COUNTER<<endl;
	}
}

//void* timerTester (void* arg){
//
//    /* pointer to shared memory object */
//    void* ptr2;
//
//    /* open the shared memory object */
//    auto shm_fd2 = shm_open(nameOfTimer, O_RDONLY, 0666);
//
//    /* memory map the shared memory object */
//    ptr2 = mmap(0, sizeof(int), PROT_READ, MAP_SHARED, shm_fd2, 0);
//
//    while (1){
//    	sleep(S_PILL);
//        printf("Timer is %s", (char*)ptr2);
//    }
//    /* read from the shared memory object */
//
//    /* remove the shared memory object */
//    shm_unlink(nameOfTimer);
//    return 0;
//}

int max(Pipe * pipes){
	int max = 0;
	for (int i=0; i<PIPES_COUNT; i++){
		if (pipes[i].input_fd>max)
			max = pipes[i].input_fd;
		if (pipes[i].output_fd>max)
			max = pipes[i].output_fd;
	}
	return max;
}
pthread_t globaltimer_id;

void initializeTimer(){
    pthread_create(&globaltimer_id, NULL, globalTimer, NULL);
//    pthread_create(&test_id, NULL, timerTester, NULL);
    while (detachGlobalTimer != true){ }
//    pthread_join(globaltimer_id, &tret1);
    cout<<"Timer has detached."<<endl;
}

void initializePipes(){
    int p[2];
	for (int i=0; i<PIPES_COUNT; i++){
		if (pipe(p) < 0)	    // error checking for pipe
			exit(1);
		pipes[i].output_fd = p[0];
		pipes[i].input_fd = p[1];

		//Create Non-Blocking Pipes
		//toDo: Inspect for errors
		fcntl(pipes[i].output_fd, F_SETFL, O_NONBLOCK);
		fcntl(pipes[i].input_fd, F_SETFL, O_NONBLOCK);

		std::cout<<"Pipe '"<<pipes[i].name<<"' created: fd[0]="<<pipes[i].output_fd<<", fd[1]="<<pipes[i].input_fd<<"\n";
	}
}

void initializeStates(){
	string path = "";
	string arg1 = "";
	pid_t childPid;

	for (int i=0; i<STATES_COUNT; i++){
		if ( (childPid = fork()) ){
			states[i].id = childPid;		//store the p_ids in an array for later use
		}
		else{
			path = DEFAULT_PATH + states[i].name;
			vector <string> arguments;
			arguments.push_back("./" + states[i].name);

			vector <int> pipesToSend;

							// ---------------------------------------- Configuring Arguments for Each State
							//											__________________________________________
							//											DESCRIPTION							INDEX
			if (states[i].name == "New"){					// New
				pipesToSend.push_back(pipes[0].input_fd);			    //input to pipe Admit				1
			}
			else if (states[i].name == "Ready"){			// Ready
				pipesToSend.push_back(pipes[0].output_fd);     			//output from pipe Admit			1
				pipesToSend.push_back(pipes[1].input_fd);     			//input to pipe Dispatch			2
				pipesToSend.push_back(pipes[2].output_fd);     			//output from pipe TimeOut			3
				pipesToSend.push_back(pipes[4].output_fd);     			//output from pipe EventOccurs		4
				pipesToSend.push_back(pipes[6].input_fd);     			//input to pipe sendProc			5
				pipesToSend.push_back(pipes[6].output_fd);     			//output from pipe sendProc			6
				pipesToSend.push_back(pipes[7].input_fd);				//input to pipe sendRemaining		7
				pipesToSend.push_back(pipes[7].output_fd);				//output from pipe sendRemaining	8
			}
			else if (states[i].name == "Running"){			// Running
				pipesToSend.push_back(pipes[1].output_fd);     			//output from pipe Dispatch			1
				pipesToSend.push_back(pipes[2].input_fd);     			//input to pipe TimeOut				2
				pipesToSend.push_back(pipes[3].input_fd);     			//input to pipe EventWait			3
				pipesToSend.push_back(pipes[5].input_fd);     			//input to pipe Release				4
				pipesToSend.push_back(pipes[6].input_fd);     			//input to pipe sendProc			5
				pipesToSend.push_back(pipes[6].output_fd);     			//output from pipe sendProc			6
				pipesToSend.push_back(pipes[7].input_fd);				//input to pipe sendRemaining		7
				pipesToSend.push_back(pipes[7].output_fd);				//output from pipe sendRemaining	8
			}
			else if (states[i].name == "Blocked"){			// Blocked
				pipesToSend.push_back(pipes[3].output_fd);     			//output from pipe EventWait		1
				pipesToSend.push_back(pipes[4].input_fd);     			//input to pipe EventOccurs			2
			}
			else if (states[i].name == "Exit"){				// Exit
				pipesToSend.push_back(pipes[5].output_fd);     			//output from pipe Release			1
			}

			//															___________________________________________



			// Replace stdout file descriptor of State with a LogFile file descriptor
			string LogFileName = states[i].name + "Log.txt";
		    if (truncate(LogFileName.c_str(), 0) == -1){		//empty existing logfile
		      cout<<"Could not truncate. Creating new file."<<endl;
		    }
			int LogFileDescriptor = open (LogFileName.c_str(), O_CREAT | O_WRONLY, 0666);
		    if (LogFileDescriptor<0){
		    		cout<< "Error opening the file '"<<LogFileName<<"'"<<endl;
		            throw logic_error ("Yikes");
		    }
		    dup2(LogFileDescriptor, 1);


			auto MAX_OF_PIPES = max(pipes);

			// close all unrelated pipes
			closeAllPipesExcept(pipesToSend, MAX_OF_PIPES);


			// Turn all pipesToSend into arguments
			for (int i=0; (unsigned)i<pipesToSend.size(); i++){
				arguments.push_back(to_string(pipesToSend[i]));
			}


			arguments.push_back(to_string(MAX_OF_PIPES));				//Max of pipes (As the pipes were created sequentially, we will use this to close all other pipes)
			char ** argv  = new char * [arguments.size() + 1];
			for (int i=0; (unsigned)i<arguments.size(); i++){						//Thank you, C++, for forcing me to rethink my life choices
				char *cstr = new char[arguments[i].length() + 1];
				strcpy(cstr, arguments[i].c_str());
				argv[i] = cstr;
			}
			argv[arguments.size()] = NULL;

			execv(path.c_str(), argv);
			cout<<"Exec failed "<<path<<" "<<arg1<<endl;
			exit(-1);
		}
	}

}

void inputLoop(){
	char * input;
	input = new char [50];
	do {
	    Color::Modifier red(Color::FG_RED);
	    Color::Modifier def(Color::FG_DEFAULT);
	    Color::Modifier green(Color::FG_GREEN);
	    cout << red << "you@YOUR-COMPUTER"<< def<<":"  << green<<"path/to/mard" << def <<"$ ";
		cin.getline(input, 50);
		vector <char *> arguments;
		arguments.push_back( strtok (input," ") );
		int argument_count = 0;
		while (arguments[argument_count] != NULL)
		{
			argument_count++;
			arguments[argument_count] = strtok (NULL, " ");
		}
		if (fork()==0){
			//is a command
			if (strchr(arguments[0],'/') == nullptr){
				string path = DEFAULT_PATH + arguments[0];
				char ** argv  = &arguments[0];
				execv(path.c_str(), argv);
				cout<<"Command '"<<arguments[0]<<"' not found"<<endl;
			}
			//is a file or directory
			else{
				string path = arguments[0];
				char ** argv  = &arguments[0];
				execv(path.c_str(), argv);
			}
			//everything failed
			cout<<"mard: "<<arguments[0]<<": "<<"No such file or directory\n";
			exit(1);

		}
		else{
			wait(NULL);
			sleep (2);
		}
	} while(1);
}


// ---------ASSISTIVE TERMINAL FUNCTIONS END HERE---------



// Driver function
int main() {
	initializeTimer();
	initializePipes();
	initializeStates();

	sleep (S_PILL);

	for (int i=0; i<STATES_COUNT; i++){
		cout << states[i].name<<" PID = "<<states[i].id <<endl;
	}
	cout<<"This program's ID "<<getpid()<<endl;

	inputLoop();

	return 0;
}
