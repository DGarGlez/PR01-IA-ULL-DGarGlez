#include "./include/busqueda.h"

/**
 * @brief Función para mostrar la información de los nodos generados.
 * 
 * @param nodos_generados 
 * @param archivo_salida 
 */
void InformacionNodosGenerados(const std::vector<Nodo>& nodos_generados, std::ofstream& archivo_salida) {
  archivo_salida << "Nodos Generados: ";
  for (size_t i = 0; i < nodos_generados.size(); ++i) {
    archivo_salida << nodos_generados[i].GetID() + 1;  // Mostrar nodos indexados a partir de 1.
    if (i != nodos_generados.size() - 1) {  // Compara el índice, no el valor del nodo.
      archivo_salida << ", ";
    }
  }
  archivo_salida << std::endl;
}

/**
 * @brief Función para mostrar la información de los nodos inspeccionados.
 * 
 * @param nodos_inspeccionados 
 * @param archivo_salida 
 */
void InformacionNodosInspeccionados(const std::vector<Nodo>& nodos_inspeccionados, std::ofstream& archivo_salida) {
  archivo_salida << "Nodos Inspeccionados: ";
  if (nodos_inspeccionados.size() >= 1) {
    for (unsigned i = 0; i < nodos_inspeccionados.size(); ++i) {
      archivo_salida << nodos_inspeccionados[i].GetID() + 1;  // Mostrar nodos indexados a partir de 1.
      if (i < nodos_inspeccionados.size() - 1) {
        archivo_salida << ", ";
      }
    }
  } else {
    archivo_salida << "-";
  }
  archivo_salida << std::endl;
  archivo_salida << "──────────────────────────────────────────────────" << std::endl;
}

/**
 * @brief Función para escribir el título de la búsqueda en el archivo de salida.
 * 
 * @param titulo 
 * @param id_inicial 
 * @param id_final 
 * @param archivo_salida 
 */
void TituloBusqueda(const std::string& titulo, int id_inicial, int id_final, int n_vertices, int n_aristas, std::ofstream& archivo_salida) {
    // Escribimos la cabecera en el fichero de salida
    archivo_salida << "-- BÚSQUEDA EN " << titulo << " --" << std::endl;
    archivo_salida << "──────────────────────────────────────────────────" << std::endl;
    archivo_salida << "Número de nodos del grafo: " << n_vertices << std::endl;
    archivo_salida << "Número de aristas del grafo: " << n_aristas << std::endl;
    archivo_salida << "Vértice origen: " << id_inicial + 1 << std::endl;
    archivo_salida << "Vértice destino: " << id_final + 1<< std::endl;
    archivo_salida << "──────────────────────────────────────────────────" << std::endl;
}


void ResultadoBusqueda(const std::vector<Nodo>& camino, double costo_total, std::ofstream& archivo_salida) {
    // Añado mensaje de nodo final encontrado
    archivo_salida << "Camino: ";
    for (size_t j = 0; j < camino.size(); ++j) {
        archivo_salida << camino[j].GetID() + 1; // Mostrar nodos indexados a partir de 1.
        if (j != camino.size() - 1) {
            archivo_salida << " -> ";
        }
    }
    archivo_salida << std::endl;
    archivo_salida << "──────────────────────────────────────────────────" << std::endl;
    archivo_salida << "Costo: " << costo_total << std::endl;
    archivo_salida << "──────────────────────────────────────────────────" << std::endl;
}

/**
 * @brief Función para comprobar si un nodo está en la rama.
 * Esto quiere decir que si un nodo ya ha sido visitado, no se vuelve a visitar.
 * 
 * @param recorrido 
 * @param id 
 * @param pos_actual 
 * @return true 
 * @return false 
 */
bool NodoEnLaRama(const std::vector<Nodo>& recorrido, const int& id, int pos_actual)
{
    std::vector<Nodo> recorrido_copy = recorrido; // Hago una copia del vector para no modificar el original
	while (pos_actual != -1)
	{
        if (recorrido_copy[pos_actual].GetID() == id) { return true; }
        pos_actual = recorrido_copy[pos_actual].GetRecorrido().empty() ? -1 : recorrido_copy[pos_actual].GetRecorrido().back().GetID(); // Obtener el padre del nodo actual
        if (pos_actual != -1) {
            recorrido_copy[pos_actual].GetRecorrido().pop_back(); // Elimino el último elemento para no quedarme en un bucle infinito
        }
	}
	return false;
}

/**
 * @brief Función para realizar la búsqueda en amplitud.
 * 
 * @param grafo 
 * @param id_inicial 
 * @param id_final 
 * @param costo_total 
 * @return std::vector<int> El camino encontrado
 */
std::vector<Nodo> Busqueda::BusquedaEnAmplitud(const Grafo& grafo, const int& id_inicial, const int& id_final, double& costo_total, std::ofstream& fichero_salida) {

    TituloBusqueda("AMPLITUD", id_inicial, id_final, grafo.GetNumeroVertices(), grafo.GetNumeroAristas(), fichero_salida);

	Nodo nodo_inicial;
	nodo_inicial.SetID(id_inicial);     // Establecemos el ID del nodo inicial
	nodo_inicial.SetCoste(0);			// El coste para llegar hasta él mismo es 0.
	nodo_inicial.SetRecorrido(std::vector<Nodo>{Nodo()});     // El recorrido empieza en el mismo nodo.

	std::vector<Nodo> recorrido;        // Nodos recorridos
	std::vector<Nodo> inspeccionados;   // Nodos inspeccionados
	std::vector<Nodo> generados;		// Nodos generados
	std::vector<Nodo> camino;           // El camino que se devolverá

	recorrido.push_back(nodo_inicial);  // Añadimos el nodo inicial al recorrido
	generados.push_back(nodo_inicial);  // Añadimos el nodo inicial a los generados

	bool esfinal = false, hijo_anadido = true;   // Indicador de si se ha encontrado el nodo final y si se ha añadido un hijo
	unsigned ultimo_anadido = 0;    // Índice del último nodo añadido y posición del nodo final
	int iteracion = 1; // Contador de iteraciones

    // Mostramos la iteración
    fichero_salida << "Iteración " << iteracion++ << std::endl;     // Iteración inicial
	InformacionNodosGenerados(generados, fichero_salida);           // Imprimir nodos generados
	InformacionNodosInspeccionados(inspeccionados, fichero_salida); // Imprimir nodos inspeccionados

    // Mientras no se haya recorrido todo el grafo y no se haya encontrado el nodo final
	while (hijo_anadido && !esfinal) {
        // Variables auxiliares para actualizar el recorrido
		hijo_anadido = false;   // Indicador de si se ha añadido un hijo en esta iteración
		// aux = ultimo_anadido;   // Actualizamos el índice del último nodo añadido
		long unsigned kTamanyo{recorrido.size()};   // Tamaño actual del recorrido

		// Bucle para recorrer los nodos generados, a partir del último nodo añadido hasta el final
		for (unsigned i = ultimo_anadido; i < kTamanyo; ++i) {
            // Mostramos la iteración
			fichero_salida << "Iteración " << iteracion << std::endl;

            if (recorrido[i].GetID() == id_final) { // Si hemos encontrado el nodo final
				esfinal = true;
                camino = recorrido[i].GetRecorrido();
                camino.push_back(recorrido[i]); // Añado el nodo final al camino
                // Borro el primer elemento que es un nodo vacío
                if (!camino.empty() && camino[0].GetID() == -1) {
                    camino.erase(camino.begin());
                }

                // Sumo los costes del camino
                costo_total = 0;
                for (size_t j = 1; j < camino.size(); ++j) {
                    costo_total += camino[j].GetCoste();
                }

                // Imprimir nodos generados
				InformacionNodosGenerados(generados, fichero_salida);
				inspeccionados.push_back(recorrido[i]);
                // Imprimir nodos inspeccionados
				InformacionNodosInspeccionados(inspeccionados, fichero_salida);
                // Imprimir resultado de la búsqueda
                ResultadoBusqueda(camino, costo_total, fichero_salida);
				break;
			}

			// Si no es el nodo destino, generamos los hijos que no estén en la rama.
			const std::vector<double>& vecinos = grafo.GetMatrizCoste()[recorrido[i].GetID()];

            // Bucle para recorrer los vecinos del nodo actual
			for (int j = 0; j < vecinos.size(); j++) {
                // Si hay conexión
				if (vecinos[j] != -1 && j != recorrido[i].GetID()) {
                    std::vector<Nodo> recorr_aux = recorrido[i].GetRecorrido();
                    recorr_aux.push_back(recorrido[i]); // Añado el nodo padre
					Nodo nuevo_nodo(j, vecinos[j], recorr_aux); // Nodo hijo

                    // Si el nodo no está en la rama
					if (!NodoEnLaRama(recorrido, j, i)) {
						recorrido.push_back(nuevo_nodo);    // Añadimos el nuevo nodo al recorrido
						generados.push_back(nuevo_nodo);    // Añadimos el nodo generado
						hijo_anadido = true;            // Indicamos que se ha añadido un hijo
					}
				}
			}

            
            // Después de generar los hijos, mostramos la información de los nodos generados e inspeccionados
			InformacionNodosGenerados(generados, fichero_salida);
			inspeccionados.push_back(recorrido[i]); // Añadimos el nodo inspeccionado
			++ultimo_anadido;   // Actualizamos el índice del último nodo añadido
            // Imprimir nodos inspeccionados
			InformacionNodosInspeccionados(inspeccionados, fichero_salida);
			++iteracion; // Incrementamos el contador de iteraciones
		}

		if (esfinal) { break; } // Si ya hemos encontrado el nodo final, salimos del bucle
		if (!hijo_anadido) { break; } // No hay solución, no existe camino
	}
    return camino;
}

std::vector<Nodo> Busqueda::BusquedaEnProfundidad(const Grafo& grafo, const int& id_inicial, const int& id_final, double& costo_total, std::ofstream& fichero_salida) {
    TituloBusqueda("PROFUNDIDAD", id_inicial, id_final, grafo.GetNumeroVertices(), grafo.GetNumeroAristas(), fichero_salida);

    Nodo nodo_inicial;
    nodo_inicial.SetID(id_inicial);   // Establecemos el ID del nodo inicial
    nodo_inicial.SetCoste(0);         // El coste para llegar hasta él mismo es 0.
    nodo_inicial.SetRecorrido(std::vector<Nodo>{Nodo()});   // El recorrido empieza en el mismo nodo.

    std::vector<Nodo> recorrido;        // Nodos recorridos
    std::vector<Nodo> inspeccionados;   // Nodos inspeccionados
    std::vector<Nodo> generados;        // Nodos generados
    std::vector<Nodo> camino;           // El camino que se devolverá

    recorrido.push_back(nodo_inicial);  // Añadimos el nodo inicial al recorrido
    generados.push_back(nodo_inicial);  // Añadimos el nodo inicial a los generados

    bool esfinal = false;   // Indicador de si se ha encontrado el nodo final
    int iteracion = 1;      // Contador de iteraciones

    fichero_salida << "Iteración " << iteracion++ << std::endl;     // Iteración inicial
    InformacionNodosGenerados(generados, fichero_salida);           // Imprimir nodos generados
    InformacionNodosInspeccionados(inspeccionados, fichero_salida); // Imprimir nodos inspeccionados

    // Mientras no se haya recorrido todo el grafo y no se haya encontrado el nodo final
    while (!recorrido.empty() && !esfinal) {
        Nodo actual = recorrido.back(); // Tomamos el último nodo añadido (pila LIFO)
        recorrido.pop_back();         // Lo eliminamos de la pila

        fichero_salida << "Iteración " << iteracion++ << std::endl;    // Mostramos la iteración

        if (actual.GetID() == id_final) {   // Si hemos encontrado el nodo final
            esfinal = true;                 // Marcamos que hemos encontrado el nodo final
            camino = actual.GetRecorrido(); // Obtenemos el recorrido hasta el nodo actual
            camino.push_back(actual);       // Añado el nodo final al camino
            // Borro el primer elemento que es un nodo vacío
            if (!camino.empty() && camino[0].GetID() == -1) {
                camino.erase(camino.begin());
            }
            // Sumo los costes del camino
            costo_total = 0;    
            for (size_t j = 1; j < camino.size(); ++j) {
                costo_total += camino[j].GetCoste();
            }
            InformacionNodosGenerados(generados, fichero_salida);        // Imprimir nodos generados
            inspeccionados.push_back(actual);                  // Añadimos el nodo inspeccionado
            InformacionNodosInspeccionados(inspeccionados, fichero_salida); // Imprimir nodos inspeccionados
            ResultadoBusqueda(camino, costo_total, fichero_salida); // Imprimir resultado de la búsqueda
            break;
        }

        const std::vector<double>& vecinos = grafo.GetMatrizCoste()[actual.GetID()];    // Obtenemos los vecinos del nodo actual
        for (int j = vecinos.size() - 1; j >= 0; --j) { // Para DFS, recorrer de mayor a menor
            if (vecinos[j] != -1 && j != actual.GetID()) {  // Si hay conexión
                std::vector<Nodo> recorr_aux = actual.GetRecorrido();   // Copiamos el recorrido del nodo actual
                recorr_aux.push_back(actual);   // Añado el nodo padre
                Nodo nuevo_nodo(j, vecinos[j], recorr_aux); // Nodo hijo

                // Permitir nodos con mismo id pero distinto recorrido
                bool ya_en_rama = false;    
                for (const auto& n : recorr_aux) {
                    if (n.GetID() == j) {
                        ya_en_rama = true;
                        break;
                    }
                }
                if (!ya_en_rama) {
                    recorrido.push_back(nuevo_nodo);
                    generados.push_back(nuevo_nodo);
                }
            }
        }

        InformacionNodosGenerados(generados, fichero_salida);
        inspeccionados.push_back(actual);
        InformacionNodosInspeccionados(inspeccionados, fichero_salida);
    }

    return camino;
}
