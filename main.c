#include "profiler.h"
#include <unistd.h>

/*
 * EXAMPLE
 */
int main()
{
    profiler_init(2);
    for(int i=0; i<3; i++){
        for(int x=0; x<2; x++){
            profiler_start(0);
            sleep(1);
            profiler_stop(0);
            profiler_start(1);
            sleep(2);
            profiler_stop(1);
        }
        profiler_update(PROF_INV_TRUE);
    }
}
