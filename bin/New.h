/*
 * New.h
 *
 *  Created on: Mar 22, 2020
 *      Author: saad
 */
#ifndef NEW_H_
#define NEW_H_
#include "../include/Pipe.h"
#include "../include/packman.h"
#include "../include/init.h"
#include "../include/State.h"
#include <queue>

#include <iostream>
using namespace std;


static string fileNames [] = {
	{"Sample_1_RR.txt"},
	{"Sample_2_FCFS.txt"},
	{"Sample_3_SRTF.txt"},
	{"Sample_4_SJF.txt"}
};
// ---------------------------------------- Configuring Arguments for Each State
//__________________________________________
// DESCRIPTION						INDEX
// New
     //input to pipe Admit				1
// Ready
     //output from pipe Admit			1
     //input to pipe Dispatch			2
     //output from pipe TimeOut			3
     //output from pipe EventOccurs		4
     //input to pipe sendProc			5
     //output from pipe sendProc		6
     //input to pipe sendRemaining		7
     //output from pipe sendRemaining	8
// Running
     //output from pipe Dispatch		1
     //input to pipe TimeOut			2
     //input to pipe EventWait			3
     //input to pipe Release			4
     //input to pipe sendProc			5
     //output from pipe sendProc		6
     //input to pipe sendRemaining		7
     //output from pipe sendRemaining	8
// Blocked
     //output from pipe EventWait		1
     //input to pipe EventOccurs		2
// Exit
     //output from pipe Release			1
//___________________________________________


int MAX_PIPE = -1;

static int TOTAL_NUMBER_OF_PIPES = 1;
Pipe* pipes = new Pipe [1] {
	{"Admit"} 			//New To Ready				0
};


string STATE_NAME = "";



#endif /* NEW_H_ */
