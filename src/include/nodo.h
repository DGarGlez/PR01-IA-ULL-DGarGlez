#pragma once
#include <vector>

/**
 * @brief Clase que representa un nodo en un árbol de búsqueda.
 */
class Nodo {

    private:
        int id_;            // ID del nodo
        double coste_;         // Coste de la arista hacia este nodo
        std::vector<Nodo> recorrido_; // Recorrido desde el nodo raíz hasta este nodo

    public:
        // Constructores de la clase
        Nodo();
        Nodo(const int&);
        Nodo(const int&, const int&, std::vector<Nodo>);

        // Métodos getter y setter para acceder a los atributos
        int GetID() const;
        double GetCoste() const;
        std::vector<Nodo> GetRecorrido() const;

        // Métodos setter para establecer los atributos
        void SetID(int);
        void SetCoste(int);
        void SetRecorrido(std::vector<Nodo>);

        // Sobrecarga del operador == para comparar nodos por su ID y los de los padres de forma recursiva
        bool operator==(const Nodo&) const;
};