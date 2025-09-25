#include "./include/nodo.h"

/**
 * @brief Constructor por defecto de la clase Nodo.
 * 
 */
Nodo::Nodo() : id_(-1), coste_(0), recorrido_({}) {}

/**
 * @brief Constructor de la clase Nodo.
 * 
 * @param id ID del nodo.
 */
Nodo::Nodo(const int& id) : id_(id), coste_(0), recorrido_({}) {}

/**
 * @brief Constructor de la clase Nodo.
 * 
 * @param id ID del nodo.
 * @param coste Coste de la arista hacia este nodo.
 * @param recorrido Vector con el recorrido desde el nodo raíz hasta este nodo.
 */
Nodo::Nodo(const int& id, const int& coste, const std::vector<Nodo> recorrido) : id_(id), coste_(coste), recorrido_(recorrido) {}

/**
 * @brief Método getter para obtener el ID del nodo.
 * 
 * @return int 
 */
int Nodo::GetID() const { return id_; }

/**
 * @brief Método getter para obtener el coste de la arista hacia este nodo.
 * 
 * @return int 
 */
double Nodo::GetCoste() const { return coste_; }

/**
 * @brief Método getter para obtener la posición del padre en la estructura de nodos.
 * 
 * @return int 
 */
std::vector<Nodo> Nodo::GetRecorrido() const { return recorrido_; }

/**
 * @brief Método setter para establecer el ID del nodo.
 * 
 * @param id ID del nodo.
 */
void Nodo::SetID(int id) { id_ = id; }

/**
 * @brief Método setter para establecer el coste de la arista hacia este nodo.
 * 
 * @param coste Coste de la arista hacia este nodo.
 */
void Nodo::SetCoste(int coste) { coste_ = coste; }

/**
 * @brief Método setter para establecer el recorrido desde el nodo raíz hasta este nodo.
 * 
 * @param recorrido Vector con el recorrido desde el nodo raíz hasta este nodo.
 */
void Nodo::SetRecorrido(std::vector<Nodo> recorrido) { recorrido_ = recorrido; }

/**
 * @brief Sobrecarga del operador == para comparar nodos por sus atributos.
 * 
 * @param otro Nodo a comparar.
 * @return true Si los nodos son iguales.
 * @return false Si los nodos son diferentes.
 */
bool Nodo::operator==(const Nodo& otro) const {
    return id_ == otro.id_ && coste_ == otro.coste_ && recorrido_ == otro.recorrido_;
}