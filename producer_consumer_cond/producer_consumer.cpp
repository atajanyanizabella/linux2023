#include <iostream>
#include <pthread.h>
#include <queue>

#define BUFF_SIZE 15
#define PRODUCERS_COUNT 20
#define CONSUMERS_COUNT 30

struct args_t
{
    std::queue<int>* buffer;
    pthread_mutex_t* lock;
    pthread_cond_t* is_not_full;
    pthread_cond_t* is_not_empty;
};

void* producer(void* args)
{
    args_t* arg = (args_t*) args;
    while (true) {
        int new_value = 2;
        pthread_mutex_lock(arg->lock);
        while (arg->buffer->size() == BUFF_SIZE) {
            pthread_cond_wait(arg->is_not_full, arg->lock);
        }
        arg->buffer->push(new_value);
        std::cout << "Producer added: " << arg->buffer->size() << std::endl;
        pthread_mutex_unlock(arg->lock);
        pthread_cond_signal(arg->is_not_empty);
    }
    return NULL;
}

void* consumer(void* args)
{
    args_t* arg = (args_t*) args;
    while (true) {
        pthread_mutex_lock(arg->lock);
        while (arg->buffer->size() == 0) {
            pthread_cond_wait(arg->is_not_empty, arg->lock);
        }
        arg->buffer->pop();
        std::cout << "Consumer removed: " << arg->buffer->size() << std::endl;
        pthread_mutex_unlock(arg->lock);
        pthread_cond_signal(arg->is_not_full);
    }
    return NULL;
}

int main()
{
    std::queue<int>* buffer = new std::queue<int>;
    pthread_mutex_t* lock = new pthread_mutex_t;
    pthread_mutex_init(lock, NULL);
    pthread_cond_t* is_not_full = new pthread_cond_t;
    pthread_cond_t* is_not_empty = new pthread_cond_t;
    pthread_cond_init(is_not_full, NULL);
    pthread_cond_init(is_not_empty, NULL);
    pthread_t* producers = new pthread_t[PRODUCERS_COUNT];
    pthread_t* consumers = new pthread_t[CONSUMERS_COUNT];
    for (int i = 0; i < PRODUCERS_COUNT; ++i) {
        args_t* arg = new args_t;
        arg->buffer = buffer;
        arg->lock = lock;
        arg->is_not_full = is_not_full;
        arg->is_not_empty = is_not_empty;
        int result = pthread_create(&producers[i], NULL, producer, (void*)arg);
        if (result != 0) {
            std::cout << "Producer: Error while creating thread...\n";
            exit(result);
        }
    }

    for (int i = 0; i < CONSUMERS_COUNT; ++i) {
        args_t* arg = new args_t;
        arg->buffer = buffer;
        arg->lock = lock;
        arg->is_not_full = is_not_full;
        arg->is_not_empty = is_not_empty;
        int result = pthread_create(&producers[i], NULL, consumer, (void*)arg);
        if (result != 0) {
            std::cout << "Consumer: Error while creating thread...\n";
            exit(result);
        }
    }

    for (int i = 0; i < PRODUCERS_COUNT; ++i) {
        int result = pthread_join(producers[i], NULL);
        if (result != 0) {
            std::cout << "Producer: Error while joining thread...\n";
            exit(result);
        }
    }

    for (int i = 0; i < CONSUMERS_COUNT; ++i) {
        int result = pthread_join(consumers[i], NULL);
        if (result != 0) {
            std::cout << "Consumer: Error while joining thread...\n";
            exit(result);
        }
    }

    return 0;
}
