CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./Interface # Incluye la carpeta "interface"
LDFLAGS = -pthread

# Archivos fuente
SERVER_SRC = Interface/TCPServer.cpp server.cpp
CLIENT_SRC = Interface/TCPClient.cpp client.cpp
PROCESSSERVER_SRC = Interface/TCPClient.cpp processingServer.cpp

# Objetos
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
CLIENT_OBJ = $(CLIENT_SRC:.cpp=.o)
PROCESSSERVER_OBJ = $(PROCESSSERVER_SRC:.cpp=.o)

# Ejecutables
SERVER_TARGET = server
CLIENT_TARGET = client
PROCESSSERVER_TARGET = processingServer

# Compilación de todos
all: $(SERVER_TARGET) $(CLIENT_TARGET) $(PROCESSSERVER_TARGET)

# Compilación del servidor
$(SERVER_TARGET): $(SERVER_OBJ)
	$(CXX) $(SERVER_OBJ) -o $(SERVER_TARGET) $(LDFLAGS)

# Compilación del cliente
$(CLIENT_TARGET): $(CLIENT_OBJ)
	$(CXX) $(CLIENT_OBJ) -o $(CLIENT_TARGET) $(LDFLAGS)

# Compilación del proceso servidor
$(PROCESSSERVER_TARGET): $(PROCESSSERVER_OBJ)
	$(CXX) $(PROCESSSERVER_OBJ) -o $(PROCESSSERVER_TARGET) $(LDFLAGS)

# Regla para objetos
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ) $(SERVER_TARGET) $(CLIENT_TARGET) $(PROCESSSERVER_OBJ) $(PROCESSSERVER_TARGET)
