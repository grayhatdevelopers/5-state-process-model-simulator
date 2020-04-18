/*
 * Blocked.cpp
 *
 *  Created on: Mar 22, 2020
 *      Author: saad
 */

#include "Blocked.h"

int main(int argc, char ** argv){
     if (pthread_mutex_init(&lock, NULL) != 0) { 
          cout<<"mutex init has failed"<<endl; 
          return 1; 
     } 
     
	srand(time(NULL));
     cout<<"I am "<<setStateName(argv[0], STATE_NAME)<<"!"<<endl;

	for (int i=0; i<STATE_NAME.length(); i++){
		STATE_NAME[i] = toupper (STATE_NAME[i]);
	}
     

	pipes[0].output_fd = atoi(argv[1]);
	pipes[1].input_fd = atoi(argv[2]);

	MAX_PIPE = atoi(argv[argc-1]);
     
     int x = 0, y = 1, z = 2;
     pthread_create(&tid1, NULL, handle_BlockedGeneric, &x);
     pthread_create(&tid2, NULL, handle_BlockedGeneric, &y);
     pthread_create(&tid3, NULL, handle_BlockedGeneric, &z);


     while (1){
          
          sleep(S_PILL);

          CPU_TIME = getCPUTime();





          //LOGIC FOR EVENT WAIT
          if(unpackPipe(pipes[0].output_fd, procedures, NUMBER_OF_PROCS) < 0){
               // cout<<"BLOCKED "<<pipes[0].name<<": "<<"An error occurred in the unpacking.\n"<<endl;
          }
          else{
               for (int i=0; i<NUMBER_OF_PROCS; i++){

                    procedures[i].blockedTime  = 15 + rand()%10;
                    if (procedures[i].blockType == 0)
                         BlockedQueueOutput.push(procedures[i]);
                    else if (procedures[i].blockType == 1)
                         BlockedQueueInput.push(procedures[i]);
                    else if (procedures[i].blockType == 2)
                         BlockedQueuePrinter.push(procedures[i]);


                    cout<<STATE_NAME<<" "<<pipes[0].name<<" ===> ";
                    procedures[i].print();
                    // cout<<"BlockType: "<<procedures[i].blockType<<endl;

               }

          }


          //LOGIC FOR EVENT OCCURS
          // handle_BlockedGeneric(pipes[1], BlockedQueueOutput, STATE_NAME);
          // handle_BlockedGeneric(pipes[1], BlockedQueueInput, STATE_NAME);
          // handle_BlockedGeneric(pipes[1], BlockedQueuePrinter, STATE_NAME);



     

     }

     pthread_mutex_destroy(&lock); 
     exit(0);
}
