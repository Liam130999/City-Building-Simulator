all: clean run

# The run target links all the object files into the final executable
run: bin/main.o bin/Settlement.o bin/Facility.o bin/SelectionPolicy.o bin/Plan.o bin/Simulation.o bin/Auxiliary.o bin/Action.o
	g++ -o bin/main bin/main.o bin/Settlement.o bin/Facility.o bin/SelectionPolicy.o bin/Plan.o bin/Simulation.o bin/Auxiliary.o bin/Action.o

# Compiling main.cpp into main.o
bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

# Compiling Settlement.cpp into Settlement.o
bin/Settlement.o: src/Settlement.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Settlement.o src/Settlement.cpp

# Compiling Facility.cpp into Facility.o
bin/Facility.o: src/Facility.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Facility.o src/Facility.cpp


# Compiling SelectionPolicy.cpp into SelectionPolicy.o
bin/SelectionPolicy.o: src/SelectionPolicy.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp

# Compiling Plan.cpp into Plan.o
bin/Plan.o: src/Plan.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Plan.o src/Plan.cpp

# Compiling Simulation.cpp into Simulation.o
bin/Simulation.o: src/Simulation.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp

# Compiling Auxiliary.cpp into Auxiliary.o
bin/Auxiliary.o: src/Auxiliary.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp


# Compiling Action.cpp into Action.o
bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

# Clean target to remove the object files and the final executable
clean:
	rm -f bin/*

