CXX     = g++
CXXSTD  = -std=c++17 -O2 -Wall
SYSLIBS = -lGL -lm -lpthread -ldl -lrt -lX11

FONT_REGULAR = /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf
FONT_BOLD    = /usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf

TARGET = atraso_maximo
SRC    = main.cpp

all: check-deps $(TARGET)

check-deps:
	@echo "--- Checking dependencies ---"
	@# raylib
	@LIB=$$(find "$(HOME)" /usr /opt -name "libraylib.a" 2>/dev/null | head -1); \
	if [ -z "$$LIB" ]; then \
		echo ""; \
		echo "ERROR: libraylib.a not found."; \
		echo "  Build raylib from source:"; \
		echo "    git clone https://github.com/raysan5/raylib && cd raylib/src"; \
		echo "    make PLATFORM=PLATFORM_DESKTOP && sudo make install"; \
		echo ""; \
		exit 1; \
	else \
		echo "  [OK] raylib: $$LIB"; \
	fi
	@# fonts
	@if [ ! -f "$(FONT_REGULAR)" ] || [ ! -f "$(FONT_BOLD)" ]; then \
		echo ""; \
		echo "ERROR: DejaVu fonts not found at /usr/share/fonts/truetype/dejavu/."; \
		echo "  Install with:"; \
		echo "    sudo apt install fonts-dejavu-core   # Debian/Ubuntu"; \
		echo "    sudo dnf install dejavu-sans-fonts   # Fedora"; \
		echo "    sudo pacman -S ttf-dejavu             # Arch"; \
		echo ""; \
		exit 1; \
	else \
		echo "  [OK] fonts: DejaVuSans.ttf, DejaVuSans-Bold.ttf"; \
	fi
	@# g++
	@if ! command -v $(CXX) >/dev/null 2>&1; then \
		echo ""; \
		echo "ERROR: $(CXX) not found."; \
		echo "  Install with: sudo apt install build-essential"; \
		echo ""; \
		exit 1; \
	else \
		echo "  [OK] compiler: $$($(CXX) --version | head -1)"; \
	fi
	@# OpenGL / X11
	@for lib in libGL.so libX11.so; do \
		if ! ldconfig -p 2>/dev/null | grep -q "$$lib"; then \
			echo "  [WARN] $$lib not found in ldconfig — compilation may fail."; \
			echo "    Install with: sudo apt install libgl-dev libx11-dev"; \
		fi; \
	done
	@echo "--- All checks passed ---"
	@echo ""

$(TARGET): $(SRC)
	@LIB=$$(find "$(HOME)" /usr /opt -name "libraylib.a" 2>/dev/null | head -1); \
	INC=$$(dirname "$$LIB")/include; \
	echo "Compiling..."; \
	$(CXX) $(CXXSTD) -I"$$INC" $(SRC) -o $(TARGET) "$$LIB" $(SYSLIBS) && \
	echo "Done: ./$(TARGET)"

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
