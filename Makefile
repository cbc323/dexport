all: bin dexport_objs

bin:
	mkdir bin

dexport_objs:
	$(MAKE) -C dexport all

dexport_bin:
	g++ -o bin/dexport -ltsk *.o

test:test.cpp
	g++ -m64 -g -Wall -std=c++1y -ltsk -o test test.cpp

clean:
	$(MAKE) -C dexport clean
	rm -rf bin
