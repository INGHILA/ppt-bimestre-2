#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;
int menu = 6;

typedef struct{
    string nombre;
    int eleccion;
    int victoriasCPU = 0;
    int derrotasCPU = 0;
    int empatesCPU = 0;
    int victoriasVS = 0;
    int derrotasVS = 0;
    int empatesVS = 0;
}jugador;

typedef struct{
    string nombre;
    int victoriasCPU;
    int derrotasCPU;
    int empatesCPU;
    int victoriasVS;
    int derrotasVS;
    int empatesVS;
    int totalPartidas;
}historialJugador;

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void imprimirMenu() {
    cout << "\n----------Menu----------" << endl;
    cout << "0. Salida (Guarda automaticamente)" << endl;
    cout << "1. J vs CPU" << endl;
    cout << "2. J1 vs J2" << endl;
    cout << "3. J vs CPU (modo secreto)" << endl;
    cout << "4. J1 vs J2 (modo secreto)" << endl;
    cout << "5. Marcadores de la sesion actual" << endl;
    cout << "6. Cambiar jugadores" << endl;
    cout << "7. Ver historial de todos los jugadores" << endl;
}

void guardarMarcadores(jugador& j1, jugador& j2) {
    string path = "ejemplo.txt";
    ofstream file(path);
    
    if (file.is_open()) {
        file << "Jugador,Vic_CPU,Der_CPU,Emp_CPU,Vic_VS,Der_VS,Emp_VS\n";
        file << j1.nombre << "," << j1.victoriasCPU << "," << j1.derrotasCPU << "," << j1.empatesCPU << ","
             << j1.victoriasVS << "," << j1.derrotasVS << "," << j1.empatesVS << "\n";
        file << j2.nombre << "," << j2.victoriasCPU << "," << j2.derrotasCPU << "," << j2.empatesCPU << ","
             << j2.victoriasVS << "," << j2.derrotasVS << "," << j2.empatesVS << "\n";
        file.close();
        cout << "[Sistema] Marcadores guardados exitosamente." << endl;
    } else {
        cerr << "[Error] No se pudo actualizar el archivo de marcadores." << endl;
    }
}

void guardarEnHistorial(const jugador& jug) {
    if (jug.nombre.empty()) return;
    
    string path = "historial.txt";
    vector<historialJugador> listaHistorial;
    string line;
    bool existe = false;
    
    ifstream readFile(path);
    if (readFile.is_open()) {
        while (getline(readFile, line)) {
            if (!line.empty()) {
                stringstream ss(line);
                historialJugador h;
                string vC, dC, eC, vV, dV, eV;
                
                if (getline(ss, h.nombre, ',') && getline(ss, vC, ',') && getline(ss, dC, ',') && getline(ss, eC, ',') &&
                    getline(ss, vV, ',') && getline(ss, dV, ',') && getline(ss, eV, ',')) {
                    
                    h.victoriasCPU = stoi(vC); h.derrotasCPU = stoi(dC); h.empatesCPU = stoi(eC);
                    h.victoriasVS = stoi(vV);   h.derrotasVS = stoi(dV);   h.empatesVS = stoi(eV);
                    h.totalPartidas = h.victoriasCPU + h.derrotasCPU + h.empatesCPU + h.victoriasVS + h.derrotasVS + h.empatesVS;
                    
                    if (h.nombre == jug.nombre) {
                        existe = true;
                        h.victoriasCPU += jug.victoriasCPU;
                        h.derrotasCPU += jug.derrotasCPU;
                        h.empatesCPU += jug.empatesCPU;
                        h.victoriasVS += jug.victoriasVS;
                        h.derrotasVS += jug.derrotasVS;
                        h.empatesVS += jug.empatesVS;
                        h.totalPartidas = h.victoriasCPU + h.derrotasCPU + h.empatesCPU + h.victoriasVS + h.derrotasVS + h.empatesVS;
                    }
                    listaHistorial.push_back(h);
                }
            }
        }
        readFile.close();
    }
    
    if (!existe) {
        historialJugador nuevo;
        nuevo.nombre = jug.nombre;
        nuevo.victoriasCPU = jug.victoriasCPU; nuevo.derrotasCPU = jug.derrotasCPU; nuevo.empatesCPU = jug.empatesCPU;
        nuevo.victoriasVS = jug.victoriasVS;   nuevo.derrotasVS = jug.derrotasVS;   nuevo.empatesVS = jug.empatesVS;
        nuevo.totalPartidas = nuevo.victoriasCPU + nuevo.derrotasCPU + nuevo.empatesCPU + nuevo.victoriasVS + nuevo.derrotasVS + nuevo.empatesVS;
        listaHistorial.push_back(nuevo);
    }
    
    sort(listaHistorial.begin(), listaHistorial.end(), [](const historialJugador& a, const historialJugador& b) {
        return (a.victoriasCPU + a.victoriasVS) > (b.victoriasCPU + b.victoriasVS);
    });
    
    if (listaHistorial.size() > 7) {
        listaHistorial.resize(7);
        cout << "[Sistema] Limite de historial alcanzado. Se conservo el Top 7 de mejores jugadores." << endl;
    }
    
    ofstream writeFile(path);
    if (writeFile.is_open()) {
        for (const auto& h : listaHistorial) {
            writeFile << h.nombre << "," << h.victoriasCPU << "," << h.derrotasCPU << "," << h.empatesCPU << ","
                      << h.victoriasVS << "," << h.derrotasVS << "," << h.empatesVS << "\n";
        }
        writeFile.close();
    }
}

void guardarJugadoresEnHistorial(const jugador& j1, const jugador& j2) {
    guardarEnHistorial(j1);
    guardarEnHistorial(j2);
}

vector<historialJugador> cargarHistorial() {
    vector<historialJugador> historial;
    string path = "historial.txt";
    ifstream file(path);
    
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                stringstream ss(line);
                historialJugador h;
                string vC, dC, eC, vV, dV, eV;
                
                if (getline(ss, h.nombre, ',') && getline(ss, vC, ',') && getline(ss, dC, ',') && getline(ss, eC, ',') &&
                    getline(ss, vV, ',') && getline(ss, dV, ',') && getline(ss, eV, ',')) {
                    
                    h.victoriasCPU = stoi(vC);
                    h.derrotasCPU = stoi(dC);
                    h.empatesCPU = stoi(eC);
                    h.victoriasVS = stoi(vV);
                    h.derrotasVS = stoi(dV);
                    h.empatesVS = stoi(eV);
                    h.totalPartidas = h.victoriasCPU + h.derrotasCPU + h.empatesCPU + h.victoriasVS + h.derrotasVS + h.empatesVS;
                    historial.push_back(h);
                }
            }
        }
        file.close();
    }
    return historial;
}

void cargarMarcadores(jugador& j1, jugador& j2) {
    string path = "ejemplo.txt";
    ifstream file(path);
    string read;
    
    if (file.is_open()) {
        getline(file, read); // Saltar cabecera
        
        if (getline(file, read)) {
            stringstream ss(read);
            string vC, dC, eC, vV, dV, eV;
            if (getline(ss, j1.nombre, ',') && getline(ss, vC, ',') && getline(ss, dC, ',') && getline(ss, eC, ',') &&
                getline(ss, vV, ',') && getline(ss, dV, ',') && getline(ss, eV, ',')) {
                j1.victoriasCPU = stoi(vC); j1.derrotasCPU = stoi(dC); j1.empatesCPU = stoi(eC);
                j1.victoriasVS = stoi(vV);   j1.derrotasVS = stoi(dV);   j1.empatesVS = stoi(eV);
            }
        }
        if (getline(file, read)) {
            stringstream ss(read);
            string vC, dC, eC, vV, dV, eV;
            if (getline(ss, j2.nombre, ',') && getline(ss, vC, ',') && getline(ss, dC, ',') && getline(ss, eC, ',') &&
                getline(ss, vV, ',') && getline(ss, dV, ',') && getline(ss, eV, ',')) {
                j2.victoriasCPU = stoi(vC); j2.derrotasCPU = stoi(dC); j2.empatesCPU = stoi(eC);
                j2.victoriasVS = stoi(vV);   j2.derrotasVS = stoi(dV);   j2.empatesVS = stoi(eV);
            }
        }
        file.close();
        cout << "[Sistema] Marcadores previos cargados con exito." << endl;
    } else {
        cout << "[Sistema] No se encontro archivo previo. Se creara uno nuevo al jugar." << endl;
    }
}

void cambiarJugadores(jugador& j1, jugador& j2) {
    if (!j1.nombre.empty() && !j2.nombre.empty()) {
        guardarJugadoresEnHistorial(j1, j2);
        cout << "[Sistema] Jugadores anteriores respaldados en el historial." << endl;
    }
    
    cout << "\n--- Cambiar Jugadores ---" << endl;
    cout << "Jugador 1 ingrese su nombre: ";
    cin >> j1.nombre;
    cout << "Jugador 2 ingrese su nombre: ";
    cin >> j2.nombre;
    
    j1.victoriasCPU = 0; j1.derrotasCPU = 0; j1.empatesCPU = 0; j1.victoriasVS = 0; j1.derrotasVS = 0; j1.empatesVS = 0;
    j2.victoriasCPU = 0; j2.derrotasCPU = 0; j2.empatesCPU = 0; j2.victoriasVS = 0; j2.derrotasVS = 0; j2.empatesVS = 0;
    
    cout << "[Sistema] Nuevos jugadores registrados." << endl;
    guardarMarcadores(j1, j2);
}

void mostrarHistorial() {
    vector<historialJugador> historial = cargarHistorial();
    
    if (historial.empty()) {
        cout << "\nNo hay historial de jugadores aun." << endl;
        return;
    }
    
    sort(historial.begin(), historial.end(), [](const historialJugador& a, const historialJugador& b) {
        return (a.victoriasCPU + a.victoriasVS) > (b.victoriasCPU + b.victoriasVS);
    });
    
    cout << "\n================================================================================" << endl;
    cout << "                          HISTORIAL GENERAL DE JUGADORES (TOP 7)" << endl;
    cout << "================================================================================" << endl;
    cout << setw(12) << left << "Jugador" 
         << "| Mod: vs CPU (V/D/E)   " 
         << "| Mod: J1 vs J2 (V/D/E) " 
         << "| Total Partidas" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    
    for (const auto& h : historial) {
        string stringCPU = to_string(h.victoriasCPU) + "/" + to_string(h.derrotasCPU) + "/" + to_string(h.empatesCPU);
        string stringVS  = to_string(h.victoriasVS) + "/" + to_string(h.derrotasVS) + "/" + to_string(h.empatesVS);
        
        cout << setw(12) << left << h.nombre << " | "
             << setw(21) << left << stringCPU << " | "
             << setw(21) << left << stringVS << " | "
             << h.totalPartidas << endl;
    }
    cout << "================================================================================" << endl;
}

int main() {
    srand(time(0));
    jugador j1;
    jugador j2;

    cargarMarcadores(j1, j2);

    if (j1.nombre.empty() || j2.nombre.empty()) {
        cout << "\nNo se encontraron jugadores previos. Ingrese los datos:" << endl;
        cout << "Jugador 1 ingrese su nombre: ";
        cin >> j1.nombre;
        cout << "Jugador 2 ingrese su nombre: ";
        cin >> j2.nombre;
        guardarMarcadores(j1, j2);
    } else {
        cout << "\n¡Bienvenidos de vuelta " << j1.nombre << " y " << j2.nombre << "!" << endl;
    }

    while(menu != 0) {
        imprimirMenu();
        cout << "Elija una opcion: ";
        cin >> menu;

        while (cin.fail() || menu < 0 || menu > 7) {
            limpiarBuffer();
            imprimirMenu();
            cout << "Ingrese un numero valido (0-7): ";
            cin >> menu;
        }

        int cpu1 = 1 + rand() % 3;
        int cpu2 = 1 + rand() % 5;

        switch (menu) {
        case 0:
            guardarJugadoresEnHistorial(j1, j2);
            guardarMarcadores(j1, j2);
            cout << "Buen juego. ¡Hasta la proxima!" << endl;
            return 0;

        case 1: { // J vs CPU (Selección manual en consola)
            cout << "\n¿Quien va a jugar contra la CPU?" << endl;
            cout << "1. " << j1.nombre << endl;
            cout << "2. " << j2.nombre << endl;
            cout << "Elija una opcion (1 o 2): ";
            int seleccionJugador;
            cin >> seleccionJugador;

            while (cin.fail() || (seleccionJugador != 1 && seleccionJugador != 2)) {
                limpiarBuffer();
                cout << "Opcion invalida. Elija 1 para " << j1.nombre << " o 2 para " << j2.nombre << ": ";
                cin >> seleccionJugador;
            }

            // Asignamos el puntero según lo que se digitó en la terminal
            jugador* actual = (seleccionJugador == 1) ? &j1 : &j2;

            cout << "\n--- TURNO DE: " << actual->nombre << " ---" << endl;
            cout << "Piedra=1, Papel=2, Tijera=3" << endl;
            cout << "Elija su opcion: ";
            cin >> actual->eleccion;

            while (cin.fail() || actual->eleccion < 1 || actual->eleccion > 3) {
                limpiarBuffer();
                cout << "Ingrese un valor valido (1-3): ";
                cin >> actual->eleccion;
            }

            cout << "La CPU eligio: " << cpu1 << endl;

            if (actual->eleccion == cpu1) {
                cout << "=> Empate" << endl;
                actual->empatesCPU++;
            }
            else if (actual->eleccion + 1 == cpu1 || actual->eleccion - 2 == cpu1) {
                cout << "=> Perdiste. Gana la CPU." << endl;
                actual->derrotasCPU++;
            }
            else {
                cout << "=> ¡Ganaste!" << endl;
                actual->victoriasCPU++;
            }
            guardarMarcadores(j1, j2);
            break;
        }

        case 2: { // J1 vs J2
            cout << "\nPiedra=1, Papel=2, Tijera=3" << endl;
            cout << j1.nombre << ", elija su opcion: ";
            cin >> j1.eleccion;
            while (cin.fail() || j1.eleccion < 1 || j1.eleccion > 3) {
                limpiarBuffer();
                cout << "Ingrese un valor valido (1-3): ";
                cin >> j1.eleccion;
            }

            cout << string(30, '\n');

            cout << "Piedra=1, Papel=2, Tijera=3" << endl;
            cout << j2.nombre << ", elija su opcion: ";
            cin >> j2.eleccion;
            while (cin.fail() || j2.eleccion < 1 || j2.eleccion > 3) {
                limpiarBuffer();
                cout << "Ingrese un valor valido (1-3): ";
                cin >> j2.eleccion;
            }

            cout << "\nResultados:" << endl;
            cout << j1.nombre << " eligio: " << j1.eleccion << endl;
            cout << j2.nombre << " eligio: " << j2.eleccion << endl;

            if (j1.eleccion == j2.eleccion) {
                cout << "=> ¡Empate!" << endl;
                j1.empatesVS++;
                j2.empatesVS++;
            }
            else if (j1.eleccion + 1 == j2.eleccion || j1.eleccion - 2 == j2.eleccion) {
                cout << "=> ¡" << j2.nombre << " gana la ronda!" << endl;
                j2.victoriasVS++;
                j1.derrotasVS++;
            }
            else {
                cout << "=> ¡" << j1.nombre << " gana la ronda!" << endl;
                j1.victoriasVS++;
                j2.derrotasVS++;
            }
            guardarMarcadores(j1, j2);
            break;
        }

        case 3: { // J vs CPU (Modo Secreto - Selección manual en consola)
            cout << "\n¿Quien va a jugar en Modo Secreto contra la CPU?" << endl;
            cout << "1. " << j1.nombre << endl;
            cout << "2. " << j2.nombre << endl;
            cout << "Elija una opcion (1 o 2): ";
            int seleccionJugador;
            cin >> seleccionJugador;

            while (cin.fail() || (seleccionJugador != 1 && seleccionJugador != 2)) {
                limpiarBuffer();
                cout << "Opcion invalida. Elija 1 para " << j1.nombre << " o 2 para " << j2.nombre << ": ";
                cin >> seleccionJugador;
            }

            jugador* actual = (seleccionJugador == 1) ? &j1 : &j2;

            cout << "\n--- TURNO DE: " << actual->nombre << " (Modo Secreto) ---" << endl;
            cout << "Piedra=1, Papel=2, Tijera=3, Lagarto=4, Spock=5" << endl;
            cout << "Elija su opcion: ";
            cin >> actual->eleccion;

            while (cin.fail() || actual->eleccion < 1 || actual->eleccion > 5) {
                limpiarBuffer();
                cout << "Ingrese un valor valido (1-5): ";
                cin >> actual->eleccion;
            }

            cout << "La CPU eligio: " << cpu2 << endl;

            if (actual->eleccion == cpu2) {
                cout << "=> Empate" << endl;
                actual->empatesCPU++;
            }
            else if ((actual->eleccion==3 && cpu2==2) || (actual->eleccion==2 && cpu2==1) ||
                     (actual->eleccion==1 && cpu2==4) || (actual->eleccion==4 && cpu2==5) ||
                     (actual->eleccion==5 && cpu2==3) || (actual->eleccion==3 && cpu2==4) ||
                     (actual->eleccion==4 && cpu2==2) || (actual->eleccion==2 && cpu2==5) ||
                     (actual->eleccion==5 && cpu2==1) || (actual->eleccion==1 && cpu2==3)) {
                cout << "=> ¡Ganaste!" << endl;
                actual->victoriasCPU++;
            }
            else {
                cout << "=> Perdiste" << endl;
                actual->derrotasCPU++;
            }
            guardarMarcadores(j1, j2);
            break;
        }

        case 4: { // J1 vs J2 (Modo Secreto)
            cout << "\nPiedra=1, Papel=2, Tijera=3, Lagarto=4, Spock=5" << endl;
            cout << j1.nombre << ", elija su opcion: ";
            cin >> j1.eleccion;
            while (cin.fail() || j1.eleccion < 1 || j1.eleccion > 5) {
                limpiarBuffer();
                cout << "Ingrese un valor valido (1-5): ";
                cin >> j1.eleccion;
            }

            cout << string(30, '\n');

            cout << "Piedra=1, Papel=2, Tijera=3, Lagarto=4, Spock=5" << endl;
            cout << j2.nombre << ", elija su opcion: ";
            cin >> j2.eleccion;
            while (cin.fail() || j2.eleccion < 1 || j2.eleccion > 5) {
                limpiarBuffer();
                cout << "Ingrese un valor valido (1-5): ";
                cin >> j2.eleccion;
            }

            cout << "\nResultados (Modo Secreto):" << endl;
            cout << j1.nombre << " eligio: " << j1.eleccion << endl;
            cout << j2.nombre << " eligio: " << j2.eleccion << endl;

            if (j1.eleccion == j2.eleccion) {
                cout << "=> ¡Empate!" << endl;
                j1.empatesVS++;
                j2.empatesVS++;
            }
            else if ((j1.eleccion==3 && j2.eleccion==2) || (j1.eleccion==2 && j2.eleccion==1) ||
                     (j1.eleccion==1 && j2.eleccion==4) || (j1.eleccion==4 && j2.eleccion==5) ||
                     (j1.eleccion==5 && j2.eleccion==3) || (j1.eleccion==3 && j2.eleccion==4) ||
                     (j1.eleccion==4 && j2.eleccion==2) || (j1.eleccion==2 && j2.eleccion==5) ||
                     (j1.eleccion==5 && j2.eleccion==1) || (j1.eleccion==1 && j2.eleccion==3)) {
                cout << "=> ¡" << j1.nombre << " gana la ronda!" << endl;
                j1.victoriasVS++;
                j2.derrotasVS++;
            }
            else {
                cout << "=> ¡" << j2.nombre << " gana la ronda!" << endl;
                j2.victoriasVS++;
                j1.derrotasVS++;
            }
            guardarMarcadores(j1, j2);
            break;
        }

        case 5: { // Mostrar Marcadores actuales estructurados
            string path = "ejemplo.txt";
            try {
                ifstream file(path);
                string line;
                if (!file) throw runtime_error("No se pudo abrir el archivo de marcadores.");
                
                cout << "\n=========================================================================" << endl;
                cout << "                     MARCADORES DE LA SESION ACTUAL                      " << endl;
                cout << "=========================================================================" << endl;
                if (getline(file, line)) {
                    cout << setw(12) << left << "Jugador" << " | vs CPU (V/D/E) | J1 vs J2 (V/D/E)" << endl;
                    cout << "-------------------------------------------------------------------------" << endl;
                }

                while (getline(file, line)) {
                    stringstream ss(line);
                    string nombre, vC, dC, eC, vV, dV, eV;
                    if (getline(ss, nombre, ',') && getline(ss, vC, ',') && getline(ss, dC, ',') && getline(ss, eC, ',') &&
                        getline(ss, vV, ',') && getline(ss, dV, ',') && getline(ss, eV, ',')) {
                            cout << setw(12) << left << nombre << " | "
                             << vC << "/" << dC << "/" << eC << setw(11) << right << " | "
                             << vV << "/" << dV << "/" << eV << endl;
                    }
                }
                cout << "=========================================================================" << endl;
                file.close();
            }
            catch (exception& e) {
                cerr << e.what() << endl;
            }
            break;
        }

        case 6: { // Cambiar jugadores
            cout << "\n--- Cambiar Jugadores ---" << endl;
            cout << "Los marcadores de ambos modos se acumularan en el historial general." << endl;
            cout << "¿Esta seguro que desea cambiar los jugadores?" << endl;
            cout << "1. Si, cambiar jugadores" << endl;
            cout << "2. No, volver al menu" << endl;
            cout << "Elija una opcion: ";
            
            int confirmar;
            cin >> confirmar;
            
            while (cin.fail() || (confirmar != 1 && confirmar != 2)) {
                limpiarBuffer();
                cout << "Opcion no valida. Elija 1 o 2: ";
                cin >> confirmar;
            }
            
            if (confirmar == 1) {
                cambiarJugadores(j1, j2);
            }
            else cout << "Operacion cancelada." << endl;
            break;
        }

        case 7:
            mostrarHistorial();
            break;

        default:
            cout << "Opcion no valida." << endl;
            break;
        }
    }
    return 0;
}
