#ifndef CONFIG_HPP
#define CONFIG_HPP

const char* const IP_ADDRESS = "127.0.0.1";
//const char* const IP_ADDRESS = "4.201.139.31";
const int PORT = 8080;

// Matriz de Caracteristicas
//const char* const H_FILE = "Dataset/H_caracteristicas.txt";
const char* const H_FILE = "Dataset/FeatureMatrix_Counts_With_NodeID.csv";
const int H_rows = 10;
const int H_cols = 44;

// Matriz de Pesos
//const char* const W_FILE = "Dataset/W_pesos.txt";
const char* const W_FILE = "Dataset/weights_matrix2.csv";
const int W_rows = 44;
const int W_cols = 64;

// Matriz resultante
const char* const M_FILE = "Dataset/matrix_W_H.txt";
const char* const M_FILE_T = "Dataset/matrix_W_H_transpuesta.txt";
#endif // CONFIG_HPP
