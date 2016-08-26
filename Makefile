#-------------------------------------------------------------------------------
# Makefile
#-------------------------------------------------------------------------------
SRC = .
SRC_INC = -I$(SRC)
SRC_LIB = -L$(SRC)

#-------------------------------------------------------------------------------
# set files
#-------------------------------------------------------------------------------
CC_DEPS   = $(shell find . -name "*.h"   )
CC_FILES  = $(shell find . -name "*.cc"  )
OBJS += $(CC_FILES:.cc=.o)


#-------------------------------------------------------------------------------
# set opt
#-------------------------------------------------------------------------------
CC        = g++ -std=c++11 -Wall
DEBUGFLAG = -g
OPTFLAG   = -O2

INCS   = $(SRC_INC)
LIBS   = $(SRC_LIB) -lpthread
CFLAGS = -c $(DEBUGFLAG) $(OPTFLAG) -fPIC


TARGET = test

.SUFFIXES: .h .cc .o

default : all
all : $(TARGET)

.cc.o:
	$(CC) $(INCS) $(CFLAGS) -c $< -o $@

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(INC)
  
clean :
	rm -f $(OBJS) $(TARGET)
