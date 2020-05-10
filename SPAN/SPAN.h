//==============================================================================================
// SPAN.h - Word Ladder game
//​
// Sets up the template for the SPAN.cpp file. It sets up the public methods, class variables,
// and private methods. The spanning trees are created using linked lists and heaps. No vectors 
// set or anything from the STL library was allowed. I only used the strings to store the key 
// in nodes. 
//​
// Author:    Logan Dabney, University of Toledo​
// Date:      April 26, 2020​
// Copyright: Copyright 2020 by Logan Dabney. All rights reserved.
//==============================================================================================

#pragma once
#include <string>

using namespace std;

class SPAN {

public:

	SPAN(string file);		// contructs the vertex linked lists and heaps
	~SPAN();				// deletes them
	void Kruskal();			// finds the spanning tree with kruskal's alg
	void Prim();			// finds the spannign tree with prim's alg

private:
	struct node				// node for the linked list of vertices and the heap of edges
	{							// if just an edge
		string key;				// it has the key (if an edge this is the left side of the edge	or alphabetical first)
		node* up = nullptr;		// gets to the node connected above it in the linked list
		node* down = nullptr;	// below in linked list
		node* adj = nullptr;	// when in the linked list the adj pointer is the second part of an edge that is connected to the main vertex

		string key2;			// for the right side of the edge
		double w = 0;			// weight of the edge
		bool used = false;		// if the edge will be used or not
	};

	node* vertices = new node();	// linked list for kruskal
	node* primVert = new node();	// linked list for prim
	node* heap;						// will become the sorted heap
	node* alphaHeap;				// stay in alphabetical order
	int numOfV = 0;					// number of vertexs
	int numOfE = 0;					// number of edges
	int heapsize = 0;				// heapsize

	void FillHeap();								// fills the heap in alpha order
	void FillVertices(ifstream& inFile);			// fills the linked list of vertices
	void MaxHeapify(int i);							// sorts the heap one at a time called by heapsort
	void HeapSort();								// starts the heap sorting process and calls buildmaxheap and maxheapify
	void BuildMaxHeap();							// makes the max heap property valid
	node* FindSet(string key);						// finds a set for the kruskal alg
	void Union(node* set, node* set2);				// unions to sets together
	void PrintAlpha(double totW);					// prints the edges in alphabetical order
	void SetUsedEdge(string key, string key2);		// sets the edge that will be used in the alphaHeap
	bool FindVertex(string vertex);					// finds the vertex in linked list
	double FindPrimEdge(node* graph);				// finds the next edge to connect the two separate graphs for Prmim's alg
};
