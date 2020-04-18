/*
 * Ready.h
 *
 *  Created on: Mar 22, 2020
 *      Author: saad
 */

#ifndef READY_H_
#define READY_H_
#include "../include/Pipe.h"
#include "../include/packman.h"
#include "../include/init.h"
#include "../include/State.h"

#include <unistd.h>
#include <fcntl.h>

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


int CPU_TIME = 0;
string STATE_NAME = "";
int MAX_PIPE = -1;

static int TOTAL_NUMBER_OF_PIPES = 6;
Pipe* pipes = new Pipe [6] {
	{"Admit"}, 			//New To Ready				1
	{"Dispatch"}, 		     //Ready To Running			2
	{"TimeOut"}, 		     //Running To Ready			3
	{"EventOccurs"},		//Blocked To Ready			4
     {"sendProc"},		//Signalling b/w Ready & Running   5
	{"sendRemaining"}	//Sending the remaining burst time	6
};


/*
* 
* Keeping the Queues and stuff global because I really don't want to 
* keep passing arguments :)
* 
*/

//real work starts here
Proc * procedures = nullptr;
int NUMBER_OF_PROCS;


//DATA STRUCTURE FOR: Procedures coming from new
     // Creates a queue of Procedures via STL
priority_queue <Proc, vector<Proc>, myArrivalComparator> ReadyQueueArrival; 

//DATA STRUCTURE FOR: RR and FCFS
     // Creates a queue of Procedures via STL
queue <Proc> ReadyQueue;

//DATA STRUCTURE FOR: SJF and SRTF
     // Creates a Min heap of Procedures (order by myBurstComparator) 
priority_queue <Proc, vector<Proc>, myBurstComparator> ReadyQueueSJF; 

pthread_t tid1, tid2, tid3;  

/* 
* End
*/


void evaluateArrivalTimes(){
     if (ReadyQueueArrival.size()!=0){
               //acrobatics time...
          vector<Proc> tempReadyArrivalQueue;

          int tempsize = ReadyQueueArrival.size();
          for (int i=0; i<tempsize; i++){
               // cout<<"ReadyQueueArrival top is == ";
               // ReadyQueueArrival.top().print();
               // cout<<"Arrival time is... "<<ReadyQueueArrival.top().arrivalTime<<endl;
               tempReadyArrivalQueue.push_back(ReadyQueueArrival.top());
               ReadyQueueArrival.pop();
               if (tempReadyArrivalQueue[i].arrivalTime>0)  
                    tempReadyArrivalQueue[i].arrivalTime--;

          }
          for (int i=0; i<tempsize; i++){

               ReadyQueueArrival.push(tempReadyArrivalQueue[tempReadyArrivalQueue.size()-1]);
               tempReadyArrivalQueue.pop_back();

          }
          while (ReadyQueueArrival.top().arrivalTime <= 0){
               if (ReadyQueueArrival.top().algorithm == "RR" or ReadyQueueArrival.top().algorithm == "FCFS"){
                    ReadyQueue.push(ReadyQueueArrival.top());
               }
               else if (ReadyQueueArrival.top().algorithm == "SJF" or ReadyQueueArrival.top().algorithm == "SRTF"){
                    ReadyQueueSJF.push(ReadyQueueArrival.top());
               }
               ReadyQueueArrival.pop();
          }
     }

     // if (ReadyQueueSJF.size()!=0){
     //      if (ReadyQueueSJF.top().arrivalTime > 0){
     //           //more acrobatics incoming...
     //           // cout<<"In evaluateArrivalTimes... arrivaltime was -> "<<ReadyQueueSJF.top().name<<" "<<ReadyQueueSJF.top().arrivalTime<<endl;
     //           Proc tempProc = ReadyQueueSJF.top();
     //           ReadyQueueSJF.pop();
     //           tempProc.arrivalTime--;
     //           ReadyQueueSJF.push(tempProc);

     //      }
     // }
}


int getProcsReady (Pipe pipeToRead, Pipe pipeToSignal, queue<Proc> & theQueue, priority_queue <Proc, vector<Proc>, myBurstComparator> & theOtherQueue) {
     Proc * procedures = nullptr;
     int NUMBER_OF_PROCS = 0;
     if(unpackPipe(pipeToRead.output_fd, procedures, NUMBER_OF_PROCS) < 0){
          // cout<<"READY "<<pipes[0].name<<": An error occurred in the unpacking.\n"<<endl;
          return -2;
     }
     else{
          for (int i=0; i<NUMBER_OF_PROCS; i++){
               procedures[i].waitingTime_start = CPU_TIME;
               // cout<<"CPU TIME WHEN PROCEDURE ARRIVES "<<CPU_TIME<<endl;
               if (procedures[i].completionTime < 0){           // The Proc has arrived for the first time! Fresh meat.
                    cout<<"CPU TIME WHEN PROCEDURE ARRIVES "<<CPU_TIME<<endl;
                    procedures[i].waitingTime = 0;
                    procedures[i].completionTime = CPU_TIME;
                    procedures[i].responseTime = CPU_TIME;
                    ReadyQueueArrival.push(procedures[i]);
                    cout<<STATE_NAME<<" "<<pipeToRead.name<<" ===> ";
                    procedures[i].print();
               }
               else {
                    if (procedures[i].algorithm == "RR" or procedures[i].algorithm == "FCFS"){
                         theQueue.push(procedures[i]);
                         cout<<STATE_NAME<<" "<<pipeToRead.name<<" ===> ";
                         theQueue.back().print();
                    }
                    else if (procedures[i].algorithm == "SJF" or procedures[i].algorithm == "SRTF"){
                         theOtherQueue.push(procedures[i]);
                         cout<<STATE_NAME<<" "<<pipeToRead.name<<" ===> ";
                         procedures[i].print();
                         if (procedures[i].algorithm == "SRTF"){
                              if (packPipe(pipeToSignal.input_fd, procedures[i].burstTime) != 0){
                                   
                              }
                              else{
                                   
                              }
                         }
                    }
               }
          }
     }

}


//toDo: Still very skeptical of this. Test on other algo files, and try to take two arguments
void* listenToPipe_ReadyGeneric( void* arg ){
     int pipe = *(int*) arg;
     cout<<"Pipe number is "<<pipe<<endl;

     //since we are dedicating an independent thread for listening, we will restore the pipe to Blocking Mode
     fcntl(pipe, F_SETFL,
          fcntl(pipe, F_GETFL) &
          ~O_NONBLOCK);


     //LOGIC FOR PROC EXTRACTION
     while (1){
          sleep(S_PILL);
          // cout<<"I am checking "<<pipe<<endl;
          getProcsReady(pipes[pipe], pipes[5], ReadyQueue, ReadyQueueSJF);
     }
}

void* listenToPipe_ReadyDispatch( void* arg ){
     string data = "";
     int data_bytes = 0;

     // LOGIC FOR DISPATCH
     int spaceInRunning = -1;
     if  (unpackPipe(pipes[4].output_fd,spaceInRunning) < 0){
          // cout<<"READY "<<pipes[4].name<<": An error occurred in the unpacking.\n"<<endl;
     }
     else{
          if (spaceInRunning == 0){
               cout<<STATE_NAME<<" "<<"Running might be full."<<endl;                    
          }
          else {
               if (ReadyQueue.size() != 0){            //Preferring RR and FCFS
                    if (ReadyQueue.front().arrivalTime <= 0){
                         int tempWaiting = ReadyQueue.front().waitingTime_start;
                         // if (ReadyQueue.front().waitingTime < 0){
                              ReadyQueue.front().waitingTime += (CPU_TIME - tempWaiting); 
                         // }
                         if(packPipe(pipes[1].input_fd, ReadyQueue.front(), 1, data, data_bytes) != 0)
                         {
                              ReadyQueue.front().waitingTime -= (CPU_TIME - tempWaiting);
                              // cout <<"READY "<<pipes[1].name<<": There was an error writing to pipe\n"<<endl;    // strictly not an error, it is allowable for fewer characters than requested to be written.
                         }
                         else {
                              cout<<STATE_NAME<<" "<<pipes[1].name<<" <=== ";
                              ReadyQueue.front().print();
                              ReadyQueue.pop();
                         }
                    }
               }
               else if (ReadyQueueSJF.size() != 0){    //... over SJF and SRTF
                    if (ReadyQueueSJF.top().arrivalTime <= 0){
                         int tempWaiting = ReadyQueueSJF.top().waitingTime_start;
                         // if (ReadyQueueSJF.top().waitingTime < 0){
                              // ReadyQueueSJF.top().waitingTime = CPU_TIME - tempWaiting; //time to do some acrobatics, since STL decides that the heap is unmodifiable. Smh.
                              Proc tempProc = ReadyQueueSJF.top();
                              ReadyQueueSJF.pop();
                              tempProc.waitingTime += (CPU_TIME - tempWaiting);
                              ReadyQueueSJF.push(tempProc);
                         // }
                         if(packPipe(pipes[1].input_fd, ReadyQueueSJF.top(), 1, data, data_bytes) != 0)
                         {
                              // Restore waiting time
                                   // more acrobatics incoming...
                              Proc tempProc = ReadyQueueSJF.top();
                              ReadyQueueSJF.pop();
                              tempProc.waitingTime -= (CPU_TIME - tempWaiting);
                              ReadyQueueSJF.push(tempProc);


                              // cout <<"READY "<<pipes[1].name<<": There was an error writing to pipe\n"<<endl;    // strictly not an error, it is allowable for fewer characters than requested to be written.
                         }
                         else {
                              cout<<STATE_NAME<<" "<<pipes[1].name<<" <=== ";
                              ReadyQueueSJF.top().print();
                              ReadyQueueSJF.pop();
                         }
                    }
               }
          }
     }
}



#endif /* READY_H_ */
