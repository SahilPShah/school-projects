#include <iostream>
#include <string>

#include "adjacency_matrix.h"

using namespace std;

/*
 Implement the functions below
 */

bool containsEdge(Graph const * const g, int src, int dest) {
    if(g->adjMatrix[src][dest])     //index into adjMatrixmand see if the value in that location is 1
        return true;
    return false;
}
void addEdge(Graph* g, int src, int dest) {
    int n = g->n;
    if(containsEdge(g, src, dest))                          //dont add edge if it already exists
        return;
    if(src >= n || dest >= n){                              //resize the array
        bool ** newMatrix = new bool*[2*n];                  //double the height
        for(int i = 0; i < 2*n; i++)
            newMatrix[i] = new bool[2*n];                    //double the width
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                newMatrix[i][j] = (g->adjMatrix)[i][j];     //copy elements from other array
            }
        }
        newMatrix[src][dest] = 1;                           //add new edge
        free(g->adjMatrix);                                 //free old memory
        g->adjMatrix = newMatrix;                           //set adjMatrix to newMatrix
        g->n = 2*n;                                          //set the new size
    }
    else{                                                   //no need to resize the array
        (g->adjMatrix)[src][dest] = 1;
    }
}

int numOutgoingEdges(Graph const * const g, int v) {
    bool *edgeList = (g->adjMatrix)[v];
    int count = 0;
    for(int i = 0; i < g->n; i++){                           //iterate through each possible destination vertex
        if(edgeList[i])                                      //if it is 1, there is an outgoing edge from v to edgeList[i]
            count++;                                         //increment count
    }
    return count;
}


int numIncomingEdges(Graph const * const g, int v) {
    int count = 0;
    int n = g->n;
    for(int i = 0; i < n; i++){                             //iterate through all possible source vertices
        if((g->adjMatrix)[i][v])                            //if the entry is 1 is the matrix[i][v] where i is the source and v is the vertex
            count++;                                        //increment count
    }
    return count;
}

void printGraph(Graph const * const g) {
        cout << "Adjacency Matrix: " << endl;
        for (int i = 0; i < g->n; i += 1) {
                for (int j = 0; j < g->n; j += 1) {
                        bool neighbor = g->adjMatrix[i][j];
                        cout << neighbor << " ";
                }
                cout << "\n\n";
        }
}

Graph* createVertices(int numV) {
    // No need to modify this function
    Graph* g = new Graph();
    g->n = numV;
    g->adjMatrix = new bool*[numV];
    for (int i = 0; i < numV; i += 1) {
        g->adjMatrix[i] = new bool[numV];
        for (int j = 0; j < numV; j += 1) {
            g->adjMatrix[i][j] = 0;
        }
    }
    return g;
}
