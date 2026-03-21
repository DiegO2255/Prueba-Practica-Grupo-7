#ifndef TORRE_H
#define TORRE_H

#include <string>

using namespace std;

struct Torre {
    int id;
    string nombre;
    string tipo;
    int posicion;
    int danio;
    int rango;
    int costo;
    bool activa;
    
    Torre() : id(0), nombre(""), tipo(""), posicion(0), danio(0), rango(0), costo(0), activa(true) {}
    
    Torre(int _id, string _nombre, string _tipo, int _pos, int _dan, int _ran, int _cos)
        : id(_id), nombre(_nombre), tipo(_tipo), posicion(_pos), danio(_dan), rango(_ran), costo(_cos), activa(true) {}
};

class ListaSecuencial {
private:
    Torre* torres;
    int capacidad;
    int tamanio;
    
public:
    ListaSecuencial(int cap = 10) : capacidad(cap), tamanio(0) {
        torres = new Torre[capacidad];
    }
    
    ~ListaSecuencial() {
        delete[] torres;
    }
    
    bool insertar(const Torre& torre) {
        if (tamanio >= capacidad) {
            Torre* temp = new Torre[capacidad * 2];
            for (int i = 0; i < tamanio; i++) {
                temp[i] = torres[i];
            }
            delete[] torres;
            torres = temp;
            capacidad *= 2;
        }
        torres[tamanio] = torre;
        tamanio++;
        return true;
    }
    
    bool eliminar(int id) {
        for (int i = 0; i < tamanio; i++) {
            if (torres[i].id == id) {
                for (int j = i; j < tamanio - 1; j++) {
                    torres[j] = torres[j + 1];
                }
                tamanio--;
                return true;
            }
        }
        return false;
    }
    
    Torre* buscar(int id) {
        for (int i = 0; i < tamanio; i++) {
            if (torres[i].id == id) {
                return &torres[i];
            }
        }
        return nullptr;
    }
    
    Torre* obtener(int indice) {
        if (indice >= 0 && indice < tamanio) {
            return &torres[indice];
        }
        return nullptr;
    }
    
    int getTamanio() const { return tamanio; }
    
    int contar() const { return tamanio; }
    
    int contarActivas() const {
        int count = 0;
        for (int i = 0; i < tamanio; i++) {
            if (torres[i].activa) count++;
        }
        return count;
    }
};

#endif
