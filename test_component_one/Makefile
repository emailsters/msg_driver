CC = g++
FLAGS = -g -Wall 
INCLUDES = -I../platform
LIBS = ../platform/libplatform.a
BIN = libtest_component_one.so
BIN_PATH = ../bin

SOURCE = $(wildcard *.cpp)  
OBJS = $(patsubst %.cpp,%.o,$(SOURCE))

$(BIN):$(OBJS)
	$(CC) --shared -fPIC -o $@ $^ $(FLAGS) $(LIBS) -lpthread 
	cp $(BIN) $(BIN_PATH)/$(BIN)

$(OBJS):%.o:%.cpp
	$(CC) -g -c $< $(INCLUDES) $(FLAGS)	

.PHONY:clean
clean:
	-rm -f $(OBJS) $(BIN) $(BIN_PATH)/$(BIN)