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
TEST_TARGET = test.out
MAIN_OBJ = obj/main.o obj/2D_point.o obj/logradouro.o obj/palavra.o obj/pesquisa_palavra.o obj/consulta.o obj/endereco.o
TEST_OBJ = obj/test.o obj/2D_point.o obj/logradouro.o obj/palavra.o obj/pesquisa_palavra.o obj/consulta.o obj/endereco.o

# --------------------------------------------------------------
# COMPILAÇÃO
# --------------------------------------------------------------
all: dirs $(MAIN_OBJ) 
	$(CXX) $(CXXFLAGS) $(MAIN_OBJ) -o $(BIN_DIR)/$(TARGET)

test: dirs $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(TEST_OBJ) -o $(BIN_DIR)/$(TEST_TARGET)
	@rm -f output/*.txt
	$(BIN_DIR)/$(TEST_TARGET)

dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

obj/main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

obj/test.o: $(SRC_DIR)/test.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/test.cpp -o $(OBJ_DIR)/test.o

obj/2D_point.o: $(SRC_DIR)/2D_point.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/2D_point.cpp -o $(OBJ_DIR)/2D_point.o

obj/logradouro.o: $(SRC_DIR)/logradouro.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/logradouro.cpp -o $(OBJ_DIR)/logradouro.o

obj/palavra.o: $(SRC_DIR)/palavra.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/palavra.cpp -o $(OBJ_DIR)/palavra.o

obj/pesquisa_palavra.o: $(SRC_DIR)/pesquisa_palavra.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/pesquisa_palavra.cpp -o $(OBJ_DIR)/pesquisa_palavra.o

obj/consulta.o: $(SRC_DIR)/consulta.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/consulta.cpp -o $(OBJ_DIR)/consulta.o

obj/endereco.o: $(SRC_DIR)/endereco.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/endereco.cpp -o $(OBJ_DIR)/endereco.o	

ex:
	$(BIN_DIR)/$(TARGET)

runtest:
	$(BIN_DIR)/$(TEST_TARGET)

clean: 
	@rm -f $(BIN_DIR)/*.out
	@rm -f $(OBJ_DIR)/*.o
	@rm -f output/*.txt