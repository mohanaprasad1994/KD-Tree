#include "KDTree.h"
#include <fstream>

bool test_distance_function(){
	return 1.99 < dist<DoublePoint, double>(vector<double>(4,1), vector<double>(4,2)) and
			dist<DoublePoint, double>(vector<double>(4,1), vector<double>(4,2)) < 2.001;
}

bool test_build_and_search_tree_default(){
	bool flag = true;
	int K = 2;
	int NUM_POINTS = 10;
	vector<DoublePoint> points;
	for (int i = 0; i < NUM_POINTS; i++){
		DoublePoint p(K,i);
		points.push_back(p);
	}
	KDTree<DoublePoint, double> Tree;
	Tree.build(points, K, 1);
	for (int i = 0; i < NUM_POINTS; i++){
			DoublePoint p(K,i);
			int ind = Tree.nearest_neighbor(p);
			if (ind != i)
				flag = false;
			//cout<<"nearest neighbor of ("<<i<<","<<i<<") is ("<< points[ind][0]<<","<<points[ind][1]<<")\n";

		}
	return flag;
}

bool test_build_and_search_tree_max_range(){
	bool flag = true;
	int K = 2;
	int NUM_POINTS = 10;
	vector<DoublePoint> points;
	for (int i = 0; i < NUM_POINTS; i++){
		DoublePoint p(K,i);
		points.push_back(p);
	}
	KDTree<DoublePoint, double> Tree;
	Tree.build(points, K, 1, "max_range");
	for (int i = 0; i < NUM_POINTS; i++){
			DoublePoint p(K,i);
			int ind = Tree.nearest_neighbor(p);
			if (ind != i)
				flag = false;
			//cout<<"nearest neighbor of ("<<i<<","<<i<<") is ("<< points[ind][0]<<","<<points[ind][1]<<")\n";

		}
	return flag;
}

bool test_build_and_search_tree_median_of_median(){
	bool flag = true;
	int K = 2;
	int NUM_POINTS = 10;
	vector<DoublePoint> points;
	for (int i = 0; i < NUM_POINTS; i++){
		DoublePoint p(K,i);
		points.push_back(p);
	}
	KDTree<DoublePoint, double> Tree;
	Tree.build(points, K, 1, "max_range", "median_of_median");
	for (int i = 0; i < NUM_POINTS; i++){
			DoublePoint p(K,i);
			int ind = Tree.nearest_neighbor(p);
			if (ind != i)
				flag = false;
			//cout<<"nearest neighbor of ("<<i<<","<<i<<") is ("<< points[ind][0]<<","<<points[ind][1]<<")\n";

		}
	return flag;
}

bool test_save_load(){
	bool flag = true;
	int K = 2;
	int NUM_POINTS = 10;
	vector<DoublePoint> points;
	for (int i = 0; i < NUM_POINTS; i++){
		DoublePoint p(K,i);
		points.push_back(p);
	}
	KDTree<DoublePoint, double> Tree;
	Tree.build(points, K, 1, "max_range", "median_of_median");

	std::ofstream ofs;
	ofs.open("tree.txt");
	Tree.save_tree(ofs);
	ofs.close();
	KDTree<DoublePoint, double> Tree2;
	std::ifstream ifs("tree.txt");
	Tree2.load_tree(ifs);
	for (int i = 0; i < NUM_POINTS; i++){
			DoublePoint p(K,i);
			int ind = Tree2.nearest_neighbor(p);
			if (ind != i)
				flag = false;
			//cout<<"nearest neighbor of ("<<i<<","<<i<<") is ("<< points[ind][0]<<","<<points[ind][1]<<")\n";

		}
//	DoublePoint p;
//	p.push_back(0);
//	p.push_back(1.1);
//	int ind = Tree2.nearest_neighbor(p);
//	cout<<"nearest neighbor  ("<< points[ind][0]<<","<<points[ind][1]<<")\n";
	return flag;
}

int main(){


	if(test_build_and_search_tree_default())
		cout<<"####### BUILD AND SEARCH TEST DEFAULT (Round robin axis and median split point) - PASSED ############\n";
	else
		cout<<"####### BUILD AND SEARCH TEST DEFAULT (Round robin axis and median split point) - FAILED ############\n";

	if(test_build_and_search_tree_max_range())
		cout<<"####### BUILD AND SEARCH TEST DEFAULT (maximum range axis selection) - PASSED ############\n";
	else
		cout<<"####### BUILD AND SEARCH TEST DEFAULT (maximum range axis selection) - FAILED ############\n";

	if(test_build_and_search_tree_median_of_median())
		cout<<"####### BUILD AND SEARCH TEST DEFAULT (median of median split selection) - PASSED ############\n";
	else
		cout<<"####### BUILD AND SEARCH TEST DEFAULT (median of median split selection) - FAILED ############\n";

	if(test_save_load())
			cout<<"####### SAVE LOAD TEST - PASSED ############\n";
		else
			cout<<"####### SAVE LOAD TEST - FAILED ############\n";
	return 0;
}
