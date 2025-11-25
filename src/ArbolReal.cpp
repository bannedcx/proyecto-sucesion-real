#include "ArbolReal.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <queue>

using namespace std;

ArbolReal::ArbolReal() {
    raiz = NULL;
}

ArbolReal::~ArbolReal() {
    destruirArbol(raiz);
}

void ArbolReal::destruirArbol(Nodo* nodo) {
    if (nodo != NULL) {
        destruirArbol(nodo->primogenito);
        destruirArbol(nodo->segundo);
        delete nodo;
    }
}

Nodo* ArbolReal::buscarNodoPorId(int id) {
    for (size_t i = 0; i < todosLosNodos.size(); i++) {
        if (todosLosNodos[i]->id == id) {
            return todosLosNodos[i];
        }
    }
    return NULL;
}

Nodo* ArbolReal::buscarReyActual() {
    for (size_t i = 0; i < todosLosNodos.size(); i++) {
        if (todosLosNodos[i]->is_king && !todosLosNodos[i]->is_dead) {
            return todosLosNodos[i];
        }
    }
    return NULL;
}

bool ArbolReal::cargarDesdeCSV(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        return false;
    }
    
    string linea;
    getline(archivo, linea); // Saltar encabezado
    
    // Leer todos los nodos
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        vector<string> campos;
        
        while (getline(ss, campo, ',')) {
            campos.push_back(campo);
        }
        
        if (campos.size() == 9) {
            int id = atoi(campos[0].c_str());
            string name = campos[1];
            string last_name = campos[2];
            char gender = campos[3][0];
            int age = atoi(campos[4].c_str());
            int id_father = atoi(campos[5].c_str());
            bool is_dead = (campos[6] == "1");
            bool was_king = (campos[7] == "1");
            bool is_king = (campos[8] == "1");
            
            Nodo* nuevoNodo = new Nodo(id, name, last_name, gender, age, 
                                       id_father, is_dead, was_king, is_king);
            todosLosNodos.push_back(nuevoNodo);
        }
    }
    
    archivo.close();
    
    //construimos el arbol
    construirRelacionesPadreFijo();
    
    cout << "Arbol cargado exitosamente con " << todosLosNodos.size() << " nodos." << endl;
    return true;
}

void ArbolReal::construirRelacionesPadreFijo() {
    // Encontrar la raiz
    for (size_t i = 0; i < todosLosNodos.size(); i++) {
        if (todosLosNodos[i]->id_father == 0 || todosLosNodos[i]->id_father == -1) {
            raiz = todosLosNodos[i];
            break;
        }
    }
    
    // Construir relaciones padre-hijo
    for (size_t i = 0; i < todosLosNodos.size(); i++) {
        Nodo* nodo = todosLosNodos[i];
        if (nodo->id_father > 0) {
            Nodo* padre = buscarNodoPorId(nodo->id_father);
            if (padre != NULL) {
                nodo->padre = padre;
                
                // Asignar como primogenito o segundo hijo
                if (padre->primogenito == NULL) {
                    padre->primogenito = nodo;
                } else if (padre->segundo == NULL) {
                    padre->segundo = nodo;
                }
            }
        }
    }
}

void ArbolReal::mostrarNodo(Nodo* nodo, int nivel, bool esPrimogenito) {
    if (nodo == NULL) return;
    
    for (int i = 0; i < nivel; i++) cout << "  ";
    
    cout << "|- ";
    if (nodo->is_king) cout << "[REY] ";
    if (nodo->is_dead) cout << "[MUERTO] ";
    
    cout << nodo->name << " " << nodo->last_name 
         << " (" << nodo->gender << ", " << nodo->age << " anios, ID:" << nodo->id << ")";
    
    if (esPrimogenito) cout << " [PRIMOGENITO]";
    
    cout << endl;
    
    mostrarNodo(nodo->primogenito, nivel + 1, true);
    mostrarNodo(nodo->segundo, nivel + 1, false);
}

void ArbolReal::mostrarArbol() {
    cout << "\n========== ARBOL GENEALOGICO REAL ==========" << endl;
    mostrarNodo(raiz, 0, true);
    cout << "============================================\n" << endl;
}

void ArbolReal::obtenerLineaSuccesionRecursiva(Nodo* nodo, vector<Nodo*>& linea, bool esPrimogenito) {
    if (nodo == NULL) return;
    
    // Agregar nodo actual si es candidato valido
    if (!nodo->is_dead && !nodo->is_king) {
        if (nodo->gender == 'H' && nodo->age < 70) {
            linea.push_back(nodo);
        } else if (nodo->gender == 'M' && nodo->age >= 15 && nodo->age < 70) {
            linea.push_back(nodo);
        }
    }
    
    //Primero primogenito, luego segundo
    obtenerLineaSuccesionRecursiva(nodo->primogenito, linea, true);
    obtenerLineaSuccesionRecursiva(nodo->segundo, linea, false);
}

void ArbolReal::mostrarLineaSuccesion() {
    cout << "\n== LINEA DE SUCESION ACTUAL ==" << endl;
    
    Nodo* reyActual = buscarReyActual();
    if (reyActual != NULL) {
        cout << "Rey actual: " << reyActual->name << " " << reyActual->last_name << endl;
        cout << "\nSucesores en orden:" << endl;
    }
    
    vector<Nodo*> linea;
    
    if (reyActual != NULL) {
        obtenerLineaSuccesionRecursiva(reyActual, linea, true);
        
        // Si no hay sucesores en la rama del rey, se busca en otras ramas
        if (linea.empty() && raiz != NULL) {
            obtenerLineaSuccesionRecursiva(raiz, linea, true);
        }
    } else {
        obtenerLineaSuccesionRecursiva(raiz, linea, true);
    }
    
    int posicion = 1;
    for (size_t i = 0; i < linea.size(); i++) {
        cout << posicion++ << ". " << linea[i]->name << " " << linea[i]->last_name 
             << " (" << linea[i]->gender << ", " << linea[i]->age << " anios)" << endl;
    }
    
    if (linea.empty()) {
        cout << "No hay sucesores validos." << endl;
    }
    
    cout << "===========================================\n" << endl;
}

Nodo* ArbolReal::buscarPrimerVaronVivo(Nodo* nodo) {
    if (nodo == NULL) return NULL;
    
    // Si este nodo puede ser rey, retornarlo
    if (nodo->puedeSerRey()) {
        return nodo;
    }
    
    Nodo* encontrado = buscarPrimerVaronVivo(nodo->primogenito);
    if (encontrado != NULL) return encontrado;
    
    // Buscamos en segundo hijo si el primogenito no dio resultado
    return buscarPrimerVaronVivo(nodo->segundo);
}
Nodo* ArbolReal::buscarAncestroConDosHijos(Nodo* nodo) {
    if (nodo == NULL) return NULL;
    
    Nodo* actual = nodo->padre;
    while (actual != NULL) {

if (actual->primogenito != NULL && actual->segundo != NULL) {
            return actual;
        }
        actual = actual->padre;
    }
    
    return NULL;
}

int ArbolReal::calcularDistanciaAPrimogenitos(Nodo* nodo) {
    int distancia = 0;
    Nodo* actual = nodo;
    
    while (actual != NULL && actual->padre != NULL) {
        if (actual->padre->segundo == actual) {
            distancia++;
        }
        actual = actual->padre;
    }
    
    return distancia;
}

Nodo* ArbolReal::buscarMejorCandidatoMujer() {
    Nodo* mejorCandidato = NULL;
    int menorDistancia = 1000;
    int menorEdad = 100;
    
    for (size_t i = 0; i < todosLosNodos.size(); i++) {
        Nodo* nodo = todosLosNodos[i];
        if (nodo->puedeSerReina()) {
            int distancia = calcularDistanciaAPrimogenitos(nodo);
            
            // Prioridad: 1) Menor distancia, 2) Menor edad
            if (distancia < menorDistancia  
                (distancia == menorDistancia && nodo->age < menorEdad)) {
                mejorCandidato = nodo;
                menorDistancia = distancia;
                menorEdad = nodo->age;
            }
        }
    }
    
    return mejorCandidato;
}




