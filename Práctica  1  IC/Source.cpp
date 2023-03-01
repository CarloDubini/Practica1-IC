#include<iostream>
#include<vector>
#include<unordered_map>
#include<map>
#include"Nodo.h"

//cosas por añadir: faltan los nodos peligrosos y prohibidos.
//liberar espacio como por ejemplo sustituir nodos por coordenadas.
//cambiar map por unordered_map. 
//cambiar ints por double debido a raices cuadradas.

using namespace std;

void costeH(vector<vector<tNodo>>& mapa, tNodo fin);

void expandir(vector<vector<tNodo>>& mapa, tNodo n, unordered_map<pair<int, int>, tNodo>& abierta, unordered_map<pair<int, int>, int>& cerrada, tNodo& fin, tNodo ini);

tNodo escojerNodo(unordered_map<pair<int, int>, tNodo> abierta);

int costeANodo(int x1, int x2, int y1, int y2);

void algoritmo(vector<vector<tNodo>> mapa, tNodo inicio, tNodo fin ) {
	unordered_map<pair<int,int>, tNodo> cerrada;
	unordered_map<pair<int,int>,tNodo> abierta; //{posición, Nodo}
	bool alcanzado = false;

	//cuando vayamos a insertar los way points, calculemos el coste h para que el algoritmo se centre en asignarles. Al llegar al wave point, 
	//se volverá a calcular el costeH para llegar a fin.
	costeH(mapa, fin);

	//se inserta el nodo de inicio
	tNodo nodo = inicio;
	abierta.insert({ {nodo.y,nodo.x},nodo });
	
	
	// en este bucle tendremos que insertar el nodo mas barato f(x) en la lista cerrada, quitarlo de la lista abierta, expandir ese nodo y volver a asignar un nodo nuevo
	while (!alcanzado) {
		cerrada.insert({ nodo.y,nodo.x }, nodo);
		abierta.erase({ nodo.y,nodo.x });

		alcanzado = expandir(mapa, nodo, abierta, cerrada, fin,inicio);

		if (abierta.size() == 0 && !alcanzado) {
			cout << "no hay solución" << endl;
		}
		else {
			nodo = escojerNodo(abierta);
		}
	}
}

void costeH(vector<vector<tNodo>>& mapa, tNodo fin) {//se añade el coste h(x,y) a todas las posiciones.
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

//cuando se añade un nodo a la vista cerrada, se añaden los nuevos posibles nodos a la lista abierta y se corrigen los nodos de la lista cerrada
bool expandir(vector<vector<tNodo>>& mapa, tNodo n, unordered_map<pair<int, int>,tNodo>& abierta, unordered_map<pair<int, int>, tNodo>& cerrada, tNodo& fin,tNodo ini) {
	
	int x = n.x, y = n.y; //las coordenadas del nuevo nodo


	//se examinan los nodos adyacientes del nodo insertado
	for (int i = y - 1; i < y + 2; i++) {
		for (int j = x - 1; j < x + 2; j++) {
			if (mapa[i][j].h <= ini.h + 1) {
				if (i == fin.y && j == fin.x) { // es el fin?
					fin.padre = { n.y,n.x };
					cerrada.insert({ {fin.y,fin.x},fin });
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
							expandir(mapa, mapa[i][j], abierta, cerrada, fin,ini);
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
					abierta.insert({ { i,j }, mapa[i][j] });
				}
			}
		}
	}
	return false;
}
// Esto cumple esto? si

tNodo escojerNodo(unordered_map<pair<int, int>, tNodo> abierta) {
	tNodo asignado;
	bool hecho=true; //no me funciona el null así que he hecho esta caca :(

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
int costeANodo(int x1, int x2, int y1, int y2) {
	if ((x1 + x2) - (y1 + y2) % 2 == 0)
		return 1;
	else
		return sqrt(2); //de momento lo dejo así a lo mejor pongo una constante o algo
}
