#include "data_visualization.h"
#include <iostream>
using namespace std;

DataVisualization::DataVisualization() : sorting(window), graph(window) {
	window.create(sf::VideoMode(1280, 720), "Sorting");
	TYPE = SORT;
	reset();
}

void DataVisualization::run() {
	while (window.isOpen()) {
		processInput();
		render();
	}
}
void DataVisualization::reset() {
	sorting.reset();
	graph.reset();
}
void DataVisualization::processInput() {
	while (window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::KeyPressed:
			switch (event.key.code) {
			case sf::Keyboard::Escape:
				window.close();
				break;
			case sf::Keyboard::Num1:
				TYPE = SORT;
				window.setTitle("Sorting");
				cout << "====================" << endl;
				cout << "system: SORT" << endl;
				cout << "r/R: reset" << endl;
				cout << "b/B: bubble sort" << endl;
				cout << "i/I: insertion sort" << endl;
				cout << "s/S: selection sort" << endl;
				cout << "m/M: merge sort" << endl;
				cout << "q/Q: quick sort" << endl;
				cout << "====================" << endl;
				break;
			case sf::Keyboard::Num2:
				TYPE = GRAPH;
				window.setTitle("Graph");
				cout << "====================" << endl;
				cout << "system: GRAPH" << endl;
				cout << "q/Q: create default graph" << endl;
				cout << "w/W: create random graph" << endl;
				cout << "e/E: create double circular graph" << endl;
				cout << "r/R: create net circular graph" << endl;
				cout << "t/T: create net square graph" << endl;
				cout << "b/B: BFS" << endl;
				cout << "====================" << endl;
				break;
			default:
				break;
			}

		default:
			handleEvent();
			break;
		}
	}

}
void DataVisualization::handleEvent() {
	switch (TYPE) {
	case SORT:
		sorting.run(event);
		break;
	case GRAPH:
		graph.run(event);
		break;
	default:
		break;
	}
}

void DataVisualization::draw() {
	switch (TYPE) {
	case SORT:
		sorting.draw();
		break;
	case GRAPH:
		graph.draw();
		break;
	default:
		break;
	}
}
void DataVisualization::render() {
	//clear the window
	window.clear();
	//draw anything here...
	draw();
	//end the current frame(things are drawn in a buffer first, not directly to the window)
	window.display();
}
