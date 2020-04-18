/*
 * Running.cpp
 *
 *  Created on: Mar 22, 2020
 *      Author: saad
 */

#include "Running.h"

int main(int argc, char ** argv){

     srand (time(NULL));
	cout<<"I am "<<setStateName(argv[0], STATE_NAME)<<"!"<<endl;

	for (int i=0; i<STATE_NAME.length(); i++){
		STATE_NAME[i] = toupper (STATE_NAME[i]);
	}

	pipes[0].output_fd = atoi(argv[1]);
	pipes[1].input_fd = atoi(argv[2]);
	pipes[2].input_fd = atoi(argv[3]);
	pipes[3].input_fd = atoi(argv[4]);

     pipes[4].input_fd = atoi(argv[5]);
	pipes[4].output_fd = atoi(argv[6]);

     pipes[5].input_fd = atoi(argv[7]);
	pipes[5].output_fd = atoi(argv[8]);

	MAX_PIPE = atoi(argv[argc-1]);


     while (1){

          // cout<<"RunningQueue size is "<<RunningQueue.size()<<endl;          
          sleep(S_PILL);


          CPU_TIME = getCPUTime();


          if (RunningQueue.size()!=0)
               RunningQueue.front().burstTime--;


          PROGRAM_TIME++;


          if (RunningQueue.size()!=0){
               if (CPU_TIME % 5 == 0){
                    INTERRUPT = rand()%2;
               }
          }
          else{
               INTERRUPT = -1;
          }
     



          string data="";
          int data_bytes = 0;


          //LOGIC FOR RELEASE
          if (RunningQueue.size()!=0){
               if (RunningQueue.front().burstTime == 0){

                    int tempCompletionTime = RunningQueue.front().completionTime;
                    RunningQueue.front().completionTime = getCPUTime() - tempCompletionTime;
                    cout<<"CPU TIME WHEN RELEASING "<<getCPUTime()<<endl;
                    if(packPipe(pipes[3].input_fd, RunningQueue.front(), 1, data, data_bytes) != 0)
                    {
                         // cout <<"RUNNING "<<pipes[3].name<<": There was an error writing to pipe\n"<<endl; 
                         //restore original Completion Time in case of error
                         RunningQueue.front().completionTime = tempCompletionTime;
                    }
                    else {
                         cout<<STATE_NAME<<" "<<pipes[3].name<<" <=== ";
                         RunningQueue.front().print();
                         RunningQueue.pop();
                    }                    
               }
          }


          //LOGIC FOR DISPATCH
          if(unpackPipe(pipes[0].output_fd, procedures, NUMBER_OF_PROCS) < 0){
               // cout<<"RUNNING "<<pipes[0].name<<": "<<"An error occurred in the unpacking.\n"<<endl;
          }
          else{
               for (int i=0; i<NUMBER_OF_PROCS; i++){
                    if (procedures[i].responseTime < 0){    //Proc has arrived in Running for the first time! Fresh meat.
                         procedures[i].responseTime = CPU_TIME + procedures[i].responseTime;        //Calculates the time difference (Remember, response time is negative before coming in Running)
                    }
                    RunningQueue.push(procedures[i]);
               }

               cout<<"RUNNING "<<pipes[0].name<<" ===> ";
               RunningQueue.back().print();

               PROGRAM_TIME = 0;
          }


          
          //LOGIC FOR TIME OUT (and quantum checking in case of SRTF)
          if (RunningQueue.size()!=0){

               //SRTF Logic
               if (RunningQueue.front().algorithm == "SRTF"){
                    int remainingBurstTime = 0;
                    if (unpackPipe(pipes[5].output_fd, remainingBurstTime) != 0){
                         // Error occurred
                    }
                    else{
                         if (remainingBurstTime < RunningQueue.front().burstTime){
                              if(packPipe(pipes[1].input_fd, RunningQueue.front(), 1, data, data_bytes) != 0)
                              {
                                   // cout <<"RUNNING "<<pipes[1].name<<": There was an error writing to pipe\n"<<endl;    // strictly not an error, it is allowable for fewer characters than requested to be written.
                              }
                              else {
                                   cout<<STATE_NAME<<" "<<"(SRTF Switch) "<<pipes[1].name<<" <=== ";
                                   RunningQueue.front().print();
                                   RunningQueue.pop();
                              }
                         }
                    }
               }




               if (RunningQueue.front().quantum != -1 and RunningQueue.front().quantum == PROGRAM_TIME){
                    if (RunningQueue.size() != 0){
                         if(packPipe(pipes[1].input_fd, RunningQueue.front(), 1, data, data_bytes) != 0)
                         {
                              // cout <<"RUNNING "<<pipes[1].name<<": There was an error writing to pipe\n"<<endl;    // strictly not an error, it is allowable for fewer characters than requested to be written.
                         }
                         else {
                              cout<<STATE_NAME<<" "<<pipes[1].name<<" <=== ";
                              RunningQueue.front().print();
                              RunningQueue.pop();
                         }
                    }
               }
          }


          //LOGIC FOR EVENT WAIT
          if (INTERRUPT == 0){
               if (RunningQueue.size() != 0){

                    RunningQueue.front().blockType = rand()%3;
                    RunningQueue.front().timesBlocked++;

                    if(packPipe(pipes[2].input_fd, RunningQueue.front(), 1, data, data_bytes) != 0)
                    {
                         // cout <<"RUNNING "<<pipes[2].name<<": There was an error writing to pipe\n"<<endl;    // strictly not an error, it is allowable for fewer characters than requested to be written.
                         // restore timesBlocked
                         RunningQueue.front().timesBlocked--;
                    }
                    else {
                         cout<<STATE_NAME<<" "<<pipes[2].name<<" <=== ";
                         RunningQueue.front().print();
                         RunningQueue.pop();
                    }
               }
          }

          
          //LOGIC FOR SENDPROC
          int spaceLeft = MAX_PROCESSES - RunningQueue.size();
          if (spaceLeft > 0){
               if(packPipe(pipes[4].input_fd, spaceLeft) != 0)
               {
                    // cout <<"RUNNING "<<pipes[4].name<<": There was an error writing to pipe\n"<<endl;    // strictly not an error, it is allowable for fewer characters than requested to be written.
               }
               else {
                    // continue
               }
          }



     }


}
