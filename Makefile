TARGET=benesm44

CXX=g++
LD=g++
CXXFLAGS=-Wall -pedantic -Wextra -std=c++17 -g
MKDIR=mkdir -p

SOURCE_DIR=src
BUILD_DIR=build
DOC_DIR=doc

#https://stackoverflow.com/questions/2483182/recursive-wildcards-in-gnu-make
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SOURCE=$(call rwildcard, $(SOURCE_DIR), *.cpp)
HEADER=$(call rwildcard, $(SOURCE_DIR), *.h)
OBJECT=$(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCE))

.PHONY: all
all: deps compile doc

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

deps: 
	$(CXX) -MM $(SOURCE) > Makefile.d

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/ $(TARGET) $(DOC_DIR)/ $(TARGET).zip

.PHONY: doc
doc: Doxyfile README.md $(HEADER)
	doxygen Doxyfile

$(TARGET).zip: README.md zadani.txt prohlaseni.txt Makefile Doxyfile $(HEADER) $(SOURCE)
	$(MKDIR) .archive/$(TARGET)/
	cp -r README.md zadani.txt prohlaseni.txt Makefile Doxyfile $(SOURCE_DIR) .archive/$(TARGET)/
	cd .archive/; zip -r ../$(TARGET).zip $(TARGET)/
	rm -r .archive/

-include Makefile.d