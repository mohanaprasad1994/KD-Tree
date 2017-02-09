#include "KDTree.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
int main(int argc, char *argv[]){
	ifstream ifs(argv[2]);
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
	ifs.close();
	// load tree
	KDTree<DoublePoint, double> Tree;
	ifs.open(argv[1]);
	Tree.load_tree(ifs);

	// query and write output to file
	ofstream ofs(argv[3]);
	double distance;
	for (int i = 0; i < points.size(); i++){
		int ind = Tree.nearest_neighbor(points[i], distance);
		ofs<<ind<<", "<<distance<<endl;
	}


}
