#pragma once
#include <SFML/Graphics.hpp>

class Sorting {
public:
	Sorting(sf::RenderWindow& window);
	void run(sf::Event& event);
	void update();
	void reset();
	void draw();
private:
	std::vector<sf::RectangleShape> data;
	sf::RenderWindow& window;
	sf::Font font;
	sf::Text text;
	int width;
	int n;
	int numberOfComparison;
	time_t start;
	time_t end;
	sf::Color Orange = sf::Color(255, 140, 0, 255);
	sf::Color Light_Green = sf::Color(178, 255, 102, 255);
	void finishSorting();
	void bubbleSort();
	void selectionSort();
	void insertionSort();
	void merge(int start, int end);
	void mergeSort(int start, int end);
	int partition(int start, int end);
	void quickSort(int front, int end);
};

