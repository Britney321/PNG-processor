/**
 * @file        tripletree_private.h
 * @description student declarations of private functions
 *              for TripleTree, CPSC 221 PA3
 *
 *              THIS FILE WILL BE SUBMITTED.
 *
 *              Simply declare your function prototypes here.
 *              No other scaffolding is necessary.
 */

 // begin your declarations below
 Node* CopyTree(Node *other);
 void clearTree(Node *node);
 bool isLeaf(Node* node) const; 
 int leafCount(Node* node) const; 
 void flip(Node* node);
 void renderHelper(PNG* output, Node* node) const; 
 void pruneHelper(double tol, Node* node); 
 void changeUL(Node* node, int difference);
 void rotate(Node* node, int width); 
 bool prune(Node* node, Node* parent, double tol);
 void leafList(Node* node, vector<Node*>* arr);