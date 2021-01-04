LSH_HEADERS = headers/
LSH_MODULES = modules/

COMMONOBJS = manhattan-hashing.o manhattan-distance.o modulo.o handle-input.o read-file.o hashtable.o

HEADER = $(LSH_HEADERS)handle-input.hpp $(LSH_HEADERS)distances.hpp $(LSH_HEADERS)manhattan-hashing.hpp $(LSH_HEADERS)modulo.hpp $(LSH_HEADERS)hashtable.hpp $(LSH_HEADERS)cluster.hpp $(LSH_HEADERS)common.hpp
SOURCE =  lsh.cpp $(LSH_MODULES)manhattan-distance.cpp $(LSH_MODULES)handle-input.cpp $(LSH_MODULES)read-file.cpp $(LSH_MODULES)manhattan-hashing.cpp $(LSH_MODULES)modulo.cpp $(LSH_MODULES)hashtable.cpp $(LSH_MODULES)cluster.cpp
OBJS = lsh.o cluster.o $(COMMONOBJS)

CXX = g++
FLAGS = -std=c++11 -O3 -g -c -Wall
OUT = cluster

all: $(OUT)

# Executables
cluster : $(OBJS)
	$(CXX) -g $(OBJS) -o cluster

# Final object files
lsh.o : $(SOURCE) $(HEADER)
	$(CXX) $(FLAGS) $(SOURCE)

# FOR LSH
manhattan-distance.o : $(LSH_MODULES)manhattan-distance.cpp
	$(CXX) $(FLAGS) $(LSH_MODULES)manhattan-distance.cpp

handle-input.o : $(LSH_MODULES)handle-input.cpp
	$(CXX) $(FLAGS) $(LSH_MODULES)handle-input.cpp

read-file.o : $(LSH_MODULES)read-file.cpp
	$(CXX) $(FLAGS) $(LSH_MODULES)read-file.cpp

manhattan-hashing.o : $(LSH_MODULES)manhattan-hashing.cpp
	$(CXX) $(FLAGS) $(LSH_MODULES)manhattan-hashing.cpp

modulo.o : $(LSH_MODULES)modulo.cpp
	$(CXX) $(FLAGS) $(LSH_MODULES)modulo.cpp

hashtable.o : $(LSH_MODULES)hashtable.cpp
	$(CXX) $(FLAGS) $(LSH_MODULES)hashtable.cpp

cluster.o : $(LSH_MODULES)cluster.cpp
	$(CXX) $(FLAGS) $(LSH_MODULES)cluster.cpp

clean:
	rm -f $(OBJS) $(OUT)