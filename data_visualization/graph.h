#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Node {
	Node() : pos(sf::Vector2f(0, 0)) {}
	Node(sf::Vector2f pos) : pos(pos) {}
	int id;
	int dynamicID;
	sf::Vector2f pos;
	sf::Vector2f center;
	double r = 10.0;
	std::vector<int> edgeID;
	bool passivelyselected = false;
	bool selected = false;
	int depth = 0;
};
struct Edge {
	int id;
	int dynamicID;
	bool active;
	int nodeID[2];
};
class Graph {
public:
	Graph(sf::RenderWindow& window);
	void init();
	void createDefaultGraph();
	void createRandomGraph();
	void createDoubleCircle();
	void createNetCircular();
	void createNetSquare();
	void reset();
	void run(sf::Event& event);
	void draw();
	void render();
	void passivelySelect(double x, double y);
	void activelySelect(double x, double y);
	void deSelect();
	void drag(int id, double x, double y);
	int onNode(double x, double y);
	double getDistance(double x1, double y1, double x2, double y2);
	Node* getFreeNode();
	Edge* getFreeEdge();
	void addOrDelete(double x, double y);
	int addNode(sf::Vector2f pos);
	int addEdge(int id_0, int id_1);
	void deleteNode(int id);
	void deleteEdgeFromNode(int id);
	void deleteEdge(int id);
	void DFS(int id);
	void BFS(int id);
	void resetDepth();
	int find(int id);
	void merge(int a, int b);
	void mst();

private:
	sf::RenderWindow& window;
	sf::Event* event;
	Node* node;
	Edge* edge;
	int maxNumNodes;
	int maxNumEdges;
	int *freeNode;
	int *freeEdge;
	int curNumFreeNodes;
	int curNumFreeEdges;
	int *activeNode;
	int *activeEdge;
	int curNumActiveNodes;
	int curNumActiveEdges;
	sf::Vector2f selectedNodePrevPos;
	sf::Vector2f selectedNodeCurPos;
	Node* selectedNode = NULL;
	int maxDepth = 0;
	int dragID = -1;
	bool mouseHold = false;
	sf::Font font;
	int *p;
};