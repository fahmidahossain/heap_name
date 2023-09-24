#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
const int INF = 2e8;
vector<vector<int>> graph, fw_weight, parent;
int number_of_vertex,number_of_edges;

void floyd_warshall()
{
    fw_weight = graph;
//the principle diagonal will be all zero
    for (int i = 1; i <= number_of_vertex; i++)
    {
        for (int j = 1; j <= number_of_vertex; j++) fw_weight[i][i] = 0;
    }

    for (int j = 1; j <= number_of_vertex; j++)
    {
        for (int i = 1; i <= number_of_vertex; i++)
        {
            for (int k = 1; k <= number_of_vertex; k++)
            {
                // compare whether we need intermediate steps or not
                if (fw_weight[i][k] > fw_weight[i][j] + fw_weight[j][k])
                {
                    fw_weight[i][k] = fw_weight[i][j] + fw_weight[j][k];
                    if (i != k) parent[i][k] = parent[j][k];
                }
            }
        }
    }
}

int main()
{
    ifstream infile;
    ofstream outfile("output.txt");
    infile.open("input.txt");


    infile >> number_of_vertex >> number_of_edges;
    graph.assign(number_of_vertex + 2, vector<int>(number_of_vertex + 2, INF));
    fw_weight.assign(number_of_vertex + 2, vector<int>(number_of_vertex + 2, INF));
    parent.assign(number_of_vertex + 2, vector<int>(number_of_vertex + 2, -1));

    for (int i = 0; i < number_of_edges; i++)
    {
        int u, v, w;
        infile >> u >> v >> w;
        graph[u][v] = min(graph[u][v], w);
        parent[u][v] = u;
    }

    floyd_warshall();

    outfile << "Shortest distance matrix\n";
    for (int i = 1; i <= number_of_vertex; i++)
    {
        for (int j = 1; j <= number_of_vertex; j++)
        {
            if (fw_weight[i][j] == INF)
                outfile << "INF ";
            else
                outfile << fw_weight[i][j] << " ";
        }
        outfile << "\n";
    }

    //cout << "Predecessor matrix\n";
    //for (int i = 1; i <= n; i++) {
    //for (int j = 1; j <= n; j++) {
    //if (parent[i][j] == -1)
    //cout << "NIL ";
    // else
    //cout << parent[i][j] << " ";
    // }
    // cout << "\n";
    // }

    infile.close();
    return 0;
}
