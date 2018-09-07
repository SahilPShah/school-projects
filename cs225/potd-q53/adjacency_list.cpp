#include <iostream>
#include <string>
#include "adjacency_list.h"

using namespace std;

bool containsEdge(Graph const * const g, int src, int dest) {
        // Your code here
        LinkedListNode* edges = g->adjLists[src].edges;
        while(edges){
            if(edges->v == dest)
                return true;
            edges = edges->next;
        }
        return false;
}
void addEdge(Graph* g, int src, int dest) {
    //check if edge exists
    if(containsEdge(g, src, dest))
        return;
    LinkedListNode* newNode = new LinkedListNode();
    newNode->v = dest;
    newNode->next = g->adjLists[src].edges;
    g->adjLists[src].edges = newNode;

}

int numOutgoingEdges(Graph const * const g, int v) {
    int count = 0;
    LinkedListNode* temp = g->adjLists[v].edges;
    while(temp){
        count++;
        temp = temp->next;
    }
    return count;
}


int numIncomingEdges(Graph const * const g, int v) {
        // Your code here
        int count = 0;
        for(int i = 0; i < g->n; i++){
            if(containsEdge(g, g->adjLists[i].vertex, v))
                count++;
        }
        return count;
}

// No need to modify the functions below

Graph* createVertices(int numV) {
        Graph* g = new Graph();
        g->adjLists = new AdjacencyList[numV];
        g->n = numV;

        // Initialize the vertices
        for (int i = 0; i < numV; i += 1) {
                g->adjLists[i].vertex = i;
                g->adjLists[i].edges = NULL;
        }

        return g;
}

void printGraph(Graph const * const g) {
    for (int i = 0; i < g->n; i += 1) {
        AdjacencyList adjList = g->adjLists[i];

        int v = adjList.vertex;
        // Vertex
        cout << "Vertex: " << v << endl;

        // Print number of incoming and outgoing edges
        int inc = numIncomingEdges(g, v);
        int out = numOutgoingEdges(g, v);
        cout << "Number of incoming edges: " << inc << endl;
        cout << "Number of outgoing edges: " << out << endl;

        cout << "\n" << endl;
    }
}
