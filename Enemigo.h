#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <string>

using namespace std;

struct NodoEnemigo {
    int id;
    string tipo;
    int vida;
    int velocidad;
    int posicion;
    int recompensa;
    
    NodoEnemigo* anterior;
    NodoEnemigo* siguiente;
    
    NodoEnemigo(int _id, string _tipo, int _vida, int _vel, int _pos, int _rec)
        : id(_id), tipo(_tipo), vida(_vida), velocidad(_vel), posicion(_pos), recompensa(_rec),
          anterior(nullptr), siguiente(nullptr) {}
};

class ListaDoblementeLazada {
private:
    NodoEnemigo* primero;
    NodoEnemigo* ultimo;
    int tamanio;
    
public:
    ListaDoblementeLazada() : primero(nullptr), ultimo(nullptr), tamanio(0) {}
    
    ~ListaDoblementeLazada() {
        NodoEnemigo* actual = primero;
        while (actual != nullptr) {
            NodoEnemigo* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
    
    bool insertarAlFinal(int id, string tipo, int vida, int velocidad, int posicion, int recompensa) {
        NodoEnemigo* nuevo = new NodoEnemigo(id, tipo, vida, velocidad, posicion, recompensa);
        
        if (primero == nullptr) {
            primero = ultimo = nuevo;
        } else {
            nuevo->anterior = ultimo;
            ultimo->siguiente = nuevo;
            ultimo = nuevo;
        }
        tamanio++;
        return true;
    }
    
    bool eliminarPorId(int id) {
        NodoEnemigo* actual = primero;
        
        while (actual != nullptr) {
            if (actual->id == id) {
                if (actual->anterior != nullptr) {
                    actual->anterior->siguiente = actual->siguiente;
                } else {
                    primero = actual->siguiente;
                }
                
                if (actual->siguiente != nullptr) {
                    actual->siguiente->anterior = actual->anterior;
                } else {
                    ultimo = actual->anterior;
                }
                
                delete actual;
                tamanio--;
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }
    
    NodoEnemigo* buscarPorId(int id) {
        NodoEnemigo* actual = primero;
        while (actual != nullptr) {
            if (actual->id == id) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }
    
    void actualizarPosiciones() {
        NodoEnemigo* actual = primero;
        while (actual != nullptr) {
            actual->posicion += actual->velocidad;
            actual = actual->siguiente;
        }
    }
    
    void aplicarDanio(int idEnemigo, int danio) {
        NodoEnemigo* enemigo = buscarPorId(idEnemigo);
        if (enemigo != nullptr) {
            enemigo->vida -= danio;
        }
    }
    
    NodoEnemigo* getPrimero() const { return primero; }
    
    NodoEnemigo* getUltimo() const { return ultimo; }
    
    int getTamanio() const { return tamanio; }
    
    bool estaVacia() const { return tamanio == 0; }
};

#endif
