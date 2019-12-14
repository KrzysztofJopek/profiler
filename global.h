#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define CHECK(A, M, ...) if(!(A)) { LOG_ERR(M, ##__VA_ARGS__); errno=0; goto error; } 
#define CHECK_MEM(A) if(!(A)) { LOG_ERR("Can't allocate memory"); errno=0; goto error; } 
#define LOG_ERR(M, ...) fprintf(stderr, "[ERROR] (%s:%d:%s: errno: %s) " M "\n", __FILE__, __LINE__, __func__, clean_errno(), ##__VA_ARGS__)
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#endif
