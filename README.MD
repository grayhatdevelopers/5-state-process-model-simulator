

## Welcome to Saad's Five State Process Model simulator!
In a multitasking computer system, processes may occupy a variety of states. These distinct states 
may not be recognized as such by the operating system kernel. However, they are a useful abstraction 
for the understanding of processes. The 5 state process model is a basic visualization which handles such 
processes coming in from the memory, using the following states:

        1-   New: A process has been created but has not yet been admitted to the pool of executable processes.
        2-   Ready: Processes that are prepared to run if given an opportunity. That is, they are not waiting on anything except the CPU availability.
        3-   Running: The process that is currently being executed. (Assume single processor for simplicity.)
        4-   Blocked: A process that cannot execute until a specified event such as an IO completion occurs.
        5-   Exit: A process that has been released by OS either after normal termination or after abnormal termination (error).

        (source: https://cse.buffalo.edu/~bina/cse421/spring00/lec3/tsld008.htm)

This program comes with an integrated terminal called "mard". Mard is loaded with a default command, kill,
which can be used to terminate programs running haywire (like the States you'll see soon). You can also use 
Linux/UNIX system calls by prefixing "system" before them, for example:

> ls -l

would become
    
> system ls -l


## HOW TO BUILD:
_(assuming you've opened a terminal in this folder)_
-   cd bin
-   g++ New.cpp -lrt -o New && g++ Ready.cpp -pthread -lrt -o Ready && g++ Running.cpp -lrt -o Running && g++ Blocked.cpp -pthread -lrt -o Blocked && g++ Exit.cpp -lrt -o Exit && g++ kill.cpp -o kill && g++ system.cpp -o system
-   cd ../
-   g++ 5_state_model_simulator.cpp -lrt -pthread -o 5_state_model_simulator

## HOW TO RUN:
-   To set which file to open in New, go to around line 27, where a string "fileToOpen" holds the 
name of the file. By default, it opens "Sample_3_SRTF.txt". You will also have to go to "Exit.cpp", where
a variable "StopAt" holds the number of Procedures at which it should stop and print the Report
-   You can change whether or not it sends the data to a log file or the console itself (you'll have to 
comment a block of code in "5_state_model_simulator.cpp" (lines 227-236)
-   After building, just enter "./5_state_model_simulator" to execute
-   Since the States go in an infinite loop, you can kill them using "kill [pidofprocess]". Multiple pids can
be killed with spaces, e.g. "kill [pid1] [pid2] [pid3]"



