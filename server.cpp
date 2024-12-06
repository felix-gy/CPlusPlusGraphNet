#include "Interface/TCPServer.h"
#include "config.hpp"
#include <iostream>
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
using namespace std;

TCPServer server(PORT);

set<int> processingServerList; 
int clientsoket;

// Servidores de Procesamiento
string convertirArrayATexto(const array<float, H_rows>& M_col) {
    stringstream ss;

    for (size_t i = 0; i < M_col.size(); ++i) {
        ss << fixed << setprecision(7) << M_col[i];  // Establecer precisión a 3 decimales
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

void manageProcessingServer(int PServerSocket)
{
    string message;
    stringstream ss;
    while (true) {

        message = server.receive(PServerSocket, 1);
        if (message.empty()) break;
        switch (message[0]) {
            case 'o':
                processingServerList.erase(PServerSocket);
                cout << "Processing server disconnected - "<< PServerSocket << endl;
                break;
            case 'r':{
                int id = stoi(server.receive(clientsoket, 5));
                int size = stoi(server.receive(clientsoket, 15));
                float data = stof(server.receive(clientsoket, size));
                lock_guard<mutex> lock(mtx);
                Row_part.push_back(data);
                count++;
                if (count == 4) {
                    float suma = accumulate(Row_part.begin(), Row_part.end(), 0.0f);
                    string data = convertirArrayATexto(M_col);
                    // ss << "R" << setw(5) << setfill('0') << id << setw(5) << setfill('0') << data.length() << data;
                    // limpiarArray(M_col);
                    server.send(ss.str(),clientsoket);
                    server.send( "O",PServerSocket); // fin de la operacion Columna y fila
                }
                else {
                    server.send( "A",clientsoket); // confimacion de recepcion;
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

// Función para dividir un texto en 4 partes iguales
void dividirEnCuatroPartes(const string& texto, vector<string>& partes, int totalNumeros) {
    vector<string> numeros;
    stringstream ss(texto);
    string segmento;
    // Extraer los números del texto
    while (getline(ss, segmento, ':')) {
        numeros.push_back(segmento);
    }
    int tamanioParte = (totalNumeros + 3) / 4; // Calcular tamaño de cada parte, redondeando hacia arriba
    for (int i = 0; i < 4; ++i) {
        stringstream parteSS;
        for (int j = 0; j < tamanioParte; ++j) {
            int index = i * tamanioParte + j;
            if (index < totalNumeros) {
                parteSS << numeros[index];
                if (j < tamanioParte - 1 && index < totalNumeros - 1) {
                    parteSS << ":";
                }
            }
        }
        partes.push_back(parteSS.str());
    }
}

// Cliente
void manageClient(int clientSocket)
{
    vector<string> partes;
    string message;
    stringstream ss;
    int i;
    while (true) {
        ss.str("");
        message = server.receive(clientSocket, 1);
        if (message.empty()) break;
        switch (message[0]) {
            case 'o':
            //
                cout << "Client disconnected - "<< clientSocket << endl;
                break;
            case 'm':{
                char tipo = server.receive(clientSocket, 1)[0];
                int id = stoi(server.receive(clientSocket, 5));
                int size = stoi(server.receive(clientSocket, 5));
                string data = server.receive(clientSocket, size);
                i = 0;
                dividirEnCuatroPartes(data, partes, W_cols);
                for (auto& sock : processingServerList) {
                    ss << "M" << tipo << setw(5) << setfill('0') << id << setw(5) << setfill('0') << partes[i].length() << partes[i++];
                    server.send(ss.str(), sock);
                }
                cout << "Received data: " << data << endl;
                partes.clear();
                server.send( "A",clientSocket); // confimacion de recepcion
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
    string message;
    message = server.receive(clientSocket, 1);
    if (message == "P") { // Para los servidores de procesamiento
        processingServerList.insert(clientSocket);
        manageProcessingServer(clientSocket);
    }
    else if (message == "C") { // Para los clientes
        clientsoket = clientSocket;
        manageClient(clientSocket);
    }

}

int main()
{
    server.start();

    int clientSocket;
    while (true) {
        clientSocket = server.connectToClient();
        thread(socketThread, clientSocket).detach();
    }
    return 0;
}