#ifndef STATE_H_
#define STATE_H_

#include <string>
#include <iostream>

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

using namespace std;

struct State {
	string name;
	int id;

	int NumberOfProcesses;
	string TopProc;
	
	State (string Name): name(Name), id (-1), NumberOfProcesses (0), TopProc (""){}

};

// timer functionality
const char * nameOfTimer = "/timer";

/* open the shared memory object */
auto shm_fd2 = shm_open(nameOfTimer, O_RDONLY, 0666);

/* memory map the shared memory object */
void* timer_ptr = mmap(0, sizeof(int), PROT_READ, MAP_SHARED, shm_fd2, 0);

int getCPUTime(){
	return stoi((char*)timer_ptr);
}


// Generic function to get Procs using packman
void getProcs (){

} 
  
string setStateName (string argv, string &STATE_NAME){
	argv.erase(0,2);
	STATE_NAME = argv;
	return STATE_NAME;
}


#endif /* STATE_H_ */
