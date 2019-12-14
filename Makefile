CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99
LDFLAGS = 
OUTPUT_OPTION = -MMD -MP -o $@
.DEFAULT_GOAL = profiler

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
DEPS = $(SRCS:%.c=%.d)
-include $(DEPS)

#---------------

profiler: $(OBJS)
		$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS)

#---------------


.PHONY: clean clean_tags


tags: cscope.out
	ctags -R

cscope.out:
	-cscope -bR

clean:
	@rm -f profiler $(OBJS) $(DEPS)
	@echo Clean finished

clean_tags:
	@rm -f tags cscope.out
	@echo Clean finished
