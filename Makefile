# GNU Make

# Parallel compile
ifndef MAKEFLAGS
    MAKEFLAGS=-j 2
endif

ifndef CXX
    CXX=g++
endif

ifndef OBJDIR
    OBJDIR=obj
endif

ifndef BINDIR
    BINDIR=bin
endif

ifndef CXXFLAGS
    CXXFLAGS=-std=c++14 -g -Wall -Werror -I${PWD}
endif

# Lib: multi-thread; boost
LIBS+=-lpthread -L/usr/lib/x86_64-linux-gnu/ -lboost_program_options 


# Directories
LIBFLOWOBJ      =       $(OBJDIR)/Main.o \

LIBCONFIGOBJ    =       $(OBJDIR)/ConfigDb.o \
                        $(OBJDIR)/ConfigAnalyzer.o

LIBRULEOBJ      =       $(OBJDIR)/RuleDb.o \
                        $(OBJDIR)/RuleAnalyzer.o

LIBFILEOBJ      =       $(OBJDIR)/FileDb.o \
                        $(OBJDIR)/FileAnalyzer.o    \
                        $(OBJDIR)/DiffFileDb.o    \


LIBFORMATOBJ    =       $(OBJDIR)/Format.o \

LIBUTILSOBJ     =       $(OBJDIR)/Timer.o \

LIBFLOWOBJ      =       $(OBJDIR)/Main.o \

# Binary
cppFormat: mkbindir $(LIBFLOWOBJ) $(LIBCONFIGOBJ) $(LIBFILEOBJ) $(LIBRULEOBJ) $(LIBFORMATOBJ) $(LIBUTILSOBJ) $(LIBFLOWOBJ)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/cppFormat \
           $(LIBDATABASEOBJ) $(LIBCONFIGOBJ) $(LIBFILEOBJ) $(LIBRULEOBJ) $(LIBFORMATOBJ) $(LIBUTILSOBJ) $(LIBFLOWOBJ)\
           $(LIBS)

# Object dir
mkobjdir  :
	mkdir -p $(OBJDIR)

# Binary dir
mkbindir  :
	mkdir -p $(BINDIR)

$(OBJDIR)/Timer.o : mkobjdir libUtils/private/Timer.cc libUtils/public/Timer.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/Timer.o libUtils/private/Timer.cc

$(OBJDIR)/ConfigDb.o : mkobjdir libConfig/private/ConfigDb.cc   libConfig/public/ConfigDb.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/ConfigDb.o libConfig/private/ConfigDb.cc

$(OBJDIR)/ConfigAnalyzer.o : mkobjdir libConfig/private/ConfigAnalyzer.cc   libConfig/public/ConfigAnalyzer.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/ConfigAnalyzer.o libConfig/private/ConfigAnalyzer.cc

$(OBJDIR)/RuleDb.o : mkobjdir libRule/private/RuleDb.cc libRule/public/RuleDb.hh libConfig/public/ConfigDb.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/RuleDb.o libRule/private/RuleDb.cc

$(OBJDIR)/RuleAnalyzer.o : mkobjdir libRule/private/RuleAnalyzer.cc   libRule/public/RuleAnalyzer.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/RuleAnalyzer.o libRule/private/RuleAnalyzer.cc

$(OBJDIR)/FileDb.o : mkobjdir libFile/private/FileDb.cc libFile/public/FileDb.hh libConfig/public/ConfigDb.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/FileDb.o libFile/private/FileDb.cc

$(OBJDIR)/DiffFileDb.o : mkobjdir libFile/private/DiffFileDb.cc libFile/private/DiffFileDb.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/DiffFileDb.o libFile/private/DiffFileDb.cc

$(OBJDIR)/FileAnalyzer.o : mkobjdir libFile/private/FileAnalyzer.cc   libFile/public/FileAnalyzer.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/FileAnalyzer.o libFile/private/FileAnalyzer.cc

$(OBJDIR)/Format.o : mkobjdir libFormat/private/Format.cc libFormat/public/Format.hh libConfig/public/ConfigDb.hh libRule/public/RuleDb.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/Format.o libFormat/private/Format.cc

$(OBJDIR)/Main.o : mkobjdir libFlow/private/Main.cc
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/Main.o libFlow/private/Main.cc

# Clean
clean:
	rm -rf ${OBJDIR} ${BINDIR}
