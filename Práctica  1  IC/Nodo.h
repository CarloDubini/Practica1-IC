#include<iostream>
#include<set>
#include<vector>

using namespace std;


struct tNodo {
	int x = 0, y = 0; //posición del nodo
	pair<int, int> padre; //para saber quien es su padre. Te acuerdas de las flechitas en los ejercicios de IC? pues eso
	vector<pair<int, int>> hijos; //son los hijos del nodo.
	int h = 0, g = 0;
	int f = 0;
	string tipo = "libre"; //{libre,peligroso,bloqueado}
	string estado = "n"; //{n=nada, s=solucion,c=cerrada,a=abierta}
	bool penalizacion = false; 
};

/*
reglas:
- Si está en la lista abierta o cerrada, el nodo tiene un padre.


*/
