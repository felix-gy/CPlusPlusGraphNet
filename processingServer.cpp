#include "Interface/TCPClient.h"
#include "config.hpp"
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <iomanip>
using namespace std;

TCPClient processingServer(IP_ADDRESS, PORT);

// Función para convertir una cadena de texto a un vector de enteros
void convertirTextoAVector(const string &texto, vector<float> &vectorNumeros) {
  vectorNumeros.clear(); // Limpiar el vector antes de llenarlo
  stringstream ss(texto);
  string segmento;

  while (getline(ss, segmento, ':')) {
    vectorNumeros.push_back(
        stoi(segmento)); // Convertir cada segmento a un número entero
  }
}

// Función para calcular el producto y la suma de dos vectores
float productoSumaVectores(const vector<float> &fila,
                           const vector<float> &columna) {
  if (fila.size() != columna.size()) {
    cerr << "Los vectores deben tener el mismo tamaño." << endl;
    return -1; // Indicar un error en caso de tamaños diferentes
  }

  float resultado = 0;
  for (size_t i = 0; i < fila.size(); ++i) {
    resultado += fila[i] * columna[i];
  }

  return resultado;
}

void manageProcessingServer() {
  string message;
  vector<float> Col_part;
  vector<float> Row_part;
  stringstream ss;
  int i = 0;
  int col = 0;
  int row = 0;
  int id = 0;
  while (true) {
    ss.str("");
    message = processingServer.receive(1);
    if (message.empty())
      break;
    switch (message[0]) {
    case 'O':{
        cout << "Reincio" << endl;
        i = 0;
        break;
    }
    case 'M': {

      char tipo = processingServer.receive(1)[0];
      if (tipo == 'C') {
        col = stoi(processingServer.receive(5));
        int size = stoi(processingServer.receive(5));
        string data = processingServer.receive(size);
        convertirTextoAVector(data, Col_part);
        i++;
      } else if (tipo == 'F') {
        id = stoi(processingServer.receive(5));
        int size = stoi(processingServer.receive(5));
        string data = processingServer.receive(size);
        convertirTextoAVector(data, Row_part);
        i++;
      }
      if (i >= 2) {

        float resultado = productoSumaVectores(Row_part, Col_part);
        // Convertir el número flotante a una cadena para calcular su longitud
        stringstream tempStream;
        tempStream << fixed << setprecision(7)<< resultado; // Controla la precisión aquí
        string resultadoStr = tempStream.str();

        // Obtener la longitud del número flotante como cadena
        int longitud = resultadoStr.length();

        // Formatear la salida con longitud y número flotante
        ss << "r" <<  setw(5) << setfill('0') << id << setw(15) << setfill('0') << longitud << resultadoStr;
        processingServer.send(ss.str());
        
      }

      break;
    }
    default: {
      cout << "Unknown message" << endl;
      break;
    }
    }
  }
}

int main() {
  processingServer.connectToServer();
  string message = "P";
  processingServer.send(message);
  message = "";
  thread(manageProcessingServer).detach();
  while (true) {
    cout << "Enter message: ";
    getline(cin, message);
    if (message == "exit") {
      processingServer.send("o");
      break;
    }
  }
  return 0;
}