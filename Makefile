CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
EXE = tema2

AUTHOR = VanceaCosminRazvan
GROUP = 313CA
HOMEWORK = 2

build: main.o Driver.o DriverPool.o solver.o
	$(CXX) $(CXXFLAGS) -o $(EXE) $^

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c $^

Driver.o: src/Driver.cpp
	$(CXX) $(CXXFLAGS) -c $^

DriverPool.o: src/DriverPool.cpp
	$(CXX) $(CXXFLAGS) -c $^

solver.o: src/solver.cpp
	$(CXX) $(CXXFLAGS) -c $^


pack: Makefile README.md
	zip -q -9 -r "$(GROUP)_$(AUTHOR)_Tema$(HOMEWORK).zip" src/ $^

clean_objects:
	rm -f *.o
clean: clean_objects
	rm -f $(EXE)
	rm -f time.out
