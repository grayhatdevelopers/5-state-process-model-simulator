/*
 * Ready.cpp
 *
 *  Created on: Mar 22, 2020
 *      Author: saad
 */

#include "Ready.h"

int main(int argc, char ** argv){

	cout<<"I am "<<setStateName(argv[0], STATE_NAME)<<"!"<<endl;

	for (int i=0; i<STATE_NAME.length(); i++){
		STATE_NAME[i] = toupper (STATE_NAME[i]);
	}

	pipes[0].output_fd = atoi(argv[1]);
	pipes[1].input_fd = atoi(argv[2]);
	pipes[2].output_fd = atoi(argv[3]);
	pipes[3].output_fd = atoi(argv[4]);

     pipes[4].input_fd = atoi(argv[5]);
	pipes[4].output_fd = atoi(argv[6]);

     pipes[5].input_fd = atoi(argv[7]);
	pipes[5].output_fd = atoi(argv[8]);
     
	MAX_PIPE = atoi(argv[argc-1]);

     int x = 0, y = 2, z = 3;
     pthread_create(&tid1, NULL, listenToPipe_ReadyGeneric, &x);
     pthread_create(&tid2, NULL, listenToPipe_ReadyGeneric, &y);
     pthread_create(&tid3, NULL, listenToPipe_ReadyGeneric, &z);

     while (1){
          sleep(S_PILL);
          CPU_TIME = getCPUTime();
          evaluateArrivalTimes();
          listenToPipe_ReadyDispatch(NULL);
     }
     exit(0);
}
