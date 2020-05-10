//==============================================================================================
// main.cpp - word ladder game implementation
//​
// This is a command prompt program wehre the main function takes in arguments from the command prompt
// It initiates the Spanning tree as long as the predetermine requirments are met. Those being
// the argument count should be 2, the first word should be SPAN, the second should be the file
// that contains the vertices count, the vertices and also the edge matrix.
// As long as these are met it will make the spanning tree and display the results of Kruskal's
// and Prim's algorithm. They are used to find the Minimum spanning tree
//​
// Author:    Logan Dabney, University of Toledo​
// Date:      April 26, 2020​
// Copyright: Copyright 2020 by Logan Dabney. All rights reserved.
//==============================================================================================

#include <iostream>
#include "SPAN.h"

using namespace std;

int main(int argc, char* argv[]) 
{
	if (argc == 2 && (string)argv[0] == "SPAN")		// if the first word is SPAN and the arg count is 2
	{
		SPAN* Stree = new SPAN((string)argv[1]);	// create the span tree from the file given
		Stree->Kruskal();							// do Kruskal's alg
		Stree->Prim();								// do Prim alg
		Stree->~SPAN();
	}
	else											// if it isn't print error message
	{
		cout << "Command entered wrong!" << endl;
	}
}