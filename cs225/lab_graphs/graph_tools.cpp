/**
 * @file graph_tools.cpp
 * This is where you will implement several functions that operate on graphs.
 * Be sure to thoroughly read the comments above each function, as they give
 *  hints and instructions on how to solve the problems.
 */

#include "graph_tools.h"

/**
 * Finds the minimum edge weight in the Graph graph.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to search
 * @return the minimum weighted edge
 *
 * @todo Label the minimum edge as "MIN". It will appear blue when
 *  graph.savePNG() is called in minweight_test.
 *
 * @note You must do a traversal.
 * @note You may use the STL stack and queue.
 * @note You may assume the graph is connected.
 *
 * @hint Initially label vertices and edges as unvisited.
 */
int GraphTools::findMinWeight(Graph& graph)
{
    vector<Vertex> vertices = graph.getVertices();
    vector<Edge> edges = graph.getEdges();
    int minWeight = INT_MAX;
    int temp = 0;  //this will be the min edge from each traversal
    for(Vertex v : vertices){
        graph.setVertexLabel(v, "UNEXPLORED");
    }
    for(Edge e : edges){
        graph.setEdgeLabel(e.source, e.dest, "UNEXPLORED");
    }
    for(Vertex v : vertices){
        if(graph.getVertexLabel(v) == "UNEXPLORED"){
            temp = BFS(graph, v);
            if(minWeight > temp)
                minWeight = temp;
        }
    }
    for(vector<Edge>::iterator it = edges.begin(); it != edges.end(); ++it){
        Edge currEdge = *it;
        if(currEdge.getWeight() == minWeight)
            graph.setEdgeLabel(currEdge.source, currEdge.dest, "MIN");
    }
    return minWeight;
}

int GraphTools::BFS(Graph& graph, Vertex start){
    int minWeight = INT_MAX;
    queue<Vertex> q;
    graph.setVertexLabel(start, "VISITED");
    q.push(start);

    while(!q.empty()){
        Vertex v = q.front();
        q.pop();
        vector<Vertex> adjacent = graph.getAdjacent(v);
        for(Vertex w : adjacent){
            if(minWeight > graph.getEdgeWeight(v,w))
                minWeight = graph.getEdgeWeight(v,w);
            if(graph.getVertexLabel(w) == "UNEXPLORED"){
                graph.setEdgeLabel(v,w,"DISCOVERY");
                graph.setVertexLabel(w, "VISITED");
                q.push(w);
            }
            else if(graph.getEdgeLabel(v,w) == "UNEXPLORED")
                graph.setEdgeLabel(v,w, "CROSS");
        }
    }
    return minWeight;
}

int GraphTools::BFS_path(Graph& graph, Vertex start, Vertex end){
    queue<Vertex> q;
    unordered_map<Vertex, Vertex> parent;
    graph.setVertexLabel(start, "VISITED");
    q.push(start);
    int shortestPath = 0;
    while(!q.empty()){
        Vertex v = q.front();
        q.pop();
        vector<Vertex> adjacent = graph.getAdjacent(v);
        for(Vertex w : adjacent){
            if(graph.getVertexLabel(w) == "UNEXPLORED"){
                graph.setVertexLabel(w, "VISITED");
                q.push(w);
                parent[w] = v;
            }
        }
    }
    bool pathFound = false;
    Vertex pathVertex = parent[end];
    graph.setEdgeLabel(parent[end], end, "MINPATH");
    while(!pathFound){
        shortestPath++;
        if(pathVertex == start)
            pathFound = 1;
        else{
            graph.setEdgeLabel(parent[pathVertex], pathVertex, "MINPATH");
            pathVertex = parent[pathVertex];
        }
    }
    return shortestPath;
}
/**
 * Returns the shortest distance (in edges) between the Vertices
 *  start and end.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to search
 * @param start - the vertex to start the search from
 * @param end - the vertex to find a path to
 * @return the minimum number of edges between start and end
 *
 * @todo Label each edge "MINPATH" if it is part of the minimum path
 *
 * @note Remember this is the shortest path in terms of edges,
 *  not edge weights.
 * @note Again, you may use the STL stack and queue.
 * @note You may also use the STL's unordered_map, but it is possible
 *  to solve this problem without it.
 *
 * @hint In order to draw (and correctly count) the edges between two
 *  vertices, you'll have to remember each vertex's parent somehow.
 */
int GraphTools::findShortestPath(Graph& graph, Vertex start, Vertex end)
{
    int shortestPath = INT_MAX;
    vector<Vertex> vertices = graph.getVertices();
    vector<Edge> edges = graph.getEdges();
    for(Vertex v : vertices){
        graph.setVertexLabel(v, "UNEXPLORED");
    }
    for(Edge e : edges){
        graph.setEdgeLabel(e.source, e.dest, "UNEXPLORED");
    }
    return BFS_path(graph, start, end);
}

/**
 * Finds a minimal spanning tree on a graph.
 * THIS FUNCTION IS GRADED.
 *
 * @param graph - the graph to find the MST of
 *
 * @todo Label the edges of a minimal spanning tree as "MST"
 *  in the graph. They will appear blue when graph.savePNG() is called.
 *
 * @note Use your disjoint sets class from MP 7.1 to help you with
 *  Kruskal's algorithm. Copy the files into the dsets.h and dsets.cpp .
 * @note You may call std::sort instead of creating a priority queue.
 */
void GraphTools::findMST(Graph& graph)
{
    DisjointSets mst;
    vector<Vertex> vertices = graph.getVertices();
    vector<Edge> edges = graph.getEdges();
    int counter = 0;
    mst.addelements((int)vertices.size());
    std::sort(edges.begin(), edges.end());
    while(counter < vertices.size() - 1){
        Edge mstEdge = edges[0];
        edges.erase(edges.begin());
        int index1 = findIndex(vertices, mstEdge.source);
        int index2 = findIndex(vertices, mstEdge.dest);
        if(mst.find(index1) != mst.find(index2)){
            graph.setEdgeLabel(mstEdge.source, mstEdge.dest, "MST");
            mst.setunion(mst.find(index1), mst.find(index2));
            counter++;
        }
    }
}

int GraphTools::findIndex(vector<Vertex>& vertices, Vertex query){
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i] == query)
            return i;
    }
    return -1;  //if it doesnt exist
}
