#include <signal.h>
#include <string>
#include <iostream>
int main(int argc, char ** argv){
    std::cout<<"Killer activated\n";
    std::cout<<argv[0]<<"\n";
    std::cout<<argv[1]<<"\n";
    for (int i=1; i<argc; i++)
        kill(std::stoi(argv[i]),15);
}
