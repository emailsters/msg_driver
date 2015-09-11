CC = g++
FLAGS = -g -Wall 
INCLUDES = -I../platform
LIBS =  ../platform/libplatform.a ../thirdparty/tinyxml/libtinyxml.a -lpthread -ldl -rdynamic
BIN = main
BIN_PATH = ../bin

SOURCE = $(wildcard *.cpp)  
OBJS = $(patsubst %.cpp,%.o,$(SOURCE))

$(BIN):$(OBJS)
	$(CC) -o $@ $^ $(FLAGS)  $(LIBS)
	cp -f $(BIN) $(BIN_PATH)/$(BIN)

$(OBJS):%.o:%.cpp
	$(CC) -g -c $< $(INCLUDES) $(FLAGS)

.PHONY:clean
clean:
	-rm -f $(OBJS) $(BIN) $(BIN_PATH)/$(BIN)