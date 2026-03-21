#ifndef OLEADA_H
#define OLEADA_H

#include <string>

using namespace std;

struct NodoOleada {
    int idOleada;
    int cantidadEnemigos;
    string tipoEnemigo;
    int vidaBase;
    int velocidadBase;
    int recompensaBase;
    int posicionInicial;
    
    NodoOleada* siguiente;
    
    NodoOleada(int _id, int _cant, string _tipo, int _vida, int _vel, int _rec, int _pos)
        : idOleada(_id), cantidadEnemigos(_cant), tipoEnemigo(_tipo), vidaBase(_vida), velocidadBase(_vel),
          recompensaBase(_rec), posicionInicial(_pos), siguiente(nullptr) {}
};

class ListaCircular {
private:
    NodoOleada* ultimo;
    NodoOleada* actual;
    int tamanio;
    
public:
    ListaCircular() : ultimo(nullptr), actual(nullptr), tamanio(0) {}
    
    ~ListaCircular() {
        if (ultimo == nullptr) return;
        
        NodoOleada* primero = ultimo->siguiente;
        NodoOleada* temp = primero;
        
        do {
            NodoOleada* aux = temp;
            temp = temp->siguiente;
            delete aux;
        } while (temp != primero);
        
        ultimo = nullptr;
        actual = nullptr;
    }
    
    bool registrarOleada(int id, int cantidad, string tipo, int vida, int velocidad, int recompensa) {
        int posInicial = 0;
        
        NodoOleada* nueva = new NodoOleada(id, cantidad, tipo, vida, velocidad, recompensa, posInicial);
        
        if (ultimo == nullptr) {
            ultimo = nueva;
            nueva->siguiente = nueva;
            actual = nueva;
        } else {
            nueva->siguiente = ultimo->siguiente;
            ultimo->siguiente = nueva;
            ultimo = nueva;
        }
        tamanio++;
        return true;
    }
    
    bool avanzarOleada() {
        if (actual == nullptr || actual->siguiente == nullptr) {
            return false;
        }
        actual = actual->siguiente;
        return true;
    }
    
    void reiniciarCiclo() {
        if (ultimo != nullptr) {
            actual = ultimo->siguiente;
        }
    }
    
    NodoOleada* obtenerActual() const { return actual; }
    
    NodoOleada* getUltimo() const { return ultimo; }
    
    int getTamanio() const { return tamanio; }
    
    bool estaVacia() const { return tamanio == 0; }
};

#endif
