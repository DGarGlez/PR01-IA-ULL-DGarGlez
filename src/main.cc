// main.cc
#include "./include/grafo.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Uso: " << argv[0] << " <fichero_grafo>" << std::endl;
        return 1;
    }
    std::string fichero = argv[1];
    Grafo g(fichero);
    std::cout << "\nMatriz de adyacencia del grafo:\n";
    g.MostrarGrafo();
    std::cout << "\nNúmero de vértices: " << g.GetNumeroVertices() << std::endl;
    std::cout << "Número de aristas: " << g.GetNumeroAristas() << std::endl;
    // abro el fichero de salida
    std::ofstream fichero_salida("salidaTest.txt");
    if (!fichero_salida.is_open()) {
        std::cerr << "Error al abrir el fichero de salida." << std::endl;
        return 1;
    }
    g.InformacionConexiones(fichero_salida);
    fichero_salida.close();
    return 0;
}
