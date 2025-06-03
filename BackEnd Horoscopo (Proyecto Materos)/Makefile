# Makefile para compilar el programa con Swiss Ephemeris
# Asume:
# - Swiss Ephemeris está clonada en ./swisseph_src
# - Los headers se encuentran en swisseph_src/src
# - La librería está en swisseph_src/src (por ejemplo, libswedll.a o libswe.a)

CXX = g++
CXXFLAGS = -std=c++11 -I./swisseph_src/src
LDFLAGS = -L./swisseph_src/src -lswe

TARGET = mi_programa
SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
