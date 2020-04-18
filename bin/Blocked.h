/*
 * Blocked.h
 *
 *  Created on: Mar 22, 2020
 *      Author: saad
 */

#ifndef BLOCKED_H_
#define BLOCKED_H_
#include "../include/Pipe.h"
#include "../include/packman.h"
#include "../include/init.h"
#include "../include/MinHeap.h"
#include "../include/State.h"

#include <queue>
#include <time.h>
#include <cstring>
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


/*
* 
* Keeping the Queues and stuff global because I really don't want to 
* keep passing arguments :)
* 
*/
pthread_mutex_t lock; 


int CPU_TIME = 0;



int MAX_PIPE = -1;

static int TOTAL_NUMBER_OF_PIPES = 2;
Pipe* pipes = new Pipe [2] {
	{"EventWait"},		//Running To Blocked		1
	{"EventOccurs"}	//Blocked To Ready			2
};

string STATE_NAME = "";

pthread_t tid1, tid2, tid3;  


//real work starts here
Proc * procedures;
int NUMBER_OF_PROCS;

// queue <Proc> BlockedQueue;

// Creates a Min heap of Procedures (order by myWaitingComparator) 
priority_queue <Proc, vector<Proc>, myWaitingComparator > BlockedQueueInput; 
// Creates a Min heap of Procedures (order by myWaitingComparator) 
priority_queue <Proc, vector<Proc>, myWaitingComparator > BlockedQueueOutput; 
// Creates a Min heap of Procedures (order by myWaitingComparator) 
priority_queue <Proc, vector<Proc>, myWaitingComparator > BlockedQueuePrinter; 



/* 
* End
*/

void* handle_BlockedGeneric (void*arg){
     int type = *(int*)arg;
     priority_queue <Proc, vector<Proc>, myWaitingComparator>& theQueue = ( type == 0 ? 
                                                                           BlockedQueueOutput 
                                                                           : ( type == 1 ? 
                                                                           BlockedQueueInput 
                                                                           : BlockedQueuePrinter));

     Pipe pipe = pipes[1];

     while (1){
          sleep(S_PILL);
          string data = "";
          int data_bytes = 0;
          if (theQueue.size() != 0){
               if (theQueue.top().blockedTime<=0){
                    pthread_mutex_lock(&lock); 
                    if(packPipe(pipe.input_fd, theQueue.top(), 1, data, data_bytes) != 0)
                    {
                         // cout <<"BLOCKED "<<pipes[1].name<<": There was an error writing to pipe\n"<<endl;    // strictly not an error, it is allowable for fewer characters than requested to be written.
                    }
                    else {
                         cout<<STATE_NAME<<" "<<pipe.name<<" <=== ";
                         theQueue.top().print();
                         // cout<<"BlockType: "<<theQueue.top().blockType<<endl;
                         theQueue.pop();
                    }
                    pthread_mutex_unlock(&lock); 
               }
          }

          if (theQueue.size()!=0){

                    // cout<<"In the queue minuser..."<<endl;
                    vector<Proc> tempBlockedQueue;

                    int tempsize = theQueue.size();
                    for (int i=0; i<tempsize; i++){
                         // cout<<"BlockedQueue top is == ";
                         // theQueue.top().print();
                         // cout<<"Blocked time is... "<<theQueue.top().blockedTime<<endl;
                         tempBlockedQueue.push_back(theQueue.top());
                         theQueue.pop();
                         if (tempBlockedQueue[i].blockedTime>0)  
                              tempBlockedQueue[i].blockedTime--;

                    }
                    for (int i=0; i<tempsize; i++){

                         theQueue.push(tempBlockedQueue[tempBlockedQueue.size()-1]);
                         tempBlockedQueue.pop_back();

                    }
               }
     }

}



void* handle_BlockedGeneric (Pipe pipe, priority_queue <Proc, vector<Proc>, myWaitingComparator> &theQueue, string STATE_NAME){
     string data = "";
     int data_bytes = 0;
     if (theQueue.size() != 0){
          if (theQueue.top().blockedTime==0){
               if(packPipe(pipe.input_fd, theQueue.top(), 1, data, data_bytes) != 0)
               {
                    // cout <<"BLOCKED "<<pipes[1].name<<": There was an error writing to pipe\n"<<endl;    // strictly not an error, it is allowable for fewer characters than requested to be written.
               }
               else {
                    cout<<STATE_NAME<<" "<<pipe.name<<" <=== ";
                    theQueue.top().print();
                    // cout<<"BlockType: "<<theQueue.top().blockType<<endl;
                    theQueue.pop();
               }
          }
     }

     if (theQueue.size()!=0){

               // cout<<"In the queue minuser..."<<endl;
               vector<Proc> tempBlockedQueue;

               int tempsize = theQueue.size();
               for (int i=0; i<tempsize; i++){
                    // cout<<"BlockedQueue top is == ";
                    // theQueue.top().print();
                    // cout<<"Blocked time is... "<<theQueue.top().blockedTime<<endl;
                    tempBlockedQueue.push_back(theQueue.top());
                    theQueue.pop();
                    if (tempBlockedQueue[i].blockedTime>0)  
                         tempBlockedQueue[i].blockedTime--;

               }
               for (int i=0; i<tempsize; i++){

                    theQueue.push(tempBlockedQueue[tempBlockedQueue.size()-1]);
                    tempBlockedQueue.pop_back();

               }
          }

} 






#endif /* BLOCKED_H_ */
