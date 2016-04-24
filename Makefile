## Folders ##
INC     = inc
SRC     = src
OBJ     = obj

## Compiler specific ##
CC      = g++ -std=c++11
CFLAGS  = -g -Wall -MMD -I${INC} -O2

## Linker specific ##
LPATHS  =
LDFLAGS =

## Target ##
TARGET  = matrix_appl
VPATH   = src

## Sources ##
SRCS    = matrix_appl.cpp

OBJS    = $(addprefix $(OBJ)/,$(subst .cpp,.o,$(SRCS)))



all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

$(OBJ)/%.o:$(SRC)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f ./obj/*.o *.a *.obj matrix_appl

