
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define SIZE 1024
#define THREADS_NUMBER 3

#define NETWORK 1
#define STORAGE 2

// Nodes
struct node {
  char* json[SIZE];
  struct node* next;
};
typedef struct node* Node;

// Queue
struct queue {
  Node head;
  Node tail;
  int length;
};
typedef struct queue* Queue;

// Threads
void* producer();
void* consumer();
void* manager();

// Functions
Queue InitQueue();
int produce(Queue queue, char *json[]);
int send(Queue queue, int location);
int inventory();

// Globals
bool mqttConnection = true;
pthread_mutex_t lock;
Queue buffer;
int stored = 0;
int sent = 0;

char file[SIZE] = "{ 'name': 'Default JSON template', 'sections': { 'main': { 'type': 'text' } } }";

int main (int argc, char *argv[]) {
  
  buffer = InitQueue();

  pthread_mutex_init(&lock, NULL);
  
  pthread_t thread[THREADS_NUMBER];

  if ( pthread_create(&thread[0], NULL, &producer, NULL) != 0)
	return EXIT_FAILURE;

  if ( pthread_create(&thread[1], NULL, &consumer, NULL) != 0)
	return EXIT_FAILURE;

  if ( pthread_create(&thread[2], NULL, &manager, NULL) != 0)
	return EXIT_FAILURE;

  for (int cycle = 0; cycle < THREADS_NUMBER; cycle++) {
	if ( pthread_join(thread[cycle], NULL) != 0)
	  return EXIT_FAILURE;
  }

  pthread_mutex_destroy(&lock);

  return EXIT_SUCCESS;

}

Queue InitQueue() {
  Queue queue = (Queue)malloc(sizeof(struct queue));
  queue->head = NULL;
  queue->tail = NULL;
  queue->length = 0;
  return queue;
}

int produce(Queue queue, char *json[]) {

  Node temp_node = (Node)malloc(sizeof(struct node));

  if (temp_node == NULL) {
  	return -1;
  }

  strncpy(temp_node->json, json, SIZE);

  temp_node->next = NULL;

  if (queue->tail == NULL || queue->head == NULL) {
  	queue->head = temp_node;
  	queue->tail = temp_node;
  } else {
  	queue->tail->next = temp_node;
  	queue->tail = temp_node;
  }

  queue->length += 1; // Increase Size
  return 0;
}

int send(Queue queue, int location) {

  Node sent = queue->head; // Node to be modified
  
  if(sent == NULL || queue == NULL)
  	return -1;

  // Send
  if (location == NETWORK) {
	printf("Publish sent->json\n");
  }
  
  else if (location == STORAGE) {
	FILE *writer;
	int jointSize = (int)("json.txt" + stored);
	char fileName[jointSize];
	
	sprintf(fileName, "json%d.txt", stored);
	
    writer = fopen(fileName, "w");
	if (writer == NULL)
	  return -2;
	
    fputs(sent->json, writer);
    fclose(writer);
	
	stored += 1;
  }

  queue->head = queue->head->next;

  free(sent); // Frees allocated Node memory
  queue->length -= 1; // Decrease Size

  if(queue->head == NULL)
	queue->tail = NULL;

  return 0;
}

int inventory() {
  
  FILE *reader;
  char fileName[10];
  char json[SIZE];
  
  sprintf(fileName, "json%d.txt", sent);
  
  reader = fopen(fileName, "r");

  if (reader != NULL) {
	fgets(json, SIZE, reader);
	fclose(reader);
  
	printf("Publish sent->json\n");
	remove(fileName);
  }

  sent += 1;
  
  if (sent == stored) {
	stored = 0;
	sent = 0;
  }
  
  return 0;
}

void* producer() {
  while (true) {

	sleep(1);
	printf("Producer Get JSON\n");

	pthread_mutex_lock(&lock);

	if (mqttConnection == true) {

	  printf("Producer Produce\n");
	  
	  produce(buffer, file);

	}
				 
	else if (mqttConnection == false) {
	  
	  printf("Producer Store\n");
	  printf("Producer Produce\n");
	  
	  send(buffer, STORAGE);
	  produce(buffer, file);
	  
	}
				 
	pthread_mutex_unlock(&lock);

  }
}

void* consumer() {
  while(true){

	sleep(1);
	pthread_mutex_lock(&lock);

	if (mqttConnection == true && stored == 0) {

	  printf("Consumer Consume Buffer\n");
	  
	  send(buffer, NETWORK);

	}
	
	else if (mqttConnection == true && stored != 0) {

	  printf("Consumer Consume Storage\n");
	  
	  inventory();

	}

	pthread_mutex_unlock(&lock);

  }
}

void* manager() {
  while(true){
	
	sleep(5);
	printf("Manager Get Status\n");
	
	mqttConnection = !mqttConnection;
	printf("%d\n\n", mqttConnection);
	
  }
}





