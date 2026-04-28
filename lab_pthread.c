#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // Compile with "-lpthread"
#include <string.h>

int total = 0; // to record total number of messages from the two threads
pthread_t t1, t2;

int total_prints;
int current_count = 0;

int t1_count = 0;
int t2_count = 0;

pthread_mutex_t lock;

void * forward_print(void*arg) {
	char* str = (char*)arg;

	while (1) {
		pthread_mutex_lock(&lock);

		if (current_count >= total_prints) {
			pthread_mutex_unlock(&lock);
			break;
		}

		printf("T1: %s\n", str);
		current_count++;
		t1_count++;

		pthread_mutex_unlock(&lock);
	}

	return NULL;
}

void * backward_print(void* arg) {
	char* str = (char*)arg;
	int len = strlen(str);

	while (1) {
		pthread_mutex_lock(&lock);

		if (current_count >= total_prints) {
			pthread_mutex_unlock(&lock);
			break;
		}

		printf("T2: ");
		for (int i = len - 1; i >= 0; i--) {
			printf("%c", str[i]);
		}
		printf("\n");

		current_count++;
		t2_count++;

		pthread_mutex_unlock(&lock);
	}

	return NULL;
}

void * thread_work(void * arg) {
    long self_counter = 0; // to record how many times printed by the current thread

    return (void *)self_counter;
}

int main(int argc, char * argv[]) {
    long t1_c = 0, t2_c = 0; // to record numbers of times printed by T1 and T2

    if (argc < 3) {
        printf("There must be 2 arguments following the binary!\n");
        return 0;
    }

    total = atoi(argv[1]); 

    char buffer[256] = "";
    for (int i = 2; i < argc; i++) {
	    strcat(buffer, argv[i]);
	    if (i < argc - 1) strcat(buffer, " ");
    }

    pthread_mutex_init(&lock, NULL);
    
    pthread_create(&t1, NULL, thread_work, argv + 2);
    pthread_create(&t2, NULL, thread_work, argv + 2);
    pthread_join(t1, (void *)&t1_c);
    pthread_join(t2, (void *)&t2_c);
    
    printf("In main thread: T1 printed %ld times. T2 printed %ld times.\n", t1_c, t2_c);
    
    return 0;
}


