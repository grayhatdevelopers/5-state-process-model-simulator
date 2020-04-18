#ifndef PROC_H_
#define PROC_H_

#include <string>

struct Proc {
    std::string name;
    int arrivalTime, burstTime;
    int quantum;
    std::string algorithm;

    int blockedTime;

    int waitingTime, waitingTime_start, responseTime, completionTime;

    int blockType, timesBlocked;

    int arrivalTime_orig, burstTime_orig;

    Proc (): arrivalTime (-1), burstTime (-1), quantum (-1), blockedTime(-1), waitingTime(-1), waitingTime_start(-1), responseTime(-1), completionTime(-1), blockType(-1), timesBlocked(-1), arrivalTime_orig (-1), burstTime_orig (-1) { }

    Proc (std::string Name): arrivalTime (-1), burstTime (-1), quantum (-1), blockedTime(-1), waitingTime(-1), waitingTime_start(-1), responseTime(-1), completionTime(-1), blockType(-1), timesBlocked(-1), arrivalTime_orig (-1), burstTime_orig (-1) {
        name = Name;
    }

    void print() const{
        cout<<"Name: " <<name<<", ";
		cout<<"Arrival Time: "<<arrivalTime<<", ";
		cout<<"Burst Time: "<<burstTime<<", ";
		cout<<"Algorithm: "<<algorithm<<", ";
		cout<<"Quantum: "<<quantum<<endl;
    }

    string getFullData(string separator = ", ") const{
        string data = "";
        data+="Name: " +name+separator;
		data+="Final Arrival Time: "+to_string(arrivalTime)+separator;
		data+="Final Burst Time: "+to_string(burstTime)+separator;
		data+="Algorithm: "+algorithm+separator;
		data+="Quantum: "+to_string(quantum)+separator;
        data+="Blocked Time: "+to_string(blockedTime)+separator;
        data+="Time to arrive in Ready (most recent): "+to_string(waitingTime_start)+separator;
        data+="Response Time: "+to_string(responseTime)+separator;
        data+="Waiting Time: "+to_string(waitingTime)+separator;
        data+="I/O Time: "+to_string(completionTime - waitingTime - burstTime_orig)+separator;
        data+="Turnaround Time: "+to_string(completionTime)+separator;
        data+="Block Type: "+to_string(blockType)+separator;
        data+="Number of Times Blocked: "+to_string(timesBlocked + 1)+separator;
        data+="Original Arrival Time: "+to_string(arrivalTime_orig)+separator;
        data+="Original Burst Time: "+to_string(burstTime_orig);
        data+="\n";
        return data;
    }
        // cout<<"Name: " <<name<<separator;
		// cout<<"Arrival Time: "<<arrivalTime<<separator;
		// cout<<"Burst Time: "<<burstTime<<separator;
		// cout<<"Algorithm: "<<algorithm<<separator;
		// cout<<"Quantum: "<<quantum<<separator;
        // cout<<"Blocked Time: "<<blockedTime<<separator;
        // cout<<"Waiting Time: "<<waitingTime<<separator;
        // cout<<"Waiting Time (START): "<<waitingTime_start<<separator;
        // cout<<"Response Time: "<<responseTime<<separator;
        // cout<<"Completion Time: "<<completionTime<<separator;
        // cout<<"Block Type: "<<blockType<<separator;
        // cout<<"Number of Times Blocked: "<<timesBlocked<<separator;
        // cout<<"Original Arrival Time: "<<arrivalTime_orig<<separator;
        // cout<<"Original Burst Time: "<<burstTime_orig;
        // cout<<endl;
    string serialize () const {
        string data = "";
        data = (this->name);
        data += '\n';
        data += to_string(this->arrivalTime);
        data += '\n';
        data += to_string(this->burstTime);
        data += '\n';
        data += to_string(this->quantum);
        data += '\n';
        data += (this->algorithm);
        data += '\n';
        data += to_string(this->blockedTime);
        data += '\n';
        data += to_string(this->waitingTime);
        data += '\n';
        data += to_string(this->waitingTime_start);
        data += '\n';
        data += to_string(this->responseTime);
        data += '\n';
        data += to_string(this->completionTime);
        data += '\n'; 
        data += to_string(this->blockType);
        data += '\n';
        data += to_string(this->timesBlocked);
        data += '\n';
        data += to_string(this->arrivalTime_orig);
        data += '\n'; 
        data += to_string(this->burstTime_orig);
        data += '\n';              
        return data;
    }

    friend ostream &operator<<( ostream &output, const Proc &procs ) { 
         output << procs.serialize ();        
         return output;            
    }

};



// To compare two Waiting times 
struct myWaitingComparator { 
    int operator() (const Proc& p1, const Proc& p2) 
    { 
        return p1.waitingTime > p2.waitingTime; 
    } 
}; 
  
// To compare two Burst times 
struct myBurstComparator { 
    int operator() (const Proc& p1, const Proc& p2) 
    { 
        if (p1.burstTime == p2.burstTime)
            return p1.arrivalTime > p2.arrivalTime;
        else
            return p1.burstTime > p2.burstTime; 
    } 
}; 

// To compare two Arrival times 
struct myArrivalComparator { 
    int operator() (const Proc& p1, const Proc& p2) 
    { 
            return p1.arrivalTime > p2.arrivalTime; 
    } 
}; 
  



#endif /* PROC_H_ */
