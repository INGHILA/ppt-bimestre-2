#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

typedef struct {
	string nombre;
	int eleccion;
	int victoriasCPU;
	int derrotasCPU;
	int empatesCPU;
	int victoriasVS;
	int derrotasVS;
	int empatesVS;
} jugador;

typedef struct {
	string nombre;
	int victoriasCPU;
	int derrotasCPU;
	int empatesCPU;
	int victoriasVS;
	int derrotasVS;
	int empatesVS;
	int totalPartidas;
} historialJugador;

void limpiarBuffer() {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// FUNCIÓN CRÍTICA: Maneja la validación de entrada en un solo lugar
// Evita repetir el mismo bucle de validación en todo el código
// Usa cin.fail() para detectar errores de tipo de dato y rango
int obtenerEntradaValida(const string& mensaje, int min, int max) {
	int opcion;
	cout << mensaje;
	cin >> opcion;
	while (cin.fail() || opcion < min || opcion > max) {
		limpiarBuffer();
		cout << "Entrada invalida. Ingrese un numero valido (" << min << "-" << max << "): ";
		cin >> opcion;
	}
	return opcion;
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
	string path = "Juego actual.txt";
	ofstream file(path);
	if (file.is_open()) {
		file << "Jugador,Vic_CPU,Der_CPU,Emp_CPU,Vic_VS,Der_VS,Emp_VS\n";
		file << j1.nombre << "," << j1.victoriasCPU << "," << j1.derrotasCPU << "," << j1.empatesCPU << ","
		     << j1.victoriasVS << "," << j1.derrotasVS << "," << j1.empatesVS << "\n";
		file << j2.nombre << "," << j2.victoriasCPU << "," << j2.derrotasCPU << "," << j2.empatesCPU << ","
		     << j2.victoriasVS << "," << j2.derrotasVS << "," << j2.empatesVS << "\n";
		file.close();
	}
}

// FUNCIÓN CLAVE: Lee el archivo CSV y lo convierte en una estructura de datos
// Importante porque el archivo guarda los datos en formato texto con comas
// y hay que parsearlos correctamente usando stringstream y getline con delimitador
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
				// PARSEO CRÍTICO: Cada campo está separado por coma
				// getline con ',' como delimitador extrae cada valor
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

// FUNCIÓN COMPLEJA: Actualiza el historial manteniendo solo el TOP 10
// 1. Carga todo el historial existente
// 2. Busca si el jugador ya existe (para acumular estadísticas)
// 3. Si no existe, lo agrega como nuevo
// 4. Ordena por victorias totales usando sort con lambda personalizada
// 5. Recorta a solo 10 elementos con resize()
// 6. Sobrescribe el archivo con los datos actualizados
void guardarEnHistorial(const jugador& jug) {
	if (jug.nombre.empty()) return;

	vector<historialJugador> listaHistorial = cargarHistorial();
	bool existe = false;

	for (auto& h : listaHistorial) {
		if (h.nombre == jug.nombre) {
			existe = true;
			h.victoriasCPU += jug.victoriasCPU;
			h.derrotasCPU += jug.derrotasCPU;
			h.empatesCPU += jug.empatesCPU;
			h.victoriasVS += jug.victoriasVS;
			h.derrotasVS += jug.derrotasVS;
			h.empatesVS += jug.empatesVS;
			h.totalPartidas = h.victoriasCPU + h.derrotasCPU + h.empatesCPU + h.victoriasVS + h.derrotasVS + h.empatesVS;
			break;
		}
	}

	if (!existe) {
		historialJugador nuevo = {jug.nombre, jug.victoriasCPU, jug.derrotasCPU, jug.empatesCPU,
		                          jug.victoriasVS, jug.derrotasVS, jug.empatesVS,
		                          jug.victoriasCPU + jug.derrotasCPU + jug.empatesCPU + jug.victoriasVS + jug.derrotasVS + jug.empatesVS
		                         };
		listaHistorial.push_back(nuevo);
	}

	// ORDENAMIENTO COMPLEJO: Lambda que compara el total de victorias
	// El operador > ordena de mayor a menor (descendente)
	sort(listaHistorial.begin(), listaHistorial.end(), [](const historialJugador& a, const historialJugador& b) {
		return (a.victoriasCPU + a.victoriasVS) > (b.victoriasCPU + b.victoriasVS);
	});

	// MANTIENE SOLO TOP 10: resize() elimina los elementos sobrantes
	if (listaHistorial.size() > 10) listaHistorial.resize(10);

	ofstream writeFile("historial.txt");
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

// FUNCIÓN CLAVE: Carga los marcadores usando una lambda para evitar duplicar código
// La lambda parseLinea encapsula la lógica de parseo de una línea de texto
void cargarMarcadores(jugador& j1, jugador& j2) {
	ifstream file("Juego actual.txt");
	string read;
	if (file.is_open()) {
		getline(file, read); // Saltar cabecera
		// LAMBDA: Función anónima que parsea una línea y la asigna a un jugador
		// Evita repetir el código de parseo para j1 y j2
		auto parseLinea = [](string& line, jugador& j) {
			stringstream ss(line);
			string vC, dC, eC, vV, dV, eV;
			if (getline(ss, j.nombre, ',') && getline(ss, vC, ',') && getline(ss, dC, ',') && getline(ss, eC, ',') &&
			        getline(ss, vV, ',') && getline(ss, dV, ',') && getline(ss, eV, ',')) {
				j.victoriasCPU = stoi(vC);
				j.derrotasCPU = stoi(dC);
				j.empatesCPU = stoi(eC);
				j.victoriasVS = stoi(vV);
				j.derrotasVS = stoi(dV);
				j.empatesVS = stoi(eV);
			}
		};
		if (getline(file, read)) parseLinea(read, j1);
		if (getline(file, read)) parseLinea(read, j2);
		file.close();
		cout << "[Sistema] Marcadores previos cargados con exito." << endl;
	} else {
		cout << "[Sistema] No se encontro archivo previo. Se creara uno nuevo al jugar." << endl;
	}
}

void resetearEstadisticas(jugador& j) {
	j.victoriasCPU = 0;
	j.derrotasCPU = 0;
	j.empatesCPU = 0;
	j.victoriasVS = 0;
	j.derrotasVS = 0;
	j.empatesVS = 0;
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
	resetearEstadisticas(j1);
	resetearEstadisticas(j2);
	cout << "[Sistema] Nuevos jugadores registrados." << endl;
	guardarMarcadores(j1, j2);
}

void mostrarHistorial() {
	vector<historialJugador> historial = cargarHistorial();
	if (historial.empty()) {
		cout << "\nNo hay historial de jugadores aun." << endl;
		return;
	}
	cout << "\n================================================================================" << endl;
	cout << "                            HISTORIAL GENERAL DE JUGADORES (TOP 10)" << endl;
	cout << "================================================================================" << endl;
	cout << setw(12) << left << "Jugador" << "| Mod: vs CPU (V/D/E)   " << "| Mod: J1 vs J2 (V/D/E) " << "| Total Partidas" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;

	for (const auto& h : historial) {
		string strCPU = to_string(h.victoriasCPU) + "/" + to_string(h.derrotasCPU) + "/" + to_string(h.empatesCPU);
		string strVS  = to_string(h.victoriasVS) + "/" + to_string(h.derrotasVS) + "/" + to_string(h.empatesVS);
		cout << setw(12) << left << h.nombre << " | " << setw(21) << left << strCPU << " | "
		     << setw(21) << left << strVS << " | " << h.totalPartidas << endl;
	}
	cout << "================================================================================" << endl;
}

// LÓGICA CRÍTICA DEL JUEGO: Evalúa las reglas de Piedra, Papel, Tijera
// La matemática detrás: en un juego de 3 elementos donde 1>3, 3>2, 2>1
// La condición j1 + 1 == j2 detecta cuando j2 es el que le gana a j1
// j1 - 2 == j2 cubre el caso especial donde 3 (tijera) gana a 1 (piedra)
int evaluarNormal(int j1, int j2) {
	if (j1 == j2) return 0;
	if (j1 + 1 == j2 || j1 - 2 == j2) return 2;
	return 1;
}

// LÓGICA CRÍTICA - MODO SECRETO: Extiende las reglas a Piedra, Papel, Tijera, Lagarto, Spock
// Más complejo porque hay más combinaciones posibles
// Las condiciones cubren todas las relaciones de victoria en el juego extendido
int evaluarSecreto(int j1, int j2) {
	if (j1 == j2) return 0;
	if (j1 + 1 == j2 || j1 - 2 == j2 || j1 + 3 == j2 || j1 - 4 == j2) return 2;
	return 1;
}

// FUNCIÓN IMPORTANTE: Maneja la lógica de juego contra la CPU
// Usa referencia (&) para modificar directamente los objetos originales
// El parámetro 'secreto' controla qué modo de juego usar
void jugarContraCPU(jugador& j1, jugador& j2, bool secreto) {
	int sel = obtenerEntradaValida("\n¿Quien va a jugar?\n1. " + j1.nombre + "\n2. " + j2.nombre + "\nElija (1 o 2): ", 1, 2);
	// Referencia al jugador que juega: evita duplicar código para j1 y j2
	jugador& actual = (sel == 1) ? j1 : j2;

	cout << "\n--- TURNO DE: " << actual.nombre << (secreto ? " (Modo Secreto)" : "") << " ---" << endl;
	string textoOpciones = secreto ? "Piedra=1, Papel=2, Tijera=3, Lagarto=4, Spock=5" : "Piedra=1, Papel=2, Tijera=3";
	int maxOp = secreto ? 5 : 3;

	actual.eleccion = obtenerEntradaValida(textoOpciones + "\nElija su opcion: ", 1, maxOp);
	int cpu = 1 + rand() % maxOp;  // Genera número aleatorio para la CPU
	cout << "La CPU eligio: " << cpu << endl;

	// Operador ternario para elegir qué función de evaluación usar según el modo
	int res = secreto ? evaluarSecreto(actual.eleccion, cpu) : evaluarNormal(actual.eleccion, cpu);

	if (res == 0) {
		cout << "=> Empate\n";
		actual.empatesCPU++;
	}
	else if (res == 1) {
		cout << "=> ¡Ganaste!\n";
		actual.victoriasCPU++;
	}
	else {
		cout << "=> Perdiste.\n";
		actual.derrotasCPU++;
	}

	guardarMarcadores(j1, j2);
}

// FUNCIÓN IMPORTANTE: Juego entre dos jugadores humanos
// Usa string(30, '\n') para "limpiar" la pantalla y ocultar la elección del primer jugador
void jugarVS(jugador& j1, jugador& j2, bool secreto) {
	string textoOpciones = secreto ? "Piedra=1, Papel=2, Tijera=3, Lagarto=4, Spock=5" : "Piedra=1, Papel=2, Tijera=3";
	int maxOp = secreto ? 5 : 3;

	cout << '\n' << textoOpciones << endl;
	j1.eleccion = obtenerEntradaValida(j1.nombre + ", elija su opcion: ", 1, maxOp);

	// OCULTAR ELECCIÓN: Imprime 30 líneas en blanco para que J2 no vea la elección de J1
	cout << string(30, '\n');
	cout << textoOpciones << endl;
	j2.eleccion = obtenerEntradaValida(j2.nombre + ", elija su opcion: ", 1, maxOp);

	cout << "\nResultados" << (secreto ? " (Modo Secreto):" : ":") << endl;
	cout << j1.nombre << " eligio: " << j1.eleccion << "\n" << j2.nombre << " eligio: " << j2.eleccion << endl;

	int res = secreto ? evaluarSecreto(j1.eleccion, j2.eleccion) : evaluarNormal(j1.eleccion, j2.eleccion);

	if (res == 0) {
		cout << "=> ¡Empate!\n";
		j1.empatesVS++;
		j2.empatesVS++;
	}
	else if (res == 1) {
		cout << "=> ¡" << j1.nombre << " gana la ronda!\n";
		j1.victoriasVS++;
		j2.derrotasVS++;
	}
	else {
		cout << "=> ¡" << j2.nombre << " gana la ronda!\n";
		j2.victoriasVS++;
		j1.derrotasVS++;
	}

	guardarMarcadores(j1, j2);
}

int main() {
	int menu = 6;
	srand(time(0));
	jugador j1, j2;

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

	while (menu != 0) {
		imprimirMenu();
		menu = obtenerEntradaValida("Elija una opcion: ", 0, 7);

		switch (menu) {
		case 0:
			guardarJugadoresEnHistorial(j1, j2);
			// PUNTO CRÍTICO: Resetea las estadísticas después de guardar en el historial
			// Así el archivo "Juego actual.txt" queda limpio para la próxima sesión
			resetearEstadisticas(j1);
			resetearEstadisticas(j2);
			guardarMarcadores(j1, j2);
			cout << "Buen juego. ¡Hasta la proxima!" << endl;
			return 0;
		case 1:
			jugarContraCPU(j1, j2, false);
			break;
		case 2:
			jugarVS(j1, j2, false);
			break;
		case 3:
			jugarContraCPU(j1, j2, true);
			break;
		case 4:
			jugarVS(j1, j2, true);
			break;
		case 5: {
			// LECTURA DIRECTA DEL ARCHIVO: Muestra los marcadores sin cargarlos en memoria
			ifstream file("Juego actual.txt");
			if (file.is_open()) {
				string line;
				cout << "\n=========================================================================\n";
				cout << "                     MARCADORES DE LA SESION ACTUAL                      \n";
				cout << "=========================================================================\n";
				if (getline(file, line)) cout << setw(12) << left << "Jugador" << " | vs CPU (V/D/E) | J1 vs J2 (V/D/E)\n-------------------------------------------------------------------------\n";
				while (getline(file, line)) {
					stringstream ss(line);
					string n, vC, dC, eC, vV, dV, eV;
					if (getline(ss, n, ',') && getline(ss, vC, ',') && getline(ss, dC, ',') && getline(ss, eC, ',') &&
					        getline(ss, vV, ',') && getline(ss, dV, ',') && getline(ss, eV, ',')) {
						cout << setw(12) << left << n << " | " << vC << "/" << dC << "/" << eC << setw(11) << right << " | " << vV << "/" << dV << "/" << eV << "\n";
					}
				}
				cout << "=========================================================================\n";
			}
			break;
		}
		case 6: {
			int confirmar = obtenerEntradaValida("\nLos marcadores se acumularan.\n¿Cambiar jugadores?\n1. Si\n2. No\nOpcion: ", 1, 2);
			if (confirmar == 1) cambiarJugadores(j1, j2);
			else cout << "Operacion cancelada.\n";
			break;
		}
		case 7:
			mostrarHistorial();
			break;
		}
	}
	return 0;
}
