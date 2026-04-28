#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> // Compile with "-lpthread"
#include <string.h>

int total = 0; // to record total number of messages from the two threads

pthread_t t1, t2;

int current_count = 0;

pthread_mutex_t lock;

char buffer[256];

int t1_flag = 1;
int t2_flag = 0;

void * thread_work(void * arg) {
    int is_forward = (int)(long)arg;

    if (arg == &t1_flag) {
	    is_forward = 1;
    } else {
	    is_forward = 0;
    }

    long self_counter = 0; // to record how many times printed by the current thread

    pthread_t tid = pthread_self();

    while (1) {
	    pthread_mutex_lock(&lock);

	    if (current_count >= total) {
		    pthread_mutex_unlock(&lock);
		    break;
	    }
	    
	    printf("%d: In thread %s (TID=%1u): ", current_count + 1, is_forward ? "1" : "2", (unsigned long)tid);

	    if (is_forward) {
		    printf("T1: %s\n", buffer);
	    } else {
		    printf("T2: ");
		    int len = strlen(buffer);
		    for (int i = len - 1; i >= 0; i--) {
			    printf("%c", buffer[i]);
		    }
		    printf("\n");
	    }

	    current_count++;
	    self_counter++;

	    pthread_mutex_unlock(&lock);
    }

    return (void *)self_counter;
}

int main(int argc, char * argv[]) {
    long *t1_c = 0, *t2_c = 0; // to record numbers of times printed by T1 and T2

    if (argc < 3) {
        printf("There must be 2 arguments following the binary!\n");
        return 0;
    }

    total = atoi(argv[1]); 

    buffer[0] = '\0';
    for (int i = 2; i < argc; i++) {
	    strcat(buffer, argv[i]);
	    if (i < argc - 1) strcat(buffer, " ");
    }

    pthread_mutex_init(&lock, NULL);
    
    pthread_create(&t1, NULL, thread_work, &t1_flag);
    pthread_create(&t2, NULL, thread_work, &t2_flag);
    pthread_join(t1, (void *)&t1_c);
    pthread_join(t2, (void *)&t2_c);
    
    printf("In main thread: T1 printed %ld times. T2 printed %ld times.\n", (long)t1_c, (long)t2_c);

    pthread_mutex_destroy(&lock);
    
    return 0;
}


