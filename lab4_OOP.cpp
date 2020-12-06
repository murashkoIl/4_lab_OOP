#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
class Node
{
private:
	T node;
public:
	string getObject(){
		return node;
	}
	Node(T _node = T()){
		node = _node;
	}
};

class Edge {
	short begin;
	short end;
public:
	Edge(short _begin = 0, short _end = 0){
		begin = _begin;
		end = _end;
	}
	short getBegin(){
		return begin;
	}
	short getEnd(){
		return end;
	}
	void setBegin(short _begin){
		begin = _begin;
	}
	void setEnd(short _end){
		end = _end;
	}
};

template <typename T>
class Graph
{
private:
	vector<Node<T>> node;
	vector<Edge> edge;
public:
	Graph(void) {};
	~Graph(void) {};

	bool is_empty(void){
		if (node.size() == 0){
			return true;
		}
		return false;
	}

	void clear(void){
		node.clear();
		edge.clear();
	}

	void addNode(T _nodeObject){
		if (isNodeExist(_nodeObject) == -1){
			node.push_back(Node<T>(_nodeObject));
		}
	}

	void addEdge(T iObject, T jObject) {
		int i = isNodeExist(iObject), j = isNodeExist(jObject);
		if (i != -1 && j != -1 && isEdgeExist(i, j) == false) {
			edge.push_back(Edge(isNodeExist(iObject), isNodeExist(jObject)));
		}
	}

	void addEdge(short i, short j){
		if (isEdgeExist(i, j) == false){
			edge.push_back(Edge(i, j));
		}
	}

	short nodeLevel(T _nodeObject){
		short level = 0;
		short nodeNumber = isNodeExist(_nodeObject);
		if (nodeNumber != -1){
			for (int i = 0; i < node.size(); i++){
				bool isExist = isEdgeExist(i, nodeNumber);
				level += isExist == true ? 1 : 0;
			}
			return isEdgeExist(nodeNumber, nodeNumber) == true ? ++level : level;
		}
		else{
			return -1;
		}
	}

	void deleteNode(T _nodeObject){
		short _nodeNumber = isNodeExist(_nodeObject);
		if (_nodeNumber != -1){
			node.erase(node.begin() + _nodeNumber);
			for (int i = 0; i < edge.size(); i++){
				if (isEdgeExist(i, _nodeNumber)){
					deleteEdge(i, _nodeNumber);
				}
			}
		}
	}

	void deleteEdge(T iObject, T jObject) {
		for (int k = 0; k < edge.size(); k++) {
			if (isEdgeExist(iObject, jObject) != -1) {
				edge.erase(edge.begin() + k);
			}
		}
	}

	void deleteEdge(short i, short j){
		for (int k = 0; k < edge.size(); k++){
			if (isEdgeExist(i, j) == true || isEdgeExist(j, i) == true){
				edge.erase(edge.begin() + k);
				for (int m = k; m < edge.size() - 1; m++){
					edge[m] = edge[m + 1];
				}
				edge.pop_back();
				if (edge.size() != 0){
					edge.pop_back();
				}
			}
		}
	}

	void edgesFromFile(string filename){ // сперва создаем узлы, потом строим между ними дуги
		ifstream file;
		file.open(filename);
		if (!file.is_open() == true){
			file.close();
			return;
		}
		short beginNode = 0, endNode = 0;
		while (file.eof() == false){
			file >> beginNode >> endNode;
			if (beginNode >= node.size() || endNode >= node.size()){
				continue;
			}
			edge.push_back(Edge(beginNode, endNode));
		}
		file.close();
	}

	bool isEdgeExist(short i, short j){
		for (int k = 0; k < edge.size(); k++){
			if ((edge[k].getBegin() == i && edge[k].getEnd() == j) || (edge[k].getBegin() == j && edge[k].getEnd() == i)){
				return true;
			}
		}
		return false;
	}
	short isEdgeExist(T iObject, T jObject){
		for (int k = 0; k < edge.size(); k++){
			if ((node[edge[k].getBegin()].getObject() == iObject && node[edge[k].getEnd()].getObject() == jObject) ||
				(node[edge[k].getBegin()].getObject() == jObject && node[edge[k].getEnd()].getObject() == iObject)){
				return k;
			}
		}
		return -1;
	}
	short isNodeExist(T _nodeObject){
		for (int i = 0; i < node.size(); i++){
			if (node[i].getObject() == _nodeObject){
				return i;
			}
		}
		return -1;
	}
	bool isNodeExist(short _nodeNumber){
		return _nodeNumber < node.size();
	}

	short countOfLoops(){
		short count = 0;
		for (int i = 0; i < node.size(); i++){
			count += isEdgeExist(i, i) == true ? 1 : 0;
		}
		return count;
	}

	short countOfNodes(){
		return node.size();
	}
	short countOfEdges(){
		return edge.size();
	}

	void showNodes(){
		cout << endl;
		for (int i = 0; i < node.size(); i++){
			cout << node[i].getObject() << endl;
		}
	}
	void showEdges(){
		cout << endl;
		for (int i = 0; i < edge.size(); i++){
			cout << node[edge[i].getBegin()].getObject() << " - " << node[edge[i].getEnd()].getObject() << endl;
		}
	}
	class Node_Iterator
	{
	private:
		Graph<T>* iterGr;
		int index;
	public:
		Node_Iterator(Graph<T>& graph){
			iterGr = &graph;
			index = 0;
		}

		bool next(){
			if (iterGr->node.size() <= index + 1){
				return false;
			}
			index++;
			return true;
		}
		bool prev(){
			if (index == 0){
				return false;
			}
			index--;
			return true;
		}

		void getAdjacentNodes(){
			short countOfAdjacent = 0;
			cout << endl;
			for (int i = 0; i < iterGr->node.size(); i++){
				if (iterGr->isEdgeExist(index, i) == true){
					countOfAdjacent++;
					cout << iterGr->node[i].getObject() << ' ';
				}
			}
			if (countOfAdjacent == 0){
				cout << "No nodes adjacent to this node";
			}
			cout << endl;
		}
		void getIncidentEdges(){
			cout << endl;
			short countOfIncident = 0;
			for (int i = 0; i < iterGr->node.size(); i++){
				if (iterGr->isEdgeExist(index, i) == true){
					countOfIncident++;
					cout << iterGr->node[index].getObject() << " - " << iterGr->node[i].getObject() << endl;
				}
			}
			if (countOfIncident == 0){
				cout << "\tNo edges incident to this node";
			}
			cout << endl;
		}
	};

	class Edge_iterator
	{
	private:
		Graph<T>* iterGr;
		int index;

	public:
		Edge_iterator(Graph<T>& graph){
			iterGr = &graph;
			index = 0;
		}
		bool next(){
			if (iterGr->edge.size() <= index + 1){
				return false;
			}
			index++;
			return true;
		}
		bool prev(){
			if (index == 0){
				return false;
			}
			index--;
			return true;
		}
	};
};

int main(void)
{
	short choise = 1;
	string str_1 = "";
	string str_2 = "";

	Graph<string>* Graph1 = new Graph<string>();

	while (true)
	{
		cout << "1. Add node" << endl;
		cout << "2. Add edge" << endl;
		cout << "3. Is node exist" << endl;
		cout << "4. Is edge exist" << endl;
		cout << "5. Level of node" << endl;
		cout << "6. Number of edges and nodes" << endl;
		cout << "7. Show nodes" << endl;
		cout << "8. Show edges" << endl;
		cout << "9. Import from file" << endl;
		cout << "10. Iterator for nodes" << endl;
		cout << "11. Iterator for edges" << endl;
		cout << "12. Delete node" << endl;
		cout << "13. Delete edge" << endl;
		cout << "14. Is container empty" << endl;
		cout << "15. Clear" << endl;
		cout << "0. Exit" << endl;
		cout << "Your choise: ";

		cin >> choise;
		switch (choise) {
		case 0:
		{
			return 0;
		}
		case 1:
		{
			cout << endl << "Enter node name: ";
			cin >> str_1;
			system("cls");
			Graph1->addNode(str_1);
			break;
		}
		case 2:
		{
			cout << endl << "Enter nodes names: ";
			cin >> str_1 >> str_2;
			system("cls");
			Graph1->addEdge(str_1, str_2);
			break;
		}
		case 3:
		{
			cout << endl << "Enter node name: ";
			cin >> str_1;
			if (Graph1->isNodeExist(str_1) != -1) {
				cout << endl << "This node exist" << endl;
			}
			else {
				cout << endl << "This node doesn't exist" << endl;
			}
			system("pause");
			system("cls");
			break;
		}
		case 4:
		{
			cout << endl << "Enter a nodes names: ";
			cin >> str_1 >> str_2;
			if (Graph1->isEdgeExist(str_1, str_2) != -1) {
				cout << endl << "This edge exist" << endl;
			}
			else {
				cout << endl << "This edge doesn't exist" << endl;
			}
			system("pause");
			system("cls");
			break;
		}
		case 5:
		{
			cout << endl << "Enter a node name: ";
			cin >> str_1;
			short level = Graph1->nodeLevel(str_1);
			if (level == -1) {
				cout << "No node with this name" << endl;
			}
			else {
				cout << "Node level: " << level;
			}
			system("pause");
			system("cls");
			break;
		}
		case 6:
		{
			cout << endl << "Nodes: " << Graph1->countOfNodes()
				<< endl << "Edges: " << Graph1->countOfEdges()
				<< endl;
			system("pause");
			system("cls");
			break;
		}
		case 7:
		{
			if (Graph1->countOfNodes() == 0) {
				cout << "No nodes" << endl;
				system("pause");
				system("cls");
				break;
			}
			Graph1->showNodes();
			system("pause");
			system("cls");
			break;
		}
		case 8:
		{
			if (Graph1->countOfEdges() == 0) {
				cout << "No edges";
				system("pause");
				system("cls");
				break;
			}
			Graph1->showEdges();
			system("pause");
			system("cls");
			break;
		}
		case 9:
		{
			cout << endl << "Enter a file name: ";
			cin >> str_1;
			system("cls");
			Graph1->edgesFromFile(str_1);
			break;
		}
		case 10:
		{
			Graph<string>::Node_Iterator Graph_iterator(*Graph1);
			while (choise != 5) {
				cout << endl << "1. Next node" << endl;
				cout << "2. Previous node" << endl;
				cout << "3.Get adjacent nodes" << endl;
				cout << "4.Get incident edges" << endl;
				cout << "5.Exit" << endl;
				cout << "Your choise: ";
				cin >> choise;

				switch (choise) {
				case 1:
				{
					if (Graph_iterator.next() == false) {
						cout << endl << "Iterator is on the last node" << endl;
					}
					else {
						cout << endl << "Iterator moved to next node" << endl;
					}
					break;
				}
				case 2:
				{
					if (Graph_iterator.prev() == false) {
						cout << endl << "Iterator is on the first node" << endl;
					}
					else {
						cout << endl << "Iterator moved to previous node" << endl;
					}
					break;
				}
				case 3:
				{
					Graph_iterator.getAdjacentNodes();
					break;
				}
				case 4:
				{
					Graph_iterator.getIncidentEdges();
					break;
				}
				default:
					system("pause");
					system("cls");
					break;
				}
			}
			break;
		}
		case 11:
		{
			Graph<string>::Node_Iterator G_iterator(*Graph1);
			while (choise != 3) {
				cout << endl << "1. Next edge" << endl;
				cout << "2. Previous edge" << endl;
				cout << "3. Exit" << endl;
				cout << endl << "Your hoose: ";
				cin >> choise;

				switch (choise) {
				case 1:
				{
					if (G_iterator.next() == false) {
						cout << endl << "Iterator is on the last edge" << endl;
					}
					else {
						cout << endl << "Iterator moved to next edge" << endl;
					}
					break;
				}
				case 2:
				{
					if (G_iterator.prev() == false) {
						cout << endl << "Iterator is on the first edge" << endl;
					}
					else {
						cout << endl << "Iterator moved to previous edge" << endl;
					}
					break;
				}
				default:
					system("pause");
					system("cls");
					break;
				}
			}
			break;
		}
		case 12:
		{
			cout << endl << "Enter node name: ";
			cin >> str_1;
			system("cls");
			Graph1->deleteNode(str_1);
			break;
		}
		case 13:
		{
			cout << endl << "Enter nodes names: ";
			cin >> str_1 >> str_2;
			system("cls");
			Graph1->deleteEdge(str_1, str_2);
			break;
		}
		case 14:
		{
			(Graph1->is_empty() == true) ? cout << endl << "Empty" << endl : cout << endl << "Not empty" << endl;
			system("pause");
			system("cls");
			break;
		}
		case 15:
		{
			Graph1->clear();
			system("pause");
			system("cls");
			break;
		}
		}

	}
}