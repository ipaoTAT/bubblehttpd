#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <pthread.h>
#include "threadpool.h"

thread_pool *rdpool = NULL;
thread_pool *wtpool = NULL;
thread_pool *procpool = NULL;

/* pool initial */
thread_pool *create_pool(int max_thread_num)
{
     thread_pool *pool = (thread_pool *) malloc (sizeof (thread_pool));

     pthread_mutex_init (&(pool->queue_lock), NULL);
     pthread_cond_init (&(pool->queue_ready), NULL);

     pool->queue_head = NULL;

     pool->max_thread_num = max_thread_num;
     pool->cur_queue_size = 0;

     pool -> shutdown = 0;

     pool -> threadid = (pthread_t *) malloc (max_thread_num * sizeof (pthread_t));
    int i = 0;
    for (i = 0; i < max_thread_num; i++)
     {
         pthread_create (&(pool->threadid[i]), NULL, thread_routine,
                 pool);
     }

    return pool;
}


/* add a task to pool*/
int pool_add_task (thread_pool *pool, void (*process) (int fd, void *arg), int fd, void *arg)
{
    /* create a new task */
     thread_task *newtask =
         (thread_task *) malloc (sizeof (thread_task));
     newtask->process = process;
     newtask->fd = fd;
     newtask->arg = arg;
     newtask->next = NULL;		

     pthread_mutex_lock (&(pool->queue_lock));
     /* add task struct to waiting queue */
     thread_task *member = pool->queue_head;	
     if (member != NULL)
     {
        while (member->next != NULL)
             member = member->next;
        member->next = newtask;
     }
    else
     {
         pool->queue_head = newtask;
     }

     assert (pool->queue_head != NULL);

     pool->cur_queue_size++;
     pthread_mutex_unlock (&(pool->queue_lock));
    /*a new task need a thread to process it */
     pthread_cond_signal (&(pool->queue_ready));
    return 0;
}


/* destroy the pool : never be exec until server shutdown */
int pool_destroy (thread_pool *pool)
{
    if (pool->shutdown)
        return -1;
     pool->shutdown = 1;

     pthread_cond_broadcast(&(pool -> queue_ready));

    int i;
    for (i = 0; i < pool->max_thread_num; i++)
         pthread_join (pool->threadid[i], NULL);
     free (pool->threadid);

     thread_task *head = NULL;
    while (pool->queue_head != NULL)
     {
         head = pool->queue_head;
         pool->queue_head = pool->queue_head->next;
         free (head);
     }

     pthread_mutex_destroy(&(pool->queue_lock));
     pthread_cond_destroy(&(pool->queue_ready));
    
     free (pool);

     pool=NULL;
    return 0;
}


void *thread_routine (void *arg)
{
	thread_pool *pool = (thread_pool *)arg;
#ifdef DEBUG
     	printf ("starting thread 0x%x\n", pthread_self ());
#endif
    	while (1)
     	{
        	 pthread_mutex_lock (&(pool->queue_lock));
        /*如果等待队列为0并且不销毁线程池，则处于阻塞状态; 注意
         pthread_cond_wait是一个原子操作，等待前会解锁，唤醒后会加锁*/
        	while (pool->cur_queue_size == 0 && !pool->shutdown)
         	{
#ifdef DEBUG
           		printf ("thread 0x%x is waiting\n", pthread_self ());
#endif
             		pthread_cond_wait (&(pool->queue_ready), &(pool->queue_lock));
         	}

        /* while destroy the pool */
        	if (pool->shutdown)
        	{
            		pthread_mutex_unlock (&(pool->queue_lock));
#ifdef DEBUG
            		printf ("thread 0x%x will exit\n", pthread_self ());
#endif
     		        pthread_exit (NULL);
         	}

#ifdef DEBUG
         	printf ("thread 0x%x is starting to work\n", pthread_self ());
#endif

         	assert (pool->cur_queue_size != 0);
         	assert (pool->queue_head != NULL);
        
        /* get the task this thread should process */
		pool->cur_queue_size--;
		thread_task *task = pool->queue_head;
		pool->queue_head = task->next;
		pthread_mutex_unlock (&(pool->queue_lock));

		/* exec task */
		(task->process)(task -> fd, task->arg);
		free (task);
		task = NULL;
     }
/*never got here*/
     pthread_exit (NULL);
}

/*   a test */
/*
void *myprocess (void *arg)
{
#ifdef DEBUG
     printf ("threadid is 0x%x, working on task %d\n", pthread_self (),*(int *) arg);
#endif
     sleep (1);
     return NULL;
}

int _main (int argc, char **argv)
{
     thread_pool *pool = create_pool(3);
     thread_pool *pool1 = create_pool(3);
    
    int *workingnum = (int *) malloc (sizeof (int) * 10);
    int i;
    for (i = 0; i < 10; i++)
     {
         workingnum[i] = i;
         pool_add_task (pool, myprocess, &workingnum[i]);
     }
    for (i = 0; i < 10; i++)
     {
         workingnum[i] = i;
         pool_add_task (pool1, myprocess, &workingnum[i]);
     }
     sleep (5);
     pool_destroy (pool);

     free (workingnum);
    return 0;
}
*/
