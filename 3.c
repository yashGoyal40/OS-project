/*
Q3)  Have to implement SJF 
     Have to calculate total time taken by cpu , average TAT , and average waiting time
     Have to make a sceduling table for the same 
     If the arrival time is 0 discard that process
     The waiting queue is checked after every process is completed (non - preemptive)
     Every time the table is checked and re-aranged that will take 2 time unit (according to question) 
*/


#include <stdio.h>  //standered i/o library for c
#include <limits.h> //used for max/min limits

//declared a global variable to keep track of time
int Current_Time = 0;
//will be increased by 2 every time a process is completed and waiting table is checked


// defined a struct datatype for processes
struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int TAT;
    int waiting_time;
};

//declaration of all the functions 
void sort_by_arival_time(struct Process processes[], int num_processes);
void sort_by_burst_time(struct Process processes[], int num_processes);
void input_from_user(struct Process processes[], int num_processes);
void execute(struct Process* p);
void run_sceduler(struct Process processes[], struct Process waiting_queue[], int num_processes);
void print_result(struct Process processes[], int num_processes);



//Definition of all the functions


//will sort the array of processes with respect to arival time
void sort_by_arival_time(struct Process processes[], int num_processes) {
    //buble sort
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = 0; j < num_processes - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                // Swap processes[j] and processes[j + 1]
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

//will sort the orray of processes with respect to burst time 
void sort_by_burst_time(struct Process processes[], int num_processes) {
    //buble sort
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = 0; j < num_processes - i - 1; j++) {
            if (processes[j].burst_time > processes[j + 1].burst_time) {
                // Swap processes[j] and processes[j + 1]
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

//taking inputs from user
void input_from_user(struct Process processes[], int num_processes) {
    printf("\nEnter the arrival and burst times of the processes:\n\n");

    for (int i = 0; i < num_processes; i++) {
        processes[i].pid = i + 1;
        printf("Enter arrival time for process P%d: ", i + 1);
        scanf("%d", &processes[i].arrival_time);

        //as per question arival time should be greater than 0 
        //this is checked and if its 0 or less user have to re-enter the value 
        if (processes[i].arrival_time < 0) {
            printf("Error: Arrival time cannot be negative or zero for process P%d. Discarding the process.\n", i + 1);
            i--;  //will go back
            continue;
        }


        //burst time is takin from user until its provided greater than 0
        do {
            printf("Enter burst time for process P%d: ", i + 1);
            scanf("%d", &processes[i].burst_time);

            if (processes[i].burst_time <= 0) {
                printf("Error: Burst time must be greater than 0 for process P%d. Please re-enter.\n", i + 1);
            }
        } while (processes[i].burst_time <= 0);

        //innitially completion time of all the processes is set to -1 
        processes[i].completion_time = -1;
    }
}

//execute function is used to calculate and set the (completion_time,TAT,waiting_time) of a process
void execute(struct Process* p) {

    p->completion_time = Current_Time + p->burst_time;
    //current time is changed
    Current_Time = p->completion_time;
    p->TAT = p->completion_time - p->arrival_time;
    p->waiting_time = p->TAT - p->burst_time;
}

//all the searching sorting and resceduling is done here
void run_sceduler(struct Process processes[], struct Process waiting_queue[], int num_processes) {

    int waiting_queue_size = 0; //size of the wating queue

    for (int i = 0; i < num_processes; i++) {
        if (processes[i].completion_time == -1 && processes[i].arrival_time <= Current_Time) {
            // processes that are not completed and they came before the current time are inserted in waiting queue
            waiting_queue[waiting_queue_size++] = processes[i];
        }
    }

    if (waiting_queue_size == 0) {
        //if all the processes are completed function is exited
        return;
    }


    //waiting queue is sorted by burst time for executing SJF
    sort_by_burst_time(waiting_queue, waiting_queue_size);

    //according to the question checking will take 2 time units so 2 is added to current time after every process is completed
    Current_Time = Current_Time + 2;



    //index of the process in process queue is searched using pid from waiting queue
    int index_in_processes;
    for(int i = 0;i<num_processes;i++){
        if(processes[i].pid == waiting_queue[0].pid){
            index_in_processes = i;
            break;
        }
    }

    //the chosen process is executed 
    execute(&processes[index_in_processes]);

    //recurcively sceduler is called after updating the value for executed process unill all the processes are completed and waiting queue is empty
    run_sceduler(processes,waiting_queue,num_processes);
}

//after all the execution every result is stored in processes array that will be displayed in this function
void print_result(struct Process processes[], int num_processes) {
    printf("\nRESULT TABLE\n");
    printf("............\n\n");
    printf("Process\t\tArrival Time\tBurst Time\tCompletion Time\t\tTurnaround Time\t\tWaiting Time\n\n");

    float total_waiting_time = 0; //for calculating average waiting time
    float total_turnaround_time = 0; //for calculating average turn around time

    //for calculating total time taken by cpu
    int max_completion_time = INT_MIN;
    int min_arival_time = INT_MAX;

    for (int i = 0; i < num_processes; i++) {
        total_turnaround_time += processes[i].TAT;
        total_waiting_time += processes[i].waiting_time;

        if(processes[i].completion_time >= max_completion_time){
            max_completion_time = processes[i].completion_time;
        }

        if(processes[i].arrival_time <= min_arival_time){
            min_arival_time = processes[i].arrival_time;
        }

        printf("P%d\t\t%d\t\t%d\t\t%d\t\t\t%d\t\t\t%d\n", processes[i].pid, processes[i].arrival_time, processes[i].burst_time, processes[i].completion_time, processes[i].TAT, processes[i].waiting_time);
    }

    float average_waiting_time = total_waiting_time / num_processes;
    float average_turnaround_time = total_turnaround_time / num_processes;
    int time_taken_by_cpu = max_completion_time-min_arival_time;

    printf("\n\n\nTotal time taken by CPU: %d\n",time_taken_by_cpu);
    printf("Average Turnaround Time: %.2f\n", average_turnaround_time);
    printf("Average Waiting Time: %.2f\n", average_waiting_time);
}



// Main Function
int main() {
    printf("\nSJF SCHEDULER\n");
    printf("______________\n\n");

    //number of processes are inserted
    int num_processes;
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    //will exit if number of processes are 0
    if (num_processes == 0) {
        printf("\nExiting the program ......\n\n");
        return 0;
    }

    printf("\n\n");


    //process queue
    struct Process processes[num_processes];

    //taking the inputs from user
    input_from_user(processes, num_processes);

    //sorting by arival time
    sort_by_arival_time(processes, num_processes);

    //waiting queue
    struct Process waiting_queue[num_processes];

    //setting current time to arival time of first came process
    Current_Time = processes[0].arrival_time;

    //executing the first process
    execute(&processes[0]);

    //executing all other processes
    run_sceduler(processes,waiting_queue,num_processes);

    //printing results for user
    print_result(processes, num_processes);

    return 0;
}
