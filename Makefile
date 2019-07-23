hw1_p2: hw1_p2.o arithmetic.o
	g++ hw1_p2.o arithmetic.o -o hw1_p2

hw1_p2.o: hw1_p2.cpp
	g++ -c hw1_p2.cpp

arithmetic.o: arithmetic.cpp arithmetic.h
	g++ -c arithmetic.cpp

clean:
	rm *.o hw1_p2

