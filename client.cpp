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

void transponerArchivo(const string& nombreArchivoEntrada, const string& nombreArchivoSalida) {
    ifstream archivoEntrada(nombreArchivoEntrada);
    ofstream archivoSalida(nombreArchivoSalida);
    
    vector<vector<string>> matrizTemporal;
    string linea;

    // Leer el archivo de entrada y almacenar temporalmente en matrizTemporal
    while (getline(archivoEntrada, linea)) {
        stringstream ss(linea);
        string item;
        vector<string> fila;

        while (ss >> item) {
            fila.push_back(item);
        }

        matrizTemporal.push_back(fila);
    }

    size_t filas = matrizTemporal.size();
    size_t columnas = filas > 0 ? matrizTemporal[0].size() : 0;

    // Escribir la transpuesta directamente en el archivo de salida
    for (size_t j = 0; j < columnas; ++j) {
        for (size_t i = 0; i < filas; ++i) {
            archivoSalida << matrizTemporal[i][j];
            if (i < filas - 1) {
                archivoSalida << " ";
            }
        }
        archivoSalida << endl;
    }

    archivoEntrada.close();
    archivoSalida.close();

    // Eliminar el archivo original
    if (remove(nombreArchivoEntrada.c_str()) != 0) {
        cerr << "Error: No se pudo eliminar el archivo original." << endl;
    } else {
        cout << "Archivo original eliminado con éxito." << endl;
    }
}

struct Matriz {
  string nombreArchivo;
  int filas;
  int columnas;
  Matriz(const string &nombreArchivo, int filas, int columnas)
      : nombreArchivo(nombreArchivo), filas(filas), columnas(columnas) {}
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
   cout << "Agregando columna al archivo" << endl;
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
  string message = "";
  ostringstream ss;

  string fila_H;
  string columna_W;

  for (int i = 0; i < W_cols; i++) {
    columna_W = W_matrix.obtenerColumna(i);
    ss.str("");
    ss << "m" << "C" << setw(5) << setfill('0') << i;
    ss << setw(5) << setfill('0') << columna_W.size() << columna_W;
    client.send(ss.str());
    cout << "{M}Env Col: " << i << endl;
    // Luego enviamos las filas de la matriz H
    for (int j = 0; j < H_rows; j++) {
      cout << "{M}Env Fil : " << j << endl;
      string confirmation(1, '\0');
      confirmation = client.receive(1);
      if (!(confirmation[0] != 'A')) {
        fila_H = H_matrix.obtenerFila(j);
        ss.str("");
        ss << "m" << "F" << setw(5) << setfill('0') << j << setw(5)
           << setfill('0') << fila_H.size() << fila_H;
        //cout << "Enviando fila: " << ss.str() << endl;
        client.send(ss.str());
      }
    }
    message = client.receive(1);
    if (message[0] == 'R') {

      int id = stoi(client.receive(5));
      int size_col = stoi(client.receive(5));

      string data(size_col, '\0');  
      cout << id << " {R}ecibiendo resultado " << data<< endl;
      data = client.receive(size_col);
      //cout << "Data: " << data << endl;
      agregarColumnaAlArchivo(data, M_FILE_T, H_rows);
    }
  }
  transponerArchivo(M_FILE_T, M_FILE);
  fflush(stdout);
  cout << "Matriz resultante guardada en " << M_FILE << endl;
}


int main() {
  client.connectToServer();
  string message = "C"; // C para indicar que es un cliente
  client.send(message);
  ostringstream ss;
  do {
    ss.str("");
    cout << "Escribe M para multiplicar matrices de H * W o exit para desconectarte: ";
    getline(cin, message);
    if (message == "exit") {
      client.send("o");
      break;
    }
    if (message == "M") {
      cout << "Nombre del archivo de la matriz resultante se guardara asi: " << M_FILE << endl;
      thread(readThread).detach();
    }
  } while (true);
  return 0;
}
