#include<stdio.h>
#include "scheduler.h"
#include "queue.h"
#include<stdlib.h>
#define STACK_SIZE 1024 * 1024

thread* current_thread ;
struct queue ready_list;
thread* new_thread;
thread * next_thread;

void thread_switch(struct thread* old, struct thread *new);
void thread_start(struct thread* old, struct thread* new);

void thread_wrap() {
current_thread -> initial_function(current_thread -> initial_argument);
current_thread->state = DONE;
 yield();
}

void scheduler_begin(){
current_thread =  malloc (sizeof(thread));
current_thread->state = RUNNING;
ready_list.head = NULL;
ready_list.tail = NULL;
}

void thread_fork(void(*target)(void*), void * arg) {
new_thread =  malloc (sizeof(thread));
new_thread->stack_base = malloc(STACK_SIZE);
new_thread->stack_pointer = new_thread->stack_base + STACK_SIZE;
new_thread->initial_argument =arg;
new_thread->initial_function =target;
current_thread->state = READY;
thread_enqueue(&ready_list, current_thread);
new_thread->state = RUNNING;
thread* temp = current_thread;
current_thread = new_thread;
new_thread = temp;
thread_start(temp,current_thread);
}

void yield(){
if(current_thread->state!= DONE) {
current_thread->state = READY;
thread_enqueue(&ready_list,current_thread);
}
next_thread =thread_dequeue(&ready_list);
next_thread->state  = RUNNING;
thread * temp = current_thread;
current_thread = next_thread;
next_thread = temp;
thread_switch(temp,current_thread);
if(next_thread->state== DONE) {
free(next_thread->stack_base);
free(next_thread);
}
}
void scheduler_end(){
while(is_empty(&ready_list)==0){
yield();

}
}

