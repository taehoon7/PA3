
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
	unsigned int length = pow(2, k);
	pair<int, int> ul;
	ul.first = (imIn.width() - length) / 2;
	ul.second = (imIn.height() - length) / 2;
	//crop image
	PNG *newimg = new PNG(length, length); //new blank image*******
	for (unsigned int row = 0; row < newimg->height(); row++) { //gets each pixel of original and puts in new img 
		for (unsigned int col = 0; col < newimg->width(); col++) {
			*(newimg->getPixel(col, row)) = *(imIn.getPixel(col + ul.first, row + ul.second));
		}
	}
	root = buildTree(newimg, k);
	delete (newimg);
/* This constructor grabs the 2^k x 2^k sub-image centered */
/* in imIn and uses it to build a quadtree. It may assume  */
/* that imIn is large enough to contain an image of that size. */

/* your code here */
}

int toqutree::size() {
	int count = 0;
	if (root != NULL) {
		count = getsubtreeSize(root) + 1;
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
	//until leaf pixel is reached, do recursion
	if (k != 0) {
		//set up length and position
		unsigned int length = pow(2, k - 1);
		pair<int, int> ul;
		ul.first = (im->width() - length) / 2;
		ul.second = (im->height() - length) / 2;
		pair<int, int> lr(ul.first + length - 1, ul.second + length - 1);
		pair<int, int> ctr(ul.first, ul.second);
		stats s(*im);
		//find split point
		double min_Entropy = getaverageEntropy(s, k - 1, ctr);
		double entropy;
		for (int row = ul.second; row <= lr.second; row++) {
			for (int col = ul.first; col <= lr.first; col++) {
				//printf("HIIII\nasdf\n");
				entropy = getaverageEntropy(s, k - 1, make_pair(col,row));
				//printf("YOOOO\n");
				if (entropy < min_Entropy) {
					ctr.first = col;
					ctr.second = row;
					min_Entropy = entropy;
				}
			}
		}
		if (k == 1) {
			ctr.first = 1;
			ctr.second = 1;
		}
		//printf("HIIII\n");
		//create new node
		ul.first = 0;
		ul.second = 0;
		lr.first = (int)im->width() - 1;
		lr.second = (int)im->height() - 1;
		Node* node = new Node(ctr, k, s.getAvg(ul, lr));

		//until leaf pixel is reached, do recursion
		//create 4 images from original image
		PNG *SE_img = new PNG(length, length);
		PNG *SW_img = new PNG(length, length);
		PNG *NE_img = new PNG(length, length);
		PNG *NW_img = new PNG(length, length);
		createPic(SE_img, SW_img, NE_img, NW_img, im, ctr, k - 1);
		//recursion
		node->SE = buildTree(SE_img, k - 1);
		node->SW = buildTree(SW_img, k - 1);
		node->NE = buildTree(NE_img, k - 1);
		node->NW = buildTree(NW_img, k - 1);
		
		delete SE_img;
		delete SW_img;
		delete NE_img;
		delete NW_img;

		return node;
	}
	else {
		return new Node(make_pair(0,0), k, *im->getPixel(0,0));
	}
	/* This constructor grabs the 2^k x 2^k sub-image centered */
	/* in imIn and uses it to build a quadtree. It may assume  */
	/* that imIn is large enough to contain an image of that size. */
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
	return render(root);
// My algorithm for this problem included a helper function
// that was analogous to Find in a BST, but it navigated the 
// quadtree, instead.
/* your code here */

}

/* oops, i left the implementation of this one in the file! */
void toqutree::prune(double tol){
	prune_helper(root, tol);
}

void toqutree::prune_helper(Node *& node, double tol){
	if (node->SE == NULL || node->SW == NULL || node->NE == NULL || node->NW == NULL) { //cannot prune starting from leaf
		return;
	}
	if (yesprune(node,node,tol)) { //if all subroot leafs within tolerance, clear subroots
		//printf("HIIII\n");
		clear(node -> NE);
		clear(node -> NW);
		clear(node -> SW);
		clear(node -> SE);
		node->NE = NULL;
		node->NW = NULL;
		node->SW = NULL;
		node->SE = NULL;
	}
	else { //recurse into subroot if not within tolerance
		prune_helper(node->NE, tol);
		prune_helper(node->NW, tol);
		prune_helper(node->SW, tol);
		prune_helper(node->SE, tol);
	}

}

bool toqutree::yesprune(Node *& node, Node *& parent, double tol){
	if (node->SE == NULL || node->SW == NULL || node->NE == NULL || node->NW == NULL) { //at leaf
		return (node->avg).dist(parent->avg) < tol;
	}
	return yesprune(node->SE, parent, tol) && yesprune(node->SW, parent, tol) && yesprune(node->NE, parent, tol) && yesprune(node->NW, parent, tol);
}

/* called by destructor and assignment operator*/
void toqutree::clear(Node * & curr){
	if (curr->NW != NULL) {
		clear(curr->NW);
	}
	if (curr->NE != NULL) {
		clear(curr->NE);
	}
	if (curr->SE != NULL) {
		clear(curr->SE);
	}
	if (curr->SW != NULL) {
		clear(curr->SW);
	}
	delete curr;
/* your code here */
}

/* done */
/* called by assignment operator and copy constructor */
toqutree::Node * toqutree::copy(const Node * other) {
	Node *newNode = new Node(other->center, other->dimension, other->avg);
	if (other->NW != NULL) {
		newNode->NW = copy(other->NW);
	}
	if (other->NE != NULL) {
		newNode->NE = copy(other->NE);
	}
	if (other->SE != NULL) {
		newNode->SE = copy(other->SE);
	}
	if (other->SW != NULL) {
		newNode->SW = copy(other->SW);
	}
	return newNode;
/* your code here */
}

double toqutree::getaverageEntropy(stats &s, int k, pair<int, int> ctr) {
	int length = pow(2, k);
	pair<int, int> ul_SE(ctr.first, ctr.second);
	pair<int, int> lr_SE((ctr.first + length - 1) % s.width, (ctr.second + length - 1) % s.height);
	int area = length*length;
	pair<int, int> ul_SW((ctr.first + length) % s.width, ctr.second);
	pair<int, int> lr_SW((ul_SW.first + length - 1) % s.width, (ul_SW.second + length - 1) % s.height);

	pair<int, int> ul_NE (ctr.first, (ctr.second + length) % s.height);
	pair<int, int> lr_NE ((ul_NE.first + length - 1) % s.width, (ul_NE.second + length - 1) % s.height);

	pair<int, int> ul_NW((ctr.first + length) % s.width, (ctr.second + length) % s.height);
	pair<int, int> lr_NW((ul_NW.first + length - 1) % s.width, (ul_NW.second + length - 1) % s.height);
	//printf("%d\n", lr_NW.second);
	
	vector<int> hist_SE = s.buildHist(ul_SE, lr_SE);
	vector<int> hist_SW = s.buildHist(ul_SW, lr_SW);
	vector<int> hist_NE = s.buildHist(ul_NE, lr_NE);
	vector<int> hist_NW = s.buildHist(ul_NW, lr_NW);
	
	return s.entropy(hist_SE, area) + s.entropy(hist_SW, area)
		+ s.entropy(hist_NE, area) + s.entropy(hist_NW, area);
}

void toqutree::createPic(PNG *SE_img, PNG *SW_img, PNG *NE_img, PNG *NW_img, PNG *im, pair<int,int> ctr, int k) {
	int length = pow(2, k);
	pair<int, int> ul_SE(ctr.first, ctr.second);
	pair<int, int> ul_SW((ctr.first + length) % im->width(), ctr.second);
	pair<int, int> ul_NE(ctr.first, (ctr.second + length) % im->height());
	pair<int, int> ul_NW((ctr.first + length) % im->width(), (ctr.second + length) % im->height());

	for (int row = 0; row < length; row++) {
		for (int col = 0; col < length; col++) {
			*(SE_img->getPixel(col, row)) = *(im->getPixel((ul_SE.first + col) % im->width(), (ul_SE.second + row) % im->height()));
			*(SW_img->getPixel(col, row)) = *(im->getPixel((ul_SW.first + col) % im->width(), (ul_SW.second + row) % im->height()));
			*(NE_img->getPixel(col, row)) = *(im->getPixel((ul_NE.first + col) % im->width(), (ul_NE.second + row) % im->height()));
			*(NW_img->getPixel(col, row)) = *(im->getPixel((ul_NW.first + col) % im->width(), (ul_NW.second + row) % im->height()));
		}
	}
}

PNG toqutree::render(Node *&node) {
	PNG SE_img, SW_img, NE_img, NW_img, im;
	int length = pow(2, node->dimension);
	im = PNG(length, length);
	//make image when you get to a leaf, may or my not be just a pixel
	//base case
	if (node->SE == NULL && node->SW == NULL && node->NE == NULL && node->NW == NULL) {
		for (int row = 0; row < length; row++) {
			for (int col = 0; col < length; col++) {
				*im.getPixel(col, row) = node->avg;
			}
		}
		return im;
	}
	SE_img = render(node->SE);
	SW_img = render(node->SW);
	NE_img = render(node->NE);
	NW_img = render(node->NW);

	overlay(SE_img, SW_img, NE_img, NW_img, im, node->center, node->dimension - 1);
	return im;
}

void toqutree::overlay(PNG SE_img, PNG SW_img, PNG NE_img, PNG NW_img, PNG &im, pair<int,int> ctr, int k) {
	int length = pow(2, k);
	pair<int, int> ul_SE(ctr.first, ctr.second);
	pair<int, int> ul_SW((ctr.first + length) % im.width(), ctr.second);
	pair<int, int> ul_NE(ctr.first, (ctr.second + length) % im.height());
	pair<int, int> ul_NW((ctr.first + length) % im.width(), (ctr.second + length) % im.height());

	for (int row = 0; row < length; row++) {
		for (int col = 0; col < length; col++) {
			*(im.getPixel((ul_SE.first + col) % im.width(), (ul_SE.second + row) % im.height())) = *(SE_img.getPixel(col, row));
			*(im.getPixel((ul_SW.first + col) % im.width(), (ul_SW.second + row) % im.height())) = *(SW_img.getPixel(col, row));
			*(im.getPixel((ul_NE.first + col) % im.width(), (ul_NE.second + row) % im.height())) = *(NE_img.getPixel(col, row));
			*(im.getPixel((ul_NW.first + col) % im.width(), (ul_NW.second + row) % im.height())) = *(NW_img.getPixel(col, row));
		}
	}

}