SHELL := /bin/bash
CFLAGS   += -g -Wall -Wextra -Iluajit-2.0/src/ -Wl,-E -MD -MP
CPPFLAGS += $(CFLAGS) 
LDFLAGS  += -Lluajit-2.0/src -lluajit 
LDDFLAGS += $(LDFLAGS)

C_SRCS= main.c\
		Character.c\

CPP_SRCS= Object.cpp\
		  Inventory.cpp\
		  Material.cpp\

OBJS= $(C_SRCS:.c=.o) $(CPP_SRCS:.cpp=.o) bindings_tmp.o

default: all

all: posta

posta: $(OBJS)
	$(CXX) $(CFLAGS) -o posta $(OBJS) $(LDFLAGS) 

bindings_tmp.o: $(C_SRCS) $(CPP_SRCS)
bindings_tmp.o:
	scripts/build_bindings.sh

clean:
	- rm -f posta $(OBJS)
	- rm -f bindings_tmp.o
	- rm -f *.d

-include $(C_SRCS:%.c=%.d)
-include $(CPP_SRCS:%.cpp=%.d)