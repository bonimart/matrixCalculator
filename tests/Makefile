TARGET=$(BUILD_DIR)/test

CXX=g++
LD=g++
CXXFLAGS=-Wall -pedantic -Wextra -std=c++17 -g
MKDIR=mkdir -p

SOURCE_DIR=../src
BUILD_DIR=../build/test

TEST_TARGET=$(BUILD_DIR)/test

#https://stackoverflow.com/questions/2483182/recursive-wildcards-in-gnu-make
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SOURCE=$(call rwildcard, $(SOURCE_DIR), *.cpp)
HEADER=$(call rwildcard, $(SOURCE_DIR), *.h)
OBJECT=$(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCE))

.PHONY: test
test: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJECT)
	$(LD) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/main.o: ./main.cpp
	$(MKDIR) $(dir $(OBJECT))
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(MKDIR) $(dir $(OBJECT))
	$(CXX) $(CXXFLAGS) $< -c -o $@

deps:
	$(CXX) -MM $(SOURCE) > Makefile.d

-include Makefile.d