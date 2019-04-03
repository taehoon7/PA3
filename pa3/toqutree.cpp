
/**
 *
 * toqutree (pa3)
 * significant modification of a quadtree .
 * toqutree.cpp
 * This file will be used for grading.
 *
 */

#include "toqutree.h"

toqutree::Node::Node(pair<int,int> ctr, int dim, HSLAPixel a)
	:center(ctr),dimension(dim),avg(a),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

toqutree::~toqutree(){
	clear(root);
}

toqutree::toqutree(const toqutree & other) {
	root = copy(other.root);
}


toqutree & toqutree::operator=(const toqutree & rhs){
	if (this != &rhs) {
		clear(root);
		root = copy(rhs.root);
	}
	return *this;
}

toqutree::toqutree(PNG & imIn, int k){ 
	buildTree(&imIn, k);
/* This constructor grabs the 2^k x 2^k sub-image centered */
/* in imIn and uses it to build a quadtree. It may assume  */
/* that imIn is large enough to contain an image of that size. */

/* your code here */
}

int toqutree::size() {
	int count = 0;
	if (root != NULL) {
		count = getsubtreeSize(root);
	}
	return count;
/* your code here */
}

int toqutree::getsubtreeSize(Node* node) {
	int count = 1;
	if (node->NW != NULL) {
		count += getsubtreeSize(node->NW);
	}
	if (node->NE != NULL) {
		count += getsubtreeSize(node->NE);
	}
	if (node->SE != NULL) {
		count += getsubtreeSize(node->SE);
	}
	if (node->SW != NULL) {
		count += getsubtreeSize(node->SW);
	}
	return count;
}


toqutree::Node * toqutree::buildTree(PNG * im, int k) {

/* your code here */

// Note that you will want to practice careful memory use
// In this function. We pass the dynamically allocated image
// via pointer so that it may be released after it is used .
// similarly, at each level of the tree you will want to 
// declare a dynamically allocated stats object, and free it
// once you've used it to choose a split point, and calculate
// an average.

}

PNG toqutree::render(){

// My algorithm for this problem included a helper function
// that was analogous to Find in a BST, but it navigated the 
// quadtree, instead.
/* your code here */

}
void toqutree::render_help(){

}
/* oops, i left the implementation of this one in the file! */
void toqutree::prune(double tol){
	if(root != NULL){
		prune_helper(root, tol);
	}
}

void toqutree::prune_helper(Node *& sroot, double tol){
	if(sroot != NULL){
		if((yesprune(sroot -> NE, sroot, tol)) && (yesprune(sroot -> NW, sroot, tol))
		&& (yesprune(sroot -> SW, sroot, tol)) && (yesprune(sroot -> SE, sroot, tol))){
			
			clear(sroot -> NE);
			clear(sroot -> NW);
			clear(sroot -> SW);
			clear(sroot -> SE);

		}
		prune_helper(sroot -> NE, tol);
		prune_helper(sroot -> NW, tol);
		prune_helper(sroot -> SW, tol);
		prune_helper(sroot -> SE, tol);
	}
}

bool toqutree::yesprune(Node *& sroot, Node *& parent, double tol){
	//if leaves are already pruned, then stop
	if(sroot == NULL){
		return false;
	}

	//if one of the child is null, then this node is leaf
	if(sroot -> NE == NULL){
		HSLAPixel p_avg = parent -> avg;	
		HSLAPixel s_avg = sroot -> avg; 

		if(p_avg.dist(s_avg) <= tol){
			return true;
		}else{
			return false;
		}

	}else{
		//look at more children, until u reach leaf
		return ((yesprune(sroot -> NE, sroot, tol)) && (yesprune(sroot -> NW, sroot, tol))
		&& (yesprune(sroot -> SW, sroot, tol)) && (yesprune(sroot -> SE, sroot, tol)));
	}
}

/* called by destructor and assignment operator*/
void toqutree::clear(Node * & curr){
	if(curr != NULL){
		clear(curr->NE);
		clear(curr->NW);
		clear(curr->SW);
		clear(curr->SE);
	}
	else{
		delete curr;
		curr = NULL;
	}
/* your code here */
}

/* done */
/* called by assignment operator and copy constructor */
toqutree::Node * toqutree::copy(const Node * other) {
	if(other == NULL){
		other = NULL;
	}
	else{
		copy_help(this -> root, other);
	}

/* your code here */
}

toqutree::Node * toqutree::copy_help(Node * & dest, const Node * other){
	
	if(other == NULL){
		dest = NULL;
	}
	else{
		Node * dest = new Node;
		dest -> center = other -> center;
		dest -> avg = other -> avg;
		dest -> dimension = other -> dimension;
		copy_help(dest -> NW, other -> NW);
		copy_help(dest -> NE, other -> NE);
		copy_help(dest -> SE, other -> SE);
		copy_help(dest -> SW, other -> SW);
	}

		// if(other == NULL){
	// 	return NULL;
	// }
	// else{
	// 	Node *node = new Node (*other);
	// 	node->NE = copy(other->NE);
	// 	node->NW = copy(other->NW);
	// 	node->SW = copy(other->SW);
	// 	node->SE = copy(other->SE);
}

