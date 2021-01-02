EMD_HEADERS = headers/
EMD_MODULES = modules/

HEADERS = $(EMD_HEADERS)common.hpp $(EMD_HEADERS)handle-input.hpp $(EMD_HEADERS)wasserstein.hpp $(EMD_HEADERS)search.hpp
SOURCE =  emd.cpp $(EMD_MODULES)wasserstein.cpp $(EMD_MODULES)handle-input.cpp $(EMD_MODULES)read-file.cpp $(EMD_MODULES)search.cpp
OBJS = emd.o handle-input.o read-file.o wasserstein.o search.o

CXX = g++
FLAGS = -std=c++11 -O3 -g -c -Wall
OUT = search

all: $(OUT)

# Executables
search : $(OBJS)
	$(CXX) $(FLAGS) -shared $(OBJS) -o search -framework Python

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

search.o : $(EMD_MODULES)search.cpp
	$(CXX) $(FLAGS) $(EMD_MODULES)search.cpp

clean:
	rm -f $(OBJS) $(OUT)