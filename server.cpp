#include "Interface/TCPServer.h"
#include "config.hpp"
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>
#include <set>
///
#include <sstream> 
#include <iomanip>
#include <sstream>
#include <array>
#include <map>
#include <mutex>
#include <numeric>
#include <vector>
using namespace std;

TCPServer server(PORT);

vector<int> processingServerList; 
int clientsoket;

// Servidores de Procesamiento
string convertirArrayATexto(const array<float, H_rows>& M_col) {
    stringstream ss;

    for (size_t i = 0; i < M_col.size(); ++i) {
        if (M_col[i] == static_cast<int>(M_col[i])) {
            // Si el número es un entero, almacénalo como entero
            ss << static_cast<int>(M_col[i]);
        } else {
            // Si el número tiene parte decimal, almacénalo con la precisión adecuada
            ss << M_col[i];
        }

        if (i < M_col.size() - 1) {
            ss << ":";  // Añadir ':' entre números
        }
    }

    return ss.str();
}

void limpiarArray(array<float, H_rows>& M_col) {
    for (size_t i = 0; i < M_col.size(); ++i) {
        M_col[i] = 0.0f;
    }
}

mutex mtx;
vector<float> Row_part;
int count = 0;
array<float, H_rows> M_col;
int col = 0;

void manageProcessingServer(int PServerSocket)
{
    cout << "Processing server connected - " << PServerSocket << endl;
    string message;
    stringstream ss;
    while (true) {

        message = server.receive(PServerSocket, 1);
        if (message.empty()) break;
        switch (message[0]) {
            case 'o':{
                //processingServerList.erase(PServerSocket);
                cout << "Processing server disconnected - "<< PServerSocket << endl;
                break;
            }
            case 'r':{
                fflush(stdout);
                int id = stoi(server.receive(PServerSocket, 5));
                int n_size = stoi(server.receive(PServerSocket, 8));
                //cout << "El tamaño del resultado es " << n_size << endl;
                string data( n_size, '\0');
                data  = server.receive(PServerSocket, n_size);
                //cout << "Resultado de uno: " << id << " - " << data << endl;
                fflush(stdout);
                unique_lock<std::mutex> lock(mtx);
                Row_part.push_back(stof(data));
                count++;
                if (count == 4) {
                    float suma = accumulate(Row_part.begin(), Row_part.end(), 0.0f);
                    M_col[id] = suma;
                    fflush(stdout);
                    count = 0;
                    Row_part.clear();

                    if ((id+1) == H_rows) {
                        cout  << id<< " - " <<  suma << " " ;
                        string data = convertirArrayATexto(M_col);
                        //cout << "Resultado final: " << id<<  " - "<< data << endl;
                        fflush(stdout);
                        limpiarArray(M_col);
                        ss.str("");
                        ss << "R" << setw(5) << setfill('0') << id << setw(5) << setfill('0') << data.length() << data;
                        server.send(ss.str(),clientsoket); // enviar resultado Columna al cliente
                        for (auto& sock : processingServerList) {
                            server.send( "E",sock); // fin de la operacion Columna y fila
                        }
                    }
                    else {
                        server.send( "A",clientsoket); // confimacion de recepcion
                    }
                }
                lock.unlock(); 
                break;
            }
            default:{
                cout << "Unknown message" << endl;
                break;
            }
        }
    }
}

// Función para dividir un texto en 4 partes iguales
vector<string> dividirEnCuatroPartes(const string& data) {
    vector<string> parts(4);
    parts.clear();
    stringstream ss(data);
    string item;
    vector<string> tokens;
    // Dividir la cadena por ':'
    while (getline(ss, item, ':')) {
        tokens.push_back(item);
    }
    size_t totalTokens = tokens.size();
    size_t minSize = totalTokens / 4;
    size_t remainder = totalTokens % 4;
    size_t currentToken = 0;
    for (size_t i = 0; i < 4; ++i) {
        size_t partSize = minSize + (i < remainder ? 1 : 0); 

        for (size_t j = 0; j < partSize; ++j) {
            if (!parts[i].empty()) {
                parts[i] += ":";
            }
            parts[i] += tokens[currentToken++];
        }
    }
    return parts;
}
// Cliente
void manageClient(int clientSocket)
{
    vector<string> partes(4);
    string message;
    stringstream ss;
    int i=0;
    while (true) {
        ss.str("");
        message = server.receive(clientSocket, 1);
        if (message.empty()) break;
        switch (message[0]) {
            case 'o':{
                cout << "Client disconnected - "<< clientSocket << endl;
                return;
            }
            case 'm':{
                cout << endl;
                char tipo = server.receive(clientSocket, 1)[0];
                //cout << "Tipo: " << tipo << endl;
                int id = stoi(server.receive(clientSocket, 5));
                int size = stoi(server.receive(clientSocket, 5));
                string data = server.receive(clientSocket, size);
                //cout << "Received data: " << data << endl;
                fflush(stdout);
                i = 0;
                partes = dividirEnCuatroPartes(data); 
                for (auto& sock : processingServerList) {
                    ss.str(""); 
                    ss << "M" << tipo << setw(5) << setfill('0') << id << setw(5) << setfill('0') << partes[i].size() << partes[i];
                    //cout << "Enviando a: " << sock << ss.str() << endl;
                    server.send(ss.str(), sock);
                    
                    i++;
                }
                if (tipo == 'C') {
                    
                    server.send( "A",clientSocket); // confimacion de recepcion
                    //cout << "Se encvia a " << clientSocket << endl;
                }
                break;
            }
            default:{
                cout << "Unknown message" << endl;
                break;
            }
        }
    }
}

void socketThread(int clientSocket)
{
    string message = server.receive(clientSocket, 1);
    if (message == "P") { // Para los servidores de procesamiento
        processingServerList.push_back(clientSocket);
        cout << "Processing server connected - " << clientSocket << endl;
        fflush(stdout);
        manageProcessingServer(clientSocket);
    }
    else if (message == "C") { // Para los clientes
        clientsoket = clientSocket;
        cout << "Client connected - " << clientSocket << endl;
        manageClient(clientSocket);
    }

}

int main()
{

    server.start();
    
    int clientSocket;
    while (true) {
        clientSocket = server.connectToClient();
        if (clientSocket == -1) {
            cout << "Error accepting connection" << endl;
            continue;
        }
        thread(socketThread, clientSocket).detach();
    }
    return 0;
}