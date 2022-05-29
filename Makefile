TARGET=benesm44

CXX=g++
LD=g++
CXXFLAGS=-Wall -pedantic -Wextra -std=c++17
MKDIR=mkdir -p

SOURCE_DIR=src
BUILD_DIR=build

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SOURCE=$(call rwildcard, $(SOURCE_DIR), *.cpp)
HEADER=$(call rwildcard, $(SOURCE_DIR), *.h)
OBJECT=$(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCE))

.PHONY: all
all: compile doc

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: compile
compile: $(TARGET)

$(TARGET): $(OBJECT)
	$(LD) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(MKDIR) $(dir $(OBJECT))
	$(CXX) $(CXXFLAGS) $< -c -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/ $(TARGET)

.PHONY: doc
doc: Doxyfile README.md $(HEADER)
	doxygen Doxyfile


	