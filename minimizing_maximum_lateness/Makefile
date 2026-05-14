RAYLIB_DIR = /home/zecarioco/Documentos/Code/Algorimos\ Avançados/backtracking/raylib/build/raylib

CXX     = g++
CXXFLAGS = -std=c++17 -O2 -Wall \
            -I$(RAYLIB_DIR)/include
LDFLAGS  = $(RAYLIB_DIR)/libraylib.a \
            -lGL -lm -lpthread -ldl -lrt -lX11

TARGET  = atraso_maximo
SRC     = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
