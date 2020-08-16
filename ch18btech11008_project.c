/*
 * project2.c
 * 
 * Copyright 2020 Vinay <vinay@vinay-ubuntu>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
#define MAX_VERTICES 10000
#define MAX_EDGES 3000
struct node //edges list of each vertex and maintaining the lists of infected,recovered,susceptible
{
	int identity;
	struct node *link;
};
struct individual //population individual
{
	char status;
	int predectedTime;
};
struct verticelist	//where we store the heads of linked list of edges of a vertex
{
	struct node *ptr;
};
struct queue	//priority queue event
{
	char action;
	int time;
	int person;	//identity
	struct queue *next;	//next link
};
typedef struct individual individual;
typedef struct queue queue;
typedef struct node node;
typedef struct verticelist verticelist;
typedef unsigned char vertex;
verticelist array[MAX_VERTICES]; //has the connected vertices to a specific node in a linked list
node nodeList[MAX_VERTICES]; //for checking status while placing in queue
individual population[MAX_VERTICES]; //the actual population each element with index i is the ith person
queue *pqhead=NULL; //priority head queue
node *infectedlist=NULL;	//head of infectedlist
node *recoveredlist=NULL;	//head of recovered list
node *susceptiblelist=NULL;	//head of susceptible list
int infCount=0;	//infected people count
int recCount=0;	//recovered people count
int susCount=10000;	//susceptible count
void insertlist(node **head,int identity){	//insert function for maintaining the lists of infected,recovered,susceptible
	node *temp=NULL;
	temp=(node *)malloc(sizeof(struct node));
	temp->identity=identity;
	temp->link=(*head);
	(*head)=temp;
}
void deletelist(node **head,int key){	//delete function for maintaining the lists of infected,recovered,susceptible
    // Store head node
    struct node* temp = *head, *prev; 
  
    // If head node itself holds the key to be deleted 
    if (temp != NULL && temp->identity == key) 
    { 
        *head = temp->link;   // Changed head 
        free(temp);               // free old head 
        return; 
    } 
  
    // Search for the key to be deleted, keep track of the 
    // previous node as we need to change 'prev->next' 
    while (temp != NULL && temp->identity != key) 
    { 
        prev = temp; 
        temp = temp->link; 
    } 
  
    // If key was not present in linked list 
    if (temp == NULL) return; 
  
    // Unlink the node from linked list 
    prev->link = temp->link; 
  
    free(temp);  // Free memory 
} 			
int min(int a,int b,int c) //returns the miniumum of three numbers
{
	int min=a;
	if(min>b){
		min=b;
	}
	if(min>c){
		min=c;
	}
	return min;
}
void creategraph(void)	//function for creating graph
{
	srand(time(0));
	int numberOfVertices = MAX_VERTICES;	/*number of nodes in a graph*/
    int vertexCounter = 0; //count for max vertices
    int edgeCounter = 0; //count for max edges
    for (vertexCounter = 0; vertexCounter < numberOfVertices; vertexCounter++){	/*start linking the graph. All vetrices need not have same number of links*/
        node *head=NULL; //head ptr for vertex linked list
        node *temp=NULL; //temp for traversal through linked list
        int maxNumberOfEdges =rand()%MAX_EDGES+1;	//random as the each vertex need not contain same number of edges
        for (edgeCounter=0; edgeCounter < maxNumberOfEdges; edgeCounter++){
			node *edge=NULL; //edge
			edge=(node *)malloc(sizeof(struct node)); 
			int linkedVertex = rand() % numberOfVertices+1; //creating a edge between the current vertex and random vertex
			edge->identity=linkedVertex; //storing identity of vertex
			edge->link=NULL;
			if(vertexCounter!=linkedVertex){
				if(temp==NULL){ //adding it to linked list
					temp=edge;
					head=edge;
				}
				else{
					temp->link=edge;
					temp=temp->link;
				}
			}
        }
        array[vertexCounter].ptr=head; //stores the of each linked list 
        population[vertexCounter].status='S';	//intiated all of the nodes to susceptible
        insertlist(&susceptiblelist,vertexCounter+1);	//adding them to the susceptible list
        population[vertexCounter].predectedTime=301;	//intializing it to infinity as we dont know when its gonna infected
    }
}
void createintialinfected(void){	//creating intial infected events
	queue *traverse;
	int num;
	int identity;
	printf("enter the number of intial infected nodes:\n");
	scanf("%d",&num);
	printf("enter the identity of nodes:\n");
	for(int i=0;i<num;i++){
		scanf("%d",&identity);
		queue *event;	//new event
		event=(queue *)malloc(sizeof(struct queue));
		event->action='T';
		population[identity-1].predectedTime=0;
		event->time=0;
		event->person=identity;
		event->next=NULL;
		if(pqhead==NULL){	//adding it in priority queue
			traverse=event;
			pqhead=event;
		}
		else{
			traverse->next=event;
			traverse=traverse->next;
		}
	}
}
queue * createRecoveryEvent(int person,int time){	//recovery event
	queue *event;
	event=(queue *)malloc(sizeof(struct queue));
	event->action='R';
	event->time=time;
	event->person=person;
	event->next=NULL;
	return event;
}
queue *createTransmitEvent(int person,int time){	//transmitevent
	queue *event;
	event=(queue *)malloc(sizeof(struct queue));
	event->action='T';
	event->time=time;
	event->person=person;
	event->next=NULL;
	return event;
}
void addToPriorityQueue(queue *event){	//adds the element to the priority queue
	queue *pqtraverse=pqhead;
	queue *prev=NULL;
	if(pqtraverse==NULL){	//if empty
		pqhead=event;
	}
	else{
		//finds the position to be added
		while( pqtraverse!=NULL&&(pqtraverse->time<event->time) ){ //traversing till the time is greater than current event
			prev=pqtraverse;
			pqtraverse=pqtraverse->next;
		}
		//adds the new event
		if(prev!=NULL&&pqtraverse==NULL){	//adding at the tail
			prev->next=event;
		}
		else{	
			if(pqhead->time>event->time){	//adding at the front
				event->next=pqhead;
				pqhead=event;
			}
			else{
				if(prev!=NULL){	//adding between two nodes
					prev->next=event;
					event->next=pqtraverse;
				}
			}
		}
	}
}
void process_rec_SIR(int person){	//recovery event
	population[person-1].status='R';
	insertlist(&recoveredlist,person);	//adding element in recovery list
	deletelist(&infectedlist,person);	//deleting it from the infected list
	recCount++;	//increasing recovery count
	infCount--;	//decreasing infected count
}
void find_trans_Sir(int source,int daysCompleted,int sourceRecTime,int gamma){	//infection from source to neighbourhood
	int tmax=300;
	node *neighbour=array[source].ptr;	//assigning headptr of edges linked list of source	
	while(neighbour!=NULL){	//till it reaches end of linked list
		if(population[neighbour->identity-1].status=='S'){	//if neighbour is susceptible
			int inftime=1;
			int k=rand()%10+1;	//probability of 0.5
			while(k>gamma){
				inftime++;
				k=rand()%10+1;
			}
			/*if the infectiontime+dayscompleted is lesstmax,rec time,existing duplicate time*/
			if(daysCompleted+inftime<min(tmax,daysCompleted+sourceRecTime,population[neighbour->identity-1].predectedTime)){
				queue *event=createTransmitEvent(neighbour->identity,daysCompleted+inftime);
				population[neighbour->identity-1].predectedTime=daysCompleted+inftime;
				addToPriorityQueue(event);	//add event to queue
				//printf("%d	",inftime);
			}
		}
		neighbour=neighbour->link;
	}
}
void process_trans_SIR(int person,int tow,int gamma,int daysCompleted){	//if the event is transmission
	int n;
	int recTime=1;
	population[person-1].status='I';	//changing the status of person
	insertlist(&infectedlist,person);	//inserting element in infected list
	deletelist(&susceptiblelist,person);	//deleting element in susceptible list
	infCount++;	//increasing infeceted count
	susCount--;	//decreasing the recovery count
	n=rand()%10+1;
	while(n>tow){	//probability of 0.2 tossing coin n>2 means tail and viceversa
		recTime++;
		n=rand()%10+1;
	}
	if(daysCompleted+recTime<=300){	//if the dayscompleted(which is the time of event)+recovery time is less than tmax
		queue *event=createRecoveryEvent(person,daysCompleted+recTime);	//creates a recoverey event
		addToPriorityQueue(event);	//add it to priority queue
	}
	find_trans_Sir(person,daysCompleted,recTime,gamma);	//for the neighbours
}
int main(){
	FILE *infout;	//for gnuplot graph txt file with time and infected count
	FILE *recout;	//for gnuplot graph txt file with time and recovered count
	infout=fopen("infectedandtime","w");
	recout=fopen("recoveredandtime","w");
	creategraph(); //calling create graph function
	srand(time(0));
	int time=-1;	//for printing the daywise information
    int tow=2;	//recovery probability 2 out of 10
    int gamma=5;	//infection probability 5 out of 10
	createintialinfected();	//intial infected nodes
	while(pqhead!=NULL){	//if priority queue is not empty
		queue *traverse=pqhead;	//traverse along priority queue
		if(traverse->action=='T'){	//if early event in priority action is transmit
			if(population[traverse->person-1].status=='S'){	//if the node status is susceptible
				fprintf(infout,"%d	%d\n",infCount,traverse->time);	//printing out the time and infected count in the file
				process_trans_SIR(traverse->person,tow,gamma,traverse->time); //processing transmission function
			}	
		}
		else if(traverse->action=='R'){	 //if action is recovery
			fprintf(recout,"%d	%d\n",recCount,traverse->time);	//printing out the time and recovery count in the file
			process_rec_SIR(traverse->person);	//recovery function
		}
		/*if(pqhead->next==NULL)
		{
			printf("%d %d %c ",pqhead->time,pqhead->person,pqhead->action);
		}*/
		if(time!=traverse->time){	//printing information day wise
			time=traverse->time;
			printf("DAY%d	I-%d	R-%d	S-%d\n",time,infCount,recCount,susCount);
		}
		pqhead=pqhead->next; //dequeueing the event
	}
    return 0;
}
/*additional*/
//respected sir you can remove the multiline comment to see the respective outputs
/*void additionaloutput(void){
	for(int i=0;i<MAX_VERTICES;i++)	//printing the population and their status
	{
		printf("%d:%c	,",i+1,population[i].status);
	}
	while(susceptiblelist!=NULL){	//prints susceptible list
		printf("%ds ,",susceptiblelist->identity);
		susceptiblelist=susceptiblelist->link;
	}
	while(infectedlist!=NULL){	//prints infected list
		printf("%di ,",infectedlist->identity);
		infectedlist=infectedlist->link;
	}
	while(recoveredlist!=NULL){	//prints recovered list
		printf("%dr ,",recoveredlist->identity);
		recoveredlist=recoveredlist->link;
	}
}*/



