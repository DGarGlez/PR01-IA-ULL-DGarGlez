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

    // Escribimos la cabecera en el fichero de salida
    fichero_salida << "Búsqueda en Amplitud" << std::endl;
    fichero_salida << "──────────────────────────────────────────────────" << std::endl;
    fichero_salida << "Número de nodos del grafo: " << grafo.GetNumeroVertices() << std::endl;
    fichero_salida << "Número de aristas del grafo: " << grafo.GetNumeroAristas() << std::endl;
    fichero_salida << "Vértice origen: " << id_inicial + 1 << std::endl;
    fichero_salida << "Vértice destino: " << id_final + 1<< std::endl;
    fichero_salida << "──────────────────────────────────────────────────" << std::endl;

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
	unsigned ultimo_anadido = 0, pos_id_final = 0, aux = ultimo_anadido;    // Índice del último nodo añadido y posición del nodo final
	int iteracion = 1; // Contador de iteraciones

    // Mostramos la iteración
    fichero_salida << "Iteración " << iteracion++ << std::endl;     // Iteración inicial
	InformacionNodosGenerados(generados, fichero_salida);           // Imprimir nodos generados
	InformacionNodosInspeccionados(inspeccionados, fichero_salida); // Imprimir nodos inspeccionados

    // Mientras no se haya recorrido todo el grafo y no se haya encontrado el nodo final
	while (hijo_anadido && !esfinal) {
        // Variables auxiliares para actualizar el recorrido
		hijo_anadido = false;   // Indicador de si se ha añadido un hijo en esta iteración
		aux = ultimo_anadido;   // Actualizamos el índice del último nodo añadido
		long unsigned kTamanyo{recorrido.size()};   // Tamaño actual del recorrido

		// Bucle para recorrer los nodos generados, a partir del último nodo añadido hasta el final
		for (unsigned i = aux; i < kTamanyo; ++i) {
            // Mostramos la iteración
			fichero_salida << "Iteración " << iteracion << std::endl;

            if (recorrido[i].GetID() == id_final) { // Si hemos encontrado el nodo final
				esfinal = true;
				pos_id_final = i;
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

                // Añado mensaje de nodo final encontrado
                fichero_salida << "Camino: ";
                for (size_t j = 0; j < camino.size(); ++j) {
                    fichero_salida << camino[j].GetID() + 1; // Mostrar nodos indexados a partir de 1.
                    if (j != camino.size() - 1) {
                        fichero_salida << " -> ";
                    }
                }
                fichero_salida << std::endl;
                fichero_salida << "──────────────────────────────────────────────────" << std::endl;
                fichero_salida << "Costo: " << costo_total << std::endl;
                fichero_salida << "──────────────────────────────────────────────────" << std::endl;

				break;
			}


            
			// Si no es el nodo destino, generamos los hijos que no estén en la rama.
			const std::vector<double>& vecinos = grafo.GetMatrizCoste()[recorrido[i].GetID()];

            // Bucle para recorrer los vecinos del nodo actual
			for (unsigned j = 0; j < vecinos.size(); ++j) {
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

    // // Si se ha encontrado el nodo final, se recupera el camino
    // if (esfinal) {
    //     // Accedo al ultimo nodo del vector de nodos recorrido, saco su atriburo recorrido y le añado el pos_id_final
    //     camino = recorrido.back().GetRecorrido();
    //     // camino.push_back(pos_id_final);

    //     std::cout << "CAMINO FINAL: ";
    //     for (size_t i = 0; i < camino.size(); ++i) {
    //         std::cout << camino[i].GetID() + 1; // Mostrar nodos indexados a partir de 1.
    //         if (i != camino.size() - 1) {
    //             std::cout << " -> ";
    //         }
    //     }
    //     std::cout << std::endl;
    // }
    return camino;
}

// /**
//  * @brief Función para realizar la búsqueda en profundidad.
//  * 
//  * @param grafo 
//  * @param nodo_origen 
//  * @param nodo_destino 
//  * @param costo_total 
//  * @param file 
//  * @return std::vector<Nodo> 
//  */
// std::vector<int> Busqueda::BusquedaEnProfundidad(const Grafo& grafo, const int& id_inicial, const int& id_final, double& costo_total, std::ofstream& archivo_salida) {
//   // Estructura para saber si un nodo ha sido visitado
//   std::unordered_map<int, bool> visitado;

//   // Vectores para “traza” (similar a tu versión iterativa)
//   std::vector<Nodo> generados;
//   std::vector<Nodo> inspeccionados;

//   // Vector para guardar el camino actual en la recursión
//   // (lo iremos construyendo a medida que descendemos)
//   std::vector<Nodo> camino_actual;

//   // Creamos el nodo inicial (similar a tu versión iterativa)
//   Nodo nodo_inicial;
//   nodo_inicial.SetID(id_inicial);
//   nodo_inicial.SetCoste(0);
//   nodo_inicial.SetPadre(nullptr);

//   // Marcamos como visitado
//   visitado[id_inicial] = true;
//   generados.push_back(nodo_inicial);

//   // Iteración para la traza
//   int iteracion = 1;

//   // Escribimos la primera traza (antes de entrar en DFSRecursivo)
//   archivo_salida << "Iteración " << iteracion++ << std::endl;
//   InformacionNodosGenerados(generados, archivo_salida);
//   InformacionNodosInspeccionados(inspeccionados, archivo_salida);

//   // Variable booleana para indicar si se ha encontrado el destino
//   bool encontrado = false;

//   // Iniciamos la búsqueda recursiva
//   DFSRecursivo(grafo, &nodo_inicial, id_final,
//                visitado, generados, inspeccionados, camino_actual,
//                costo_total, archivo_salida, iteracion, encontrado);

//   // Si al terminar la recursión no se encontró, camino_actual estará vacío
//   return camino_actual;  
// }

// void Busqueda::DFSRecursivo(const Grafo& grafo,
//                             Nodo* nodo_actual,
//                             int id_destino,
//                             std::unordered_map<int, bool>& visitado,
//                             std::vector<Nodo>& generados,
//                             std::vector<Nodo>& inspeccionados,
//                             std::vector<Nodo>& camino_actual,
//                             double& costo_total,
//                             std::ofstream& archivo_salida,
//                             int& iteracion,
//                             bool& encontrado) {
//   // Marcamos el nodo actual como visitado
//   visitado[nodo_actual->GetID()] = true;
//   // Añadimos el nodo actual a “inspeccionados”
//   inspeccionados.push_back(*nodo_actual);

//   // Añadimos también el nodo al “camino actual”
//   camino_actual.push_back(*nodo_actual);

//   // Comprobamos si es el destino
//   if (nodo_actual->GetID() == id_destino) {
//     // Mostramos la traza final
//     archivo_salida << "Iteración " << iteracion++ << std::endl;
//     InformacionNodosGenerados(generados, archivo_salida);
//     InformacionNodosInspeccionados(inspeccionados, archivo_salida);

//     // Reconstruimos el coste total (sumando coste de cada nodo en el camino)
//     costo_total = 0;
//     for (auto& nd : camino_actual) {
//       costo_total += nd.GetCoste();
//     }

//     // Marcamos que el destino fue encontrado
//     encontrado = true;
//     return;
//   }

//   // Exploramos sus vecinos
//   const std::vector<double>& fila = grafo.GetMatrizCoste()[nodo_actual->GetID()];

//   // Recorremos los vecinos
//   for (int i = 0; i < static_cast<int>(fila.size()); ++i) {
//     int costo = fila[i];
//     // Si hay conexión y no está visitado
//     if (costo != -1 && !visitado[i]) {
//       // Creamos un nuevo nodo
//       Nodo* nuevo_hijo = new Nodo();
//       nuevo_hijo->SetID(i);
//       nuevo_hijo->SetCoste(costo);
//       nuevo_hijo->SetPadre(nodo_actual);

//       generados.push_back(*nuevo_hijo);
//       archivo_salida << "Iteración " << iteracion++ << std::endl;
//       InformacionNodosGenerados(generados, archivo_salida);
//       InformacionNodosInspeccionados(inspeccionados, archivo_salida);

//       // Llamada recursiva
//       DFSRecursivo(grafo, nuevo_hijo, id_destino,
//                    visitado, generados, inspeccionados, camino_actual,
//                    costo_total, archivo_salida, iteracion, encontrado);

//       // Si el destino se encontró en la rama recursiva, no seguimos
//       if (encontrado) {
//         return;
//       }
//     }
//   }

//   // 5) Si llegamos aquí, significa que este nodo no ha llevado al destino:
//   //    lo quitamos del “camino actual” (backtracking)
//   camino_actual.pop_back();
// }