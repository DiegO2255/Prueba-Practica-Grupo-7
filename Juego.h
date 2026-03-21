#ifndef JUEGO_H
#define JUEGO_H

#include "Torre.h"
#include "Enemigo.h"
#include "Oleada.h"
#include <iostream>
using namespace std;

class Juego {
private:
    ListaSecuencial torres;
    ListaDoblementeLazada enemigos;
    ListaCircular oleadas;
    
    int vidasJugador;
    int vidaActual;
    int enemigoDestruidos;
    int generadorIdEnemigo;
    int generadorIdTorre;
    int generadorIdOleada;
    int turno;
    bool juegoActivo;
    int dinero;
    bool combateActivo;
    
public:
    Juego() : vidasJugador(3), vidaActual(100), enemigoDestruidos(0), generadorIdEnemigo(1), 
              generadorIdTorre(1), generadorIdOleada(1), turno(0), juegoActivo(true), 
              dinero(400), combateActivo(false) {}
    
    // ============ METODOS PARA TORRES ============
    
    bool registrarTorre(string nombre, string tipo, int posicion) {
        int danio = 0, rango = 0, costo = 0;
        
        string tipoLower = tipo;
        for (char &c : tipoLower) c = tolower(c);
        
        if (tipoLower == "arquero") {
            danio = 25;
            rango = 3;
            costo = 100;
        } else if (tipoLower == "canon" || tipoLower == "canon") {
            danio = 35;
            rango = 4;
            costo = 170;
        } else if (tipoLower == "mago") {
            danio = 40;
            rango = 5;
            costo = 200;
        } else {
            return false;
        }
        
        if (dinero < costo) {
            return false;
        }
        
        Torre nuevaTorre(generadorIdTorre++, nombre, tipo, posicion, danio, rango, costo);
        if (torres.insertar(nuevaTorre)) {
            dinero -= costo;
            return true;
        }
        return false;
    }
    
    bool registrarTorrePersonalizada(string nombre, string tipo, int posicion, int danio, int rango, int costo) {
        if (dinero < costo) {
            return false;
        }
        
        Torre nuevaTorre(generadorIdTorre++, nombre, tipo, posicion, danio, rango, costo);
        if (torres.insertar(nuevaTorre)) {
            dinero -= costo;
            return true;
        }
        return false;
    }
    
    bool eliminarTorre(int id) {
        Torre* torre = buscarTorre(id);
        if (torre != nullptr) {
            dinero += torre->costo / 2;
            return torres.eliminar(id);
        }
        return false;
    }
    
    Torre* buscarTorre(int id) {
        return torres.buscar(id);
    }
    
    ListaSecuencial* obtenerTorres() {
        return &torres;
    }
    
    // ============ METODOS PARA OLEADAS ============
    
    bool registrarOleadaPredefinida(string tipoEnemigo) {
        int cantidad = 0, vida = 0, velocidad = 0, recompensa = 0;
        string tipoLower = tipoEnemigo;
        for (char &c : tipoLower) c = tolower(c);
        
        if (tipoLower == "normal") {
            cantidad = 5;
            vida = 20;
            velocidad = 2;
            recompensa = 60;
        } else if (tipoLower == "rapido") {
            cantidad = 3;
            vida = 15;
            velocidad = 3;
            recompensa = 80;
        } else if (tipoLower == "fuerte") {
            cantidad = 2;
            vida = 40;
            velocidad = 1;
            recompensa = 120;
        } else {
            return false;
        }
        
        return oleadas.registrarOleada(generadorIdOleada++, cantidad, tipoLower, vida, velocidad, recompensa);
    }
    
    bool registrarOleadaPersonalizada(int cantidad, string tipo, int vida, int velocidad, int recompensa) {
        if (velocidad > 5) velocidad = 5;
        if (velocidad < 1) velocidad = 1;
        
        return oleadas.registrarOleada(generadorIdOleada++, cantidad, tipo, vida, velocidad, recompensa);
    }
    
    NodoOleada* buscarOleada(int id) {
        if (oleadas.getTamanio() == 0) return nullptr;
        
        NodoOleada* temp = oleadas.getUltimo();
        if (temp != nullptr) {
            temp = temp->siguiente;
            do {
                if (temp->idOleada == id) {
                    return temp;
                }
                temp = temp->siguiente;
            } while (temp != oleadas.getUltimo()->siguiente);
        }
        return nullptr;
    }
    
    ListaCircular* obtenerOleadas() {
        return &oleadas;
    }
    
    // ============ METODOS PARA COMBATE ============
    
    bool iniciarCombate() {
        if (combateActivo) {
            return false;
        }
        
        if (oleadas.getTamanio() == 0) {
            return false;
        }
        
        combateActivo = true;
        return true;
    }
    
    bool iniciarSiguienteOleada() {
        if (!combateActivo) {
            return false;
        }
        
        NodoOleada* oleadaActualPtr = oleadas.obtenerActual();
        
        if (oleadaActualPtr == nullptr) {
            combateActivo = false;
            return false;
        }
        
        for (int i = 0; i < oleadaActualPtr->cantidadEnemigos; i++) {
            enemigos.insertarAlFinal(generadorIdEnemigo++, oleadaActualPtr->tipoEnemigo,
                                    oleadaActualPtr->vidaBase, oleadaActualPtr->velocidadBase, 
                                    0, oleadaActualPtr->recompensaBase);
        }
        
        oleadas.avanzarOleada();
        return true;
    }
    
    void aplicarDanioVida(int danio) {
        vidaActual -= danio;
        if (vidaActual <= 0) {
            vidasJugador--;
            if (vidasJugador > 0) {
                vidaActual = 100;
                cout << "  [X] Has perdido una vida! Vidas restantes: " << vidasJugador << endl;
                cout << "  [+] Vida recuperada a 100 HP." << endl;
            } else {
                cout << "  [GAME OVER] No te quedan vidas." << endl;
            }
        }
    }
    
    void avanzarTurno(bool& huboMovimiento, bool& huboDanio) {
        if (!combateActivo) return;
        
        turno++;
        huboMovimiento = false;
        huboDanio = false;
        
        // PASO 1: Las torres atacan PRIMERO
        NodoEnemigo* actual = enemigos.getPrimero();
        while (actual != nullptr) {
            for (int i = 0; i < torres.getTamanio(); i++) {
                Torre* torre = torres.obtener(i);
                if (torre != nullptr && torre->activa) {
                    int distancia = abs(actual->posicion - torre->posicion);
                    if (distancia <= torre->rango) {
                        enemigos.aplicarDanio(actual->id, torre->danio);
                        huboDanio = true;
                    }
                }
            }
            actual = actual->siguiente;
        }
        
        // PASO 2: Verificar muertes despues del ataque
        actual = enemigos.getPrimero();
        while (actual != nullptr) {
            NodoEnemigo* siguiente = actual->siguiente;
            
            if (actual->vida <= 0) {
                enemigoDestruidos++;
                dinero += actual->recompensa;
                enemigos.eliminarPorId(actual->id);
            }
            actual = siguiente;
        }
        
        // PASO 3: Los enemigos se mueven
        enemigos.actualizarPosiciones();
        huboMovimiento = true;
        
        // PASO 4: Verificar si algun enemigo llego al final
        actual = enemigos.getPrimero();
        while (actual != nullptr) {
            NodoEnemigo* siguiente = actual->siguiente;
            
            if (actual->posicion >= 20) {
                aplicarDanioVida(actual->vida);
                enemigos.eliminarPorId(actual->id);
            }
            actual = siguiente;
        }
        
        // PASO 5: Verificar fin del combate
        if (enemigos.getTamanio() == 0 && oleadas.obtenerActual() == nullptr) {
            combateActivo = false;
            cout << "\n[FELICIDADES] Has completado todas las oleadas." << endl;
        }
    }
    
    ListaDoblementeLazada* obtenerEnemigos() {
        return &enemigos;
    }
    
    // ============ METODOS DE ESTADO ============
    
    int obtenerVidas() const { return vidasJugador; }
    
    int obtenerVidaActual() const { return vidaActual; }
    
    int obtenerTurno() const { return turno; }
    
    int obtenerEnemigoDestruidos() const { return enemigoDestruidos; }
    
    int obtenerTorresCount() const { return torres.getTamanio(); }
    
    int obtenerEnemigosCount() const { return enemigos.getTamanio(); }
    
    int obtenerOleadasCount() const { return oleadas.getTamanio(); }
    
    int obtenerDinero() const { return dinero; }
    
    bool estaActivo() const { return juegoActivo && vidasJugador > 0; }
    
    bool enCombate() const { return combateActivo; }
    
    void terminarJuego() { juegoActivo = false; }
    
    int obtenerOleadaActual() const {
        NodoOleada* actual = oleadas.obtenerActual();
        return actual ? actual->idOleada : 0;
    }
};

#endif
