CXXFLAGS+= 
LDFLAGS+=

default: all

all: libengine.a

LIBENGINE_SRC= gamebase.cpp\
          gameclock.cpp\

LIBENGINE_OBJ= $(LIBENGINE_SRC:.cpp=.o)

libengine.a: $(LIBENGINE_SRC) $(LIBENGINE_OBJ)
	@ar -rcs libengine.a $(LIBENGINE_OBJ)

clean:
	@- rm -f *.o libengine.a