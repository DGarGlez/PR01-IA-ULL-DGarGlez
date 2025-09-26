// main.cc
#include "./include/grafo.h"
#include "./include/busqueda.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Uso: " << argv[0] << " <fichero_grafo>\t<id_nodo_inicial>\t<id_nodo_final>" << std::endl;
        return 1;
    }
    std::string fichero = argv[1];
    int id_nodo_inicial = std::stoi(argv[2]);
    int id_nodo_final = std::stoi(argv[3]);
    Grafo g(fichero);
    std::cout << "\nMatriz de adyacencia del grafo:\t\"" << argv[1] << "\"" << std::endl;
    g.MostrarGrafo();
    std::cout << "\nNúmero de vértices: " << g.GetNumeroVertices() << std::endl;
    std::cout << "Número de aristas: " << g.GetNumeroAristas() << std::endl;
    // abro el fichero de salida
    std::ofstream fichero_salida1("salidaDatosGrafo.txt");
    if (!fichero_salida1.is_open()) {
        std::cerr << "Error al abrir el fichero de salida." << std::endl;
        return 1;
    }
    g.InformacionConexiones(fichero_salida1);
    fichero_salida1.close();

    std::ofstream fichero_salida2("salidaBusquedaAmplitud.txt");
    if (!fichero_salida2.is_open()) {
        std::cerr << "Error al abrir el fichero de salida." << std::endl;
        return 1;
    }

    // Realizar búsqueda en amplitud
    double costo_total = 0;
    std::vector<Nodo> camino = Busqueda::BusquedaEnAmplitud(g, id_nodo_inicial - 1, id_nodo_final - 1, costo_total, fichero_salida2);

    // Mostrar resultados
    std::cout << "\n---- RESULTADOS BÚSQUEDA AMPLITUD ----\n";
    std::cout << "Costo total del camino: " << costo_total << std::endl;
    std::cout << "Camino encontrado: ";
    for (size_t i = 0; i < camino.size(); ++i) {
        std::cout << camino[i].GetID() + 1; // Mostrar nodos indexados a partir de 1.
        if (i != camino.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;

    fichero_salida2.close();

    // Realizar búsqueda en profundidad

    std::ofstream fichero_salida3("salidaBusquedaProfundidad.txt");
    if (!fichero_salida3.is_open()) {
        std::cerr << "Error al abrir el fichero de salida." << std::endl;
        return 1;
    }
    costo_total = 0;
    camino = Busqueda::BusquedaEnProfundidad(g, id_nodo_inicial - 1, id_nodo_final - 1, costo_total, fichero_salida3);
    std::cout << "\n---- RESULTADOS BÚSQUEDA PROFUNDIDAD ----\n";
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
