#===============================================================================
# User Options
#===============================================================================

# Compiler can be set below, or via environment variable
CC    = nvcc
OPTIMIZE  = yes
DEBUG     = no


#===============================================================================
# Program name & source code list
#===============================================================================


SP = yes

program = md

source = MD.cu

obj = $(source:.cu=.o)

#===============================================================================
# Sets Flags
#===============================================================================

# Standard Flags
CFLAGS := -std=c++14 -Xcompiler -Wall

# Linker Flags
LDFLAGS = 

# Debug Flags
ifeq ($(DEBUG),yes)
  CFLAGS += -g
  LDFLAGS  += -g
endif


# Optimization Flags
ifeq ($(OPTIMIZE),yes)
  CFLAGS += -O3
endif

ifeq ($(SP),yes)
  CFLAGS += -DSINGLE_PRECISION
endif

#===============================================================================
# Targets to Build
#===============================================================================

$(program): $(obj) Makefile
	$(CC) $(CFLAGS) $(obj) -o $@ $(LDFLAGS)

%.o: %.cu MD.h Makefile
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(program) $(obj)

run: $(program)
	./$(program) 2 1 4

