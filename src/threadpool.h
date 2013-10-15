#ifndef THREADPOOL_H
#define THREADPOOL_H

/*
* task data structure
*/
typedef struct task
{
    void (*process) (int fd, void *arg);		// task callback function
    int fd;
    void *arg;					// argument
    struct task *next;

} thread_task;


/* thread pool structure */
typedef struct pool
{
    pthread_mutex_t queue_lock;
    pthread_cond_t queue_ready;

    thread_task *queue_head;			

    int shutdown;			
    pthread_t *threadid;
    int max_thread_num;			
    int cur_queue_size;		

} thread_pool;

extern thread_pool *rdpool;
extern thread_pool *wtpool;
extern thread_pool *procpool;

thread_pool *create_pool(int);
int pool_add_task (thread_pool *pool, void (*process) (int , void *arg), int, void *arg);
void *thread_routine (void *arg);
int pool_destroy(thread_pool *pool);

#endif
