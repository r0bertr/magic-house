CC := g++
BIN := bin
INCLUDE := include
EXE := $(BIN)/MagicHouse

ROOT_BUILD := build
LIB_BUILD := $(ROOT_BUILD)/lib
SPRITES_BUILD := $(ROOT_BUILD)/Sprites

ROOT_SRC := src
LIB_SRC := $(ROOT_SRC)/lib
SPRITES_SRC := $(ROOT_SRC)/Sprites

ROOT_OBJ := $(patsubst $(ROOT_SRC)/%.cpp, $(ROOT_BUILD)/%.o, $(wildcard $(ROOT_SRC)/*.cpp))
LIB_OBJ := $(patsubst $(LIB_SRC)/%.cpp, $(LIB_BUILD)/%.o, $(wildcard $(LIB_SRC)/*.cpp))
SPRITES_OBJ := $(patsubst $(SPRITES_SRC)/%.cpp, $(SPRITES_BUILD)/%.o, $(wildcard $(SPRITES_SRC)/*.cpp))

OBJ := $(ROOT_OBJ) $(LIB_OBJ) $(SPRITES_OBJ)

MACOSX_FLAGS = -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

run: $(EXE)
	@$(EXE)

$(EXE): $(OBJ)
	@mkdir -p $(BIN)
	$(CC) $^ -o $@ -lglfw $(MACOSX_FLAGS)

$(ROOT_BUILD)/%.o: $(ROOT_SRC)/%.cpp
	@mkdir -p $(ROOT_BUILD)
	@mkdir -p $(LIB_BUILD)
	@mkdir -p $(SPRITES_BUILD)
	$(CC) -I$(INCLUDE) -c $^ -o $@

$(LIB_BUILD)/%.o: $(LIB_SRC)/%.cpp
	$(CC) -I$(INCLUDE) -c $^ -o $@

$(SPRITES_BUILD)/%.o: $(SPRITES_SRC)/%.cpp
	$(CC) -I$(INCLUDE) -c $^ -o $@

all: clean $(EXE)

build: $(EXE)

rebuild: clean $(EXE)

clean:
	@rm -rf $(ROOT_BUILD)
	@rm -rf $(BIN)
