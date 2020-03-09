

CXX=g++-7
CXXFLAGS=-Wall -Wextra -pedantic -Werror -std=c++17 -O3 -g
LDFLAGS=$(CXXFLAGS)
OBJ=$(SRC:.cc=.o)

all: test

test: cache_lib.o test_cache_lib.o fifo_evictor.o LRU_evictor.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	-rm *.o $(objects)
