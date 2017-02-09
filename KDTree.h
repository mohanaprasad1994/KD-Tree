#ifndef KDTREE

#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <cmath>
using namespace std;

typedef vector<double> DoublePoint;
typedef vector<float> FloatPoint;
typedef vector<int> IntPoint;

//Function template for computing distance
template<class PointType = DoublePoint, class Type = double>
Type dist(PointType p1, PointType p2){
	assert(p1.size() == p2.size());
	Type distance = 0.0;
	for (int i = 0; i< p1.size(); i++)
		distance += (p1[i]-p2[i])*(p1[i]- p2[i]);
	return sqrt(distance);
}

// Find next axis in round robin fashion
template<class PointType = DoublePoint, class Type = double>
int axis_round_robin(vector<PointType> &points, vector <int> &indexes, int depth, int K ){
	return depth%K;
}

//Find next axis using dimension with largest range
template<class PointType = DoublePoint, class Type = double>
int axis_max_range(vector<PointType> &points, vector <int> &indexes, int depth, int K ){
	vector<Type> axis_min = points[indexes[0]];
	vector<Type> axis_max = points[indexes[0]];
	for (int i = 0; i < indexes.size(); i++){
		for (int j = 0; j < K; j++){
			axis_min[j] = (axis_min[j] < points[indexes[i]][j] )?axis_min[j]:points[indexes[i]][j];
			axis_max[j] = (axis_max[j] > points[indexes[i]][j] )?axis_max[j]:points[indexes[i]][j];

		}
	}
	int max_range_index = 0;
	Type max_range = axis_max[0] - axis_max[1];
	for (int i = 1; i < K; i++){
		if (max_range < axis_max[i] - axis_min[i]){
			max_range_index = i;
			max_range = axis_max[i] - axis_min[i];
		}
	}
	return max_range_index;
}

// Find the median by using median of median NOTE: STL nth_element is implemented using median of medians
template<class PointType = DoublePoint, class Type = double>
Type median_of_median_split_point(vector<PointType> &points, vector <int> &indexes, int axis ){
	vector<Type> axis_values;
	assert(indexes.size() != 0);
	for ( int i = 0; i < indexes.size(); i++){
		axis_values.push_back(points[indexes[i]][axis]);
	}
	int n = axis_values.size()/2;
	nth_element(axis_values.begin(), axis_values.begin()+n, axis_values.end());
	return axis_values[n];
}

// Find the median by using a NlogN sort
template<class PointType = DoublePoint, class Type = double>
Type median_split_point(vector<PointType> &points, vector <int> &indexes, int axis ){
	vector<Type> axis_values;
	assert(indexes.size() != 0);
	for ( int i = 0; i < indexes.size(); i++){
		axis_values.push_back(points[indexes[i]][axis]);
	}
	sort(axis_values.begin(), axis_values.end());
	return axis_values[axis_values.size()/2];
}


//class defining a node of the KD Tree. If its a leaf node, it contains a list of indexes, otherwise
// stores the axis, split position and pointers to children nodes
template<class PointType = DoublePoint, class Type = double>
class KDNode{

public:
	int axis;
	Type split_point;
	bool leaf_node;
	vector< int > indexes;
	KDNode* left;
	KDNode* right;

	KDNode(){
		axis = -1; split_point = 0; leaf_node = false;
		left = nullptr;
		right = nullptr;
	}

	//delete subtree rooted at this node
	void delete_subtree(){
		if (left != nullptr)
			left->delete_subtree();
		if (right != nullptr)
			right->delete_subtree();
		delete this;

	}

	//write out contents of the node
	void save(ostream& os){
		os<<axis<<'\n';
		os<<split_point<<"\n";
		os<<leaf_node<<"\n";
		if (leaf_node){
			os<<indexes.size()<<"\n";
			for ( int i = 0; i < indexes.size(); i++)
				os<<indexes[i]<<"\n";
		}

	}

	//read to a node from file
	void load(istream& is){
		is >> axis >> split_point>>leaf_node;
		if (leaf_node){
			int ct;
			int temp;
			is>>ct;
			for (int i = 0; i < ct; i++){
				is>>temp;
				indexes.push_back(temp);
			}

		}
	}

	//save tree rooted at this node
	void save_tree(ostream& os){
		save(os);
		if (!leaf_node){
			left->save_tree(os);
			right->save_tree(os);
		}
	}

	//load tree rooted at this node
	void load_tree(istream& is){
			load(is);
			if (!leaf_node){
				left = new KDNode<PointType, Type>;
				right = new KDNode<PointType, Type>;
				left->load_tree(is);
				right->load_tree(is);
			}
		}

	//recursive nearest neighbor search
	void recursive_search(vector<PointType>& points, PointType point, Type &best_dist, int  &best_point_index){
		//cerr<<"reached split axis, point "<<axis<<" "<<split_point<<endl;
		if (leaf_node){
			//cerr<<"Reached leaf node\n";
			for (int i = 0; i < indexes.size(); i++){
				PointType p = points[indexes[i]];
				if (dist<PointType, Type>(p,point) < best_dist){
					best_dist = dist<PointType, Type>(p,point);
					best_point_index = indexes[i];
				}

			}
			return;
		}
		bool moved_left = false;
		if (point[axis] < split_point){
			left->recursive_search(points, point, best_dist, best_point_index);
			moved_left = true;
		}
		else
			right->recursive_search(points, point, best_dist, best_point_index);

		if (abs(point[axis] - split_point) <= best_dist){
			if (moved_left)
				right->recursive_search(points, point, best_dist, best_point_index);
			else
				left->recursive_search(points, point, best_dist, best_point_index);
		}
		return;

	}


};

//KD Tree class that stores the points and pointer to the tree root.
template<class PointType = DoublePoint, class Type = double>
class KDTree{
	KDNode<PointType, Type>* root;
	vector <PointType> points;
	int K;
	function<int(vector<PointType> &, vector<int>&, int, int )> get_axis;
	function<Type(vector<PointType> &, vector<int>&, int)> get_split_point;
public:
	KDTree(){
		root = nullptr;
		K = -1;
		get_axis = axis_round_robin<PointType, Type>;
		get_split_point = median_split_point<PointType, Type>;
	}

	// Function to delete the KDTree
	void delete_tree(){
		if (root != nullptr)
			root->delete_subtree();
		root = nullptr;
	}
	//write out contents of points
	void save(ostream& os){
		os<<K<<'\n';
		os<<points.size()<<"\n";
		for (int i = 0; i < points.size(); i++){
			for (int j = 0; j < K; j++){
				os<<points[i][j]<<" ";
			}
			os<<"\n";
		}
	}

	// read points from file
	void load(istream& is){
		int len; Type temp;
		is >> K >> len;
		for (int i = 0; i < len; i++){
					PointType p;
					for (int j = 0; j < K; j++){
						is>>temp;
						p.push_back(temp);
					}
					points.push_back(p);
				}
	}

	//function to save the entire tree and the points
	void save_tree(ostream& os){
		save(os);
		if (root != nullptr)
			root->save_tree(os);
	}

	//function to load the entire tree and the data points
	void load_tree(istream& is){
			load(is);
			root = new KDNode<PointType, Type>;
			root->load_tree(is);
		}

	//Auxillary function for building the tree
	KDNode<PointType, Type>* build_tree(vector< int > indexes, int depth, int num_points){
		if (indexes.size() <= num_points){
			KDNode<PointType, Type>* node = new KDNode<PointType, Type>;
			node->leaf_node = true;
			node->indexes = indexes;
			return node;
		}
		int axis = get_axis(points, indexes, depth, K);
		Type split_position = get_split_point(points, indexes, axis);
		KDNode<PointType, Type>* node = new KDNode<PointType, Type>;
		node->axis = axis;
		node->split_point = split_position;

		vector<int> left_indexes;
		vector<int> right_indexes;
		for (int i = 0; i < indexes.size(); i++)
			if (points[indexes[i]][axis] < split_position)
				left_indexes.push_back(indexes[i]);
			else
				right_indexes.push_back(indexes[i]);

		node->left = build_tree(left_indexes, depth+1, num_points);
		node->right = build_tree(right_indexes, depth+1, num_points);
		return node;

	}

	// build a KD tree with the given points, K value, maximum number of points in each leaf node allowed, axis and the split position algorithms to be used
	void build(vector< PointType > data_points, int K_value, int max_num_points = 2, string axis = "round_robin", string split = "median"){

		// code to set get_axis, get_split_point
		if (axis == "max_range")
			get_axis = axis_max_range<PointType, Type>;
		if (split == "median_of_median")
			get_split_point = median_of_median_split_point<PointType, Type>;

		points = data_points;
		K = K_value;
		delete_tree();
		for (int i = 0; i < points.size(); i++)
			assert(points[i].size() == K);
		vector<int> all_points;
		for (int i = 0; i< points.size(); i++)
			all_points.push_back(i);
		root = build_tree(all_points, 0, max_num_points);

		}

	//function to find the index of the nearest neighbor
	int nearest_neighbor(PointType point){
		assert(point.size() == K);
		assert(root != nullptr);
		int best_point_index = 0;
		Type best_dist = dist<PointType, Type>(points[best_point_index],point);
		//cerr<< best_dist;
		root->recursive_search(points, point, best_dist, best_point_index);
		return best_point_index;
	}

	//overloaded nearest neighbor to also get the distance
	int nearest_neighbor(PointType point, Type& distance){
			assert(point.size() == K);
			assert(root != nullptr);
			int best_point_index = 0;
			Type best_dist = dist<PointType, Type>(points[best_point_index],point);
			//cerr<< best_dist;
			root->recursive_search(points, point, best_dist, best_point_index);
			distance = dist<PointType, Type>(points[best_point_index],point);
			return best_point_index;
		}

};

#endif
