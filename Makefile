main: SimdTest.o
	gcc -o SimdTest  SimdTest.o -lstdc++
	rm SimdTest.o
SimdTest.o: SimdTest.cpp
	gcc -c -o SimdTest.o SimdTest.cpp
clean:
	rm SimdTest
