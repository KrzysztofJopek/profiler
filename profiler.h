#ifndef _PROFILER_H_
#define _PROFILER_H_

#define PROF_INV_TRUE 1
#define PROF_INV_FALSE 0

#define PROF_OUT_FILE "/tmp/profiler.out"

/* Initalize profiler
 * int n: number of profilers
 * use only once
 */
int profiler_init(int n);

/* Start measuring time
 * int profiler: index of profiler
 */
void profiler_start(int profiler);

/* Stop measuring time
 * int profiler: index of profiler
 */
void profiler_stop(int profiler);

/* Print stats to PROF_OUT_FILE
 * int print_invocation:
 *     PROF_INV_TRUE - do print number of invocations,
 *     PROF_INV_FALSE - don't print number of invocations,
 */
void profiler_update(int print_inv);

/* Destroy profiler
 */
void profiler_destroy();

#endif
