TARGET=benesm44

CXX=g++
LD=g++
CXXFLAGS=-Wall -pedantic -Wextra -std=c++17
MKDIR=mkdir -p

SRC_DIR=src
BUILD_DIR=build

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC=$(call rwildcard, $(SRC_DIR), *.cpp)
OBJ=$(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))

.PHONY: all
all: compile

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: compile
compile: $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(MKDIR) $(dir $(OBJ))
	$(CXX) $(CXXFLAGS) $< -c -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/ $(TARGET)
	