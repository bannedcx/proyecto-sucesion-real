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
    cantidadNodos = 0;
    capacidadMaxima = 100;  //Capacidad inicial
    todosLosNodos = new Nodo*[capacidadMaxima];
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
    cout << "\n==ARBOL GENEALOGICO REAL==" << endl;
    mostrarNodo(raiz, 0, true);
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
            if (distancia < menorDistancia || 
                (distancia == menorDistancia && nodo->age < menorEdad)) {
                mejorCandidato = nodo;
                menorDistancia = distancia;
                menorEdad = nodo->age;
            }
        }
    }
    
    return mejorCandidato;
}

Nodo* ArbolReal::buscarMejorCandidatoVaron(Nodo* inicioNodo) {
    Nodo* candidato = NULL;
    
    //Buscamos en hijos del rey actual (primogenito prioritario)
    candidato = buscarPrimerVaronVivo(inicioNodo->primogenito);
    if (candidato != NULL) {
        cout << "  -> Sucesor encontrado, primogenito del rey" << endl;
        return candidato;
    }
    
    candidato = buscarPrimerVaronVivo(inicioNodo->segundo);
    if (candidato != NULL) {
        cout << "  -> Sucesor encontrado, segundo hijo del rey" << endl;
        return candidato;
    }
    
    //Buscar hermano del rey
    if (inicioNodo->padre != NULL) {
        Nodo* hermano = NULL;
        if (inicioNodo->padre->primogenito == inicioNodo) {
            hermano = inicioNodo->padre->segundo;
        } else {
            hermano = inicioNodo->padre->primogenito;
        }
        
        if (hermano != NULL) {
            //Buscar en hijos del hermano primero
            candidato = buscarPrimerVaronVivo(hermano->primogenito);
            if (candidato != NULL) {
                cout << "  -> Sucesor encontrado en rama del hermano(primogenito)" << endl;
                return candidato;
            }
            
            candidato = buscarPrimerVaronVivo(hermano->segundo);
            if (candidato != NULL) {
                cout << "  -> Sucesor encontrado en rama del hermano (segundo)" << endl;
                return candidato;
            }
            
            if (hermano->puedeSerRey()) {
                cout << "  -> El hermano del rey es el sucesor" << endl;
                return hermano;
            }
        }
        
        //Buscamos tio del rey
        if (inicioNodo->padre->padre != NULL) {
            Nodo* abuelo = inicioNodo->padre->padre;
            Nodo* tio = NULL;
            
            if (abuelo->primogenito == inicioNodo->padre) {
                tio = abuelo->segundo;
            } else {
                tio = abuelo->primogenito;
            }
            
            if (tio != NULL) {
                candidato = buscarPrimerVaronVivo(tio->primogenito);
                if (candidato != NULL) {
                    cout << "  -> Sucesor encontrado en rama del tio (primogenito)" << endl;
                    return candidato;
                }
                
                candidato = buscarPrimerVaronVivo(tio->segundo);
                if (candidato != NULL) {
                    cout << "  -> Sucesor encontrado en rama del tio (segundo)" << endl;
                    return candidato;
                }
                
                if (tio->puedeSerRey()) {
                    cout << "  -> El tio del rey es el sucesor" << endl;
                    return tio;
                }
            }
        }
    }
    
    //Buscamos ancestro con dos hijos y exploramos rama del segundo
    Nodo* ancestro = buscarAncestroConDosHijos(inicioNodo);
    if (ancestro != NULL) {
        cout << "  -> Buscando en rama del segundo hijo del ancestro" << endl;
        candidato = buscarPrimerVaronVivo(ancestro->segundo);
        if (candidato != NULL) {
            return candidato;
        }
    }
    
    //si no hay varones, buscar la mejor mujer candidata
    cout << "  -> No hay varones disponibles, buscando mujeres" << endl;
    return buscarMejorCandidatoMujer();
}

void ArbolReal::asignarReyAutomatico() {
    Nodo* reyAnterior = buscarReyActual();
    
    if (reyAnterior == NULL) {
        // Buscar quien fue rey
        for (size_t i = 0; i < todosLosNodos.size(); i++) {
            if (todosLosNodos[i]->was_king || todosLosNodos[i]->is_king) {
                reyAnterior = todosLosNodos[i];
                break;
            }
        }
    }
    
    if (reyAnterior != NULL) {
        reyAnterior->is_king = false;
        reyAnterior->was_king = true;
        
        cout << "\nBuscando sucesor del rey " << reyAnterior->name << " " << reyAnterior->last_name << "..." << endl;
        
        Nodo* nuevoRey = buscarMejorCandidatoVaron(reyAnterior);
        
        if (nuevoRey != NULL) {
            nuevoRey->is_king = true;
            cout << "\n NUEVO REY ASIGNADO " << endl;
            cout << "Nombre: " << nuevoRey->name << " " << nuevoRey->last_name << endl;
            cout << "Edad: " << nuevoRey->age << " anios" << endl;
            cout << "Genero: " << (nuevoRey->gender == 'H' ? "Hombre" : "Mujer") << endl;
        } else {
            cout << "\nNo se encontro un sucesor valido en todo el arbol." << endl;
        }
    }
}

void ArbolReal::procesarMuerteRey() {
    Nodo* reyActual = buscarReyActual();
    
    if (reyActual == NULL) {
        cout << "No hay un rey actual para procesar su muerte." << endl;
        return;
    }
    
    cout << "Procesando muerte del rey " << reyActual->name << " " << reyActual->last_name << "..." << endl;
    
    reyActual->is_dead = true;
    reyActual->is_king = false;
    reyActual->was_king = true;
    
    asignarReyAutomatico();
}

void ArbolReal::modificarNodo(int id) {
    Nodo* nodo = buscarNodoPorId(id);
    
    if (nodo == NULL) {
        cout << "Nodo con ID " << id << " no encontrado." << endl;
        return;
    }
    
    cout << "\nModificar Nodo" << endl;
    cout << "Nodo actual: " << nodo->name << " " << nodo->last_name << endl;
    cout << "\n1. Nombre: " << nodo->name;
    cout << "\n2. Apellido: " << nodo->last_name;
    cout << "\n3. Genero: " << nodo->gender;
    cout << "\n4. Edad: " << nodo->age;
    cout << "\n5. Estado (vivo/muerto): " << (nodo->is_dead ? "Muerto" : "Vivo");
    cout << "\n6. Fue rey: " << (nodo->was_king ? "Si" : "No");
    cout << "\n7. Es rey: " << (nodo->is_king ? "Si" : "No");
    cout << "\n0. Cancelar" << endl;
    
    int opcion;
    cout << "\nSelecciona la opcion a modificar: ";
    cin >> opcion;
    cin.ignore();
    
    switch(opcion) {
        case 1: {
            cout << "Nuevo nombre: ";
            getline(cin, nodo->name);
            break;
        }
        case 2: {
            cout << "Nuevo apellido: ";
            getline(cin, nodo->last_name);
            break;
        }
        case 3: {
            cout << "Nuevo genero (H/M): ";
            cin >> nodo->gender;
            break;
        }
        case 4: {
            cout << "Nueva edad: ";
            cin >> nodo->age;
            
            // Verificar si debe dejar de ser rey por edad
            if (nodo->is_king && nodo->age >= 70) {
                cout << "\nEl rey ha superado los 70 anios. Se asignara un nuevo rey." << endl;
                procesarMuerteRey();
            }
            break;
        }
        case 5: {
            int estado;
            cout << "Esta muerto? (1=Si, 0=No): ";
            cin >> estado;
            nodo->is_dead = (estado == 1);
            
            if (nodo->is_dead && nodo->is_king) {
                procesarMuerteRey();
            }
            break;
        }
        case 6: {
            int valor;
            cout << "Fue rey? (1=Si, 0=No): ";
            cin >> valor;
            nodo->was_king = (valor == 1);
            break;
        }
        case 7: {
            int valor;
            cout << "Es rey? (1=Si, 0=No): ";
            cin >> valor;
            nodo->is_king = (valor == 1);
            break;
        }
        case 0:
            cout << "Operacion cancelada." << endl;
            return;
        default:
            cout << "Opcion invalida." << endl;
            return;
    }
    
    cout << "Nodo modificado exitosamente." << endl;
}

void ArbolReal::mostrarMenu() {
    cout << "==SISTEMA DE SUCESION REAL==" << endl;
    cout << "1. Mostrar arbol genealogico completo" << endl;
    cout << "2. Mostrar linea de sucesion actual" << endl;
    cout << "3. Procesar muerte del rey actual" << endl;
    cout << "4. Modificar datos de un nodo" << endl;
    cout << "5. Salir" << endl;
    cout << "\nSeleccione una opcion: ";
}

void ArbolReal::ejecutar() {
    int opcion;
    
    do {
        mostrarMenu();
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch(opcion) {
            case 1:
                mostrarArbol();
                break;
            case 2:
                mostrarLineaSuccesion();
                break;
            case 3:
                procesarMuerteRey();
                break;
            case 4: {
                int id;
                cout << "Ingrese el ID del nodo a modificar: ";
                cin >> id;
                cin.ignore();
                modificarNodo(id);
                break;
            }
            case 5:
                cout << "\nGracias por usar el sistema." << endl;
                break;
            default:
                cout << "Opcion invalida. Intente nuevamente." << endl;
        }
        
        if (opcion != 5) {
            cout << "\nPresiona enter para seguir.";
            cin.get();
        }
        
    } while(opcion != 5);
}
