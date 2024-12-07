#include "Interface/TCPClient.h"
#include "config.hpp"
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
using namespace std;
TCPClient client(IP_ADDRESS, PORT);

struct Matriz {
  string nombreArchivo;
  int filas;
  int columnas;

  // Constructor que inicializa la estructura con el nombre del archivo y las
  // dimensiones de la matriz
  Matriz(const string &nombreArchivo, int filas, int columnas)
      : nombreArchivo(nombreArchivo), filas(filas), columnas(columnas) {}

  // Función que retorna una fila específica como un string en el formato
  // deseado
  string obtenerFila(int indice) const {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
      cerr << "Error al abrir el archivo: " << nombreArchivo << endl;
      return "";
    }

    string linea;
    int fila_actual = 0;

    while (getline(archivo, linea)) {
      if (fila_actual == indice) {
        archivo.close();
        stringstream ss(linea);
        string dato;
        vector<string> fila;

        while (ss >> dato) {
          fila.push_back(dato);
        }

        stringstream resultado;
        for (size_t i = 0; i < fila.size(); ++i) {
          resultado << fila[i];
          if (i < fila.size() - 1) {
            resultado << ":";
          }
        }
        return resultado.str();
      }
      fila_actual++;
    }

    archivo.close();
    cerr << "Índice de fila fuera de rango." << endl;
    return "";
  }

  // Función que retorna una columna específica como un string en el formato
  // deseado
  string obtenerColumna(int indice) const {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
      cerr << "Error al abrir el archivo: " << nombreArchivo << endl;
      return "";
    }

    string linea;
    vector<string> columna;
    while (getline(archivo, linea)) {
      stringstream ss(linea);
      string dato;
      int columna_actual = 0;

      while (ss >> dato) {
        if (columna_actual == indice) {
          columna.push_back(dato);
          break;
        }
        columna_actual++;
      }
    }

    archivo.close();

    if (columna.empty()) {
      cerr << "Índice de columna fuera de rango." << endl;
      return "";
    }

    stringstream resultado;
    for (size_t i = 0; i < columna.size(); ++i) {
      resultado << columna[i];
      if (i < columna.size() - 1) {
        resultado << ":";
      }
    }
    return resultado.str();
  }
};

Matriz W_matrix(W_FILE, W_rows, W_cols); // Matriz de Pesos
Matriz H_matrix(H_FILE, H_rows, H_cols); // Matriz de Caracteristicas

// Función para agregar una columna al archivo de la matriz
void agregarColumnaAlArchivo(const string& cadena, const string& nombreArchivo, int filas) {
    ofstream archivo(nombreArchivo, ios::app); // Abre el archivo en modo de adición
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    stringstream ss(cadena);
    string elemento;
    int fila_actual = 0;

    while (getline(ss, elemento, ':')) {
        if (fila_actual < filas) {
            archivo << elemento << (fila_actual == filas - 1 ? "\n" : " ");
            fila_actual++;
        } else {
            cerr << "La cadena contiene más números de los que caben en las filas especificadas." << endl;
            break;
        }
    }

    archivo.close();
}


// Función para convertir una cadena de texto a un vector de enteros
void convertirTextoAVector(const string &texto, vector<float> &vectorNumeros) {
  vectorNumeros.clear(); // Limpiar el vector antes de llenarlo
  stringstream ss(texto);
  string segmento;

  while (getline(ss, segmento, ':')) {
    vectorNumeros.push_back(
        stof(segmento)); // Convertir cada segmento a un número entero
  }
}

void readThread() {
  //TextMatrix textMatrix("Dataset/matrix_W*H.txt", H_rows, W_cols);
  string message = "";
  vector<float> Col_part;
  int c = 1;
  ostringstream ss;
  string fila_H;
  string columna_W;
  string confirmation(1, '\0');
  for (int i = 0; i < W_cols; i++) {
    ss.str("");
    cout << "i: " << i << endl;
    ss.str("");
    columna_W = W_matrix.obtenerColumna(i);
    ss << "m" << "C" << setw(5) << setfill('0') << i;
    ss << setw(5) << setfill('0') << columna_W.size() << columna_W;
    cout << ss.str() << endl;
    client.send(ss.str());
    // Luego enviamos las filas de la matriz H
    for (int j = 0; j < H_rows; j++) {
      cout << "j: " << j << endl;
      // Solo cuando el servidor halla divido la columna de W en 4 partes en los
      // servidores de procesamiento, y halla obtenido el resultado de la
      // primera multiplicacion
      confirmation = client.receive(1);
      cout << "Confirmation: " << confirmation << endl;
      if (!(confirmation[0] != 'A')) {
        cout << "Dentro de A" << endl;
        fila_H = H_matrix.obtenerFila(j);
        ss.str("");
        ss << "m" << "F" << setw(5) << setfill('0') << j << setw(5)
           << setfill('0') << fila_H.size() << fila_H;
        
        cout << "Enviando fila: " << ss.str() << endl;
        client.send(ss.str());
        fflush(stdout);
      }
    }
    message = client.receive(1);
    if (message[0] == 'R') {
      cout << "Recibiendo resultado" << endl;
      int id = stoi(client.receive(5));
      int size = stoi(client.receive(5));
      string data(size, '\0');  
      data = client.receive(size);
      cout << "Data: " << data << endl;
      agregarColumnaAlArchivo(data, M_FILE, H_rows);
    }
  }
  fflush(stdout);
  /*
  while (true) {
    message = client.receive(1);
    if (message.empty()) {
      cout << "message is empty" << endl;
      break;
    }

    switch (message[0]) {
    case 'R': {
      cout << c++ << endl;
      int id = stoi(client.receive(5));
      int size = stoi(client.receive(5));
      string data = client.receive(size);
      convertirTextoAVector(data, Col_part);
      for (int i = 0; i < Col_part.size(); i++) {
        cout << Col_part[i] << " ";
      }
      cout << endl;
      break;
    }
    default:
      cout << "Unknown message" << endl;
      break;
    }
    cout << "Received: " << message << endl;
  }
  */
}


int main() {
  client.connectToServer();
  string message = "C";
  client.send(message);
  ostringstream ss;
  int id = 0;
  do {
    ss.str("");
    cout << "Escribe m para multiplicar matrices o exit para desconectarte: ";
    getline(cin, message);
    if (message == "exit") {
      client.send("o");
      break;
    }
    if (message == "m") {
      string fila_H;
      string columna_W;
      thread(readThread).detach();
      // string confirmacion("0");
      //  Primero enviamos las columnas de la matriz W
      // client.send("e"); // Elimar las columnas de W almacenadas en los
      // servidores de procesamiento
    }

  } while (true);
  return 0;
}
