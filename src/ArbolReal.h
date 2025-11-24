#ifndef ARBOL_REAL_H
#define ARBOL_REAL_H

#include "Nodo.h"
#include <vector>
#include <string>
using namespace std;

class ArbolReal {
private:
    Nodo* raiz;
    vector<Nodo*> todosLosNodos;
    
    //Metodos aux
    Nodo* buscarNodoPorId(int id);
    Nodo* buscarReyActual();
    void construirRelacionesPadreFijo();
    void obtenerLineaSuccesionRecursiva(Nodo* nodo, vector<Nodo*>& linea, bool esPrimogenito);
    Nodo* buscarPrimerVaronVivo(Nodo* nodo);
    Nodo* buscarAncestroConDosHijos(Nodo* nodo);
    Nodo* buscarMejorCandidatoVaron(Nodo* inicioNodo);
    Nodo* buscarMejorCandidatoMujer();
    int calcularDistanciaAPrimogenitos(Nodo* nodo);
    void asignarReyAutomatico();
    void mostrarNodo(Nodo* nodo, int nivel, bool esPrimogenito);
    void destruirArbol(Nodo* nodo);
    
public:
    ArbolReal();
    ~ArbolReal();
    
    bool cargarDesdeCSV(const string& nombreArchivo);
    void mostrarArbol();
    void mostrarLineaSuccesion();
    void procesarMuerteRey();
    void modificarNodo(int id);
    void mostrarMenu();
    void ejecutar();
};

#endif
