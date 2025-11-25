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
