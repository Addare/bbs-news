#
# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#
# Define the compiler. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstdc++.
CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g
#CXXFLAGS =  -stdlib=libc++
#CPPFLAGS =  -stdlib=libc++
#CXXFLAGS += -stdlib=libc++

PROGS = client servermain servermaindisc

all: $(PROGS)

# Create the library; ranlib is for Darwin (OS X) and maybe other systems.
# Doesn't seem to do any damage on other systems.

libclientserver.a: connection.o server.o
	ar rv libclientserver.a  connection.o server.o
	ranlib libclientserver.a

client: client.o connection.o messagehandler.o
servermain: servermain.o dbinterface.o database.o discdatabase.o article.o newsgroup.o connection.o messagehandler.o server.o
servermaindisc: servermaindisc.o dbinterface.o database.o discdatabase.o article.o newsgroup.o connection.o messagehandler.o server.o


# Phony targets
.PHONY: all clean

# Standard clean
clean:
	rm -f *.o $(PROGS)

# Generate dependencies in *.d files
%.d: %.cc
	@set -e; rm -f $@; \
         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

# Include the *.d files
SRC = $(wildcard *.cc)
include $(SRC:.cc=.d)
