//
// Created by Mohammadsadegh Najafi on 2020/04/22.
//

#include "Subgraphs.h"
#include <list>

using namespace std;
Subgraphs::Subgraphs(int N) {
    adjlist.resize(N);
    bitvector.reset();
}
void Subgraphs::addvertex( int label) {
    Subgraphvertrx s = {label, false};
    bool already_have = false;

//    cout << "vertex" << label << endl;
//    cout << vertices.size() << endl;

    for (int i = 0; i < vertices.size() ; ++i) {
        if (vertices.at(i).ID == label){
            vertices.at(i).visisted = true;
            s.visisted = true;
        }
    }
    if(!s.visisted){
        vertices.push_back(s);
    }
   // bitvector.set(label);
}

void Subgraphs::addedge(int v1, int v2)
{
    Subgraphvertrx s = {-1, false};
    Subgraphvertrx t = {-1, false};

    for (int i = 0; i < vertices.size(); ++i) {

        if (vertices.at(i).ID == v1){
            s = vertices.at(i);
        }
        if (vertices.at(i).ID == v2){
            t = vertices.at(i);
        }
    }
   // cout << s.ID << " " << t.ID <<endl;
    SubgraphEdge edge = {s, t};
    subedges.push_back(edge);
    adjlist[v1].push_back(v2);
}

int Subgraphs::vertex_size() {
    return vertices.size();
}

int Subgraphs::edge_size() {
    return subedges.size();
}

void Subgraphs::print_bitvector() {
    cout<< bitvector << endl;
}

bool Subgraphs::BFS(int src, int target) {

    if (!this-has_vertex(target)){
        return false;
    }
    if (get_vertex_status(src)){
        return false;
    }
    list<Subgraphvertrx> queue;

    // Mark the current node as visited and enqueue it
    queue.push_back(this->change_vertex_status(src));

    vector<int>::iterator itr;

    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        Subgraphvertrx s  = queue.front();
        cout << s.ID << " ";
        queue.pop_front();
        for (itr = this->adjlist[s.ID].begin(); itr != this->adjlist[s.ID].end(); ++itr)
        {
            if (!get_vertex_status(*itr)){
                Subgraphvertrx visited =  this->change_vertex_status(*itr);
                if (visited.ID == target) {
                    return true;
                }
                queue.push_back(visited);
            }
        }
    }
    return false;
}

bool Subgraphs::has_vertex(int vertex) {
    if (bitvector[vertex] == 1)
        return true;
    return false;
}

bool Subgraphs::get_vertex_status(int vertex) {
    for (int i = 0; i < vertices.size() ; ++i) {
        if (vertices.at(i).ID == vertex){
            return vertices.at(i).visisted;
        }
    }
    return false;
}

Subgraphvertrx Subgraphs::change_vertex_status(int vertex) {
    for (int i = 0; i < vertices.size() ; ++i) {
        if (vertices.at(i).ID == vertex){
            vertices.at(i).visisted = true;
            return vertices.at(i);
        }
    }
}

list<int> Subgraphs::find_landmarks(int k) {
    int all_degree [adjlist.size()];
    for (int i = 0; i < adjlist.size(); ++i) {
        all_degree[i] = degree(i);
    }
    int * klarge = klargest (all_degree, edge_size(), k);
    list<int> result;
    for (int j = 0; j < adjlist.size(); ++j) {
        for (int i = 0; i < k; ++i) {
            if (klarge[i] == all_degree[j]) {
                result.push_back(j);
            }
        }
    }
    this->landmarks = result;
    return result;
}

int Subgraphs::degree(int vertex) {
    return adjlist.at(vertex).size();
}

int *Subgraphs::klargest(int arr[], int n, int k) {
    sort(arr, arr + n, greater<int>());
    int result[k];
    for (int i = 0; i < k; i++)
        result[i] = arr[i];
    return result;
}

void Subgraphs::DFSUtil(int s, int v) {
    Subgraphvertrx s_vertex = {-1, false};
    Subgraphvertrx v_vertex = {-1, false};
    for (int j = 0; j < vertices.size() ; ++j) {
        if(vertices.at(j).ID == s){
            s_vertex = vertices.at(j);
            s_vertex.landmark = true;
        }
        if(vertices.at(j).ID == v){
            v_vertex = vertices.at(j);
        }

    }
    s_vertex.tc.set(v, 1);
    // Find all the vertices reachable through v
    vector<int>::iterator i;

    for (i = this->adjlist[v].begin(); i != this->adjlist[v].end(); ++i)
        if (s_vertex.tc[*i] == 0)
            DFSUtil(s, *i);
}

void Subgraphs::transitive_closure(int k) {
    // Call the recursive helper function to print DFS
    // traversal starting from all vertices one by one
    list<int> landmarks = find_landmarks(k);
    for (int i = 0; i < k; i++) {
        int landmark = landmarks.front();
        DFSUtil(landmark, landmark); // Every vertex is reachable from self.
    }
}

bool Subgraphs::BFS_for_landmarks(int src, int target) {
    if (!this-has_vertex(target)){
        return false;
    }
    Subgraphvertrx s_vertex = {-1, false, false};
    for (int j = 0; j < vertices.size() ; ++j) {
        if(vertices.at(j).ID == src){
            s_vertex = vertices.at(j);
            if (s_vertex.landmark == true && s_vertex.tc[target] == 1){
                return true;
            }
        }
    }
    if (get_vertex_status(src)){
        return false;
    }
    list<Subgraphvertrx> queue;

    // Mark the current node as visited and enqueue it
    queue.push_back(this->change_vertex_status(src));

    vector<int>::iterator itr;

    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        Subgraphvertrx s  = queue.front();
        cout << s.ID << " ";
        queue.pop_front();
        for (itr = this->adjlist[s.ID].begin(); itr != this->adjlist[s.ID].end(); ++itr)
        {
            if (!get_vertex_status(*itr)){
                Subgraphvertrx visited =  this->change_vertex_status(*itr);
                if (visited.ID == target) {
                    return true;
                }
                queue.push_back(visited);
            }
        }
    }
    return false;
}



