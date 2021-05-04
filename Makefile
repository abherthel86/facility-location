# local.mk should define CXXFLAGS and LDFLAGS;
# it should work with CPLEX
include local.mk

# build options
DEBUG 	= -O0 -g -Wall -Werror -Wno-sign-compare -pedantic -pg
OPTIM	= -O3

# compiler
CXX = g++ -std=c++11
# linker
LD = g++ -std=c++11

ifeq ($(BUILD), debug)
BUILDFLAGS = $(DEBUG)
else
BUILDFLAGS = $(OPTIM)
endif

MAIN  = main

all: $(MAIN)

SRC = main.cpp FLPInstance.cpp FLPModel.cpp

OBJ = $(SRC:%.cpp=%.o) 

$(MAIN):  $(OBJ)
	$(LD) $(BUILDFLAGS) $(OBJ) -o $(MAIN) $(LDFLAGS)

%.o:%.cpp *.h
	$(CXX) -c $(BUILDFLAGS) $(CXXFLAGS) $< -o $(<:%.cpp=%.o)

clean:
	rm -f *.o $(MAIN)
