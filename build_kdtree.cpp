#include "KDTree.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main(int argc, char *argv[]){
	ifstream ifs(argv[1]);
	string line;

	double temp;
	vector <DoublePoint> points;
	//read csv
	while (getline( ifs, line )){

		DoublePoint p;
		stringstream ss(line);
		while (ss >> temp){
			//cout<<temp<< " ";
			p.push_back(temp);
			if (ss.peek() == ',')
				ss.ignore();
			  }
		points.push_back(p);
		}

	int K = points[0].size();
	//build tree
	KDTree<DoublePoint, double> Tree;
	Tree.build(points, K, 1, "max_range", "median_of_median");

	//save tree
	std::ofstream ofs;
	ofs.open(argv[2]);
	Tree.save_tree(ofs);
	ofs.close();

}

