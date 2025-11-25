#include "ArbolReal.h"
#include <iostream>

using namespace std;

int main() {
    cout << "SISTEMA DE SUCESION REAL DEL REINO" << endl;
    cout << "Gestión de arbol genealogico" << endl;
    
    ArbolReal arbol;
    
    string nombreArchivo = "royal_family.csv";
    cout << "Cargando datos desde " << nombreArchivo << "..." << endl;
    
    if (!arbol.cargarDesdeCSV(nombreArchivo)) {
        cout << "Error al cargar el archivo. Verifique que existe en la carpeta bin." << endl;
        return 1;
    }
    
    cout << "\nDatos cargados" << endl;
    cout << "Presiona enter para seguir.";
    cin.get();
    
    arbol.ejecutar();
    
    return 0;
}
