#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <limits>
#include <vector>
#include "Juego.h"

using namespace std;

// Prototipos de funciones
void mostrarMenuPrincipal();
void menuTorres(Juego& juego);
void menuOleadas(Juego& juego);
void mostrarOleadas(Juego& juego);
void mostrarEstadoCombate(Juego& juego);
void mostrarMapa(Juego& juego);
void mostrarEnemigosActivos(Juego& juego);
void procesarAvanzarTurno(Juego& juego);
void iniciarSiguienteOleada(Juego& juego);
void mostrarIntro();
void mostrarGameOver(Juego& juego);
void limpiarBuffer();

// ============ FUNCIONES DE VISUALIZACION ============

void mostrarIntro() {
    cout << "\n+====================================================+" << endl;
    cout << "|         BIENVENIDO A TOWER DEFENSE                 |" << endl;
    cout << "+----------------------------------------------------+" << endl;
    cout << "|  Defiende tu base de los enemigos!                 |" << endl;
    cout << "|                                                    |" << endl;
    cout << "|  Dinero inicial: $400                              |" << endl;
    cout << "|  Vidas: 3 (cada vida tiene 100 HP)                 |" << endl;
    cout << "|                                                    |" << endl;
    cout << "|  RECOMPENSAS POR ENEMIGO:                          |" << endl;
    cout << "|  * Normal: $60  (Vida:20, Velocidad:2)            |" << endl;
    cout << "|  * Rapido: $80  (Vida:15, Velocidad:3)            |" << endl;
    cout << "|  * Fuerte: $120 (Vida:40, Velocidad:1)            |" << endl;
    cout << "+====================================================+" << endl;
}

void mostrarGameOver(Juego& juego) {
    cout << "\n+====================================================+" << endl;
    cout << "|                  GAME OVER                         |" << endl;
    cout << "+----------------------------------------------------+" << endl;
    cout << "|  Turno: " << setw(44) << juego.obtenerTurno() << " |" << endl;
    cout << "|  Enemigos destruidos: " << setw(35) << juego.obtenerEnemigoDestruidos() << " |" << endl;
    cout << "|  Dinero acumulado: $" << setw(38) << juego.obtenerDinero() << " |" << endl;
    cout << "+====================================================+" << endl;
}

void mostrarMapa(Juego& juego) {
    ListaDoblementeLazada* enemigos = juego.obtenerEnemigos();
    ListaSecuencial* torres = juego.obtenerTorres();
    
    const int LONGITUD = 21;
    vector<string> mapa(LONGITUD, " . ");
    
    for (int i = 0; i < torres->getTamanio(); i++) {
        Torre* t = torres->obtener(i);
        if (t != nullptr && t->activa) {
            int pos = t->posicion;
            if (pos >= 0 && pos < LONGITUD) {
                string tipoLower = t->tipo;
                for (char &c : tipoLower) c = tolower(c);
                
                if (tipoLower == "arquero")
                    mapa[pos] = " A ";
                else if (tipoLower == "canon")
                    mapa[pos] = " C ";
                else if (tipoLower == "mago")
                    mapa[pos] = " M ";
                else
                    mapa[pos] = " T ";
            }
        }
    }
    
    NodoEnemigo* actual = enemigos->getPrimero();
    while (actual != nullptr) {
        int pos = actual->posicion;
        if (pos >= 0 && pos < LONGITUD) {
            string tipo = actual->tipo;
            if (tipo == "normal")
                mapa[pos] = " N ";
            else if (tipo == "rapido")
                mapa[pos] = " R ";
            else if (tipo == "fuerte")
                mapa[pos] = " F ";
            else
                mapa[pos] = " E ";
        }
        actual = actual->siguiente;
    }
    
    cout << "\n+====================================================+" << endl;
    cout << "|                CAMPO DE BATALLA                     |" << endl;
    cout << "+----------------------------------------------------+" << endl;
    cout << "|  ";
    for (int i = 0; i < LONGITUD; i++) {
        cout << setw(3) << i;
    }
    cout << "  |" << endl;
    cout << "|  ";
    for (int i = 0; i < LONGITUD; i++) {
        cout << "---";
    }
    cout << "  |" << endl;
    cout << "|  ";
    for (int i = 0; i < LONGITUD; i++) {
        cout << mapa[i];
    }
    cout << "  |" << endl;
    cout << "|  ";
    for (int i = 0; i < LONGITUD; i++) {
        cout << "---";
    }
    cout << "  |" << endl;
    cout << "|  Leyenda: . = vacio | A = Arquero | C = Canon |   |" << endl;
    cout << "|            M = Mago | N = Normal | R = Rapido |    |" << endl;
    cout << "|            F = Fuerte | E = Enemigo                |" << endl;
    cout << "+====================================================+" << endl;
}

void mostrarTorres(Juego& juego) {
    ListaSecuencial* torres = juego.obtenerTorres();
    
    if (torres->getTamanio() == 0) {
        cout << "\n  [!] No hay torres registradas." << endl;
        return;
    }
    
    cout << "\n  +----+------------------+------------+----------+-------+-------+--------+---------+" << endl;
    cout << "  | ID |      Nombre      |    Tipo    | Posicion | Danio | Rango | Costo  | Estado  |" << endl;
    cout << "  +----+------------------+------------+----------+-------+-------+--------+---------+" << endl;
    
    for (int i = 0; i < torres->getTamanio(); i++) {
        Torre* t = torres->obtener(i);
        if (t != nullptr) {
            cout << "  | " << setw(3) << t->id << " | "
                 << left << setw(16) << t->nombre.substr(0, 16) << " | "
                 << setw(10) << t->tipo << " | "
                 << setw(8) << t->posicion << " | "
                 << setw(5) << t->danio << " | "
                 << setw(5) << t->rango << " | "
                 << "$" << setw(6) << t->costo << " | "
                 << (t->activa ? "Activa " : "Inactiva") << " |" << endl;
        }
    }
    cout << "  +----+------------------+------------+----------+-------+-------+--------+---------+" << endl;
}

void mostrarOleadas(Juego& juego) {
    ListaCircular* oleadas = juego.obtenerOleadas();
    
    if (oleadas->getTamanio() == 0) {
        cout << "\n  [!] No hay oleadas registradas." << endl;
        return;
    }
    
    cout << "\n  +----+----------+------------------+----------+-----------+-------------+" << endl;
    cout << "  | ID | Cantidad |      Tipo       |   Vida   | Velocidad | Recompensa  |" << endl;
    cout << "  +----+----------+------------------+----------+-----------+-------------+" << endl;
    
    NodoOleada* temp = oleadas->getUltimo();
    if (temp != nullptr) {
        temp = temp->siguiente;
        do {
            cout << "  | " << setw(3) << temp->idOleada << " | "
                 << setw(8) << temp->cantidadEnemigos << " | "
                 << left << setw(16) << temp->tipoEnemigo.substr(0, 16) << " | "
                 << setw(8) << temp->vidaBase << " | "
                 << setw(9) << temp->velocidadBase << " | "
                 << "$" << setw(10) << temp->recompensaBase << " |" << endl;
            temp = temp->siguiente;
        } while (temp != oleadas->getUltimo()->siguiente);
    }
    cout << "  +----+----------+------------------+----------+-----------+-------------+" << endl;
}

void mostrarEnemigosActivos(Juego& juego) {
    ListaDoblementeLazada* enemigos = juego.obtenerEnemigos();
    
    if (enemigos->getTamanio() == 0) {
        cout << "\n  [!] No hay enemigos activos en el campo de batalla." << endl;
        return;
    }
    
    cout << "\n  +----+------------------+-------+-----------+----------+-------------+" << endl;
    cout << "  | ID |      Tipo       | Vida  | Velocidad | Posicion | Recompensa  |" << endl;
    cout << "  +----+------------------+-------+-----------+----------+-------------+" << endl;
    
    NodoEnemigo* actual = enemigos->getPrimero();
    while (actual != nullptr) {
        cout << "  | " << setw(3) << actual->id << " | "
             << left << setw(16) << actual->tipo << " | "
             << setw(5) << actual->vida << " | "
             << setw(9) << actual->velocidad << " | "
             << setw(8) << actual->posicion << " | "
             << "$" << setw(10) << actual->recompensa << " |" << endl;
        actual = actual->siguiente;
    }
    cout << "  +----+------------------+-------+-----------+----------+-------------+" << endl;
}

void mostrarEstadoCombate(Juego& juego) {
    int vidaActual = juego.obtenerVidaActual();
    
    cout << "\n+====================================================+" << endl;
    cout << "|              ESTADO DEL COMBATE                     |" << endl;
    cout << "+----------------------------------------------------+" << endl;
    cout << "|  Turno: " << setw(47) << juego.obtenerTurno() << " |" << endl;
    cout << "|  Vidas: " << juego.obtenerVidas() << " (";
    
    int barras = vidaActual / 5;
    cout << "[";
    for (int i = 0; i < 20; i++) {
        if (i < barras)
            cout << "#";
        else
            cout << ".";
    }
    cout << "] " << vidaActual << "/100 HP";
    
    cout << setw(47 - (20 + to_string(vidaActual).length() + 7)) << " " << " |" << endl;
    
    cout << "|  Dinero: $" << setw(47) << juego.obtenerDinero() << " |" << endl;
    cout << "|  Enemigos activos: " << setw(37) << juego.obtenerEnemigosCount() << " |" << endl;
    cout << "|  Enemigos destruidos: " << setw(34) << juego.obtenerEnemigoDestruidos() << " |" << endl;
    cout << "|  Torres construidas: " << setw(35) << juego.obtenerTorresCount() << " |" << endl;
    cout << "|  Oleadas registradas: " << setw(34) << juego.obtenerOleadasCount() << " |" << endl;
    cout << "|  Oleada actual: " << setw(40) << juego.obtenerOleadaActual() << " |" << endl;
    cout << "|  Estado: " << setw(47) << (juego.enCombate() ? "EN COMBATE" : "ESPERANDO") << " |" << endl;
    cout << "+====================================================+" << endl;
    
    mostrarMapa(juego);
    
    cout << "\n>> TORRES:" << endl;
    mostrarTorres(juego);
    
    cout << "\n>> ENEMIGOS EN CAMPO:" << endl;
    mostrarEnemigosActivos(juego);
}

void mostrarMenuPrincipal() {
    cout << "\n+====================================================+" << endl;
    cout << "|                  MENU PRINCIPAL                     |" << endl;
    cout << "+----------------------------------------------------+" << endl;
    cout << "|  1. Torres                                         |" << endl;
    cout << "|  2. Oleadas (Enemigos)                             |" << endl;
    cout << "|  3. Mostrar oleadas registradas                    |" << endl;
    cout << "|  4. Mostrar estado del combate                     |" << endl;
    cout << "|  5. Iniciar combate                                |" << endl;
    cout << "|  6. Iniciar siguiente oleada                       |" << endl;
    cout << "|  7. Avanzar turno                                  |" << endl;
    cout << "|  8. Salir                                          |" << endl;
    cout << "+====================================================+" << endl;
    cout << "Seleccione una opcion: ";
}

// ============ MENU TORRES ============

void menuTorres(Juego& juego) {
    char subOpcion;
    do {
        cout << "\n+====================================================+" << endl;
        cout << "|                  MENU TORRES                       |" << endl;
        cout << "+----------------------------------------------------+" << endl;
        cout << "|  a. Registrar torre (predefinida)                 |" << endl;
        cout << "|  b. Registrar torre (personalizada)               |" << endl;
        cout << "|  c. Mostrar torres registradas                    |" << endl;
        cout << "|  d. Eliminar torre                                |" << endl;
        cout << "|  e. Buscar torre por ID                           |" << endl;
        cout << "|  f. Volver al menu principal                      |" << endl;
        cout << "+====================================================+" << endl;
        cout << "Seleccione una opcion: ";
        
        cin >> subOpcion;
        cin.ignore();
        
        switch (subOpcion) {
            case 'a':
            case 'A': {
                cout << "\n--- REGISTRAR TORRE PREDEFINIDA ---" << endl;
                cout << "Dinero disponible: $" << juego.obtenerDinero() << endl;
                cout << "\nTipos disponibles:" << endl;
                cout << "  * Arquero (Danio:25, Rango:3, Costo:$100)" << endl;
                cout << "  * Canon (Danio:35, Rango:4, Costo:$170)" << endl;
                cout << "  * Mago (Danio:40, Rango:5, Costo:$200)" << endl;
                
                string nombre, tipo;
                int posicion;
                
                cout << "\nNombre de la torre: ";
                getline(cin, nombre);
                
                cout << "Tipo (Arquero/Canon/Mago): ";
                getline(cin, tipo);
                
                cout << "Posicion (0-20): ";
                cin >> posicion;
                cin.ignore();
                
                if (juego.registrarTorre(nombre, tipo, posicion)) {
                    cout << "\n[OK] Torre registrada exitosamente!" << endl;
                    cout << "  Dinero restante: $" << juego.obtenerDinero() << endl;
                } else {
                    cout << "\n[ERROR] Tipo no valido o dinero insuficiente." << endl;
                }
                break;
            }
            
            case 'b':
            case 'B': {
                cout << "\n--- REGISTRAR TORRE PERSONALIZADA ---" << endl;
                cout << "Dinero disponible: $" << juego.obtenerDinero() << endl;
                
                string nombre, tipo;
                int posicion, danio, rango, costo;
                
                cout << "Nombre de la torre: ";
                getline(cin, nombre);
                
                cout << "Tipo: ";
                getline(cin, tipo);
                
                cout << "Posicion (0-20): ";
                cin >> posicion;
                
                cout << "Danio: ";
                cin >> danio;
                
                cout << "Rango: ";
                cin >> rango;
                
                cout << "Costo: $";
                cin >> costo;
                cin.ignore();
                
                if (juego.registrarTorrePersonalizada(nombre, tipo, posicion, danio, rango, costo)) {
                    cout << "\n[OK] Torre registrada exitosamente!" << endl;
                    cout << "  Dinero restante: $" << juego.obtenerDinero() << endl;
                } else {
                    cout << "\n[ERROR] Dinero insuficiente." << endl;
                }
                break;
            }
            
            case 'c':
            case 'C':
                mostrarTorres(juego);
                break;
            
            case 'd':
            case 'D': {
                cout << "\n--- ELIMINAR TORRE ---" << endl;
                cout << "ID de la torre a eliminar: ";
                int id;
                cin >> id;
                cin.ignore();
                
                if (juego.eliminarTorre(id)) {
                    cout << "\n[OK] Torre eliminada exitosamente!" << endl;
                    cout << "  Se devolvio la mitad del costo." << endl;
                    cout << "  Dinero actual: $" << juego.obtenerDinero() << endl;
                } else {
                    cout << "\n[ERROR] Torre no encontrada!" << endl;
                }
                break;
            }
            
            case 'e':
            case 'E': {
                cout << "\n--- BUSCAR TORRE POR ID ---" << endl;
                cout << "ID de la torre: ";
                int id;
                cin >> id;
                cin.ignore();
                
                Torre* torre = juego.buscarTorre(id);
                if (torre != nullptr) {
                    cout << "\n[OK] Torre encontrada:" << endl;
                    cout << "  +-------------------------+" << endl;
                    cout << "  | ID: " << torre->id << endl;
                    cout << "  | Nombre: " << torre->nombre << endl;
                    cout << "  | Tipo: " << torre->tipo << endl;
                    cout << "  | Posicion: " << torre->posicion << endl;
                    cout << "  | Danio: " << torre->danio << endl;
                    cout << "  | Rango: " << torre->rango << endl;
                    cout << "  | Costo: $" << torre->costo << endl;
                    cout << "  | Estado: " << (torre->activa ? "Activa" : "Inactiva") << endl;
                    cout << "  +-------------------------+" << endl;
                } else {
                    cout << "\n[ERROR] Torre no encontrada!" << endl;
                }
                break;
            }
            
            case 'f':
            case 'F':
                cout << "\nVolviendo al menu principal..." << endl;
                return;
            
            default:
                cout << "\n[ERROR] Opcion invalida. Intente nuevamente." << endl;
        }
    } while (true);
}

// ============ MENU OLEADAS ============

void menuOleadas(Juego& juego) {
    char subOpcion;
    do {
        cout << "\n+====================================================+" << endl;
        cout << "|                 MENU OLEADAS                        |" << endl;
        cout << "+----------------------------------------------------+" << endl;
        cout << "|  a. Registrar oleada (predefinida)                 |" << endl;
        cout << "|  b. Registrar oleada (personalizada)               |" << endl;
        cout << "|  c. Mostrar oleadas registradas                    |" << endl;
        cout << "|  d. Buscar oleada por ID                           |" << endl;
        cout << "|  e. Volver al menu principal                       |" << endl;
        cout << "+====================================================+" << endl;
        cout << "Seleccione una opcion: ";
        
        cin >> subOpcion;
        cin.ignore();
        
        switch (subOpcion) {
            case 'a':
            case 'A': {
                cout << "\n--- REGISTRAR OLEADA PREDEFINIDA ---" << endl;
                cout << "Tipos de enemigos disponibles:" << endl;
                cout << "  * Normal (Vida:20, Velocidad:2, Recompensa:$60)" << endl;
                cout << "  * Rapido (Vida:15, Velocidad:3, Recompensa:$80)" << endl;
                cout << "  * Fuerte (Vida:40, Velocidad:1, Recompensa:$120)" << endl;
                
                string tipo;
                cout << "\nTipo de enemigo: ";
                getline(cin, tipo);
                
                if (juego.registrarOleadaPredefinida(tipo)) {
                    cout << "\n[OK] Oleada registrada exitosamente!" << endl;
                    cout << "  Los enemigos comenzaran en posicion 0." << endl;
                } else {
                    cout << "\n[ERROR] Tipo de enemigo no valido." << endl;
                }
                break;
            }
            
            case 'b':
            case 'B': {
                cout << "\n--- REGISTRAR OLEADA PERSONALIZADA ---" << endl;
                int cantidad, vida, velocidad, recompensa;
                string tipo;
                
                cout << "Cantidad de enemigos: ";
                cin >> cantidad;
                cin.ignore();
                
                cout << "Tipo de enemigo: ";
                getline(cin, tipo);
                
                cout << "Vida base: ";
                cin >> vida;
                
                cout << "Velocidad base (1-5, recomendado 1-3): ";
                cin >> velocidad;
                
                cout << "Recompensa: $";
                cin >> recompensa;
                cin.ignore();
                
                if (juego.registrarOleadaPersonalizada(cantidad, tipo, vida, velocidad, recompensa)) {
                    cout << "\n[OK] Oleada registrada exitosamente!" << endl;
                    cout << "  Los enemigos comenzaran en posicion 0." << endl;
                } else {
                    cout << "\n[ERROR] Error al registrar la oleada." << endl;
                }
                break;
            }
            
            case 'c':
            case 'C':
                mostrarOleadas(juego);
                break;
            
            case 'd':
            case 'D': {
                cout << "\n--- BUSCAR OLEADA POR ID ---" << endl;
                cout << "ID de la oleada: ";
                int id;
                cin >> id;
                cin.ignore();
                
                NodoOleada* oleada = juego.buscarOleada(id);
                if (oleada != nullptr) {
                    cout << "\n[OK] Oleada encontrada:" << endl;
                    cout << "  +-------------------------+" << endl;
                    cout << "  | ID: " << oleada->idOleada << endl;
                    cout << "  | Cantidad: " << oleada->cantidadEnemigos << endl;
                    cout << "  | Tipo: " << oleada->tipoEnemigo << endl;
                    cout << "  | Vida: " << oleada->vidaBase << endl;
                    cout << "  | Velocidad: " << oleada->velocidadBase << endl;
                    cout << "  | Recompensa: $" << oleada->recompensaBase << endl;
                    cout << "  | Posicion inicial: " << oleada->posicionInicial << endl;
                    cout << "  +-------------------------+" << endl;
                } else {
                    cout << "\n[ERROR] Oleada no encontrada!" << endl;
                }
                break;
            }
            
            case 'e':
            case 'E':
                cout << "\nVolviendo al menu principal..." << endl;
                return;
            
            default:
                cout << "\n[ERROR] Opcion invalida. Intente nuevamente." << endl;
        }
    } while (true);
}

// ============ FUNCIONES DE COMBATE ============

void procesarAvanzarTurno(Juego& juego) {
    if (!juego.enCombate()) {
        cout << "\n[!] No hay combate activo. Inicie el combate primero (opcion 5)." << endl;
        return;
    }
    
    if (juego.obtenerEnemigosCount() == 0) {
        cout << "\n[!] No hay enemigos en el campo de batalla." << endl;
        if (juego.obtenerOleadaActual() > 0 && juego.obtenerOleadas()->obtenerActual() != nullptr) {
            cout << "  Use la opcion 6 para iniciar la siguiente oleada." << endl;
        }
        return;
    }
    
    bool huboMovimiento = false;
    bool huboDanio = false;
    int turnoAnterior = juego.obtenerTurno();
    
    juego.avanzarTurno(huboMovimiento, huboDanio);
    
    cout << "\n+====================================================+" << endl;
    cout << "|                 TURNO " << setw(3) << (turnoAnterior + 1) << "                            |" << endl;
    cout << "+----------------------------------------------------+" << endl;
    
    if (huboDanio) {
        cout << "|  [*] Las torres han atacado a los enemigos!      |" << endl;
    } else {
        cout << "|  [ ] No hubo ataques en este turno.              |" << endl;
    }
    
    if (huboMovimiento) {
        cout << "|  [>] Los enemigos han avanzado!                  |" << endl;
    }
    
    cout << "|  [+] Vidas: " << juego.obtenerVidas() << " | HP: " << juego.obtenerVidaActual() << "/100";
    cout << setw(47 - (13 + to_string(juego.obtenerVidas()).length() + to_string(juego.obtenerVidaActual()).length())) << " " << " |" << endl;
    cout << "|  [$] Dinero: $" << setw(47) << juego.obtenerDinero() << " |" << endl;
    cout << "|  [E] Enemigos activos: " << setw(37) << juego.obtenerEnemigosCount() << " |" << endl;
    cout << "+====================================================+" << endl;
    
    mostrarMapa(juego);
}

void iniciarSiguienteOleada(Juego& juego) {
    if (!juego.enCombate()) {
        cout << "\n[!] No hay combate activo. Inicie el combate primero (opcion 5)." << endl;
        return;
    }
    
    if (juego.obtenerEnemigosCount() > 0) {
        cout << "\n[!] Aun hay enemigos en el campo de batalla." << endl;
        cout << "  Debes eliminar a todos los enemigos antes de iniciar la siguiente oleada." << endl;
        return;
    }
    
    NodoOleada* oleadaActual = juego.obtenerOleadas()->obtenerActual();
    if (oleadaActual == nullptr) {
        cout << "\n[!] No hay mas oleadas disponibles. Has completado todas las oleadas." << endl;
        return;
    }
    
    cout << "\n+====================================================+" << endl;
    cout << "|                 OLEADA " << setw(3) << oleadaActual->idOleada << "                            |" << endl;
    cout << "+----------------------------------------------------+" << endl;
    cout << "|  Enemigos: " << setw(46) << oleadaActual->cantidadEnemigos << " |" << endl;
    cout << "|  Tipo: " << setw(50) << oleadaActual->tipoEnemigo << " |" << endl;
    cout << "|  Vida: " << setw(50) << oleadaActual->vidaBase << " |" << endl;
    cout << "|  Velocidad: " << setw(45) << oleadaActual->velocidadBase << " |" << endl;
    cout << "|  Recompensa: $" << setw(43) << oleadaActual->recompensaBase << " |" << endl;
    cout << "+====================================================+" << endl;
    
    if (juego.iniciarSiguienteOleada()) {
        cout << "\n[OK] " << oleadaActual->cantidadEnemigos << " enemigos entran al campo de batalla!" << endl;
        cout << "  Todos comienzan en posicion 0." << endl;
        mostrarMapa(juego);
    } else {
        cout << "\n[ERROR] No se pudo iniciar la siguiente oleada." << endl;
    }
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============ MAIN ============

int main() {
    Juego juego;
    mostrarIntro();
    
    int opcion;
    do {
        if (!juego.estaActivo()) {
            mostrarGameOver(juego);
            break;
        }
        
        mostrarMenuPrincipal();
        cin >> opcion;
        
        switch (opcion) {
            case 1:
                menuTorres(juego);
                break;
            
            case 2:
                menuOleadas(juego);
                break;
            
            case 3:
                cout << "\n=== OLEADAS REGISTRADAS ===" << endl;
                mostrarOleadas(juego);
                break;
            
            case 4:
                mostrarEstadoCombate(juego);
                break;
            
            case 5: {
                cout << "\n=== INICIAR COMBATE ===" << endl;
                
                if (juego.enCombate()) {
                    cout << "[!] El combate ya esta activo." << endl;
                    break;
                }
                
                if (juego.obtenerOleadasCount() == 0) {
                    cout << "[!] No hay oleadas registradas. Registre al menos una oleada primero." << endl;
                    break;
                }
                
                if (juego.obtenerTorresCount() == 0) {
                    cout << "[!] No hay torres construidas. Construya al menos una torre primero." << endl;
                    char respuesta;
                    cout << "Desea continuar sin torres? (s/n): ";
                    cin >> respuesta;
                    if (respuesta != 's' && respuesta != 'S') {
                        break;
                    }
                }
                
                if (juego.iniciarCombate()) {
                    cout << "\n[OK] COMBATE INICIADO!" << endl;
                    cout << "  Los enemigos comenzaran en posicion 0." << endl;
                    cout << "  Las torres atacaran ANTES de que los enemigos se muevan." << endl;
                    
                    char iniciar;
                    cout << "\nDesea iniciar la primera oleada? (s/n): ";
                    cin >> iniciar;
                    
                    if (iniciar == 's' || iniciar == 'S') {
                        iniciarSiguienteOleada(juego);
                    }
                } else {
                    cout << "[ERROR] Error al iniciar el combate." << endl;
                }
                break;
            }
            
            case 6:
                cout << "\n=== INICIAR SIGUIENTE OLEADA ===" << endl;
                iniciarSiguienteOleada(juego);
                break;
            
            case 7:
                cout << "\n=== AVANZAR TURNO ===" << endl;
                procesarAvanzarTurno(juego);
                break;
            
            case 8:
                cout << "\n+====================================================+" << endl;
                cout << "|     Gracias por jugar Tower Defense!               |" << endl;
                cout << "+====================================================+" << endl;
                break;
            
            default:
                cout << "\n[ERROR] Opcion invalida. Intente nuevamente." << endl;
                limpiarBuffer();
        }
        
    } while (opcion != 8);
    
    return 0;
}
