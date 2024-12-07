#include "Interface/TCPClient.h"
#include "config.hpp"
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <iomanip>
using namespace std;

TCPClient processingServer(IP_ADDRESS, PORT);
  
string floatToString(float num) {
    stringstream ss;
    ss << fixed << num;
    string str = ss.str();

    // Eliminar ceros finales innecesarios
    str.erase(str.find_last_not_of('0') + 1, string::npos);
    // Si termina en punto decimal, también eliminarlo
    if (str.back() == '.') {
        str.pop_back();
    }

    return str;
}

// Función para convertir una cadena de texto a un vector de enteros
void convertirTextoAVector(const string &texto, vector<float> &vectorNumeros) {
  vectorNumeros.clear(); // Limpiar el vector antes de llenarlo
  stringstream ss(texto);
  string segmento;

  while (getline(ss, segmento, ':')) {
    vectorNumeros.push_back(
        stof(segmento)); // Convertir cada segmento a un número 
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
  bool flag_1 = false;
  bool flag_2 = false;
  while (true) {
    ss.str("");
    message = processingServer.receive(1);
    if (message.empty())
      break;
    switch (message[0]) {
    case 'E':{
      flag_1 = flag_2 = false;
        cout << "Confirmacion de que se proceso una Columna, reinicio de variables" << endl;
        Col_part.clear();
        Row_part.clear();
        i = 0;
        break;
    }
    case 'M': {
      char tipo = processingServer.receive(1)[0];
      if (tipo == 'C') {
        col = stoi(processingServer.receive(5));
        int size = stoi(processingServer.receive(5));
        string data(size, '\0');
        data = processingServer.receive(size);
        // cout <<"tipo:" << tipo << " ";
        // cout << "id" << id << " - " << data << endl;
        fflush(stdout);
        convertirTextoAVector(data, Col_part);
        i++;
        // cout << i<< endl;
        flag_1 = true;
      } else if (tipo == 'F') {
        id = stoi(processingServer.receive(5));
        int size = stoi(processingServer.receive(5));
        string data(size, '\0');
        data = processingServer.receive(size);
        // cout <<"tipo:" << tipo << " ";
        // cout << "id" << id << " - " << data << endl;
        fflush(stdout);
        Row_part.clear();
        convertirTextoAVector(data, Row_part);
        flag_2 = true;
      }
      if (flag_1 && flag_2) {
        flag_2 = false;
        // cout << "Calculando producto y suma de vectores" << endl;
        // cout << i << endl;
        float resultado = productoSumaVectores(Row_part, Col_part);

        string resultadoStr = floatToString(resultado);;

        // Obtener la longitud del número flotante como cadena
        int longitud = resultadoStr.length();

        // Formatear la salida con longitud y número flotante
        ss << "r" << setw(5) << setfill('0') << id;
        ss << setw(8) << setfill('0') << longitud << resultadoStr;        
        processingServer.send(ss.str());
        //cout << "Resultado: " << id << " - " << ss.str() << endl;
      }

      break;
    }
    default: {
      // cout << "Unknown message" << endl;
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
    // cout << "Escribe exit para desconectarte: ";
    getline(cin, message);
    if (message == "exit") {
      processingServer.send("o");
      break;
    }
  }
  return 0;
}