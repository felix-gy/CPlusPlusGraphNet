#ifndef CONFIG_HPP
#define CONFIG_HPP

const char* const IP_ADDRESS = "127.0.0.1";
const int PORT = 8080;

// Matriz de Caracteristicas
const char* const H_FILE = "Dataset/H_caracteristicas.txt";
const int H_rows = 10;
const int H_cols = 5;

// Matriz de Pesos
const char* const W_FILE = "Dataset/W_pesos.txt";
const int W_rows = 5;
const int W_cols = 18;

// Matriz resultante
const char* const M_FILE = "Dataset/matrix_W_H.txt";
const char* const M_FILE_T = "Dataset/matrix_W_H_transpuesta.txt";
#endif // CONFIG_HPP
