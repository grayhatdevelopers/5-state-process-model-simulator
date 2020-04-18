/*
    Packman is a utility library used to extract from and store information
    in pipes. Primary pack/unpack features are:
        1. Procedures in Files
        2. Procedures in Pipes 
        3. Notifier signals in pipes
*/

#ifndef PACKMAN_H_
#define PACKMAN_H_

#include "./Proc.h"

#include <cerrno>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;

 /**
   *  @brief  Check whether or not a string is a number.
   *
   *  @param word  The string to check.
   *
   *  @note Null is not considered a number.
   *
  */
bool isNum (string word){
    if (word == "\0")
        return false;
    for (int i=0; i< word.length(); i++){
        if (word[i] < '0' or word[i]>'9')
            return false;
    }
    return true;
}



 /**
   *  @brief  Store some procedures in a pipe.
   *
   *  @param fd                 File descriptor of pipe.
   *  @param procs              The proc which you want to store.
   *  @param number_of_procs    The number of procs which you want to store.
   *  @param data               By the end, contains the corresponding string which was passed into the pipe
   *  @param bytes              By the end, contains the total bytes written
   * 
   *  @note It can potentially work for more than one proc, but I've restricted it for now
   *
  */
int packPipe(int fd, Proc procs, int number_of_procs, string & data, int & bytes){
    data = "";
    bytes = 0;

    for (int i=0; i<number_of_procs; i++){
        data = procs.serialize();
    }

    data.pop_back();
    data += '\0';

    bytes = data.length();

    // cout<<"Writing to fd "<<fd<<endl;
    if (write(fd, data.c_str(), bytes) != bytes){
        cout<<"There was an error in write"<<endl;
        return -1;
    }

    // cout<<"Finished writing to fd"<<endl;
    return 0;
}


 /**
   *  @brief  Store a variable in a pipe.
   *
   *  @param fd                 File descriptor of pipe.
   *  @param spaceInState       The variable you want to store.
   *
  */
int packPipe(int fd, int spaceInState){
    string data = "";
    int bytes = 0;

    data += to_string(spaceInState);
    data += '\0';
    bytes = data.length();

    // cout <<"DATA is "<<data<<", bytes are "<<bytes<<endl;
    // cout<<"Writing to fd "<<fd<<endl;
    if (write(fd, data.c_str(), bytes) != bytes){
        // cout<<"There was an error in write"<<endl;
        return -1;
    }

    // cout<<"Finished writing to fd"<<endl;
    return 0;
}


 /**
   *  @brief  Extract procedures from a pipe.
   *
   *  @param fd                 File descriptor of pipe.
   *  @param procs              By the end, contains the Procs which have been extracted.
   *  @param number_of_procs    By the end, contains the number of Procs which have been extracted.
   *
  */
int unpackPipe(int fd, Proc * & procs, int & number_of_procs){

    string returnThis = "";
    int errRet = 0;
    // cout<<"hello unpack pipe"<<endl;

    vector<Proc> tempProc;
    string algorithm;
    int quantum = -1;

    char * d = new char [1];

    *d = 'a';

    //read Body
    int Ncounter = 0;
    while (*d != '\0'){
        memset(d, '\0', 1);
        if ((errRet = read(fd, d, 1))<=0){
            if (errRet == -1){
                // cout<<"File was empty"<<endl;
            }
            else{
                // cout<<"Encountered error during reading"<<endl;
            }
            return -1;
        }
        if (d[0] == '\n' or d[0] == '\0'){
            switch (Ncounter){
            case 0:
                    // cout<<"Return this in case 0 = "<<returnThis<<endl;                    
                    tempProc.push_back(returnThis);
                    break;
            case 1:
                    // cout<<"Return this in case 1 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].arrivalTime = stoi (returnThis);
                    break;
            case 2:
                    // cout<<"Return this in case 2 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].burstTime = stoi (returnThis);
                    break;
            case 3:
                    // cout<<"Return this in case 3 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].quantum = stoi (returnThis);
                    break;
            case 4:
                    // cout<<"Return this in case 4 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].algorithm = (returnThis);
                    break;
            case 5:
                    // cout<<"Return this in case 5 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].blockedTime = stoi (returnThis);
                    break;  
            case 6:
                    // cout<<"Return this in case 6 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].waitingTime = stoi (returnThis);
                    break;    
            case 7:
                    // cout<<"Return this in case 7 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].waitingTime_start = stoi (returnThis);
                    break;    
            case 8:
                    // cout<<"Return this in case 8 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].responseTime = stoi (returnThis);
                    break;                                        
            case 9:
                    // cout<<"Return this in case 9 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].completionTime = stoi (returnThis);
                    break;
            case 10:
                    // cout<<"Return this in case 10 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].blockType = stoi (returnThis);
                    break;
            case 11:
                    // cout<<"Return this in case 11 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].timesBlocked = stoi (returnThis);
                    break;
            case 12:
                    // cout<<"Return this in case 12 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].arrivalTime_orig = stoi (returnThis);
                    break;                    
            case 13:
                    // cout<<"Return this in case 13 = "<<returnThis<<endl;
                    tempProc[tempProc.size()-1].burstTime_orig = stoi (returnThis);
                    break;
            default:
                    break;
            }
            returnThis = "";
            // cout<<"Ncounter is "<< Ncounter<<endl;
            Ncounter = (Ncounter + 1) % 14;
        }
        else{
            if (*d != '\0')
                returnThis += d[0];
        }

    }
    returnThis="";

    procs = new Proc [tempProc.size()];

    for (int i=0; i<tempProc.size(); i++){
        procs[i] = tempProc[i];
    }

    number_of_procs = tempProc.size();

    return 0;   
}


 /**
   *  @brief  Extract a variable from a pipe.
   *
   *  @param fd                 File descriptor of pipe.
   *  @param spaceInState       By the end, contains the variable in the pipe.
   * 
   *  @note This method converts the variable to an integer as well.
   *
  */
int unpackPipe(int fd, int & spaceInState){

    string returnThis = "";
    int errRet = 0;

    char * d = new char [1];

    *d = 'a';

    //read Body
    int Ncounter = 0;
    while (*d != '\0' and *d != '\n'){
        memset(d, '\0', 1);
        if ((errRet = read(fd, d, 1))<=0){
            if (errRet == 0){
                // cout<<"Pipe was empty, EOF reached"<<endl;
            }
            else{
                // std::cout << "Pipe read failed: " << std::strerror(errno) << '\n';
            }
            return -1;
        }
        if (d[0] == '\n' or d[0] == '\0'){
            switch (Ncounter){
            case 0:
                    // cout<<"Return this in case 0 = "<<returnThis<<endl;                    
                    spaceInState = stoi(returnThis);
                    break;
            default:
                    break;
            }
            returnThis = "";
            Ncounter = (Ncounter + 1) % 7;
        }
        else{
            if (*d != '\0')
                returnThis += d[0];
        }

    }
    returnThis="";
    return 0;   
}



// ============================================ File I/O (as provided in assignment) 

 /**
   *  @brief  Store some procedures in a file.
   *
   *  @param fd                 File descriptor of file.
   *  @param procs              The proc which you want to store.
   *  @param number_of_procs    The number of procs which you want to store.
   *  @param data               By the end, contains the corresponding string which was passed into the pipe
   *  @param bytes              By the end, contains the total bytes written
   * 
   *  @note It can potentially work for more than one proc, but I've restricted it for now.
   *
  */
int packFile (int fd, Proc * procs, int number_of_procs, string & data, int & bytes){
    data = "";
    bytes = 0;

    // write(  fd,   procs[0].algorithm.c_str(),   procs[0].algorithm.length());
    data += procs[0].algorithm.c_str();
    data += '\n';

    if (procs[0].quantum != -1){
        data += to_string(procs[0].quantum);
        data += '\n';
    }

    for (int i=0; i<number_of_procs; i++){
        data = procs[i].serialize();
    }

    data.pop_back();
    data += '\0';

    bytes = data.length();

    cout<<"Writing to fd "<<fd<<endl;
    if (write(fd, data.c_str(), bytes) != bytes){
        cout<<"There was an error in write"<<endl;
        return -1;
    }

    cout<<"Finished writing to fd"<<endl;
    return 0;
}

 /**
   *  @brief  Extract procedures from a File.
   *
   *  @param fd                 File descriptor of file.
   *  @param procs              By the end, contains the Procs which have been extracted.
   *  @param number_of_procs    By the end, contains the number of Procs which have been extracted.
   *
  */
int unpackFile(int fd, Proc * & procs, int & number_of_procs){

    string returnThis = "";

    int errRet = 0;

    vector<Proc> tempProc;
    string algorithm;
    int quantum = -1;
    bool isQuantum = false;

    char * d = new char [1];
    if ((errRet = read(fd, d, 1))<=0){
        if (errRet == -1){
            // cout<<"File was empty"<<endl;
        }
        else{
            // cout<<"Encountered error during reading"<<endl;
        }
        return -1;
    }
    returnThis += d[0];

    //read Header
    while (*d != '\n' and *d != '\0'){
        memset(d, '\0', 1);
        if (read(fd, d, 1)<0){
        cout<<"Encountered error during reading"<<endl;
        return -1;
        }  
        if (d[0] == '\n'){
            returnThis.pop_back();
            algorithm = returnThis;
            returnThis = "";

            break;
        }
        else{
                returnThis += d[0];
        }
    }

    //read Body
    int Ncounter = 0;
    while (*d != '\0'){
        memset(d, '\0', 1);
        if ((errRet = read(fd, d, 1))<0){
            if (errRet == -1)
                cout<<"File was empty"<<endl;
            else
                cout<<"Encountered error during reading"<<endl;
            return -1;
        }
        if (d[0] == '\n' or d[0] == '\0'){
            switch (Ncounter){
            case 0:
                    if (returnThis == "\0")
                        continue;
                    // cout<<"Return this in case 0 = "<<returnThis<<endl;
                        if (!isNum(returnThis))
                            returnThis.pop_back();

                    if (isNum(returnThis)){
                        // cout<<returnThis<<" is a number... Or is it?"<<endl;
                        quantum = stoi (returnThis);
                        Ncounter --;
                        // cout<<"Successfully set quantum"<<endl;
                    }
                    else {
                        tempProc.push_back(returnThis);
                    }
                    break;
            case 1:
                    // cout<<"Return this in case 1 = "<<returnThis<<endl;
                    if (!isNum(returnThis))
                        returnThis.pop_back();;
                    if (isNum(returnThis))
                        tempProc[tempProc.size()-1].arrivalTime = stoi (returnThis);
                        tempProc[tempProc.size()-1].arrivalTime_orig = stoi (returnThis);
                    break;
            case 2:
                    // cout<<"Return this in case 2 = "<<returnThis<<endl;
                    if (!isNum(returnThis))
                        returnThis.pop_back();
                    if (isNum(returnThis)){
                        // cout<<"Im boutta"<<endl;
                        // cout<<"Length = "<<returnThis.length()<<endl;
                        // cout<<"0: "<<returnThis[0]<<", 1: "<<returnThis[1]<<endl;
                        tempProc[tempProc.size()-1].burstTime = stoi (returnThis);
                        tempProc[tempProc.size()-1].burstTime_orig = stoi (returnThis);
                    }
                    break;
            default:
                    break;
            }
            if (Ncounter == 0){
                tempProc[tempProc.size() - 1].algorithm = algorithm;
                tempProc[tempProc.size() - 1].quantum = quantum;
            }
            returnThis = "";
            Ncounter = (Ncounter + 1) % 3;
        }
        else{
            if (*d != '\0')
                returnThis += d[0];
        }

    }
    returnThis="";

    procs = new Proc [tempProc.size()];

    for (int i=0; i<tempProc.size(); i++){
        procs[i] = tempProc[i];
        // cout<<"Proooooc "<<i<<procs[i].algorithm<<endl;
    }

    number_of_procs = tempProc.size();

    return 0;   
}

// ============================================ END ========= File I/O (as provided in assignment)




#endif /* PACKMAN_H_ */
