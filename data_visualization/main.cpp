#include "data_visualization.h"
#include <iostream>
using namespace std;

int main()
{
	DataVisualization dataVisualization;
	cout << "====================" << endl;
	cout << "Select SYSTEM:" << endl;
	cout << "1: sorting" << endl;
	cout << "2: graph" << endl;
	cout << "====================" << endl;
	dataVisualization.run();
	return EXIT_SUCCESS;
}