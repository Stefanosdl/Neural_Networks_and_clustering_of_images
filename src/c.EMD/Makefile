EMD_HEADERS = headers/
EMD_MODULES = modules/

COMMONOBJS = handle-input.o read-file.o wasserstein.o

HEADERS = $(EMD_HEADERS)common.hpp $(EMD_HEADERS)handle-input.hpp $(EMD_HEADERS)wasserstein.hpp
SOURCE =  emd.cpp $(EMD_MODULES)wasserstein.cpp $(EMD_MODULES)handle-input.cpp $(EMD_MODULES)read-file.cpp
OBJS = emd.o $(COMMONOBJS)

CXX = g++
FLAGS = -std=c++11 -O3 -g -c -Wall
OUT = search

all: $(OUT)

# Executables
search : $(OBJS)
	$(CXX) -g $(OBJS) -o search

# Final object files
emd.o : $(SOURCE) $(HEADERS)
	$(CXX) $(FLAGS) $(SOURCE)

# FOR EMD
wasserstein.o : $(EMD_MODULES)wasserstein.cpp
	$(CXX) $(FLAGS) $(EMD_MODULES)wasserstein.cpp

handle-input.o : $(EMD_MODULES)handle-input.cpp
	$(CXX) $(FLAGS) $(EMD_MODULES)handle-input.cpp

read-file.o : $(EMD_MODULES)read-file.cpp
	$(CXX) $(FLAGS) $(EMD_MODULES)read-file.cpp

clean:
	rm -f $(OBJS) $(OUT)