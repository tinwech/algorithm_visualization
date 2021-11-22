#include "graph.h"
#include <iostream>
#include <queue>
#include <list>
using namespace std;

Graph::Graph(sf::RenderWindow& window) : window(window) {
	reset();
}
void Graph::init() {
	maxNumNodes = 1000;
	maxNumEdges = 1000;
	delete[] node;
	delete[] edge;
	delete[] freeNode;
	delete[] freeEdge;
	delete[] activeNode;
	delete[] activeEdge;
	curNumFreeNodes = maxNumNodes;
	curNumFreeEdges = maxNumEdges;
	curNumActiveNodes = 0;
	curNumActiveEdges = 0;
	freeNode = new int[maxNumNodes];
	freeEdge = new int[maxNumNodes];
	activeNode = new int[maxNumNodes];
	activeEdge = new int[maxNumNodes];
	selectedNode = NULL;
	node = new Node[maxNumNodes];
	for (int i = 0; i < maxNumNodes; i++) {
		freeNode[i] = i;
		node[i].id = i;
	}
	edge = new Edge[maxNumEdges];
	for (int i = 0; i < maxNumEdges; i++) {
		freeEdge[i] = i;
		edge[i].id = i;
	}
}
void Graph::createDefaultGraph() {
	init();
	int id_0 = addNode(sf::Vector2f(100, 200));
	int id_1 = addNode(sf::Vector2f(500, 300));
	addEdge(id_0, id_1);
	render();
}
void Graph::createRandomGraph() {
	init();
	int nodeNum = 20;
	double x = window.getSize().x;
	double y = window.getSize().y;
	vector<int> nodeID;
	for (int i = 0; i < nodeNum; i++) {
		double ratioX = rand() / (double)RAND_MAX;
		double ratioY = rand() / (double)RAND_MAX;
		cout << ratioX << " " << ratioY << endl;
		int id = addNode(sf::Vector2f(x * ratioX, y * ratioY));
		nodeID.push_back(id);
	}
	for (int i = 0; i < nodeNum; i++) {
		for (int j = i; j < nodeNum; j++) {
			double ratio = rand() / (double)RAND_MAX;
			if (ratio < 0.05) {
				addEdge(nodeID[i], nodeID[j]);
			}
		}
	}
	render();
}
void Graph::createDoubleCircle() {
	init();
	double dx = 5.0;
	double dz = 5.0;
	double r = 100; // radius
	double d = 50; // layer distance
	double offset_x = window.getSize().x / 2;
	double offset_y = window.getSize().y / 2;
	int n = 20;
	vector<int> nodeID;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < n; j++) {
			int radius = r + d * i;
			double theta = 2 * 3.14 * (j / (double)n);
			double x = radius * cos(theta);
			double y = radius * sin(theta);
			nodeID.push_back(addNode(sf::Vector2f(offset_x + x, offset_y + y)));
		}
	}
	for (int i = 0; i < n; i++) {
		int n1 = i;
		int n2 = (i + (rand() % 5 - 2)) % n + n;
		addEdge(nodeID[n1], nodeID[n2]);
	}
}
void Graph::createNetCircular() {
	init();
	double dx = 5.0;
	double dz = 5.0;
	double r = 50; // radius
	double offset_x = window.getSize().x / 2;
	double offset_z = window.getSize().y / 2;
	vector<int> nodeID;
	for (int layer = 1; layer <= 4; layer++) {
		for (int i = 0; i < 12; i++) {
			int radius = r * layer;
			double theta = 2 * 3.14 * (i / 12.0);
			double x = radius * cos(theta);
			double z = radius * sin(theta);
			nodeID.push_back(addNode(sf::Vector2f(offset_x + x, offset_z + z)));
			if (layer > 1) {
				int n1 = 12 * (layer - 2) + i;
				int n2 = 12 * (layer - 1) + i;
				addEdge(nodeID[n1], nodeID[n2]);
			}
		}
		if (layer < 4) {
			for (int i = 0; i < 12; i++) {
				int n1 = 12 * (layer - 1) + i;
				int n2 = 12 * (layer - 1) + (i + 1) % 12;
				addEdge(nodeID[n1], nodeID[n2]);
			}
		}
	}
}
void Graph::createNetSquare() {
	init();
	float dx = 30;
	float dz = 30;
	float offset_x = window.getSize().x / 4;
	float offset_z = window.getSize().y / 4;
	vector<vector<int>> nodeID;
	int num_layers = 16;
	int n = 0;
	int lowerBound = (num_layers - n) / 2 - 1;
	int upperBound = lowerBound + n + 1;
	for (int i = 0; i < num_layers; i++) {
		vector<int> row;
		for (int j = 0; j < num_layers; j++) {
			if (i > lowerBound && i < upperBound && j > lowerBound && j < upperBound) {
				row.push_back(-1);
			}
			else {
				float x = dx * j;
				float z = dz * i;
				row.push_back(addNode(sf::Vector2f(offset_x + x, offset_z + z)));
			}
		}
		nodeID.push_back(row);
	}
	for (int i = 0; i < num_layers; i++) {
		for (int j = 0; j < num_layers - 1; j++) {
			int n1 = nodeID[i][j];
			int n2 = nodeID[i][j + 1];
			if (n1 == -1 || n2 == -1) continue;
			addEdge(n1, n2);
		}
	}
	for (int i = 0; i < num_layers - 1; i++) {
		for (int j = 0; j < num_layers; j++) {
			int n1 = nodeID[i][j];
			int n2 = nodeID[i + 1][j];
			if (n1 == -1 || n2 == -1) continue;
			addEdge(n1, n2);
		}
	}
}
void Graph::reset() {
	init();
	createDefaultGraph();
}
void Graph::passivelySelect(double mouseX, double mouseY) {
	for (int i = 0; i < curNumActiveNodes; i++) {
		Node* n = &node[activeNode[i]];
		double d2 = getDistance(n->center.x, n->center.y, mouseX, mouseY);
		double r2 = n->r * n->r;
		if (d2 <= r2) {
			n->passivelyselected = true;
		}
		else {
			n->passivelyselected = false;
		}
	}
}
void Graph::drag(int id, double mouseX, double mouseY) {
	Node* n = &node[id];
	n->center = sf::Vector2f(mouseX, mouseY);
	n->pos.x = n->center.x - n->r;
	n->pos.y = n->center.y - n->r;
	render();
}
int Graph::onNode(double mouseX, double mouseY) {
	for (int i = 0; i < curNumActiveNodes; i++) {
		Node* n = &node[activeNode[i]];
		double d2 = getDistance(n->center.x, n->center.y, mouseX, mouseY);
		double r2 = n->r * n->r;
		if (d2 <= r2) {
			return n->id;
		}
	}
	return -1;
}
void Graph::deSelect() {
	if (selectedNode) {
		selectedNode->selected = false;
		selectedNode = NULL;
	}
}
void Graph::activelySelect(double mouseX, double mouseY) {
	int id = onNode(mouseX, mouseY);
	if (id == -1 && selectedNode) {
		selectedNode->selected = false;
		selectedNode = NULL;
	}
	else {
		Node* n = &node[id];
		if (n->selected) {
			deSelect();
		}
		else {
			n->selected = true;
			if (!selectedNode)
				selectedNode = n;
			else {
				addEdge(n->id, selectedNode->id);
				n->selected = false;
				deSelect();
			}
		}
	}
}
void Graph::addOrDelete(double mouseX, double mouseY) {
	bool found = false;
	int id;
	for (int i = 0; i < curNumActiveNodes; i++) {
		Node* n = &node[activeNode[i]];
		double d2 = getDistance(n->center.x, n->center.y, mouseX, mouseY);
		double r2 = n->r * n->r;
		if (d2 <= r2) {
			found = true;
			id = n->id;
			break;
		}
	}
	if (!found) {
		addNode(sf::Vector2f(mouseX, mouseY));
	}
	else {
		deleteNode(id);
	}
}
void Graph::deleteNode(int id) {
	Node* n = &node[id];
	while (n->edgeID.size() != 0) {
		deleteEdge(n->edgeID[0]);
	}
	int dynamicID = n->dynamicID;
	freeNode[curNumFreeNodes++] = id;
	swap(activeNode[dynamicID], activeNode[--curNumActiveNodes]);
	n = &node[activeNode[dynamicID]];
	n->dynamicID = dynamicID;
}
void Graph::deleteEdgeFromNode(int id) {
	Edge *e = &edge[id];
	Node *n0 = &node[e->nodeID[0]];
	Node *n1 = &node[e->nodeID[1]];
	int size = n0->edgeID.size();
	for (int i = 0; i < size; i++) {
		if (n0->edgeID[i] == id) {
			swap(n0->edgeID[i], n0->edgeID[size - 1]);
			break;
		}
	}
	n0->edgeID.resize(size - 1);
	size = n1->edgeID.size();
	for (int i = 0; i < size; i++) {
		if (n1->edgeID[i] == id) {
			swap(n1->edgeID[i], n1->edgeID[size - 1]);
			break;
		}
	}
	n1->edgeID.resize(size - 1);
}
void Graph::deleteEdge(int id) {
	Edge *e = &edge[id];
	int dynamicID = e->dynamicID;
	freeEdge[curNumFreeEdges++] = id;
	swap(activeEdge[dynamicID], activeEdge[--curNumActiveEdges]);
	e = &edge[activeEdge[dynamicID]];
	e->dynamicID = dynamicID;
	deleteEdgeFromNode(id);
}
void Graph::run(sf::Event& event) {
	this->event = &event;
	if (event.type == sf::Event::MouseMoved) {
		passivelySelect(event.mouseMove.x, event.mouseMove.y);
		if (dragID != -1 && mouseHold) {
			selectedNodePrevPos = node[dragID].pos;
			drag(dragID, event.mouseMove.x, event.mouseMove.y);
			selectedNodeCurPos = node[dragID].pos;
			if (selectedNodePrevPos != selectedNodeCurPos) {
				deSelect();
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			mouseHold = true;
			dragID = onNode(event.mouseButton.x, event.mouseButton.y);
			activelySelect(event.mouseButton.x, event.mouseButton.y);
		}
		else if (event.mouseButton.button == sf::Mouse::Right) {
			addOrDelete(event.mouseButton.x, event.mouseButton.y);
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			mouseHold = false;
			dragID = -1;
		}
	}
	else if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Q) {
			createDefaultGraph();
		}
		else if (event.key.code == sf::Keyboard::W) {
			createRandomGraph();
		}
		else if (event.key.code == sf::Keyboard::E) {
			createDoubleCircle();
		}
		else if (event.key.code == sf::Keyboard::R) {
			createNetCircular();
		}
		else if (event.key.code == sf::Keyboard::T) {
			createNetSquare();
		}
		else if (event.key.code == sf::Keyboard::B) {
			resetDepth();
			if (selectedNode)
				BFS(selectedNode->id);
		}
	}
}
void Graph::draw() {
	//draw edges
	for (int i = 0; i < curNumActiveEdges; i++) {
		Edge* e = &edge[activeEdge[i]];
		Node* n1 = &node[e->nodeID[0]];
		Node* n2 = &node[e->nodeID[1]];
		sf::Vertex line[] = {
			sf::Vertex(n1->center),
			sf::Vertex(n2->center)
		};
		window.draw(line, 2, sf::Lines);
	}
	//draw nodes
	for (int i = 0; i < curNumActiveNodes; i++) {
		Node* n = &node[activeNode[i]];
		sf::CircleShape circle(n->r);
		circle.setPosition(n->pos);
		if (n->passivelyselected || dragID == n->id) {
			circle.setFillColor(sf::Color::Blue);
		}
		else if (n->selected) {
			if (!mouseHold)
				circle.setFillColor(sf::Color::Red);
			else
				circle.setFillColor(sf::Color::Blue);
		}
		else if (n->depth != 0) {
			circle.setFillColor(sf::Color(255 - 255 * n->depth / (double)(maxDepth + 2), 50, 100));
		}
		else {
			circle.setFillColor(sf::Color(180, 180, 180));
		}
		window.draw(circle);
	}
}
double Graph::getDistance(double x1, double y1, double x2, double y2) {
	double dx = x1 - x2;
	double dy = y1 - y2;
	return dx * dx + dy * dy;
}
void Graph::render() {
	window.clear();
	draw();
	window.display();
}
Node* Graph::getFreeNode() {
	Node* n = NULL;
	if (curNumFreeNodes <= 0) return n;

	int index = freeNode[--curNumFreeNodes];
	n = &node[index];
	n->dynamicID = curNumActiveNodes;
	activeNode[curNumActiveNodes++] = index;
	n->edgeID.clear();
	return n;
}
int Graph::addNode(sf::Vector2f pos) {
	Node* n = getFreeNode();
	if (!n) {
		cout << "No nodes available" << endl;
		exit(1);
	}
	n->pos = pos;
	n->center.x = pos.x + n->r;
	n->center.y = pos.y + n->r;
	return n->id;
}
Edge* Graph::getFreeEdge() {
	Edge* e = NULL;
	if (curNumFreeEdges <= 0) return e;

	int index = freeEdge[--curNumFreeEdges];
	e = &edge[index];
	e->dynamicID = curNumActiveEdges;
	activeEdge[curNumActiveEdges++] = index;
	e->nodeID[0] = -1;
	e->nodeID[1] = -1;
	return e;
}
int Graph::addEdge(int id_0, int id_1) {
	Edge* e = getFreeEdge();
	if (!e) {
		cout << "No edges available" << endl;
		exit(1);
	}
	e->nodeID[0] = id_0;
	e->nodeID[1] = id_1;
	node[id_0].edgeID.push_back(e->id);
	node[id_1].edgeID.push_back(e->id);
	return e->id;
}
void Graph::DFS(int id) {

}
void Graph::resetDepth() {
	for (int i = 0; i < curNumActiveNodes; i++) {
		Node* n = &node[activeNode[i]];
		n->depth = 0;
	}
}
void Graph::BFS(int id) {
	int depth = 0;
	queue<int, list<int> > q;
	Node *n = &node[id];
	node->depth = depth;
	q.push(id);
	while (!q.empty()) {
		id = q.front();
		q.pop();
		Node *n = &node[id];
		for (int i = 0; i < n->edgeID.size(); i++) {
			int edgeID = n->edgeID[i];
			Edge *e = &edge[edgeID];
			int adjID;
			if (id == e->nodeID[0])
				adjID = e->nodeID[1];
			else
				adjID = e->nodeID[0];

			Node *adj = &node[adjID];
			if (adj->depth != 0) continue;
			q.push(adjID);
			adj->depth = n->depth + 1;
		}
		if (q.size() == 0)
			maxDepth = n->depth + 1;
	}
	deSelect();
}
