COMMONOBJS = manhattan-distance.o handle-input.o read-file.o kmeansPP.o silhouette.o

HEADER = headers/handle-input.hpp headers/kmeansPP.hpp headers/common.hpp
SOURCE = cluster.cpp modules/manhattan-distance.cpp modules/handle-input.cpp modules/read-file.cpp modules/kmeansPP.cpp modules/silhouette.cpp
OBJS = cluster.o $(COMMONOBJS)

CXX = g++
FLAGS = -std=c++11 -O3 -g -c -Wall
OUT = cluster

all: $(OUT)

# Executables
cluster : $(OBJS)
	$(CXX) -g $(OBJS) -o cluster

cluster.o : $(SOURCE) $(HEADER)
	$(CXX) $(FLAGS) $(SOURCE)

manhattan-distance.o : modules/manhattan-distance.cpp
	$(CXX) $(FLAGS) modules/manhattan-distance.cpp

handle-input.o : modules/handle-input.cpp
	$(CXX) $(FLAGS) modules/handle-input.cpp

read-file.o : modules/read-file.cpp
	$(CXX) $(FLAGS) modules/read-file.cpp

kmeansPP.o : modules/kmeansPP.cpp
	$(CXX) $(FLAGS) modules/kmeansPP.cpp

silhouette.o : modules/silhouette.cpp
	$(CXX) $(FLAGS) modules/silhouette.cpp

clean:
	rm -f $(OBJS) $(OUT)