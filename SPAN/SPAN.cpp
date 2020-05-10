//==============================================================================================
// SPAN.cpp - Word Ladder game
//​
// This class implments the spanning tree program using heaps, linked lists and fstream. 
// It reads in the file that contains the number of vertices, the verticies, and the edge
// matrix. The values are placed into linked lists and heaps accordingly. Once that is done
// the heap is sorted into a max heap. The next step is to call the two different functions,
// Prim and Kruskal. Each will used the algroithm describe by those to find the minimum
// spanning tree. Each run in O(nlogn) time and are described below. Each of the algorithms
// call PrintAlpha to print the results of the edges of the graph in alphabetical order.
// It's important to mention that there maybe multiple different solution to a MST and the
// alg's only print out one of them.
//​
// Author:    Logan Dabney, University of Toledo​
// Date:      April 26, 2020​
// Copyright: Copyright 2020 by Logan Dabney. All rights reserved.
//==============================================================================================

#include "SPAN.h"
#include <fstream>
#include <iostream>
#include <iomanip>

SPAN::SPAN(string file)
{
	ifstream inFile;

	inFile.open(file, ios::binary);					// opens file then checks to see if it can open
	if (!inFile)
	{
		cout << "Could not open file!";
		exit(-1);
	}

	FillVertices(inFile);							// Fills kruskals vertices and primVert
	FillHeap();										// creates the heap for kruskal
}

SPAN::~SPAN()
{
	while(primVert != nullptr)						// deleteing linked list
	{
		node* adj = primVert->adj;					// sets it to the adj
		while (adj != nullptr)						// runs through the adj deleting each
		{
			node* next = adj->adj;
			delete adj;
			adj = next;
		}

		node* next = primVert->down;				// iterates down the linked list of vertices
		delete primVert;
		primVert = next;
	}
}

/* This is a variation of Kruskal algorithm. It was teaked so I can use it in the instance
   I have created. It find the best path through the graph that has the lowest
   total weight and prints it out. */
void SPAN::Kruskal()
{
	double totW = 0;		// holds the total weight for the graph found
	HeapSort();				// sorts the heap

	for (int i = 1; i < numOfE + 1; i++)				// loops through all of the edges
	{
		node* set = FindSet(heap[i].key);				// finds the set of the first side of edge
		node* set2 = FindSet(heap[i].key2);				// finds the set of the right side of edge
		if (set != set2) {								// if the sets arem't the same
			SetUsedEdge(heap[i].key, heap[i].key2);	// mark the edges as being used
			totW += heap[i].w;

			Union(set, set2);							// join the two sets together
		}
	}

	cout << "Kruskal" << endl;
	PrintAlpha(totW);									// print the results
}

// This is a variation of Prim's algorithm. It uses the same steps as layed out in Prim's alg
// but is formed to fit the format I choose to store my values in. It finds the lowest weighted
// path through a graph 
void SPAN::Prim()
{
	node* graph = new node();					// used to hold all the verticies that are currently in the best path for graph
	double totW = 0;							// holds the total weight

	graph = primVert->down;						// place the first vertex from primVert onto graph
	primVert->down = primVert->down->down;		// takes it off of primVert
	primVert->down->up = primVert;
	graph->down = nullptr;

	for (int i = 0; i < numOfV - 1; i++)		// loop to get all the vertcies onto graph
	{
		totW += FindPrimEdge(graph);			// find the edges and add the return value to the total weight
	}

	cout << endl << "Prim" << endl;				// print out the Prim path 
	PrintAlpha(totW);
}

// This method is used to fill the heap with the edges obtained from reading in the file.
// The edges were placed as adjecent pointers in the set linked list. The heap will be an 
// array of nodes that use the weight and key portions of the node. each edge gets it's
// own node in the heap and is in alphabetical order. 
void SPAN::FillHeap()
{
	heap = new node[numOfE + 1];											// creates a heap with the number of edges read in
	alphaHeap = new node[numOfE + 1];

	int i = 1;																// used to iterate through the heap
	node* edge = vertices->down;											// hold the left side of the edge string value this maintains that the values will stay in alphabetical order
	while (edge != nullptr)													// while is not nullptr continue to loop
	{
		node* edgeAdj = edge->adj;											// set edgeAdj to the first adj to edge
		while (edgeAdj != nullptr)											// while edgeAdj isn't null, loop
		{
			heap[i].key = edge->key;										// create the heap edge with edge as the left value and the adj value as the right value
			heap[i].key2 = edgeAdj->key;
			heap[i].w = edgeAdj->w;
			alphaHeap[i] = heap[i];

			edgeAdj = edgeAdj->adj;											// set edgeAdj to it's adjacent
			i++;															// interate i for the next value in heap
		}

		edge->adj = nullptr;												// sets the edjes adj to null because it is no longer needed
		edge = edge->down;													// set edge to the node below it
	}
}


// This method is called to Fill the node array named vertices. Which holds the different verticies that will turn into sets
// It is done by reading in the intial verticies provided by the file inputed. Then it reads in the edge matrix
// since this is a non-diorected graph only half of the matrix is needed to get every edge since they will be repeated
// twice. Each edge will be placed adjacent, in the linked list, to the first alphabetical letter that the edge connects 
void SPAN::FillVertices(ifstream& inFile)
{
	node* column = vertices;
	node* row = vertices;
	node* pRow = primVert;
	node* vert = vertices;
	node* pVert = primVert;
	double weight;
	int need = 0;

	inFile >> numOfV;
	vertices->key = "";									// first node in vertices is a place holder
	primVert->key = "";

	for (int i = 0; i < numOfV; i++)					// loops through the verticies and places them in vert which is vertices and into primVert
	{
		vert->down = new node();
		pVert->down = new node();

		vert->down->up = vert;
		pVert->down->up = pVert;

		inFile >> vert->down->key;
		pVert->down->key = vert->down->key;

		vert = vert->down;
		pVert = pVert->down;
	}

	// This is used to saved the values of edges while counting the amount of edges later used to create the heap
	for (int i = 0; i < numOfV; i++)										// this loop maintains the row positions
	{
		row = row->down;													// iterate the row value to whats next in the vertices
		pRow = pRow->down;													// same for primVert

		for (int j = 0; j < numOfV; j++)									// this loop maintains the column position
		{
			column = column->down;											// iterate the column value to waht's next in the vertices
			inFile >> weight;

			if (weight != (double)0 )										// if the weight isn't 0 and is on the second half of the matrix 
			{																// half being split down the diagonal								- 0 0 0  <- this side plus the digonal
				node* E = row;												// set row to E														0 - 0 0
				node* PE = pRow;											//																	0 0 - 0
																			// put the edge on the end of the adj linked list for row(E)		0 0 0 -

				if (j >= need)													// Kruskel only needs half the matrix
				{
					while (E->adj != nullptr)
						E = E->adj;
					E->adj = new node();										// create and set the adj's key and weight for col
					E->adj->key = column->key;									// the adj is E's key
					E->adj->w = weight;											// weight from file

					numOfE++;													// increment number of Edges for heap
				}

				while (PE->adj != nullptr)									// Prim needs all of it to work
					PE = PE->adj;
				PE->adj = new node();
				PE->adj->key = column->key;
				PE->adj->w = weight;
			}
		}
		column = vertices;													// reset column to vertices
		need++;																// add one to one to keep on the right side of the matrix
	}
}


// This method does most of the work for the heapsort. It makes sure that a give node has the
// highest priority when copmpared to it's children. If it isn't then the larger priority child takes
// it place and then Maxheapify is called again on the new max value.
void SPAN::MaxHeapify(int i)
{
	int max;
	int l = 2 * i;										// left child spot in array
	int r = 2 * i + 1;									// right child spot in array

	if (l <= heapsize && heap[l].w > heap[i].w)			// if the left child exists and it has higher priority 
		max = l;										// set as max
	else												// otherwise set i to max
		max = i;
	if (r <= heapsize && heap[r].w > heap[max].w)		// if right child exist and is larger than max set as new max
		max = r;

	if (max != i)										// if max wasn't i
	{
		node maxNode = heap[max];

		heap[max] = heap[i];							// switch i and max's spot
		heap[i] = maxNode;

		MaxHeapify(max);								// then call maxheapify on max
	}
}

// This is a O(nlogn) sort that puts the highest priority values in the front and goes in decesending order
// through the rest of the array. First the max-heap property must be valid, done by calling buildmaxheap()
// Then fully sorts the heap with loop.
void SPAN::HeapSort()
{
	BuildMaxHeap();						// called to validate the Max-heap proptery

	for (int i = numOfE; i > 1; i--)	// loops through all of the edges besides the last one
	{
		node firstNode = heap[1];

		heap[1] = heap[i];
		heap[i] = firstNode;
		heapsize--;						// decrements heapsize

		MaxHeapify(1);					// sends the first spot to maxheapify
	}
}

// This method is used to create a valid Max-Heap so the Heapsort will correctly sort
// the values into a complete decending order. Makes the Max-Heap property valid.
// This property is that the parent in the heap tree is of a higher priorty than both of it's children. 
void SPAN::BuildMaxHeap()
{
	heapsize = numOfE;							// sets the heapsize to the number of edges

	for (int i = (numOfE / 2); i > 0; i--)		// loops through the parents in the "heap tree"
	{
		MaxHeapify(i);							// call maxheapify to see if the parent has the highest priority of it's children
	}
}

// This method is used to find a set that conatins the following key provided. This "set"
// is actually a linked list. It uses the original vertices linked list to do this.
SPAN::node* SPAN::FindSet(string key)
{
	node* vert = vertices->down;						// set vert to the vertices linked list

	while (vert != nullptr)								// while vert isn't nullptr
	{
		node* vertAdj;

		if (vert->key != key)							// if vert does contain the same key
		{
			vertAdj = vert->adj;						// set vertAdj to vert's adj node

			while (vertAdj != nullptr)					// while vertAdj isn't check if it contains the key if not continue to set it to the next adj
			{
				if (vertAdj->key == key) return vert;
				vertAdj = vertAdj->adj;
			}
		}
		else
			return vert;								// if vert does contain the key

		vert = vert->down;								// otherwise go down the linked list
	}

}

// This method is used to combine two set together, set and set2. It will only 
// be called if the 2 sets are different from each other.
void SPAN::Union(node* set, node* set2)
{
	set2->up->down = set2->down;					// set set2's parents child to set2's child

	while (set->adj != nullptr) set = set->adj;		// while set's adj isn't null iterate through

	set->adj = set2;								// set set2 to the final adj found in set
}

// This function is used to print out the totW results and the edges that will be used in the Spanned tree
void SPAN::PrintAlpha(double totW)
{
	cout << setprecision(3) << totW << endl;		// prints the weight the a precision of 3 decimals
	for (int i = 1; i < numOfE; i++)				// loops through the edges
	{
		if (alphaHeap[i].used)
		{
			cout << alphaHeap[i].key << "-" << alphaHeap[i].key2 << ": " << setprecision(3) << alphaHeap[i].w << endl;	// prints the used edge
			alphaHeap[i].used = false;
		}
	}
}

// This function is used to find an edge that will be used in the spanning tree. In it's node 
// it is marked as beig used with the used boolean. 
void SPAN::SetUsedEdge(string key, string key2)
{
	if (key > key2)
	{
		string temp = key;
		key = key2;
		key2 = temp;
	}

	for (int i = 1; i < numOfE; i++)															// loops through the edges
	{
		if (alphaHeap[i].key == key && alphaHeap[i].key2 == key2) alphaHeap[i].used = true;		// once it's found mark as being used
	}
}

// This method is used to find if a vertex currently exists in the linked list that is carrying 
// remianing vertexes to be put in the graph for Prim's algorithm. It iterates through the
// linked list and return true if it is there false if it isnt
bool SPAN::FindVertex(string vertex)
{
	node* vert = primVert->down;				// set vert to primVerts second value 
	while (vert != nullptr)						// while vert isn't nullptr
	{
		if (vertex == vert->key) return true;	// if the vertex is in it return true
		vert = vert->down;						// otherwise iterate through linked likst
	}
	return false;								// if not found return false
}

// This method is used to find the next safe vertex that connects one of the remaining vertices to
// the current graph. The only parameter required is the current graph. It uses the primVert to hold
// the vertices that have yet to be attached to the graph. 
double SPAN::FindPrimEdge(node* graph)
{
	double low = numeric_limits<double>::max();				// sets low to max value
	node* l = nullptr;										// for left side of an edge
	node* r = nullptr;										// for the left side of an edge
	node* vert = primVert->down;							// holds a vertex from primVert
	node* lag = nullptr;									// lags behind graph
	node* g = graph;										// g is a holder for graph so graph will not be changed 

	while (g != nullptr)										// while g still has a vertex to look at
	{
		node* adj = g->adj;									// look at the vertices that create edges with g

		while (adj != nullptr)								// while the vertex still has edges 
		{
			if (adj->w < low && FindVertex(adj->key))		// if the vertex still exists in primVert and the weight is lower than the curennt lowest
			{
				low = adj->w;								// set new lowest to low
				l = g;										// set left side of the edge to l
				r = adj;									// set the right side of the edge to r
			}
			adj = adj->adj;									// iterate through adj
		}
		lag = g;											// set lag to stay one step behind g
		g = g->down;										// iterate g 
	}

	SetUsedEdge(l->key, r->key);							// Sets edge to being used

	while (vert->key != r->key) vert = vert->down;			// finds r spot in primVert

	lag->down = vert;										// adds r's spot from primVert to the graph and removes it from primvert
	vert->up->down = vert->down;							//  |

	if (vert->down != nullptr)								//  |
	{														//  |
		vert->down->up = vert->up;							//  |
		vert->down = nullptr;								//  V
	}

	return low;												// returns weight to add to totW

}