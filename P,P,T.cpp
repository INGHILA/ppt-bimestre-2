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

using namespace std;
int menu = 6;

struct jugador {
	string nombre;
	int eleccion;
	int victorias = 0;
	int derrotas = 0;
	int empates = 0;
};

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
	cout << "5. Marcadores" << endl;
}

// NUEVA FUNCIÓN: Guarda o actualiza los marcadores en ejemplo.txt manteniendo tu formato
void guardarMarcadores(jugador& j1,jugador& j2) {
	string path = "ejemplo.txt";
	ofstream file(path); // Sobrescribe el archivo para actualizar los datos actuales
	
	if (file.is_open()) {
		// Cabecera obligatoria
		file << "Victorias,derrotas,empates\n";
		file << j1.nombre << " " << j1.victorias << "," << j1.derrotas << "," << j1.empates << "\n";
		file << j2.nombre << " " << j2.victorias << "," << j2.derrotas << "," << j2.empates << "\n";
		file.close();
	} else {
		cerr << "[Error] No se pudo actualizar el archivo de marcadores." << endl;
	}
}

void cargarMarcadores(jugador& j1, jugador& j2) {
	string path = "ejemplo.txt";
	ifstream file(path);
	string read;
	
	if (file.is_open()) {
		// Ignorar la primera línea de cabecera ("Victorias,derrotas,empates")
		getline(file, read);
		
		// Leer Jugador 1 si existe
		if (getline(file, read)) {
			stringstream ss(read);
			string v, d, e;
			ss >> j1.nombre; // Lee el nombre (hasta el espacio)
			if (getline(ss, v, ',') && getline(ss, d, ',') && getline(ss, e, ',')) {
				// El primer getline después de 'ss >> j1.nombre' atrapa el espacio y el número de victorias
				// Para limpiarlo de manera segura, convertimos las strings a enteros:
				j1.victorias = stoi(v);
				j1.derrotas = stoi(d);
				j1.empates = stoi(e);
			}
		}
		// Leer Jugador 2 si existe
		if (getline(file, read)) {
			stringstream ss(read);
			string v, d, e;
			ss >> j2.nombre;
			if (getline(ss, v, ',') && getline(ss, d, ',') && getline(ss, e, ',')) {
				j2.victorias = stoi(v);
				j2.derrotas = stoi(d);
				j2.empates = stoi(e);
			}
		}
		file.close();
		cout << "[Sistema] Marcadores previos cargados con exito." << endl;
	} else {
		cout << "[Sistema] No se encontro archivo previo. Se creara uno nuevo al jugar." << endl;
	}
}

int main() {
	srand(time(0));

	jugador j1;
	jugador j2;

	
	cargarMarcadores(j1, j2);

	
	if (j1.nombre.empty() || j2.nombre.empty()) {
		cout << "Jugador 1 ingrese su nombre: ";
		cin >> j1.nombre;
		cout << "Jugador 2 ingrese su nombre: ";
		cin >> j2.nombre;
		guardarMarcadores(j1, j2); // Crea el archivo inicial
	} else {
		cout << "\n¡Bienvenidos de vuelta " << j1.nombre << " y " << j2.nombre << "!" << endl;
	}

	while(menu != 0) {
		imprimirMenu();
		cout << "Elija una opcion: ";
		cin >> menu;

		while (cin.fail() || menu < 0 || menu > 5) {
			limpiarBuffer();
			imprimirMenu();
			cout << "Ingrese un numero valido (0-5): ";
			cin >> menu;
		}

		int cpu1 = 1 + rand() % 3;
		int cpu2 = 1 + rand() % 5;

		switch (menu) {
		case 0:
			guardarMarcadores(j1, j2); // Guardado final de seguridad
			cout << "Buen juego. ¡Hasta la proxima!" << endl;
			return 0;

		case 1: { // J1 vs CPU
			cout << "\nPiedra=1, Papel=2, Tijera=3" << endl;
			cout << j1.nombre << ", elija su opcion: ";
			cin >> j1.eleccion;

			while (cin.fail() || j1.eleccion < 1 || j1.eleccion > 3) {
				limpiarBuffer();
				cout << "Ingrese un valor valido (1-3): ";
				cin >> j1.eleccion;
			}

			cout << "La CPU eligio: " << cpu1 << endl;

			if (j1.eleccion == cpu1) {
				cout << "=> Empate" << endl;
				j1.empates++;
			}
			else if (j1.eleccion + 1 == cpu1 || j1.eleccion - 2 == cpu1) {
				cout << "=> Perdiste. Gana la CPU." << endl;
				j1.derrotas++;
			}
			else {
				cout << "=> ¡Ganaste!" << endl;
				j1.victorias++;
			}
			guardarMarcadores(j1, j2); // Guarda el progreso de la ronda
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
				j1.empates++;
				j2.empates++;
			}
			else if (j1.eleccion + 1 == j2.eleccion || j1.eleccion - 2 == j2.eleccion) {
				cout << "=> ¡" << j2.nombre << " gana la ronda!" << endl;
				j2.victorias++;
				j1.derrotas++;
			}
			else {
				cout << "=> ¡" << j1.nombre << " gana la ronda!" << endl;
				j1.victorias++;
				j2.derrotas++;
			}
			guardarMarcadores(j1, j2); // Guarda el progreso de la ronda
			break;
		}

		case 3: { // J1 vs CPU (Modo Secreto)
			cout << "\nPiedra=1, Papel=2, Tijera=3, Lagarto=4, Spock=5" << endl;
			cout << j1.nombre << ", elija su opcion: ";
			cin >> j1.eleccion;

			while (cin.fail() || j1.eleccion < 1 || j1.eleccion > 5) {
				limpiarBuffer();
				cout << "Ingrese un valor valido (1-5): ";
				cin >> j1.eleccion;
			}

			cout << "La CPU eligio: " << cpu2 << endl;

			if (j1.eleccion == cpu2) {
				cout << "=> Empate" << endl;
				j1.empates++;
			}
			else if ((j1.eleccion==3 && cpu2==2) || (j1.eleccion==2 && cpu2==1) ||
			         (j1.eleccion==1 && cpu2==4) || (j1.eleccion==4 && cpu2==5) ||
			         (j1.eleccion==5 && cpu2==3) || (j1.eleccion==3 && cpu2==4) ||
			         (j1.eleccion==4 && cpu2==2) || (j1.eleccion==2 && cpu2==5) ||
			         (j1.eleccion==5 && cpu2==1) || (j1.eleccion==1 && cpu2==3)) {
				cout << "=> ¡Ganaste!" << endl;
				j1.victorias++;
			}
			else {
				cout << "=> Perdiste" << endl;
				j1.derrotas++;
			}
			guardarMarcadores(j1, j2); // Guarda el progreso de la ronda
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
				j1.empates++;
				j2.empates++;
			}
			else if ((j1.eleccion==3 && j2.eleccion==2) || (j1.eleccion==2 && j2.eleccion==1) ||
			         (j1.eleccion==1 && j2.eleccion==4) || (j1.eleccion==4 && j2.eleccion==5) ||
			         (j1.eleccion==5 && j2.eleccion==3) || (j1.eleccion==3 && j2.eleccion==4) ||
			         (j1.eleccion==4 && j2.eleccion==2) || (j1.eleccion==2 && j2.eleccion==5) ||
			         (j1.eleccion==5 && j2.eleccion==1) || (j1.eleccion==1 && j2.eleccion==3)) {
				cout << "=> ¡" << j1.nombre << " gana la ronda!" << endl;
				j1.victorias++;
				j2.derrotas++;
			}
			else {
				cout << "=> ¡" << j2.nombre << " gana la ronda!" << endl;
				j2.victorias++;
				j1.derrotas++;
			}
			guardarMarcadores(j1, j2); // Guarda el progreso de la ronda
			break;
		}

		case 5: { // Marcadores modificados para procesar el formato solicitado
			string path = "ejemplo.txt";
			try {
				ifstream file;
				string line;
				file.open(path);
				if (!file) {
					throw runtime_error("No se pudo abrir el archivo de marcadores.");
				}
				
				cout << "\n=============================================" << endl;
				// Leemos la primera línea (cabecera)
				if (getline(file, line)) {
					cout << setw(15) << left << "Jugador" << " | " << line << endl;
					cout << "---------------------------------------------" << endl;
				}

				// Leemos línea por línea los datos de los jugadores
				while (getline(file, line)) {
					stringstream ss(line);
					string nombre, vic, der, emp;
					
					ss >> nombre;
					
					// Extraemos el resto de datos separados por comas
					if (getline(ss, vic, ',') && getline(ss, der, ',') && getline(ss, emp, ',')) {
						cout << setw(15) << left << nombre << " | "
						     << setw(10) << left << vic << ", "
						     << setw(9) << left << der << ", "
						     << emp << endl;
					}
				}
				cout << "=============================================" << endl;
				file.close();
			}
			catch (exception& e) {
				cerr << e.what() << endl;
			}
		}
		break;

		default:
			cout << "Opcion no valida." << endl;
			break;
		}
	}
	return 0;
}
