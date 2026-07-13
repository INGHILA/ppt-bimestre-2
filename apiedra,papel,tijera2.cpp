#include<iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;
int menu=6;

typedef struct
{
	string nombre;
	int eleccion;
	int vicorias;
	int derrotas;
	int empates;
} jugador;


void limpiarBuffer() {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
};
void imprimirMenu() {
	cout<<"----------Menu----------"<<endl;
	cout<<"0.Salida"<<endl;
	cout<<"1.j vs cpu"<<endl;
	cout<<"2.j1 vs j2"<<endl;
	cout<<"3.j vs cpu(modo secreto)"<<endl;
	cout<<"4.j1 vs j2(modo secreto)"<<endl;
	cout<<"5.Marcadores"<<endl;
};



int main() {
	
	jugador j1;
	j1.nombre;
	jugador j2;
	j2.nombre;
	cout<<"Jugador 1 ingrese su nombre: ";
	cin>>j1.nombre;
	cout<<"Jugador 2 ingrese su nombre: ";
	cin>>j2.nombre;

	while(menu!=0) {
		imprimirMenu();
		cout<<"Elija una opcion: ";
		cin>>menu;
		srand(time(0));
	int cpu1=1+std::rand()%3;
	int cpu2=1+std::rand()%5;
		while (cin.fail()){
		limpiarBuffer();	
			imprimirMenu();
			cout << "Ingrese un número válido: ";
			cin>>menu;}
		switch (menu)
		{
		case 0:
			cout<<"Buen juego";
			return 0;
			break;
		case 1:
		{
			jugador j1;
			jugador victorias1;
			jugador derrotas1;
			jugador empates1;
			j1.eleccion;
			cout<<"Piedra=1,Papel=2,Tijera=3"<<endl;
			cout<<"Elija su opcion: ";
			cin>>j1.eleccion;
			
			while (cin.fail() || j1.eleccion < 1 || j1.eleccion > 3)
			{	
				limpiarBuffer();
				cout<<"Ingrese un valor valido: ";
				cin>>j1.eleccion;
				}
				if(j1.eleccion+1==cpu1 || j1.eleccion-2==cpu1)
			{
				cout<<"Usted perdio"<<endl;
				j1.derrotas++;
			}
			else if(j1.eleccion==cpu1) {
				cout<<"Empate"<<endl;
				j1.empates++;
			}
			else{
				cout<<"Usted gano"<<endl;
			cout<<"La opcion de la computadora fue: "<<cpu1<<endl;
			j1.vicorias++;
			cout<<j1.empates<<endl;
			cout<<j1.derrotas<<endl;
			cout<<j1.vicorias<<endl;
			}
			}
			break;
			
			case 3:{
            jugador j1;
			j1.eleccion;
			cout<<"Piedra=1,Papel=2,Tijera=3"<<endl;
			cout<<"Elija su opcion: ";
			cin>>j1.eleccion;
			
			while (cin.fail() || j1.eleccion < 1 || j1.eleccion > 3)
			{	
				limpiarBuffer();
				cout<<"Ingrese un valor valido: ";
				cin>>j1.eleccion;
				}
				if(j1.eleccion+1==cpu2 || j1.eleccion-2==cpu2)
			{
				cout<<"Usted perdio"<<endl;
				j1.derrotas++;
			}
			else if(j1.eleccion==cpu2) {
				cout<<"Empate"<<endl;
				j1.empates++;
			}
			else{
				cout<<"Usted gano"<<endl;
			cout<<"La opcion de la computadora fue: "<<cpu1<<endl;
			j1.vicorias++;
			cout<<j1.empates<<endl;
			cout<<j1.derrotas<<endl;
			cout<<j1.vicorias<<endl;
			
			}
			}
		break;
		default:
			cout<<"opcion no valida:"<<endl;
			break;
		}
	}
}
