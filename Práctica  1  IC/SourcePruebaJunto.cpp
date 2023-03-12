#include<iostream>
#include<vector>
#include<unordered_map>
#include<map>
#include"Nodo.h"
#include<iomanip>
#include<stdlib.h>
#include <windows.h>
#include <conio.h>
#include<stdio.h>
#include <cstdlib>
#include <locale>
#include<cmath>
#include "Vista.h"
using namespace std;

//cosas por añadir: faltan los nodos peligrosos y prohibidos.
//liberar espacio como por ejemplo sustituir nodos por coordenadas.
//cambiar map por unordered_map. 
//cambiar ints por double debido a raices cuadradas.


//*******************************************************************************************
//se añade el coste h(x,y) a todas las posiciones.
void costeH(vector<vector<tNodo>>& mapa, tNodo fin) {
	int fil = mapa[1].size(), col = mapa.size();
	int x = fin.x, y = fin.y;

	for (int i = 0; i < col; i++) {
		for (int j = 0; j < fil; j++) {
			if (i == y && j == x) {
				mapa[i][j].h = 0;
			}
			else {
				mapa[i][j].h = sqrt(pow(x - j, 2) + pow(y - i, 2)); //de momento lo dejo así a lo mejor pongo una constante o algo
			}

		}
	}
}

int costeANodo(int x1, int x2, int y1, int y2) {
	if ((x1 + x2) - (y1 + y2) % 2 == 0)
		return 1;
	else
	return sqrt(2); //de momento lo dejo así a lo mejor pongo una constante o algo
}

tNodo escogerNodo(unordered_map<pair<int, int>, tNodo> abierta) {
	tNodo asignado;
	bool hecho = true; //no me funciona el null así que he hecho esta caca :(

	for (auto n : abierta) {
		if (hecho) {
			asignado = n.second;
			hecho = false;
		}
		else {
			if (asignado.f > n.second.f) {
				asignado = n.second;
			}
		}
	}
	return asignado;
}


void algoritmo(vector<vector<tNodo>> mapa, tNodo inicio, tNodo fin) {
	unordered_map<pair<int, int>, tNodo> cerrada;
	unordered_map<pair<int, int>, tNodo> abierta; //{posición, Nodo}
	bool alcanzado = false;

	//cuando vayamos a insertar los way points, calculemos el coste h para que el algoritmo se centre en asignarles. Al llegar al wave point,
	//se volverá a calcular el costeH para llegar a fin.
//	costeH(mapa, fin);

	//se inserta el nodo de inicio
	tNodo nodo = inicio;
	nodo.estado = "a";
	abierta.insert({ {nodo.y,nodo.x},nodo });


	// en este bucle tendremos que insertar el nodo mas barato f(x) en la lista cerrada, quitarlo de la lista abierta, expandir ese nodo y volver a asignar un nodo nuevo
	while (!alcanzado) {
		nodo.estado = "c";
		//por que es y x y  no xy??
		cerrada.insert({ { nodo.y,nodo.x }, nodo });
		abierta.erase({ nodo.y,nodo.x });

		//(vector<vector<tNodo>>& mapa, tNodo n, unordered_map<pair<int, int>, tNodo>& abierta, unordered_map<pair<int, int>, tNodo>& cerrada, tNodo& fin, tNodo ini)

		//alcanzado = expandir(mapa, nodo, abierta, cerrada, fin, inicio);
		alcanzado = true;
		if (abierta.size() == 0 && !alcanzado) {
			cout << "no hay solución" << endl;
		}
		else {
			nodo = escogerNodo(abierta);
		}
	}
}


/*

//cuando se añade un nodo a la vista cerrada, se añaden los nuevos posibles nodos a la lista abierta y se corrigen los nodos de la lista cerrada
bool expandir(vector<vector<tNodo>>& mapa, tNodo n, unordered_map<pair<int, int>, tNodo>& abierta, unordered_map<pair<int, int>, tNodo>& cerrada, tNodo& fin, tNodo ini) {

	int x = n.x, y = n.y; //las coordenadas del nuevo nodo


	//se examinan los nodos adyacientes del nodo insertado
	for (int i = y - 1; i < y + 2; i++) {
		for (int j = x - 1; j < x + 2; j++) {
			if (mapa[i][j].h <= ini.h + 1) {
				if (i == fin.y && j == fin.x) { // es el fin?
					fin.padre = { n.y,n.x };
					cerrada.insert({ {fin.y,fin.x},fin });
					fin.estado = "c";
					return true;
				}

				int coste = n.g + costeANodo(n.y, n.x, i, j); //se calcula el coste g(x) del supuesto hijo del nodo insertado.

				if (cerrada.count({ i,j }) != 0) {// Si está cerrada. Hay que optimizarlo.

					//no puede ser su padre
					// pongo este if dentro de la lista cerrada porque el padre del nodo tiene que estar insertado en esa lista.
					if (i != n.padre.first && j != n.padre.second) {

						if (coste < mapa[i][j].g) {// compara costes g(x) y elige el mas barato
							mapa[i][j].g = coste;
							mapa[i][j].f = mapa[i][j].h + mapa[i][j].g;
							mapa[i][j].padre = { n.y,n.x };
							mapa[n.y][n.x].hijos.push_back({ i,j });
							expandir(mapa, mapa[i][j], abierta, cerrada, fin, ini);
						}
					}
				}
				else if (abierta.count({ i,j }) != 0) {//el nodo está en abierta compara los costes y elige el mas barato
					if (mapa[i][j].g > coste) {
						mapa[i][j].padre = { n.y,n.x };
						mapa[n.y][n.x].hijos.push_back({ i,j });
						mapa[i][j].g = coste;
						mapa[i][j].f = mapa[i][j].g + mapa[i][j].h;
						tNodo& nodo = abierta.at({ i,j });
						nodo.g = coste;
						nodo.f = nodo.g + nodo.h;
					}
				}
				else { //no está en la lista abierta, añadelo
					mapa[i][j].padre = { n.y,n.x };
					mapa[n.y][n.x].hijos.push_back({ i,j });
					mapa[i][j].g = coste;
					mapa[i][j].f = mapa[i][j].g + mapa[i][j].h;
					mapa[i][j].estado = "a";
					abierta.insert({ { i,j }, mapa[i][j] });
				}
			}
		}
	}
	return false;
}
// Esto cumple esto? si
*/


/*

void costeH(vector<vector<tNodo>>& mapa, tNodo fin);

bool expandir(vector<vector<tNodo>>& mapa, tNodo n, unordered_map<pair<int, int>, tNodo>& abierta, unordered_map<pair<int, int>, tNodo>& cerrada, tNodo& fin, tNodo ini);

tNodo escogerNodo(unordered_map<pair<int, int>, tNodo> abierta);

//esto creo que deberia ser double porq sino la raiz de dos pierde decimales
int costeANodo(int x1, int x2, int y1, int y2);

*/


//*********************************************************************************

void cambiarColor(vector<vector<tNodo>> Mapa, int x, int y) {
	if (Mapa[x][y].estado == "c") {
		cout << ORANGE;
	}
	else if (Mapa[x][y].estado == "a") {
		cout << ROSE;
	}
	else if (Mapa[x][y].estado == "s") {
		cout << LGREEN;
	}
	else {
		cout << WHITE;
	}
}

bool posValida(int x, int y, vector<vector<tNodo>> Mapa) {
	return ((x > 0 && x <= Mapa.size()) && (y > 0 && y <= Mapa[0].size()) && Mapa[x - 1][y - 1].tipo != "inicio" && Mapa[x - 1][y - 1].tipo != "meta");
}

vector<vector<tNodo>> CrearVacio(int N, int M) {
	vector<vector<tNodo>> Mapa(N, vector<tNodo>(M));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			tNodo n;
			Mapa[i][j] = { n };
		}
	}
	return Mapa;
}

void mostarPorPantalla(vector<vector<tNodo>> Mapa) {
	cout << " \n MAPA\n";
	cout << " _";
	for (int i = 0; i < Mapa[0].size(); i++) {
		cout << "__";
	}
	cout << "\n";
	for (int i = 0; i < Mapa.size(); i++) {
		cout << " |";
		for (int j = 0; j < Mapa[i].size(); j++) {

			cambiarColor(Mapa, i, j);

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
	tNodo ini, meta;
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
				ini = Mapa[x - 1][y - 1];
			}
			system("cls");
			break;
		case 2:
			cout << " Introduce las coordenadas donde quieras poner la meta (solo hay una meta)" << endl;
			cin >> x >> y;
			if (!hayMeta && posValida(x, y, Mapa)) {
				Mapa[x - 1][y - 1].tipo = "meta";
				hayMeta = true;
				meta = ini = Mapa[x - 1][y - 1];
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

			if (hayInicio && hayMeta) {
				//llamar a la función que realice el a*
				//algoritmo(Mapa, ini, meta);
				cout << "\n El camino marcado en verde se corresponde con los nodos solución\n";
				cout << "\n El camino marcado en naranja se corresponde con los nodos que están en la lista cerrada\n";
				cout << "\n El camino marcado en rosa se corresponde con los nodos que están en la lista abierta\n";
			}
			else {
				cout << "Para poder comenzar introduce un incio y una meta\n";
			}
			break;
		case 7:
			hayInicio = false; hayMeta = false;
			meta = ini = tNodo();
			Mapa = CrearVacio(N, M);
			break;
		default:
			cout << " Opcion no valida. Intentalo de nuevo." << endl;
			system("cls");
			break;
		}
	} while (opcion != 0);

	cout << " Saliendo del programa. Vuelve pronto!\n";
	return 0;
}