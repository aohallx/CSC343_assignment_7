// two_philosophers_mutex.c
// Build: gcc -pthread -O2 two_philosophers_mutex.c -o two_phils
// Run:   ./two_phils

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define THINK_US 200000   // 0.2s
#define EAT_US   150000   // 0.15s
#define ROUNDS   10

pthread_mutex_t fork_m[2]; // fork_m[0] and fork_m[1]

static inline void think(void) { usleep(THINK_US); }
static inline void eat(void)   { usleep(EAT_US);   }

void *philosopher(void *arg) {
    int id = (int)(long)arg;    // 0 or 1
    int left  = id;
    int right = 1 - id;

    for (int r = 0; r < ROUNDS; ++r) {
        think();

        // Asymmetric order: break circular wait
        if (id == 0) {
            pthread_mutex_lock(&fork_m[left]);
            pthread_mutex_lock(&fork_m[right]);
        } else {
            pthread_mutex_lock(&fork_m[right]);
            pthread_mutex_lock(&fork_m[left]);
        }

        printf("Phil %d eating (round %d)\n", id, r + 1);
        eat();

        pthread_mutex_unlock(&fork_m[left]);
        pthread_mutex_unlock(&fork_m[right]);
    }
    return NULL;
}

int main(void) {
    pthread_t t0, t1;

    pthread_mutex_init(&fork_m[0], NULL);
    pthread_mutex_init(&fork_m[1], NULL);

    pthread_create(&t0, NULL, philosopher, (void*)0);
    pthread_create(&t1, NULL, philosopher, (void*)1);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    pthread_mutex_destroy(&fork_m[0]);
    pthread_mutex_destroy(&fork_m[1]);
    return 0;
}
