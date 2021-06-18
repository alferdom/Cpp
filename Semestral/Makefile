TARGET = alferdom

CXX = g++
MKDIR = mkdir -p

CXXFLAGS = -Wall -pedantic -Wextra -std=c++17
LDLIBS = -lncurses

SOURCE_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
HEADERS = $(wildcard $(SOURCE_DIR)/*.h)

OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.cpp=$(BUILD_DIR)/%.o)


.PHONY: all
all: compile doc
	@echo "Finished..."


.PHONY: compile
compile: $(TARGET)
	@echo "Compile..."


.PHONY: run
run: $(TARGET)
	./$(TARGET)


$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDLIBS)


$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -c -o $@


doc: Doxyfile $(HEADERS)
	doxygen Doxyfile

EXAMPLES = $(wildcard examples/*)
$(TARGET).zip: README.md zadani.txt prohlaseni.txt Makefile Doxyfile $(HEADERS) $(SOURCES) $(EXAMPLES)
	mkdir -p .archive/$(TARGET)/
	cp -r README.md zadani.txt prohlaseni.txt Makefile Doxyfile src/ examples/ .archive/$(TARGET)/
	cd .archive/; zip -r ../$(TARGET).zip $(TARGET)/
	rm -r .archive/

.PHONY: clean
clean:
	rm -rf $(TARGET) $(TARGET).zip doc/ .archive/ $(BUILD_DIR)/ 2>/dev/null

build/CApplication.o: src/CApplication.cpp src/CApplication.h src/CInterface.h \
 src/EDirection.h src/CCoord.h src/CScoreboard.h
build/CBonus.o: src/CBonus.cpp src/CBonus.h src/CCoord.h
build/CBullet.o: src/CBullet.cpp src/CBullet.h src/CCoord.h src/EField.h \
 src/EDirection.h
build/CCharacter.o: src/CCharacter.cpp src/CCharacter.h src/CCoord.h \
 src/CBonus.h src/EDirection.h src/EField.h src/CInterface.h \
 src/CScoreboard.h src/CBullet.h
build/CGame.o: src/CGame.cpp src/CGame.h src/CMap.h src/CCharacter.h \
 src/CCoord.h src/CBonus.h src/EDirection.h src/EField.h src/CInterface.h \
 src/CScoreboard.h src/CBullet.h src/CMapBuilder.h
build/CInterface.o: src/CInterface.cpp src/CInterface.h src/EDirection.h \
 src/CCoord.h src/CScoreboard.h src/CGame.h src/CMap.h src/CCharacter.h \
 src/CBonus.h src/EField.h src/CBullet.h src/CMapBuilder.h
build/CMap.o: src/CMap.cpp src/CMap.h
build/CMapBuilder.o: src/CMapBuilder.cpp src/CMapBuilder.h src/CMap.h
build/CScoreboard.o: src/CScoreboard.cpp src/CScoreboard.h
build/main.o: src/main.cpp src/CApplication.h src/CInterface.h src/EDirection.h \
 src/CCoord.h src/CScoreboard.h

