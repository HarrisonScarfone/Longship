CC := g++

ENGINE_NAME := longship
DEBUG_ENGINE := longship
PERFT := perft

ENGINE_MAIN := src/main.cpp
PERFT_MAIN := src/perft.cpp

C1FLAGS := -Ofast
C2FLAGS := -g

OUTPUT_DIR := build

src := $(wildcard src/*.cpp)
src := $(filter-out src/main.cpp, $(src))
src := $(filter-out src/perft.cpp, $(src))
dep := $(wildcard src/*.h)

.PHONY: engine optengine dengine perft dperft clean
engine: $(ENGINE_MAIN) $(src) $(dep)
	$(CC) -o $(OUTPUT_DIR)/$(ENGINE_NAME) $(ENGINE_MAIN) $(src) $(dep)

optengine: $(ENGINE_MAIN) $(src) $(dep)
	$(CC) $(C1FLAGS) -o $(OUTPUT_DIR)/$(ENGINE_NAME) $(ENGINE_MAIN) $(src) $(dep)

dengine: $(ENGINE_MAIN) $(src) $(dep)
	$(CC) $(C2FLAGS) -o $(OUTPUT_DIR)/$(ENGINE_NAME) $(ENGINE_MAIN) $(src) $(dep)

perft: $(PERFT_MAIN) $(src) $(dep)
	$(CC) -o $(OUTPUT_DIR)/$(PERFT) $(PERFT_MAIN) $(src) $(dep)

dperft: $(PERFT_MAIN) $(src) $(dep)
	$(CC) $(C2FLAGS) -o $(OUTPUT_DIR)/$(PERFT) $(PERFT_MAIN) $(src) $(dep)

clean:
	rm -rf build/*


