/*
 * Exit.cpp
 *
 *  Created on: Mar 22, 2020
 *      Author: saad
 */

#include "Exit.h"

int StopAt = 9;

int main(int argc, char ** argv){

     cout<<"I am "<<setStateName(argv[0], STATE_NAME)<<"!"<<endl;

	for (int i=0; i<STATE_NAME.length(); i++){
		STATE_NAME[i] = toupper (STATE_NAME[i]);
	}

	pipes[0].output_fd = atoi(argv[1]);

	MAX_PIPE = atoi(argv[argc-1]);	


	//real work starts here
     Proc * procedures;
     int NUMBER_OF_PROCS = 0;


     queue <Proc> ExitQueue;

     while (1){
          
          sleep(S_PILL);

          NUMBER_OF_PROCS = 0;

          if (RELEASED_PROCESSES == StopAt){
               cout<<"I have gotten all the processes..."<<endl;
               Proc* procedures = new Proc [StopAt];
               for (int i=0; i<StopAt; i++){
                    cout<<STATE_NAME<<" FINALLY: ";
                    cout<<ExitQueue.front().getFullData();
                    procedures[i] = ExitQueue.front();
                    ExitQueue.pop();
               }

               printReport(procedures, StopAt);

               break;

          }
          if(unpackPipe(pipes[0].output_fd, procedures, NUMBER_OF_PROCS) < 0){
               // cout<<"EXIT "<<pipes[0].name<<": An error occurred in the unpacking.\n"<<endl;
          }
          else{
               for (int i=0; i<NUMBER_OF_PROCS; i++){
                    // cout<<"loop"<<endl;
                    ExitQueue.push(procedures[i]);

                    cout<<STATE_NAME<<" "<<pipes[0].name<<" ===> ";
                    ExitQueue.back().print();
                    RELEASED_PROCESSES ++;
               }
          }
     }

     cout<<"Exiting Exit..."<<endl;
     return 0;
     
}
