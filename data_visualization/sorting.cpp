#pragma once
#include "sorting.h"
#include <iostream>
#include <queue>
using namespace std;

Sorting::Sorting(sf::RenderWindow& window) : window(window) {
	width = 5; //no less than 3
	numberOfComparison = 0;

	if (!font.loadFromFile("Menlo_for_Powerline.ttf")) {
		cout << "unable to load font" << endl;
		exit(1);
	}
	text.setFont(font);
	text.setCharacterSize(12);
	text.setString("hello world");
}
void Sorting::run(sf::Event &event) {
	if (event.type != sf::Event::KeyPressed) return;

	switch (event.key.code) {
	case sf::Keyboard::Num0:
		reset();
		cout << "data reseted" << endl;
		return;
	case sf::Keyboard::B:
		window.setTitle("Sorting: Bubble Sort");
		start = std::time(NULL);
		bubbleSort();
		cout << "bubbleSorted" << endl;
		break;
	case sf::Keyboard::S:
		window.setTitle("Sorting: Selection Sort");
		start = std::time(NULL);
		selectionSort();
		cout << "selectionSorted" << endl;
		break;
	case sf::Keyboard::I:
		window.setTitle("Sorting: Insertion Sort");
		start = std::time(NULL);
		insertionSort();
		cout << "insertionSorted" << endl;
		break;
	case sf::Keyboard::M:
		window.setTitle("Sorting: Merge Sort");
		start = std::time(NULL);
		mergeSort(0, n - 1);
		cout << "mergeSorted" << endl;
		break;
	case sf::Keyboard::Q:
		window.setTitle("Sorting: Quick Sort");
		start = std::time(NULL);
		quickSort(0, n - 1);
		cout << "quickSorted" << endl;
		break;
	case sf::Keyboard::R:
		window.setTitle("Sorting: Radix Sort");
		start = std::time(NULL);
		radixSort();
		cout << "radixSorted" << endl;
		break;
	default:
		return;
	}
	finishSorting();
}
void Sorting::update() {
	window.clear();
	end = std::time(NULL);
	draw();
	window.display();
}
void Sorting::draw() {
	for (int i = 0; i < n; i++) {
		int posX = i * width;
		int posY = window.getSize().y - data[i].getSize().y;
		data[i].setPosition(sf::Vector2f(posX, posY));
		window.draw(data[i]);
	}
	string statistic = "dataSize:" + to_string(n);
	statistic += " comparisons:" + to_string(numberOfComparison);
	statistic += " timespan:" + to_string(end - start);
	text.setString(statistic);
	window.draw(text);
}
void Sorting::reset() {
	sf::Vector2u size = window.getSize();
	numberOfComparison = 0;
	n = size.x / width;
	data.clear();
	data.reserve(n);
	for (int i = 0; i < n; i++) {
		sf::RectangleShape bar;
		int height = rand() % window.getSize().y;
		bar.setSize(sf::Vector2f(width, height));
		bar.setOutlineColor(sf::Color::Black);
		bar.setOutlineThickness(-1);
		data.push_back(bar);
	}
	start = 0;
	end = 0;
}
void Sorting::finishSorting() {
	for (int i = 0; i < n; i++) {
		data[i].setFillColor(Light_Green);
		update();
	}
}
void Sorting::bubbleSort() {
	sf::Color Orange(255, 140, 0, 255);
	sf::Color Light_Green(178, 255, 102, 255);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - 1 - i; j++) {
			numberOfComparison++;
			if (data[j].getSize().y > data[j + 1].getSize().y) {
				swap(data[j], data[j + 1]);
			}
			data[j].setFillColor(sf::Color::Red);
			data[j + 1].setFillColor(Orange);
			update();
			data[j].setFillColor(sf::Color::White);
			data[j + 1].setFillColor(sf::Color::White);
		}
		data[n - 1 - i].setFillColor(Orange);
	}
}
void Sorting::selectionSort() {
	for (int i = 0; i < n; i++) {
		int minIdx = i;
		data[i].setFillColor(Orange);
		for (int j = i; j < n; j++) {
			numberOfComparison++;
			if (data[minIdx].getSize().y > data[j].getSize().y) {
				minIdx = j;
			}
			data[minIdx].setFillColor(sf::Color::Red);
			update();
			data[minIdx].setFillColor(sf::Color::White);
		}
		swap(data[i], data[minIdx]);
		data[i].setFillColor(Orange);
	}
}
void Sorting::insertionSort() {
	for (int i = 1; i < n; i++) {
		int j = i - 1;
		sf::RectangleShape key = data[i];
		while (j >= 0 && key.getSize().y < data[j].getSize().y) {
			numberOfComparison++;
			data[j + 1] = data[j];
			data[j].setFillColor(sf::Color::Red);
			update();
			j--;
		}
		numberOfComparison++;
		data[j + 1] = key;
		for (int k = 0; k <= i; k++) {
			data[k].setFillColor(Orange);
		}
	}
}
void Sorting::merge(int start, int end) {
	int mid = (start + end) / 2;
	std::vector<sf::RectangleShape> left(data.begin() + start, data.begin() + mid + 1);
	std::vector<sf::RectangleShape> right(data.begin() + mid + 1, data.begin() + end + 1);
	left.insert(left.end(), sf::RectangleShape(sf::Vector2f(width, INT_MAX)));
	right.insert(right.end(), sf::RectangleShape(sf::Vector2f(width, INT_MAX)));
	int idxLeft = 0, idxRight = 0;
	for (int i = start; i <= end; i++) {
		numberOfComparison++;
		if (left[idxLeft].getSize().y <= right[idxRight].getSize().y) {
			data[i] = left[idxLeft++];
		}
		else {
			data[i] = right[idxRight++];
		}
		data[i].setFillColor(sf::Color::Red);
		update();
		data[i].setFillColor(Orange);
	}
}
void Sorting::mergeSort(int start, int end) {
	if (start < end) {
		int mid = (start + end) / 2;
		mergeSort(start, mid);
		mergeSort(mid + 1, end);
		merge(start, end);
		for (int i = start; i <= end; i++) {
			data[i].setFillColor(Orange);
		}
	}
}
int Sorting::partition(int start, int end) {
	int random = start + rand() % (end - start);
	swap(data[random], data[end]);
	sf::RectangleShape& pivot = data[end];
	pivot.setFillColor(sf::Color::Red);
	int i = start - 1;
	for (int j = start; j < end; j++) {
		numberOfComparison++;
		if (data[j].getSize().y < pivot.getSize().y) {
			i++;
			swap(data[j], data[i]);
			update();
		}
	}
	i++;
	pivot.setFillColor(sf::Color::White);
	swap(data[i], data[end]);
	return i;
}
void Sorting::quickSort(int start, int end) {
	if (start < end) {
		int pivot = partition(start, end);
		quickSort(start, pivot - 1);
		quickSort(pivot + 1, end);
		if (start - 1 >= 0) {
			data[start - 1].setFillColor(Orange);
		}
		for (int i = start; i <= end; i++) {
			data[i].setFillColor(Orange);
		}
	}
}
void Sorting::radixSort() {
	int MAX = window.getSize().y;
	vector<queue<sf::RectangleShape>> buckets(10);
	int radix = 1;
	while (radix <= MAX) {
		for (int i = 0; i < n; i++) {
			int LSD = ((int)data[i].getSize().y / radix) % 10;
			buckets[LSD].push(data[i]);
		}
		int index = 0;
		for (int i = 0; i < 10; i++) {
			while (!buckets[i].empty()) {
				data[index] = buckets[i].front();
				buckets[i].pop();
				data[index].setFillColor(sf::Color::Red);
				update();
				data[index].setFillColor(sf::Color::White);
				index++;
			}
		}
		radix *= 10;
	}
}
