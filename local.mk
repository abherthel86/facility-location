# CPLEX-section
SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic
# CPLEX common directories
CPLEXDIR   = /opt/cplex/current/cplex
#CPLEXDIR   = /opt/ibm/ILOG/CPLEX_Studio1210/cplex

CONCERTDIR = $(CPLEXDIR)/../concert
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include
# compilation flags
CPLEXBASE = -m64 -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD -Wno-deprecated-declarations -Wno-ignored-attributes
CPLEXFLAGS = $(CPLEXBASE) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)
# linking flags
CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread -ldl
# end of CPLEX section

#
# Things that _must_ be defined in this file
CXXFLAGS         = $(CPLEXFLAGS)
LDFLAGS          = $(CCLNFLAGS)
