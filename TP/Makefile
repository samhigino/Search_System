# --------------------------------------------------------------
# VARIÁVEIS DE AMBIENTE
# --------------------------------------------------------------
CXX = g++
CXXFLAGS = -std=c++11 -Iinclude

# --------------------------------------------------------------
# DIRETÓRIOS
# --------------------------------------------------------------
BIN_DIR = bin
INC_DIR = inc
OBJ_DIR = obj
SRC_DIR = src

# --------------------------------------------------------------
# OBJETOS
# --------------------------------------------------------------
TARGET = tp3.out
MAIN_OBJ = obj/main.o 

# --------------------------------------------------------------
# COMPILAÇÃO
# --------------------------------------------------------------
all: dirs $(MAIN_OBJ) 
	$(CXX) $(CXXFLAGS) $(MAIN_OBJ) -o $(BIN_DIR)/$(TARGET)

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

obj/main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

ex:
	$(BIN_DIR)/$(TARGET)

clean: 
	@rm -f $(BIN_DIR)/*.out
	@rm -f $(OBJ_DIR)/*.o