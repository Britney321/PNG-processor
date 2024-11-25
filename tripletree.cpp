/**
 * @file        tripletree.cpp
 * @description Student-implemented functions of a ternary tree for CPSC 221 PA3.
 *
 *              THIS FILE WILL BE SUBMITTED FOR GRADING
 */

#include "tripletree.h"
#include <vector> 
using namespace std;
 /**
      * Constructor that builds a TripleTree out of the given PNG.
      *
      * The TripleTree represents the subimage from (0,0) to (w-1, h-1) where
      * w-1 and h-1 are the largest valid image coordinates of the original PNG.
      * Every node corresponds to a rectangle of pixels in the original PNG,
      * represented by an (x,y) pair for the upper left corner of the
      * square and two integers for the number of pixels on the width and
      * height dimensions of the rectangular region the node defines.
      *
      * The node's three children correspond to a partition
      * of the node's rectangular region into three approximately equal-size strips.
      *
      * If the rectangular region is taller than it is wide, the region is divided
      * into horizontal strips:
      *  +-------+
      *  |   A   |
      *  |       |
      *  +-------+
      *  |   B   |
      *  |       |    (Split tall)
      *  +-------+
      *  |   C   |
      *  |       |
      *  +-------+
      *
      * If the rectangular region is wider than it is tall, the region is divided
      * into vertical strips:
      *  +---------+---------+---------+
      *  |    A    |    B    |    C    |
      *  |         |         |         |    (Split wide)
      *  +---------+---------+---------+
      *
      * Your regions are not guaranteed to have dimensions exactly divisible by 3.
      * If the dimensions of your rectangular region are 3p x q or q x 3p where 3p
      * represents the length of the long side, then your rectangular regions will
      * each have dimensions p x q (or q x p)
      *
      * If the dimensions are (3p+1) x q, subregion B gets the extra pixel of size
      * while subregions A and C have dimensions p x q.
      *
      * If the dimensions are (3p+2) x q, subregions A and C each get an extra pixel
      * of size, while subregion B has dimensions p x q.
      *
      * If the region to be divided is a square, then apply the Split wide behaviour.
      *
      * Every leaf in the constructed tree corresponds to a pixel in the PNG.
      *
      * @param imIn - the input image used to construct the tree
      */
TripleTree::TripleTree(PNG& imIn) {
    // add your implementation below

    pair<unsigned int, unsigned int> ul (0,0); 
    int w = imIn.width(); 
    int h = imIn.height();

    root = BuildNode(imIn, ul, w, h); 
    // cout << w << endl; 
    // cout << h << endl; 
	
}

/**
 * Render returns a PNG image consisting of the pixels
 * stored in the tree. It may be used on pruned trees. Draws
 * every leaf node's rectangle onto a PNG canvas using the
 * average color stored in the node.
 *
 * You may want a recursive helper function for this.
 */
PNG TripleTree::Render() const {
    // replace the line below with your implementation
    int width = root->width; 
    int height = root->height; 
    PNG output = PNG(width, height); 
    renderHelper(&output, root);
    return output;
}

/*
 * Prune function trims subtrees as high as possible in the tree.
 * A subtree is pruned (cleared) if all of its leaves are within
 * tol of the average color stored in the root of the subtree.
 * Pruning criteria should be evaluated on the original tree, not
 * on a pruned subtree. (we only expect that trees would be pruned once.)
 *
 * You may want a recursive helper function for this.
 *
 * @param tol - maximum allowable RGBA color distance to qualify for pruning
 */
void TripleTree::Prune(double tol) {
    // add your implementation below
    pruneHelper(tol, root); 
	
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be mirrored horizontally (flipped over a vertical axis).
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::FlipHorizontal() {
    // add your implementation below
    flip(root);
	
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be rotated 90 degrees counter-clockwise.
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::RotateCCW() {
    // add your implementation below

    rotate(root, root->width); 
    // PNG rotated = Render(); 
    // Clear(); 
    // TripleTree t(&rotated);

	
}

/*
 * Returns the number of leaf nodes in the tree.
 *
 * You may want a recursive helper function for this.
 */
int TripleTree::NumLeaves() const {
    // replace the line below with your implementation
    int count = leafCount(root); 

    return count; 
}

/**
     * Destroys all dynamically allocated memory associated with the
     * current TripleTree object. To be completed for PA3.
     * You may want a recursive helper function for this one.
     */
void TripleTree::Clear() {
    // add your implementation below

    clearTree(root);
    delete root; 
    root = NULL;
	
}

/**
 * Copies the parameter other TripleTree into the current TripleTree.
 * Does not free any memory. Called by copy constructor and operator=.
 * You may want a recursive helper function for this one.
 * @param other - The TripleTree to be copied.
 */
void TripleTree::Copy(const TripleTree& other) {
    // add your implementation below
    if (other.root == NULL) { 
        root = NULL;
    } else {
        root = CopyTree(other.root);
    }

	
}

/**
 * Private helper function for the constructor. Recursively builds
 * the tree according to the specification of the constructor.
 * @param im - reference image used for construction
 * @param ul - upper left point of node to be built's rectangle.
 * @param w - width of node to be built's rectangle.
 * @param h - height of node to be built's rectangle.
 */
Node* TripleTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
    // replace the line below with your implementation

    Node* newNode = new Node(ul, w, h); 

    // set children 
    Node* childA = NULL; 
    Node* childB = NULL; 
    Node* childC = NULL; 
    if (w == 1 && h == 1) { 
        newNode->avg = *im.getPixel(ul.first, ul.second);
        // std:: cout << (int) newNode->avg.r << std::endl; 
        return newNode; 
    }

    if (w >= h) { 
        if (w % 3 == 0) { 
            int width = w / 3; 
            childA = BuildNode(im, ul, width, h);
            pair<unsigned int, unsigned int> cornerB (ul.first + width, ul.second); 
            childB = BuildNode(im, cornerB, width, h); 
            pair<unsigned int, unsigned int> cornerC (ul.first + width * 2, ul.second); 
            childC = BuildNode(im, cornerC, width, h); 
            int red = (childA->avg.r + childB->avg.r + childC->avg.r) / 3; 
            int green = (childA->avg.g + childB->avg.g + childC->avg.g) / 3; 
            int blue = (childA->avg.b + childB->avg.b + childC->avg.b) / 3; 
            double alpha = (childA->avg.a + childB->avg.a + childC->avg.a) / 3; 
            newNode->avg = RGBAPixel(red, green, blue, alpha); 
        }
        if (w % 3 == 1) { 
            int width = (w - 1) / 3; 
            childA = BuildNode(im, ul, width, h);
            pair<unsigned int, unsigned int> cornerB (ul.first + width, ul.second); 
            childB = BuildNode(im, cornerB, width + 1, h); 
            pair<unsigned int, unsigned int> cornerC (cornerB.first + width + 1, ul.second); 
            childC = BuildNode(im, cornerC, width, h); 
            int red = (childA->avg.r * width + childB->avg.r * (width + 1) + childC->avg.r * width) / w; 
            int green = (childA->avg.g * width + childB->avg.g * (width + 1) + childC->avg.g * width) / w; 
            int blue = (childA->avg.b * width + childB->avg.b * (width + 1) + childC->avg.b * width) / w; 
            double alpha = (childA->avg.a * width + childB->avg.a * (width + 1) + childC->avg.a * width) / w; 
            newNode->avg = RGBAPixel(red, green, blue, alpha);
        }
        if (w % 3 == 2) { 
            int width = (w - 2) / 3; 
            childA = BuildNode(im, ul, width + 1, h);
            pair<unsigned int, unsigned int> cornerB (ul.first + width + 1, ul.second); 
            if (width == 0) { 
                childB = NULL; 
                pair<unsigned int, unsigned int> cornerC (cornerB.first + width, ul.second); 
                childC = BuildNode(im, cornerC, width + 1, h); 
                // average A and B 
                int red = (childA->avg.r + childC->avg.r) / 2; 
                int green = (childA->avg.g + childC->avg.g) / 2; 
                int blue = (childA->avg.b + childC->avg.b) / 2; 
                double alpha = (childA->avg.a + childC->avg.a) / 2; 
                newNode->avg = RGBAPixel(red, green, blue, alpha);
            } else { 
                pair<unsigned int, unsigned int> cornerB (ul.first + width + 1, ul.second); 
                childB = BuildNode(im, cornerB, width, h); 
                pair<unsigned int, unsigned int> cornerC (cornerB.first + width, ul.second); 
                childC = BuildNode(im, cornerC, width + 1, h); 

                int red = (childA->avg.r * (width + 1) + childB->avg.r * width + childC->avg.r * (width + 1)) / w; 
                int green = (childA->avg.g * (width + 1) + childB->avg.g * width + childC->avg.g * (width + 1)) / w; 
                int blue = (childA->avg.b * (width + 1) + childB->avg.b * width + childC->avg.b * (width + 1)) / w; 
                double alpha = (childA->avg.a * (width + 1) + childB->avg.a * width + childC->avg.a * (width + 1)) / w; 
                newNode->avg = RGBAPixel(red, green, blue, alpha);
            }
        }
    } else { 
        if (h % 3 == 0) { 
            int height = h / 3; 
            childA = BuildNode(im, ul, w, height);
            //here 
            pair<unsigned int, unsigned int> cornerB (ul.first, ul.second + height); 
            childB = BuildNode(im, cornerB, w, height); 
            pair<unsigned int, unsigned int> cornerC (ul.first, cornerB.second + height); 
            childC = BuildNode(im, cornerC, w, height); 

            int red = (childA->avg.r + childB->avg.r + childC->avg.r) / 3; 
            int green = (childA->avg.g + childB->avg.g + childC->avg.g) / 3; 
            int blue = (childA->avg.b + childB->avg.b + childC->avg.b) / 3; 
            double alpha = (childA->avg.a + childB->avg.a + childC->avg.a) / 3; 
            newNode->avg = RGBAPixel(red, green, blue, alpha); 
        }
        if (h % 3 == 1) { 
            int height = (h - 1) / 3; 
            childA = BuildNode(im, ul, w, height);
            pair<unsigned int, unsigned int> cornerB (ul.first, ul.second + height); 
            childB = BuildNode(im, cornerB, w, height + 1); 
            pair<unsigned int, unsigned int> cornerC (ul.first, cornerB.second + height + 1); 
            childC = BuildNode(im, cornerC, w, height); 

            int red = (childA->avg.r * height + childB->avg.r * (height + 1) + childC->avg.r * height) / h; 
            int green = (childA->avg.g * height + childB->avg.g * (height + 1) + childC->avg.g * height) / h; 
            int blue = (childA->avg.b * height + childB->avg.b * (height + 1) + childC->avg.b * height) / h; 
            double alpha = (childA->avg.a * height + childB->avg.a * (height + 1) + childC->avg.a * height) / h; 
            newNode->avg = RGBAPixel(red, green, blue, alpha);
        }
        if (h % 3 == 2) { 
            int height = (h - 2) / 3; 
            childA = BuildNode(im, ul, w, height + 1);
            pair<unsigned int, unsigned int> cornerB (ul.first, ul.second + height + 1); 
            if (height == 0) { 
                childB = NULL; 
                pair<unsigned int, unsigned int> cornerC (ul.first, cornerB.second + height); 
                childC = BuildNode(im, cornerC, w, height + 1); 
                int red = (childA->avg.r + childC->avg.r) / 2; 
                int green = (childA->avg.g + childC->avg.g) / 2; 
                int blue = (childA->avg.b + childC->avg.b) / 2; 
                double alpha = (childA->avg.a + childC->avg.a) / 2; 
                newNode->avg = RGBAPixel(red, green, blue, alpha);
            } else { 
                pair<unsigned int, unsigned int> cornerB (ul.first, ul.second + height + 1); 
                childB = BuildNode(im, cornerB, w, height); 
                pair<unsigned int, unsigned int> cornerC (ul.first, cornerB.second + height); 
                childC = BuildNode(im, cornerC, w, height + 1); 
                int red = (childA->avg.r * (height + 1) + childB->avg.r * height + childC->avg.r * (height + 1)) / h; 
                int green = (childA->avg.g * (height + 1) + childB->avg.g * height + childC->avg.g * (height + 1)) / h; 
                int blue = (childA->avg.b * (height + 1) + childB->avg.b * height + childC->avg.b * (height + 1)) / h; 
                double alpha = (childA->avg.a * (height + 1) + childB->avg.a * height + childC->avg.a * (height + 1)) / h; // fix this
                newNode->avg = RGBAPixel(red, green, blue, alpha);
            }
        }
    }
    newNode->A = childA; 
    newNode->B = childB; 
    newNode->C = childC; 
    // cout << ul.first << "," << ul.second << "   " << w << "x" << h << endl; 
    // cout << (int) newNode->avg.r << endl; 
    // int red = newNode->avg.r; 
    // cout << red << endl; 
    return newNode;
}

/* ===== IF YOU HAVE DEFINED PRIVATE MEMBER FUNCTIONS IN tripletree_private.h, IMPLEMENT THEM HERE ====== */


void TripleTree::clearTree(Node *node) { 
    if (node == NULL) { 
        return; 
    }
    clearTree(node->A);
    clearTree(node->B);
    clearTree(node->C);

    if (node->A != nullptr) {
        delete node->A;
        node->A = nullptr;
    }
    if (node->B != nullptr) {
        delete node->B;
        node->B = nullptr;
    }
    if (node->C != nullptr) {
        delete node->C;
        node->C = nullptr;
    }

}

Node* TripleTree::CopyTree(Node *node) { 
    if (node == NULL) { 
        return NULL; 
    }

    Node* newNode = new Node(node->upperleft, node->width, node->height); 
    newNode->avg = node->avg; 

    newNode->A = CopyTree(node->A);
    newNode->B = CopyTree(node->B); 
    newNode->C = CopyTree(node->C); 

    return newNode; 
}

bool TripleTree::isLeaf(Node* node) const { 
    return (node->A == NULL && node->B == NULL && node->C == NULL); 
 } 

int TripleTree::leafCount(Node* node) const { 
    if (node == NULL) { 
        return 0; 
    }
    if (isLeaf(node)) { 
        return 1; 
    } else { 
        return leafCount(node->A) + leafCount(node->B) + leafCount(node->C); 
    }
}

void TripleTree::flip(Node* node) { 
    if (node == NULL) {
        return; 
    }
    if (isLeaf(node)) { 
        return;
    }
    if (node->A == NULL) { 
        return; 
    }
    if (node->height <= node->width) { 
        pair<unsigned int, unsigned int> upperleftA = node->A->upperleft; 
        pair<unsigned int, unsigned int> upperleftC = node->C->upperleft; 
        int difference = upperleftC.first - upperleftA.first; 
        //cout << node->A->upperleft.first << endl;
        swap(node->A, node->C);
        //cout << node->A->upperleft.first << endl;
        changeUL(node->A, difference * -1); 
        changeUL(node->C, difference); 
    }
    flip(node->A);
    flip(node->B);
    flip(node->C);
}

void TripleTree::changeUL(Node* node, int difference) { 
    if (node == NULL) { 
        return; 
    }
    // cout << node->upperleft.first << endl;
    // cout << difference << endl;
    node->upperleft.first += difference; 
    // cout << node->upperleft.first << endl;
    changeUL(node->A, difference); 
    changeUL(node->B, difference); 
    changeUL(node->C, difference);
}

 void TripleTree::renderHelper(PNG* output, Node* node) const { 
    if (node == NULL) { 
        return;
    }
    if (isLeaf(node)) { 
        int upperleftX = node->upperleft.first; 
        int upperleftY = node->upperleft.second; 
        for (unsigned int x = 0; x < node->width; x++) { 
            for (unsigned int y = 0; y < node->height; y++) { 
                RGBAPixel* pixel = output->getPixel(upperleftX + x, upperleftY + y);
                *pixel = node->avg; 
            }
        }
        return; 
    }
    renderHelper(output, node->A);
    renderHelper(output, node->B);
    renderHelper(output, node->C);
 }

void TripleTree::pruneHelper(double tol, Node* node) { 
    if (node == NULL) { 
        return; 
    }
    vector<Node*> arr; 
    leafList(node, &arr); 
    bool prune = true; 
    for (Node* leaf : arr) { 
        if (node->avg.distanceTo(leaf->avg) >= tol) { 
            prune = false;  
        }
    }
    if (prune) { 
        clearTree(node); 
        cout << node->upperleft.first << "x" << node->upperleft.second << endl; 
    } else { 
        pruneHelper(tol, node->A); 
        pruneHelper(tol, node->B); 
        pruneHelper(tol, node->C); 
    }
    arr.clear();
    // pruneHelper(tol, node->A, pixel); 
    // pruneHelper(tol, node->B, pixel); 
    // pruneHelper(tol, node->C, pixel); 

    // if (isLeaf(node)) { 
    //     if (node->avg.distanceTo(pixel) < tol) { 
    //         delete node; 
    //         node = NULL;
    //     }
    // }
    // pruneHelper(tol, node->A, node->A->avg); 
    // pruneHelper(tol, node->B, node->B->avg); 
    // pruneHelper(tol, node->C, node->C->avg); 
    
}

void TripleTree::leafList(Node* node, vector<Node*>* arr) { 
    if (node == NULL) { 
        return;
    }
    if (isLeaf(node)) {
        arr->push_back(node); 
        cout << arr->size() << endl;
    }
    leafList(node->A, arr);
    leafList(node->B, arr);
    leafList(node->C, arr);
}


void TripleTree::rotate(Node* node, int width) { 
    if (node == NULL) {
        return; 
    }
    double mid = ((double) width - 1) / 2; 
    double difference = mid - node->upperleft.first; 
    int yValue = mid + difference; 
    int newWidth = node->height; 
    int newHeight = node->width; 
    int second = node->upperleft.second; 

    node->upperleft.first = second;
    node->upperleft.second = yValue - newHeight + 1; 

    node->width = newWidth; 
    node->height = newHeight; 
    if (node->A != NULL) { 
        if (node->A->upperleft.first != node->C->upperleft.first) {
            swap(node->A, node->C);
        }
    }

    rotate(node->A, width);
    rotate(node->B, width);
    rotate(node->C, width);

    // cout << node->upperleft.first << "x" << node->upperleft.second << endl; 
    // int red = node->avg.r; 
    // cout << red << endl

}

    // else if (node->B == NULL) { 
    //     if (node->A->avg.distanceTo(node->avg) <= tol && node->C->avg.distanceTo(node->avg) <= tol) {
    //         clearTree(node->A); 
    //         clearTree(node->C); 
    //         delete node->A; 
    //         delete node->C; 
    //         node->A = NULL; 
    //         node->C = NULL; 
    //         return; 
    //     }
    // }
    // // 1 child 
    // else if (node->A == NULL) { 
    //     if (node->B->avg.distanceTo(node->avg) <= tol) {
    //         clearTree(node->B); 
    //         delete node->B; 
    //         node->B = NULL; 
    //         return; 
    //     }
    // }
    // // 3 chlidren 
    // else if (node->A->avg.distanceTo(node->avg) <= tol && node->B->avg.distanceTo(node->avg) <= tol && node->C->avg.distanceTo(node->avg) <= tol) {
    //     clearTree(node->A); 
    //     clearTree(node->B); 
    //     clearTree(node->C); 
    //     delete node->A; 
    //     delete node->B; 
    //     delete node->C; 
    //     node->A = NULL; 
    //     node->B = NULL; 
    //     node->C = NULL; 
    //     return; 
    // }