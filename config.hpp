#ifndef CONFIG_HPP
#define CONFIG_HPP

//const char* const IP_ADDRESS = "127.0.0.1";
const char* const IP_ADDRESS = "4.201.139.31";
const int PORT = 8080;

// Matriz de Caracteristicas
//const char* const H_FILE = "Dataset/H_caracteristicas.txt";
const char* const H_FILE = "Dataset/NormalizedFeatureMatrix.txt";
const int H_rows = 40;
const int H_cols = 44;
//const int H_rows = 4;
//const int H_cols = 4;
// Matriz de Pesos
//const char* const W_FILE = "Dataset/W_pesos.txt";
const char* const W_FILE = "Dataset/weights_matrix3.txt";
const int W_rows = 44;
const int W_cols = 64;
//const int W_rows = 4;
//const int W_cols = 4;
// Matriz resultante
const char* const M_FILE = "matrix_W_H.txt";
const char* const M_FILE_T = "matrix_W_H_transpuesta.txt";
#endif // CONFIG_HPP
