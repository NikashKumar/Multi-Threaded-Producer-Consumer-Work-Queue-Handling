/***********************************************************************************************************************************************
  This program mainly creates two threads, where one thread is to handle all the producer which adds strings to the queue data structure, where as the second thread is for the consumer thread, which dequeues ech thread from the queue,prints the string and its length to the console.The problem of race condition is also being avoided by the usage of Mutex locks and condition variables.		

***********************************************************************************************************************************************/

//including all the necessary libraries
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//This structure is used to build our queue using a linked list
struct mynode
{
	char string[50];
	struct mynode *link;
};

struct mynode *start;//This is the head pointer to our queue

int first =0;
int last =0;


/*This function is used by the consumer to dequeue each thread from the queue,print the string as well as its lenth to stdout */
void retrieve()
{
	struct mynode *three,*four;
	three = start;
	if(three!=NULL)
	{
		printf("\nThe string entered is = %s and its length = %zu\n",three->string,strlen(three->string));
		three=three->link;
		free(start);
		start = three;
		first++;
	}
	else
		printf("\nqueue is already empty");
}



/*This function is used by the producer to add each string entered by the user to the queue */
void tailfunc()
{
	char *dummy = NULL;
	struct mynode *one,*two;
	one = start;
	int structsize;	
	structsize = sizeof(struct mynode);
	if(one  == NULL)
	{
		one = malloc(structsize);
		one->link = NULL;
		printf("Enter the producer string : ");
		gets(one->string);
		last++;
		start  = one;
	}
	else
	{
		while(one->link!=NULL)
			one = one->link;
		two = malloc(structsize);
		one->link = two;
		two->link = NULL;
		printf("Enter the Next string : ");
		gets(two->string);
		last++;
	}
}


void display() //This function is used to display our queue from first to last
{
	int i=0;
	struct mynode *print;
	for(print=start;print!=NULL;print=print->link)
	{
		printf("values = %s\n",print->string);
	}
}


/* This is the consumer thread which locks the retrieve function so that the producer cannot interrupt while we are retrieving the strings from 
   the queue */

void *consumer(void *argv)
{
	printf("Consumer : Hello I am consumer #%ld. Ready to consume Data\n", pthread_self());
	while(1)
	{
		pthread_mutex_lock(&mutex);
		printf("Consumer...\n");

		while(last == first)
		{
			last=0;first=0;
			pthread_cond_wait(&cond,&mutex);
		}
		retrieve();
		sleep(1);
		pthread_mutex_unlock(&mutex);

	}
}

/*This is the producer thread which locks the tailfunc() so that the consumer cannot interrupt while user is adding strings to the queue   */

void *producer(void *argv)
{
	printf("Producer : Hello I am producer #%ld. Ready to produce data\n", pthread_self());
	while(1)
	{
		if(last < 5)
		{
			pthread_mutex_lock(&mutex);
			printf("Producer...\n");

			tailfunc();
			pthread_cond_signal(&cond);
			sleep(1);
			pthread_mutex_unlock(&mutex);
		}
	}	
}



//start of the main funtion
int main()
{
	pthread_t thread[2];
	pthread_attr_t attributes;
	int tid1,tid2;
	
	printf("\nMain thread started\n");
	pthread_attr_init(&attributes);
	if((tid1 = pthread_create(&thread[0],&attributes,producer,NULL)))
	{
		printf("\nError in the producer thread\n");
		printf("\n");
	}

	if((tid2 = pthread_create(&thread[1],&attributes,consumer,NULL)))
	{
		printf("\nERror in the consumer thread\n");
	}

	pthread_join(thread[0],NULL);
	pthread_join(thread[1],NULL);
	printf("\n\t******************* DONE *********************\n");
	return 0 ;
}


