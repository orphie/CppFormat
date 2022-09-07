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

ifndef CXXFLAGS
    CXXFLAGS=-std=c++14 -g  \
                -Wall -Wno-sign-compare -Wno-unknown-pragmas -Wno-reorder -I${PWD}
endif

# Lib: multi-thread; boost
LIBS+=-lpthread -L/usr/lib/x86_64-linux-gnu/ -lboost_program_options 


# Directories
LIBDATABASEOBJ  =       $(OBJDIR)/Main.o \
                        $(OBJDIR)/FormatDb.o  \

LIBCONFIGOBJ    =       $(OBJDIR)/Config.o \
                        $(OBJDIR)/OptionParser.o

LIBRULESOBJ     =       $(OBJDIR)/Rules.o \

LIBFORMATOBJ    =       $(OBJDIR)/Format.o \

# Binary
cppFormat: $(LIBDATABASEOBJ) $(LIBCONFIGOBJ) $(LIBRULESOBJ) $(LIBFORMATOBJ)
	$(CXX) $(CXXFLAGS) -o cppFormat \
           $(LIBDATABASEOBJ) $(LIBCONFIGOBJ) $(LIBRULESOBJ) $(LIBFORMATOBJ) \
           $(LIBS)

# Object files 
mkobjdir  :
	mkdir -p $(OBJDIR)

$(OBJDIR)/OptionParser.o : mkobjdir libConfig/private/OptionParser.cc   libConfig/private/OptionParser.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/OptionParser.o libConfig/private/OptionParser.cc

$(OBJDIR)/Config.o : mkobjdir libConfig/private/Config.cc   libConfig/public/Config.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/Config.o libConfig/private/Config.cc

$(OBJDIR)/Main.o : mkobjdir libDatabase/private/Main.cc libDatabase/public/FormatDb.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/Main.o libDatabase/private/Main.cc

$(OBJDIR)/FormatDb.o : mkobjdir libDatabase/private/FormatDb.cc   libDatabase/public/FormatDb.hh libConfig/public/Config.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/FormatDb.o libDatabase/private/FormatDb.cc

$(OBJDIR)/Rules.o : mkobjdir libRules/private/Rules.cc libRules/public/Rules.hh libConfig/public/Config.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/Rules.o libRules/private/Rules.cc

$(OBJDIR)/Format.o : mkobjdir libFormat/private/Format.cc libFormat/public/Format.hh libConfig/public/Config.hh libRules/public/Rules.hh
	$(CXX) $(CXXFLAGS) -c -o ${OBJDIR}/Format.o libFormat/private/Format.cc

# Clean
clean:
	rm -rf ${OBJDIR} cppFormat
