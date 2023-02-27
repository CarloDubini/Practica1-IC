#include<iostream>
#include<iomanip>
#include<vector>
#include<stdlib.h>
#include <windows.h>
#include <conio.h>
#include<stdio.h>
#include <cstdlib>
#include <locale>
#include "colores.h"
using namespace std;

//creo que a lo mejor tenemos que añadir un campo tipo para ver si es obstaculo, libre, meta, inicio, etc
//para la mejora si nos da tiempo hay que añadir un campo penalizacion, para que los peligros se eviten y siempre se pase por los way points
struct tNodo {
	int x = 0, y=0;
	int h=0, g=0;
	int f=0;
	string tipo= "libre";
	//cerrado(en la cola cerrada), abierta(en la lista de nodos en abierta), sol(parte del camino solucion), n(sin procesar)
	string estado = "n";
	int penalizacion = 0;
};

void cambiarColor(vector<vector<tNodo>> Mapa, int x, int y) {
	if (Mapa[x][y].estado == "cerrada") {
		cout << ORANGE;
	}
	else if (Mapa[x][y].estado == "abierta") {
		cout << ROSE;
	}
	else if (Mapa[x][y].estado == "sol") {
		cout << LGREEN;
	}
	else {
		cout << WHITE;
	}
}

bool posValida(int x, int y, vector<vector<tNodo>> Mapa) {
	return ((x>0 && x<=Mapa.size()) && (y>0 && y<=Mapa[0].size()) && Mapa[x-1][y-1].tipo != "inicio" && Mapa[x-1][y-1].tipo != "meta");
}

vector<vector<tNodo>> CrearVacio(int N, int M) {
	vector<vector<tNodo>> Mapa(N, vector<tNodo>(M));
	for (int i = 0; i < N;i++) {
		for (int j = 0; j < M;j++) {
			tNodo n;
			Mapa[i][j] = {n};
		}
	}
	return Mapa;
}

void mostarPorPantalla(vector<vector<tNodo>> Mapa) {
	cout << " \n MAPA\n";
	cout << " _";
	for (int i = 0; i < Mapa[0].size();i++) {
		cout << "__";
	}
	cout << "\n";
	for (int i = 0; i < Mapa.size();i++) {
		cout << " |";
		for (int j = 0; j < Mapa[i].size(); j++) {
				
			cambiarColor(Mapa,i,j);

			if (Mapa[i][j].tipo == "libre") {
				cout << "~";
			}
			else if (Mapa[i][j].tipo == "inicio") {
				cout << "I";
			}
			else if (Mapa[i][j].tipo == "meta") {
				cout << "M";
			}
			else if (Mapa[i][j].tipo == "obstaculo") {
				cout << "X";
			}
			else if (Mapa[i][j].tipo == "peligro") {
				cout << "P";
			}
			else {
				cout << "$";
			}
			cout << WHITE << "|";
			system("color");
		}
		cout << "\n";
	}
	cout << " -";
	for (int i = 0; i < Mapa[0].size(); i++) {
		cout << "--";
		system("color");
	}
	cout << "\n";
	
}

int main() {
	std::locale::global(std::locale(""));
	system("color 37");

	int N, M;
	bool hayInicio = false, hayMeta = false;
	int x, y;
	cout << " Escribe las dimensiones que quieres que tenga tu mapa (filas y columnas):  \n";
	//n=filas y m=columnas
	cin >> N >> M;
	vector<vector<tNodo>> Mapa = CrearVacio(N, M);
	system("cls");

	int opcion;
	do {
		mostarPorPantalla(Mapa);
		cout << " MENU:" << endl;
		cout << " 0. Salir" << endl;
		cout << " 1. Añadir Inicio" << endl;
		cout << " 2. Añadir Meta" << endl;
		cout << " 3. Añadir Obstaculo (no se puede acceder)" << endl;
		cout << " 4. Añadir Peligro (se puede pasar por la casilla pero tendrá una penalización)" << endl;
		cout << " 5. Añadir WayPoint" << endl;
		cout << " 6. Iniciar el programa para encontrar el camino" << endl;
		cout << " 7. Resetear el mapa" << endl;
		cout << " Escriba su opcion: ";
		cin >> opcion;
		system("cls");
		switch (opcion) {
			case 1:
				cout << " Introduce las coordenadas donde quieras poner el inicio (solo hay un inicio)" << endl;
				cin >> x >> y;
				if (!hayInicio && posValida(x, y, Mapa)) {
					Mapa[x - 1][y - 1].tipo = "inicio";
					hayInicio = true;
				}
				system("cls");
				break;
			case 2:
				cout << " Introduce las coordenadas donde quieras poner la meta (solo hay una meta)" << endl;
				cin >> x >> y;
				if (!hayMeta && posValida(x, y, Mapa)) {
					Mapa[x - 1][y - 1].tipo = "meta";
					hayMeta = true;
				}
				system("cls");
				break;
			case 3:
				cout << " Introduce las coordenadas donde quieras poner obstaculos (para terminar escribe -1)" << endl;
				while (cin >> x && x != -1) {
					cin >> y;
					if (posValida(x, y, Mapa)) {
						Mapa[x - 1][y - 1].tipo = "obstaculo";
					}
				}
				system("cls");
				break;
			case 4:
				cout << " Introduce las coordenadas donde quieras poner peligros (para terminar escribe -1)" << endl;
				while (cin >> x && x != -1) {
					cin >> y;
					if (posValida(x, y, Mapa)) {
						Mapa[x - 1][y - 1].tipo = "peligro";
					}
				}
				system("cls");
				break;
			case 5:
				cout << " Introduce las coordenadas donde quieras poner way points (para terminar escribe -1)" << endl;
				while (cin >> x && x != -1) {
					cin >> y;
					if (posValida(x, y, Mapa)) {
						Mapa[x - 1][y - 1].tipo = "wp";
					}
				}
				system("cls");
				break;
			case 6:
				//codigo para pruebas de color
				Mapa[1][1].estado = "sol";
				Mapa[1][2].estado = "cerrada";
				Mapa[0][0].estado = "abierta";
				Mapa[0][1].estado = "sol";
				if(hayInicio && hayMeta){
					//llamar a la función que realice el a*
					cout << "\n El camino marcado en verde se corresponde con los nodos solución\n";
					cout << "\n El camino marcado en naranja se corresponde con los nodos que están en la lista cerrada\n";
					cout << "\n El camino marcado en rosa se corresponde con los nodos que están en la lista abierta\n";
				}
				else{
					cout << "Para poder comenzar introduce un incio y una meta\n";
				}
				break;
			case 7:
				hayInicio = false; hayMeta = false;
				Mapa = CrearVacio(N, M);
				break;
			default:
				cout << " Opcion no valida. Intentalo de nuevo." << endl;
				system("cls");
				break;
		}
	} while (opcion != 0);

	cout <<" Saliendo del programa. Vuelve pronto!\n";
	return 0;
}
