test: test.cpp SkipList.h SkipList.cpp
	g++ -o test test.cpp SkipList.cpp -std=c++11

clean:
	rm test.exe
