CXX     = g++
CXXSTD  = -std=c++17 -O2 -Wall
SYSLIBS = -lGL -lm -lpthread -ldl -lrt -lX11

TARGET = atraso_maximo
SRC    = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	@LIB=$$(find "$(HOME)" /usr /opt -name "libraylib.a" 2>/dev/null | head -1); \
	if [ -z "$$LIB" ]; then echo "ERROR: libraylib.a not found"; exit 1; fi; \
	INC=$$(dirname "$$LIB")/include; \
	echo "Using raylib: $$LIB"; \
	$(CXX) $(CXXSTD) -I"$$INC" $(SRC) -o $(TARGET) "$$LIB" $(SYSLIBS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
