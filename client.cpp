#include "Interface/TCPClient.h"
#include "config.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <sstream> 
#include <iomanip>
#include <vector>
using namespace std;
TCPClient client(IP_ADDRESS, PORT);

struct Matriz {
    string nombreArchivo;
    int filas;
    int columnas;

    // Constructor que inicializa la estructura con el nombre del archivo y las dimensiones de la matriz
    Matriz(const string& nombreArchivo, int filas, int columnas)
        : nombreArchivo(nombreArchivo), filas(filas), columnas(columnas) {}

    // Función que retorna una fila específica como un string en el formato deseado
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

                while (getline(ss, dato, ',')) {
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

    // Función que retorna una columna específica como un string en el formato deseado
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

            while (getline(ss, dato, ',')) {
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

void readThread()
{

    string message = "";
    while (true) {
        message = client.receive(11);
        if (message.empty()) {
            cout << "message is empty" << endl;
            break;
        }
        cout << "Received: " << message << endl;
    }
}


int main()
{
    client.connectToServer();
    thread(readThread).detach();
    string message = "C";
    client.send(message);
    ostringstream ss;
    int id = 0;
        do  {
        ss.str("");
        cout << "Enter message: ";
        getline(cin, message);
        if (message == "exit") {
            client.send("o");
            break;
        }

        if (message == "mult") {
            string fila_H;
            string columna_W;
            string confirmacion("0");
            // Primero enviamos las columnas de la matriz W
            for (int i=0; i < W_cols; i++) {
                ss.str("");
                columna_W = W_matrix.obtenerColumna(0);
                ss<<"m"<< "C" << setw(5) << setfill('0') << i << setw(5) << setfill('0')<< columna_W.size() << columna_W;
                client.send(ss.str());
                // Luego enviamos las filas de la matriz H
                for (int j = 0; j < H_rows; j++) {
                    // Solo cuando el servidor halla divido la columna de W en 4 partes en los servidores de procesamiento,
                    // y halla obtenido el resultado de la primera multiplicacion
                    confirmacion = client.receive(1);
                    if (confirmacion == "A") {                    
                        fila_H = H_matrix.obtenerFila(j);
                        ss.str("");
                        ss<<"m"<< "F" << setw(5) << setfill('0') << j << setw(4) << setfill('0')<< fila_H.size() << fila_H;
                        client.send(ss.str());
                    }
                }
                client.send("e"); // Elimar las columnas de W almacenadas en los servidores de procesamiento
            }
        }
        client.send(message);
    } while (true);

    return 0;
}