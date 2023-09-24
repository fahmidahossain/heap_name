
#include<stdio.h>
#include<iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;


class Disjoint_set_Union {
    int* rank;
    int* parent;


public:


    int find(int k)
    {
        if (parent[k] == -1)
            return k;

        return parent[k] = find(parent[k]);
    }
    Disjoint_set_Union(int v)
    {
        rank = new int[v];
        parent = new int[v];


        for (int j = 0; j < v; j++) {
                rank[j] = 1;
             parent[j] = -1;
        }
    }



    void Union(int node1, int node2)
    {
        int parent1 = find(node1);
        int parent2 = find(node2);

        if (parent1 != parent2) {
            if (rank[parent1] < rank[parent2]) {
                parent[parent1] = parent2;
            }
            else if (rank[parent1] > rank[parent2]) {
                parent[parent2] = parent1;
            }
            else {
                parent[parent2] = parent1;
                rank[parent1] += 1;
            }
        }
    }
};

class Graph {
    vector<vector<int> >list_of_edge;
    int V;

public:
    Graph(int V) { this->V = V; }
    void addEdge(int x, int y, int w)
    {
        list_of_edge.push_back({ w, x, y });
    }

    void kruskalMst(int num_of_vertex, ostream &outfile)
    {

        sort(list_of_edge.begin(), list_of_edge.end());
        Disjoint_set_Union s(V);
        int sum = 0;
        outfile << "Kruskal's Algorithm"<< endl;
        for (auto x : list_of_edge) {
            int weight = x[0];
            int u = x[1];
            int v = x[2];
            if (s.find(u) != s.find(v)) {
                s.Union(u, v);
                sum += weight;
                outfile << u << " " << v << endl;
            }
        }
        outfile << "Total weight=" << sum;
    }
};


