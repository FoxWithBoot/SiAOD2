// ЛР2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <vector>
using namespace std;

class Town { //узел графа
private:
	char name; //название узла
	vector<Town*> roads; //указатели на узлы, к которым идут дороги из данного узла

public:
	Town(char name) { //конструктор
		this->name = name;
	}

	char getName() {
		return name;
	}

	void addRoad(Town* newTown) { //добавляет дорогу из данного узла
		roads.push_back(newTown);
	}

	Town* getRoadto(int i) { //возвращает указатель на узел, к которому идет дорога из данного узла
		return roads[i];
	}

	int getSize() { //возвращает кол-во дорог из данного узла
		return roads.size();
	}
};

class Graf {
private:
	vector<Town*> towns; //указатели на узлы графа
	vector<string> roadsAtoB; //все возможные дороги из А в Б (через узел путь может проходить только один раз)
	vector<vector<int>> roads; //матрица смежности графа
public:
	void setTowns(vector<char> names, vector<vector<int>>roads) { //вводит кол-во узлов, их названия и матрицу смежности графа
		for (int i = 0; i < names.size(); i++) {
			towns.push_back(new Town(names[i]));
		}

		for (int i = 0; i < roads.size(); i++) {					//по матрице смежности устанавливает для каждого узла список указателей,
			for (int j = 0; j < roads[i].size(); j++) {				//на узлы к которым идут дороги из данного узла
				if (roads[i][j] > 0) towns[i]->addRoad(towns[j]);	//
			}
		}

		this->roads = roads;
	}

	void RoadsAtoB(char a, char b) { //возвращает все возможные пути из А в Б
		Town* at = towns[0];
		Town* bt = towns[0];
		string s = "";
		vector<string> t;
		roadsAtoB.resize(0);

		for (int i = 0; i < towns.size(); i++) {	//устанавливаем указатели  на узлы с именами a и b
			if (towns[i]->getName() == a) at = towns[i];
			if (towns[i]->getName() == b) bt = towns[i];
		}

		s = roadsBilder(at, bt, s); //записываем все пути из a в b, в том числе с двойными заходами в города

		for (int i = 0; i < roadsAtoB.size(); i++) {
			if ((roadsAtoB[i][0] == a) && (roadsAtoB[i][roadsAtoB[i].length() - 1] == b)) t.push_back(roadsAtoB[i]); //отсеиваем пути с повторяющемися городами
		}

		roadsAtoB = t;
	}

	vector<string> getRoadsAtoB() {
		return roadsAtoB;
	}

	string longestRoadAtoB(char a, char b) { //длиннейший путь из А в Б
		RoadsAtoB(a, b);
		vector<int>  lenghtRoads = getlenghtRoads();
		int max = 0;
		int maxIndex;
		for (int i = 0; i < lenghtRoads.size(); i++) {
			if (lenghtRoads[i] > max) {
				max = lenghtRoads[i];
				maxIndex = i;
			}
		}
		return roadsAtoB[maxIndex];
	}

	string shortestRoadAtoB(char a, char b) { //длиннейший путь из А в Б
		RoadsAtoB(a, b);
		vector<int>  lenghtRoads = getlenghtRoads();
		int min = -1;
		int minIndex;
		for (int i = 0; i < lenghtRoads.size(); i++) {
			if ((lenghtRoads[i] < min) || (i == 0)) {
				min = lenghtRoads[i];
				minIndex = i;
			}
		}
		return roadsAtoB[minIndex];
	}

private:
	string roadsBilder(Town* a, Town* b, string road) { //записываем все пути из a в b, в том числе с двойными заходами в города
		bool boo = false;

		for (int i = 0; i < road.length(); i++) {	//проверка на повторяющийся город
			if (road[i] == a->getName()) boo = true;
		}
		if (boo) {	//если в пути уже есть город, в котором сейчас находимся, то возвращаем и записываем этот путь
			road.insert(road.length(), 1, a->getName());
			roadsAtoB.push_back(road);
			road.erase(road.length() - 1, 1);
			return road;
		}

		if (a->getName() == b->getName()) { //если данный город, является конечным городом, то записываем и возвращаем путь
			road.insert(road.length(), 1, a->getName());
			roadsAtoB.push_back(road);
			road.erase(road.length() - 1, 1);
			return road;
		}

		road.insert(road.length(), 1, a->getName()); //добавить этот узел в путь

		for (int i = 0; i < a->getSize(); i++) { //рекурсия
			road = roadsBilder(a->getRoadto(i), b, road);
		}

		road.erase(road.length() - 1, 1);
		return road;
	}

	vector<int> getlenghtRoads() { //считает длину путей из А в Б
		vector<int> lenghts;
		for (int i = 0; i < roadsAtoB.size(); i++) {
			lenghts.push_back(getlen(roadsAtoB[i]));
		}
		return lenghts;
	}

	int getlen(string road) { //считает длину пути для одной дороги из А в Б
		int len = 0;
		for (int i = 0; i < road.length() - 1; i++) {
			len += roads[nameToindex(road[i])][nameToindex(road[i + 1])];
		}
		return len;
	}

	int nameToindex(char name) { //переводит имя узла в его индекс в векторе
		for (int i = 0; i < towns.size(); i++) {
			if (towns[i]->getName() == name) return i;
		}
	}
};

int main()
{
	vector<string> a;

	vector<char> names;
	names.push_back('A');
	names.push_back('B');
	names.push_back('C');
	names.push_back('D');
	names.push_back('E');
	names.push_back('F');
	names.push_back('G');

	vector<vector<int>> roads = {
	{0,5,8,12,0,0,0},
	{0,0,7,0,20,0,0},
	{8,0,0,0,2,0,0},
	{0,3,0,0,0,0,15},
	{0,0,0,0,0,10,0},
	{0,0,5,4,0,0,6},
	{25,0,0,0,6,0,0} };

	Graf w;
	w.setTowns(names, roads);

	w.RoadsAtoB('A', 'G');
	a = w.getRoadsAtoB();

	for (int i = 0; i < a.size(); i++) {
		cout << a[i] << '\n';
	}

	cout << "Long way: " << w.longestRoadAtoB('A', 'G') << '\n';
	cout << "Short way: " << w.shortestRoadAtoB('A', 'G') << '\n';
}
