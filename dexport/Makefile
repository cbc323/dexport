all: objs

objs: evidence.cpp file.cpp file_processor.cpp context.cpp dexport.cpp
	g++ -m64 -g -Wall -std=c++1y -lmagic -ltsk -o ../bin/dex context.cpp evidence.cpp file.cpp file_processor.cpp dexport.cpp

clean:
	rm -f *.o
