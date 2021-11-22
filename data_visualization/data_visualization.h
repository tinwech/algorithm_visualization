#pragma once
#include <SFML/Graphics.hpp>
#include "sorting.h"
#include "graph.h"

enum SystemType {
	SORT,
	GRAPH
};
class DataVisualization {
public:
	DataVisualization();
	void run();
	void processInput();
	void handleEvent();
	void reset();
	void render();
	void draw();

private:
	sf::RenderWindow window;
	sf::Event event;
	Sorting sorting;
	Graph graph;
	SystemType TYPE;
};
