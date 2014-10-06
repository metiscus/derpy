export CXXFLAGS+= -gdwarf-4 -O0 -Wall -Wextra -pedantic -Iengine
LDFLAGS+=

default: all

SUBDIRS= engine\

all: SUBDIR_BUILD game

clean: SUBDIR_CLEAN

game: main.cpp engine/libengine.a
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o game main.cpp engine/libengine.a

# subdir
.PHONY: SUBDIR_BUILD
SUBDIR_BUILD:
	@for i in $(SUBDIRS); do\
		echo Building $$i;\
		cd $$i && $(MAKE);\
	done

.PHONY: SUBDIR_CLEAN
SUBDIR_CLEAN:
	@for i in $(SUBDIRS); do\
		echo Cleaning $$i;\
		cd $$i && $(MAKE) clean;\
	done