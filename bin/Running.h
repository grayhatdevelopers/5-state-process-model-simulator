/*
 * Running.h
 *
 *  Created on: Mar 22, 2020
 *      Author: saad
 */

#ifndef RUNNING_H_
#define RUNNING_H_

#include "../include/State.h"
#include "../include/Pipe.h"
#include "../include/packman.h"
#include "../include/init.h"

#include <queue>
#include <iostream>
#include <time.h>
using namespace std;

// ---------------------------------------- Configuring Arguments for Each State
//__________________________________________
// DESCRIPTION						INDEX
// New
     //input to pipe Admit			1
// Ready
     //output from pipe Admit			1
     //input to pipe Dispatch			2
     //output from pipe TimeOut		3
     //output from pipe EventOccurs	4
     //input to pipe sendProc			5
     //output from pipe sendProc		6
     //input to pipe sendRemaining		7
     //output from pipe sendRemaining	8
// Running
     //output from pipe Dispatch		1
     //input to pipe TimeOut			2
     //input to pipe EventWait		3
     //input to pipe Release			4
     //input to pipe sendProc			5
     //output from pipe sendProc		6
     //input to pipe sendRemaining		7
     //output from pipe sendRemaining	8
// Blocked
     //output from pipe EventWait		1
     //input to pipe EventOccurs		2
// Exit
     //output from pipe Release		1
//___________________________________________

#define MAX_PROCESSES 1


int MAX_PIPE = -1;

static int TOTAL_NUMBER_OF_PIPES = 6;
Pipe* pipes = new Pipe [6] {
	{"Dispatch"}, 		//Ready To Running			     1
	{"TimeOut"}, 		//Running To Ready			     2
	{"EventWait"},		//Running To Blocked		     3
	{"Release"},		//Running To Exit			     4
     {"sendProc"},		//Signalling b/w Ready & Running   5
	{"sendRemaining"}	//Sending the remaining burst time	6
};



int CPU_TIME = 0;
int PROGRAM_TIME = 0;

string STATE_NAME = "";

//real work starts here
Proc * procedures;
int NUMBER_OF_PROCS;

queue <Proc> RunningQueue;


int INTERRUPT = -1;


#endif /* RUNNING_H_ */
