## Folders ##
INC     = inc
SRC     = src
OBJ     = obj

## Compiler specific ##
CC      = g++ -std=c++11
CFLAGS  = -g -Wall -MMD -I${INC} -O2

## Linker specific ##
LPATHS  =
LDFLAGS = -lpthread

## Target ##
TARGET  = matrix_appl
VPATH   = src

## Sources ##
SRCS    = rasp_pi_gpio.cpp \
          rt_thread.cpp \
          rgb_mtrx_ifc.cpp \
          frame_buffer.cpp \
          rgb_matrix.cpp \
          pix_driver.cpp \
          what.cpp \
          ppm_parser.cpp \
          glyph_loader.cpp \
          fontizer.cpp \
          matrix_appl.cpp

OBJS    = $(addprefix $(OBJ)/,$(subst .cpp,.o,$(SRCS)))



all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

$(OBJ)/%.o:$(SRC)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<


clean:
	rm -f ./obj/*.o *.a *.obj matrix_appl

