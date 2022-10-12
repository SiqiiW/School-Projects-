// Author: Siqi Wang
// Can't run processes containing entries over 10
// must compile with: gcc  -std=c99 -Wall -o scheduling scheduling.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int total = 0;

// A class for processes
struct process
{
    int name;
    int CPU;
    int remainingCPU;
    int blockTime;
    int arrive;
    int blockOver;
    int currentRunOver;
    int finishTime;  
    // state: 0 not arrived; 1 ready to go; 2 running; 3 blocked
    int state;  
};

// Process initializer
struct process* newProcess(int a, int b, int c, int d)
{
    struct process* temp = (struct process*)malloc(sizeof(struct process));
    temp->name = a;
    temp->CPU = b;
    temp->remainingCPU=b;
    temp->blockTime = c;
    temp->arrive = d;
    // #cycle when i/o done and process ready again
    temp->blockOver=-1;
    // #cycle when needs to be removed from running place
    temp->currentRunOver=-1;
    // finish time for process
    temp->finishTime=-1;
    // state: 0 not arrived; 1 ready to go; 2 running; 3 blocked
    temp->state=0; //not ready
    
    return temp;
}
  
// Node in linked list
struct QNode {
    struct process* key;
    struct QNode* next;
};
  
// The queue
struct Queue {
    struct QNode *head, *tail;
};

// Create a new node in queue
struct QNode* newNode(struct process* p)
{
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->key = p;
    temp->next = NULL;
    return temp;
}
  
// Create an empty queue
struct Queue* createQueue()
{
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->head = q->tail = NULL;
    return q;
}
  
// Add a node to the back of the queue
void enQueue(struct Queue* q, struct process* p)
{
    struct QNode* temp = newNode(p);
  
    // If queue is empty, the new node is both head and tail
    if (q->tail == NULL) {
        q->head = q->tail = temp;
        return;
    }

    // Else, add the new node at the end of queue and update tail
    q->tail->next = temp;
    q->tail = temp;
}
  
// Remove a node from the front of the queue
void deQueue(struct Queue* q)
{
    // If queue is empty, nothing to dequeue
    if (q->head == NULL)
        return;
  
    // else
    // get rid of the previous head
    struct QNode* temp = q->head;
    q->head = q->head->next;
  
    // If queue becomes empty after dequeue, update tail
    if (q->head == NULL)
        q->tail = NULL;
  
    free(temp);
}

// function to swap the the position of two elements in a queue 
// (used in arrivalSort())
void swap(struct process *a, struct process *b) {
  struct process temp = *a;
  *a = *b;
  *b = temp;
}

// Sort based on name
void nameSort(struct Queue* q,int total) {

   // sort in arrival time and name in array
    struct QNode* tmp = q->head;
    struct process *array[total];
    for (int i=0;i<total;i++){
        array[i]=tmp->key;
        tmp=tmp->next;
    }
    int size = sizeof(array) / sizeof(array[0]);
    for (int step = 0; step < size - 1; step++) {
        int minIndex = step;
        for (int i = step + 1; i < size; i++) {
            if (array[i]->name < array[minIndex]->name){
                minIndex = i;
            }
        }
        // put min at the correct position
        swap(array[minIndex], array[step]);
    }
    
    //put it back in queue
    tmp = q->head;
    for (int i=0;i<total;i++){
        tmp->key=array[i];
        tmp=tmp->next;
    }
}

// Read the file and put data in a list sorted by name (implemented by a queue)
struct Queue* readfile(char filename[]){
    
    struct Queue* q = createQueue();

    // read a file
    FILE * fp;
    
    fp  = fopen (filename, "r");
    if( !(fp = fopen(filename,"r"))){
      printf("Cannot open file %s\n", filename);
      exit(1);
    }

    const size_t line_size = 300;
    char* line = malloc(line_size);

    // get rid of the first line (#processes)
    fgets(line, line_size, fp);
    total = atoi(line);

    // put numbers in place (struct process)
    while (fgets(line, line_size, fp) != NULL)  {
        
        int length=strlen(line);
        char *pszChar = line;

        // tmp array for declaring process
        int a [4];
        int count = 0;


        // put line data in an array
        for (int i=0; i<length;i++){
            if (isspace(*pszChar)==0) //if not space
            {
                a[count]=atoi(pszChar);
                count++;
            }
            *pszChar++;
        }

        // put array in to process
        if(a[1]%2 != 0){ // if odd, make it even!
            a[1]++;
        }
        struct process* p = newProcess(a[0],a[1],a[2],a[3]);
        
        // add it in queue (WRONG ORDER NOW...)
        enQueue(q,p);
        
    }
    free(line); // read done

    nameSort(q,total);
    return q;
}

// FCFS
void FCFS(struct Queue* q, char *outputname){
    //ready queue
    struct Queue* ready = createQueue();
    struct process* running = NULL;    

    struct QNode* tmp = q->head;

    
    int count = 0; // counter for cycles
    int completed=0; //completed processes
    int runningthings=0; //utility

    FILE *fp2;
    fp2  = fopen (outputname, "w");


    // when some processes are not done yet
    while(completed<total){
        
        
        if(completed==total-1){
            if(running!=NULL){
                if(running->currentRunOver==count &&   running->remainingCPU==0){
                    running->finishTime=count;
                    break;
                }
            }
        }

        fprintf(fp2,"%d ",count);
        // check each process's times and see if counter matches up with any
        tmp = q->head;
        
        for(int i = 0; i< total;i++){


            // check if something is running, and update 
            if(running!=NULL){
                if (running->currentRunOver==count){
                    // time to block
                    if((running->remainingCPU == (running->CPU/2))){
                        running->state=3; //blocked
                        running->blockOver=count+running->blockTime;                    
                    }
                    else if (running->remainingCPU==0){
                        running->state=4;  //finished
                        running->finishTime=count;
                        completed++;
                    }
                    running=NULL;
                }
            }

            //if it arrives
            if(tmp->key->arrive==count){
                tmp->key->state=1; //ready to go
                enQueue(ready,tmp->key); //add to ready queue
            }

            // if i/o is done->back to ready queue
            if ( tmp->key->blockOver==count){
                tmp->key->state=1; //ready
                enQueue(ready,tmp->key);
            }

            // if ready && first in queue && CPU empty -> run!
            if(tmp->key->state==1 && tmp->key->name==ready->head->key->name && running==NULL){

                running=tmp->key; //occupied
                tmp->key->state=2; //running
                tmp->key->remainingCPU = tmp->key->remainingCPU - tmp->key->CPU/2; //decrement remaining CPU time
                tmp->key->currentRunOver=count+tmp->key->CPU/2;

                deQueue(ready);
            }

            
            //printing 
            //Not ready or done
            if(tmp->key->state!=0 && tmp->key->state!=4){
            
                fprintf(fp2,"%d:",tmp->key->name);
                if(tmp->key->state==1){
                    fprintf(fp2,"ready ");
                }
                else if(tmp->key->state==2){
                    runningthings++;
                    fprintf(fp2,"running ");
                }
                else if(tmp->key->state==3){
                    fprintf(fp2,"blocked ");
                }
            }
            
            tmp=tmp->next;
        }
        count++;
        fprintf(fp2,"\n");
        
    }

    fprintf(fp2,"\nFinishing time: %d\n",count-1);
    fprintf(fp2,"CPU utilization: %.2f\n", (float)runningthings/(count));
    tmp = q->head;
    while(tmp!=NULL){
        fprintf(fp2,"Turnaround process %d: %d\n",tmp->key->name,(tmp->key->finishTime-tmp->key->arrive));
        tmp=tmp->next;
    }
    return ;
}

// RR
void RR(struct Queue* q, char *outputname){
    //ready queue
    struct Queue* ready = createQueue();
    struct process* running = NULL;    

    struct QNode* tmp = q->head;

    
    int count = 0; // counter for cycles
    int completed=0; //completed processes
    int runningthings=0; //utility

    struct process* temporaryRunning = NULL;

    FILE *fp2;
    fp2  = fopen (outputname, "w");


    // when some processes are not done yet
    while(completed<total){
        

        
        if(completed==total-1){
            if(running!=NULL){
                if(running->currentRunOver==count &&   running->remainingCPU==0){
                    running->finishTime=count;
                    break;
                }
            }
        }

        fprintf(fp2,"%d ",count);
        // check each process's times and see if counter matches up with any
        tmp = q->head;
        
        for(int i = 0; i< total;i++){

            // check if something is running, and update 
            if(running!=NULL){
                if (running->currentRunOver==count){
                    // time to block
                    if((running->remainingCPU == (running->CPU/2))){
                        running->state=3; //blocked
                        running->blockOver=count+running->blockTime;  
                        running=NULL;                  
                    }
                    // finished
                    else if (running->remainingCPU==0){
                        running->state=4;  //finished
                        running->finishTime=count;
                        completed++;
                        running=NULL;
                    }
                    else{
                        if(running->name <= tmp->key->name){ //if this one has larger id, it can't go first.
                            running->state=1;
                            enQueue(ready,running);
                            running=NULL;
                        }
                        else{
                            temporaryRunning=running;
                            running=NULL;
                        }
                    }
                    
                }
            }

            if(temporaryRunning!=NULL){
                if(temporaryRunning->name==tmp->key->name){
                    tmp->key->state=1;
                    enQueue(ready,tmp->key);
                    temporaryRunning=NULL;
                }
            }
            

             

            //if it arrives
            if(tmp->key->arrive==count){
                tmp->key->state=1; //ready to go
                enQueue(ready,tmp->key); //add to ready queue
            }

            // if i/o is done->back to ready queue
            if ( tmp->key->blockOver==count){
                tmp->key->state=1; //ready
                enQueue(ready,tmp->key);
            }

            // if ready && first in queue && CPU empty -> run!
            if(tmp->key->state==1 && tmp->key->name==ready->head->key->name && running==NULL){

                running=tmp->key; //occupied
                tmp->key->state=2; //running

                int min=0;
                // if process is still in the first half of cpu time
                if(tmp->key->remainingCPU>tmp->key->CPU/2){
                    if(tmp->key->remainingCPU - tmp->key->CPU/2 < 2){
                        min = tmp->key->remainingCPU - tmp->key->CPU/2 ;
                    }
                    else{
                        min = 2;
                    }
                }
                else{
                    if(tmp->key->remainingCPU < 2){
                        min = tmp->key->remainingCPU;
                    }
                    else{
                        min = 2;
                    }
                }
                tmp->key->remainingCPU = tmp->key->remainingCPU - min; //decrement remaining CPU time
                tmp->key->currentRunOver=count+min;

                deQueue(ready);
            }

            
            //printing 
            //Not ready or done
            if(tmp->key->state!=0 && tmp->key->state!=4){
            
                fprintf(fp2,"%d:",tmp->key->name);
                if(tmp->key->state==1){
                    fprintf(fp2,"ready ");
                }
                else if(tmp->key->state==2){
                    runningthings++;
                    fprintf(fp2,"running ");
                }
                else if(tmp->key->state==3){
                    fprintf(fp2,"blocked ");
                }
            }
            
            tmp=tmp->next;
        }
        count++;
        fprintf(fp2,"\n");
        
    }

    fprintf(fp2,"\nFinishing time: %d\n",count-1);
    fprintf(fp2,"CPU utilization: %.2f\n", (float)runningthings/(count));
    tmp = q->head;
    while(tmp!=NULL){
        fprintf(fp2,"Turnaround process %d: %d\n",tmp->key->name,(tmp->key->finishTime-tmp->key->arrive));
        tmp=tmp->next;
    }
    return ;
}


// return the shortest remaining job in queue
// (used in SRJF)
struct process* remain (struct Queue* q){
    struct process* p = newProcess(500,500,500,500);
    struct QNode* tmp = q->head;
    while (tmp!=NULL)
    {
        if(tmp->key->remainingCPU < p->remainingCPU){
            p=tmp->key;
        }

        else if(tmp->key->remainingCPU==p->remainingCPU){
            if(tmp->key->name<p->name){
                p=tmp->key;
            }
        }

        tmp=tmp->next;
    }
    return p;
}

// Shortest remaining first
void SRJF(struct Queue* q, char *outputname){

    //ready queue
    struct Queue* ready = createQueue();
    struct process* running = NULL;    

    struct QNode* tmp = q->head;

    int count = 0; // counter for cycles
    int completed=0; //completed processes
    int runningthings=0; //utility
    struct Queue* temp=createQueue();
    struct process* shortest = newProcess(600,600,600,600);
    
    FILE *fp2;
    fp2  = fopen (outputname, "w");

   // when some processes are not done yet
    while(completed<total){

        
        if(completed==total-1){
            if(running!=NULL){
                if(running->remainingCPU==0){
                    running->finishTime=count;
                    break;
                }
            }
        }

        fprintf(fp2,"%d ",count);
        // check each process's times and see if counter matches up with any
        tmp = q->head;
        

        // if something from last cycle occupies CPU
        if(running!=NULL){
            // time to block
            if((running->remainingCPU == (running->CPU/2))){
                    running->state=3; //blocked
                    running->blockOver=count+running->blockTime;
            }
            else if (running->remainingCPU==0){
                    running->state=4;  //finished
                    running->finishTime=count;
                    completed++;
            }
            else{
                    running->state=1;
                    enQueue(ready,running);
            }
            running=NULL;
        }


        ready=createQueue();
        tmp=q->head;
        for(int i = 0; i< total;i++){
            if(tmp->key->state==1){
                enQueue(ready,tmp->key);
            }
            tmp=tmp->next;
        }

        tmp=q->head;
        for(int i = 0; i<total;i++){

            // check if something is running, and update 

            //if it arrives
            if(tmp->key->arrive==count){
                tmp->key->state=1; //ready to go

                
                enQueue(ready,tmp->key); //add to ready queue
            }

            // if i/o is done->back to ready queue
            if (tmp->key->blockOver==count){
                tmp->key->state=1; //ready
                enQueue(ready,tmp->key);
            }

            if(running!=NULL)    {
                if(running->remainingCPU>0){
                    shortest=running;
                }
            }
            /*if (shortest->remainingCPU > remain(ready)->remainingCPU){
                shortest=remain(ready);
                printf("%d is now the SHORTEST.",shortest->name);
            }

            // if ready && shortest && CPU empty -> run!
            if(tmp->key->name == shortest->name){
                printf("?!%d is same as the shortest.\n",tmp->key->name);
            }*/
            if(tmp->key->state==1 && tmp->key->name==remain(ready)->name){

                running=tmp->key; //occupied
                tmp->key->state=2; //running
                tmp->key->remainingCPU = tmp->key->remainingCPU-1; //decrement remaining CPU time
            }

            
            //printing 
            //Not ready or done            
            if(tmp->key->state!=0 && tmp->key->state!=4){
                fprintf(fp2,"%d:",tmp->key->name);
                if(tmp->key->state==1){
                    fprintf(fp2,"ready ");
                }
                else if(tmp->key->state==2){
                    runningthings++;
                    fprintf(fp2,"running ");
                }
                else if(tmp->key->state==3){
                    fprintf(fp2,"blocked ");
                }
            }
            
            tmp=tmp->next;
       }

        count++;
        fprintf(fp2,"\n");
    }

    fprintf(fp2,"\nFinishing time: %d\n",count-1);
    fprintf(fp2,"CPU utilization: %.2f\n", (float)runningthings/(count));
    tmp = q->head;
    while(tmp!=NULL){
        fprintf(fp2,"Turnaround process %d: %d\n",tmp->key->name,(tmp->key->finishTime-tmp->key->arrive));
        tmp=tmp->next;
    }

    return;
}


//Test
int main(int argc, char *argv[])
{
    
    int scheduling;
    
    // Check that the command line is correct
    if(argc != 3){
      printf("usage:  ./scheduling alg input\n");
      printf("alg: the scheduling algorithm: 0, 1, or 2\n");
      printf("input: the processes inut file\n");
      exit(1);
    }  
    
    scheduling = (int)atoi(argv[1]); // the scheduling algorithm

    struct Queue* q= readfile(argv[2]); 

    char outputname[100]="";
    
    if(scheduling==0){
        sprintf(outputname,"0-%s",argv[2]);
        FCFS(q,outputname);
    }
    else if(scheduling==1){
        sprintf(outputname,"1-%s",argv[2]);
        RR(q,outputname);
    }
    else if(scheduling==2){
        sprintf(outputname,"2-%s",argv[2]);
        SRJF(q,outputname);
    }

    return 0;   
}