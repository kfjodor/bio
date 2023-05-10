MATCHER = ./libUFMatcher.so
INC = -I /usr/include/jsoncpp
LIBS = -ljsoncpp -lpthread -levent -lusb-1.0 -lmysqlcppconn
LIBS64 = ./libUFMatcher.so.64 -ljsoncpp -lpthread -levent -lmysqlcppconn /lib/x86_64-linux-gnu/libusb-1.0.so.0

all:
	c++ -Wall -O2 --std=c++11 $(wildcard *.cpp) $(MATCHER) $(INC) $(LIBS) -o bio

all64:
	c++ -Wall -O2 --std=c++11 $(wildcard *.cpp) $(INC) $(LIBS64) -o bio64

cppcheck:
	cppcheck --enable=warning,style,performance,portability,information -I. *.cpp 2>__CPPCHECK
