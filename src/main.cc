// main.cc
#include "./include/grafo.h"
#include "./include/busqueda.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Uso: " << argv[0] << " <fichero_grafo>" << std::endl;
        return 1;
    }
    std::string fichero = argv[1];
    Grafo g(fichero);
    std::cout << "\nMatriz de adyacencia del grafo:\t" << argv[1] << std::endl;
    g.MostrarGrafo();
    std::cout << "\nNúmero de vértices: " << g.GetNumeroVertices() << std::endl;
    std::cout << "Número de aristas: " << g.GetNumeroAristas() << std::endl;
    // abro el fichero de salida
    std::ofstream fichero_salida1("salidaTest.txt");
    if (!fichero_salida1.is_open()) {
        std::cerr << "Error al abrir el fichero de salida." << std::endl;
        return 1;
    }
    g.InformacionConexiones(fichero_salida1);
    fichero_salida1.close();

    std::ofstream fichero_salida2("salidaBusqueda.txt");
    if (!fichero_salida2.is_open()) {
        std::cerr << "Error al abrir el fichero de salida." << std::endl;
        return 1;
    }

    // Realizar búsqueda en amplitud
    double costo_total = 0;
    std::vector<Nodo> camino = Busqueda::BusquedaEnAmplitud(g, 0, 3, costo_total, fichero_salida2);

    // Mostrar resultados
    std::cout << "\nResultados de la búsqueda en amplitud:\n";
    std::cout << "Costo total del camino: " << costo_total << std::endl;
    std::cout << "Camino encontrado: ";
    for (size_t i = 0; i < camino.size(); ++i) {
        std::cout << camino[i].GetID() + 1; // Mostrar nodos indexados a partir de 1.
        if (i != camino.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;

    return 0;
}
