/*
 * Exit.h
 *
 *  Created on: Mar 22, 2020
 *      Author: saad
 */

#ifndef EXIT_H_
#define EXIT_H_
#include "../include/Pipe.h"
#include "../include/Proc.h"
#include "../include/packman.h"
#include "../include/init.h"
#include "../include/State.h"

#include <queue>
#include <iostream>
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

int RELEASED_PROCESSES = 0;

static int MAX_PIPE = -1;

static int TOTAL_NUMBER_OF_PIPES = 1;
Pipe* pipes = new Pipe [1] {
	{"Release"}			//Running To Exit			0
};

string STATE_NAME = "";


void printReport (Proc* procedures, int NUMBER_OF_PROCS) {
     
     double ThroughPut = 0;
     double avgBlockedTime = 0;
     double avgWaitingTime = 0;
     // double avgTurnAroundTime = 0;

     string data = "";

     data = "--------------------INDIVIDUAL REPORTS--------------------\n\n"; 

     for (int i=0; i<NUMBER_OF_PROCS; i++){
          data+="_____________________________________\n";
          data+=procedures[i].getFullData("\n");
          ThroughPut += procedures[i].completionTime;
          avgBlockedTime += (procedures[i].completionTime - procedures[i].waitingTime - procedures[i].burstTime_orig - 1);
          avgWaitingTime += procedures[i].waitingTime;
     }

     data += '\n';
     data += '\n';
     data += "--------------------AGGREGATE REPORT--------------------\n\n"; 

     ThroughPut /= NUMBER_OF_PROCS;
     avgBlockedTime /= NUMBER_OF_PROCS;
     avgWaitingTime /= NUMBER_OF_PROCS;

     data += "Throughput: " + to_string(ThroughPut);
     data += '\n';
     data += "Total CPU Time: " + to_string(getCPUTime());
     data += '\n';
     data += "Average time in Blocked state: " + to_string(avgBlockedTime);
     data += '\n';
     data += "Average time in Ready state: " + to_string(avgWaitingTime);
     data += '\n';


     auto fd = open ("Report.txt", O_CREAT | O_TRUNC | O_RDWR, 0666);

     cout<<"Writing report to file... "<<endl;
     write(fd, (void*)data.c_str(), strlen(data.c_str())); 
     cout<<"Written report."<<endl;


}



#endif /* EXIT_H_ */
