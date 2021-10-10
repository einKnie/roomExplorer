BASE        = ..
SRC_DIR     = $(BASE)/src
INC_DIR     = $(BASE)/inc
LIB_DIR     = $(BASE)/../Logger
TARGET_DIR  = $(BASE)/bin
OBJ_DIR     = $(BASE)/obj
TARGET      = maze

CC          = g++
LIBS 				= -lcpplogging
CFLAGS      = -Wall -Wno-unused-function -Wno-format-truncation -std=c++11 -pedantic -g -I$(INC_DIR) -I$(LIB_DIR)/inc -L$(LIB_DIR)/bin $(LIBS)

SRCS       := $(wildcard $(SRC_DIR)/*.cpp)

OBJ         = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS        = $(patsubst %,$(INC_DIR)/%.h, *)
DEPS 			 += $(INC_DIR)/lilist.h

.PHONY: all clean

# make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

all: maze

clean:
	rm -frd $(OBJ_DIR) $(TARGET_DIR)/$(TARGET)

$(OBJ_DIR):
	echo $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)

$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

maze : $(OBJ) | $(TARGET_DIR)
	$(CC) -o $(TARGET_DIR)/$(TARGET) $^ $(CFLAGS)
