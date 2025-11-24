#ifndef NODO_H
#define NODO_H

#include <string>
using namespace std;

struct Nodo {
    int id;
    string name;
    string last_name;
    char gender; // 'H' o 'M'
    int age;
    int id_father;
    bool is_dead;
    bool was_king;
    bool is_king;
    
    Nodo* primogenito;  // Hijo izquierdo (primer hijo)
    Nodo* segundo;      // Hijo derecho (segundo hijo)
    Nodo* padre;
    
    Nodo(int _id, string _name, string _last_name, char _gender, 
         int _age, int _id_father, bool _is_dead, bool _was_king, bool _is_king) {
        id = _id;
        name = _name;
        last_name = _last_name;
        gender = _gender;
        age = _age;
        id_father = _id_father;
        is_dead = _is_dead;
        was_king = _was_king;
        is_king = _is_king;
        primogenito = nullptr;
        segundo = nullptr;
        padre = nullptr;
    }
    
    // Método para verificar si puede ser rey
    bool puedeSerRey() {
        return !is_dead && age < 70 && gender == 'H';
    }
    
    // Método para verificar si puede ser reina
    bool puedeSerReina() {
        return !is_dead && age >= 15 && age < 70 && gender == 'M';
    }
};

#endif
