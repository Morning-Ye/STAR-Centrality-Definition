OBJS = mEvent.o refmultCorr.o 
EXE = refmultCorr

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

INCFLAGS = -I$(ROOTSYS)/include
LDFLAGS = -L$(ROOTSYS)/lib

CXX = g++
#FLAGS = -m32 -fno-inline -Wall -g  $(INCFLAGS) $(LDFLAGS)
FLAGS = -m32 -fPIC -pipe -Wall -Woverloaded-virtual -std=c++0x -Wno-long-long -pthread  $(INCFLAGS) $(LDFLAGS)

COMPILE = $(CXX) $(FLAGS) -c

all: $(EXE)
$(EXE): $(OBJS)
	$(CXX) -m32 -fno-inline -o $(EXE) $(OBJS) $(ROOTFLAGS) $(ROOTLIBS)
	#$(CXX) -fno-inline -o $(EXE) $(OBJS) $(ROOTFLAGS) $(ROOTLIBS)

%.o: %.cxx
	$(COMPILE)  $<

clean:
	@rm -f *.o
