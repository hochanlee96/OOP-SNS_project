sns: sns.o instance.o datastructures.o
	g++ sns.o instance.o datastructures.o -o sns

sns.o: sns.cpp
	g++ -c sns.cpp

instance.o: instance.cpp
	g++ -c instance.cpp

datastructures.o: datastructures.cpp datastructures.h
	g++ -c datastructures.cpp

clean:
	rm *.o sns