#define _GNU_SOURCE
#include "global.h"
#include "profiler.h"
#include <time.h>
#include <stdlib.h>

struct clock_info{
    struct timespec start;
    struct timespec stop;
    unsigned long invocations;
    int running;
    struct timespec sum;
};

static struct clock_info* timers = NULL;
static int timers_count = 0;
static FILE* fptr = NULL;


/* ***********************************
 * PRIVATE FUNCTIONS
 */


static void add_to_sum(struct clock_info* timer);
static void timespec_sub(struct timespec *start, struct timespec *stop,
                   struct timespec *result);
static void timespec_add(struct timespec *start, struct timespec *stop,
                   struct timespec *result);
static void print_times();
static void print_invocations();


static void add_to_sum(struct clock_info* timer)
{
    struct timespec temp;
    timespec_sub(&timer->start, &timer->stop, &temp);
    timespec_add(&timer->sum, &temp, &timer->sum);
}

static void timespec_sub(struct timespec *start, struct timespec *stop,
                   struct timespec *result)
{
    struct timespec temp;
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        temp.tv_sec = stop->tv_sec - start->tv_sec - 1;
        temp.tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        temp.tv_sec = (unsigned long)stop->tv_sec - (unsigned long)start->tv_sec;
        temp.tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
    result->tv_sec = temp.tv_sec;
    result->tv_nsec = temp.tv_nsec;
}

static void timespec_add(struct timespec *start, struct timespec *stop,
                   struct timespec *result)
{
    struct timespec temp;
    temp.tv_sec = stop->tv_sec + start->tv_sec;
    temp.tv_nsec = stop->tv_nsec + start->tv_nsec;

    temp.tv_sec += temp.tv_nsec / 1000000000;
    temp.tv_nsec %= 1000000000;

    result->tv_sec = temp.tv_sec;
    result->tv_nsec = temp.tv_nsec;
}

static void print_times()
{
    static int num=0;
    fprintf(fptr, "%d,", num++);
    for(int i=0; i<timers_count; i++){
        fprintf(fptr, "%lu.%09lu", timers[i].sum.tv_sec, timers[i].sum.tv_nsec);
        if(i != timers_count-1){
            fprintf(fptr, ",");
        }
        else{
            fprintf(fptr, "\n");
        }
        timers[i].sum.tv_sec = 0;
        timers[i].sum.tv_nsec = 0;
    }
}

static void print_invocations()
{
    fprintf(fptr, "INV,");
    for(int i=0; i<timers_count; i++){
        fprintf(fptr, "%lu", timers[i].invocations);
        if(i != timers_count-1){
            fprintf(fptr, ",");
        }
        else{
            fprintf(fptr, "\n");
        }
        timers[i].invocations = 0;
    }
}

/* ***********************************
 * API DEFINITION
 */


int profiler_init(int n)
{
    CHECK(n>0, "profiler count <= 0");
    timers = calloc(n, sizeof(struct clock_info));
    timers_count = n;
    CHECK_MEM(timers);

    fptr = fopen(PROF_OUT_FILE, "a");
    if(!fptr)
        exit(1);
    fprintf(fptr, "BEGIN\n");
    
    return 1;
error:
    free(timers);
    timers = NULL;
    timers_count = 0;
    return 0;
}

void profiler_start(int profiler)
{
    CHECK(profiler>=0 && profiler<timers_count,
            "No profiler with id:%d, max:%d", profiler, timers_count);
    timers[profiler].running=1;
    timers[profiler].invocations++;
    clock_gettime(CLOCK_MONOTONIC, &timers[profiler].start);

    return;
error:
    return;
}

void profiler_stop(int profiler)
{
    CHECK(profiler>=0 && profiler<timers_count,
            "No profiler with id:%d, max:%d", profiler, timers_count);
    clock_gettime(CLOCK_MONOTONIC, &timers[profiler].stop);
    timers[profiler].running=0;
    add_to_sum(timers + profiler);

    return;
error:
    return;
}

void profiler_update(int print_inv)
{
    CHECK(timers != NULL, "profilers not inited");
    print_times();
    if(print_inv == PROF_INV_TRUE){
        print_invocations();
    }
    fflush(fptr);
    return;
error:
    return;
}

void profiler_destroy()
{
    free(timers);
    timers = NULL;
    if(fptr){
        fclose(fptr);
        fptr = NULL;
    }
}

