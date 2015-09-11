CC = g++
AR = ar
FLAGS = -g -Wall 
INCLUDES = -I../thirdparty/tinyxml
LIBS = ../thirdparty/tinyxml/libtinyxml.a
BIN = libplatform.a
BIN_PATH = 

SOURCE = $(wildcard *.cpp)  
OBJS = $(patsubst %.cpp,%.o,$(SOURCE))

$(BIN):$(OBJS)
	$(AR) -r $@ $^ $(LIBS)

$(OBJS):%.o:%.cpp
	$(CC) -g -c $< $(INCLUDES) $(FLAGS)	

.PHONY:clean
clean:
	-rm -f $(OBJS) $(BIN)