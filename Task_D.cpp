#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>


struct edge {
    int ver1;
    int ver2;
    int weight;
};


class graph {
    public:

    graph(int n) :
        ver(n),
        edge_list(n, std::vector<std::pair<int, int>>()),
        input_edge_list(),
        parent(n, -1),
        depth(n, 0),
        parent_weight(n, 0),
        tree(false) {}

    void add_edge(int ver1, int ver2, int weight) {
        edge_list[ver1].push_back(std::make_pair(ver2, weight));
        edge_list[ver2].push_back(std::make_pair(ver1, weight));

        input_edge_list.push_back({ver1, ver2, weight});
    }


    int ver;
    std::vector<std::vector<std::pair<int, int>>> edge_list;
    std::vector<edge> input_edge_list;

    std::vector<int> parent;
    std::vector<int> depth;
    std::vector<int> parent_weight;

    bool tree;
};


class dsu {
    public:

    dsu(int n) :
        parent(n),
        rank(n, 0) {
            for (int i = 0; i < n; i++) {
                parent[i] = i;
            }
        }

    int find(int v) {
        if (parent[v] == v) return v;

        parent[v] = find(parent[v]);

        return parent[v];
    }

    bool merge(int ver1, int ver2) {
        ver1 = find(ver1);
        ver2 = find(ver2);

        if (ver1 == ver2) return false;

        if (rank[ver1] < rank[ver2]) {
            std::swap(ver1, ver2);
        }

        parent[ver2] = ver1;

        if (rank[ver1] == rank[ver2]) {
            rank[ver1]++;
        }

        return true;
    }


    std::vector<int> parent;
    std::vector<int> rank;
};


void dfs_hang(graph &g, int v) {
    for (auto edge : g.edge_list[v]) {
        int to = edge.first;
        int weight = edge.second;

        if (to == g.parent[v]) continue;

        g.parent[to] = v;
        g.depth[to] = g.depth[v] + 1;
        g.parent_weight[to] = weight;

        dfs_hang(g, to);
    }
}


void hang(graph &g, int root) {
    if (!g.tree) return;

    g.parent[root] = -1;
    g.depth[root]  = 0;

    dfs_hang(g, root);
}


int lca(graph &g, int ver1, int ver2) {
    while (g.depth[ver1] > g.depth[ver2]) {
        ver1 = g.parent[ver1];
    }

    while (g.depth[ver2] > g.depth[ver1]) {
        ver2 = g.parent[ver2];
    }

    while (ver1 != ver2) {
        ver1 = g.parent[ver1];
        ver2 = g.parent[ver2];
    }

    return ver1;
}


int min_edge(graph &g, int ver1, int ver2) {
    if (ver1 == ver2) return 0;

    int ancestor = lca(g, ver1, ver2);
    int answer = INT_MAX;

    while (ver1 != ancestor) {
        answer = std::min(answer, g.parent_weight[ver1]);

        ver1 = g.parent[ver1];
    }

    while (ver2 != ancestor) {
        answer = std::min(answer, g.parent_weight[ver2]);

        ver2 = g.parent[ver2];
    }

    return answer;
}


int main() {
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    int n = 0;
    int m = 0;
    int k = 0;

    in >> n >> m >> k;

    graph graph1(n);
    graph graph2(n);

    for (int i = 0; i < m; i++) {
        int ver1   = 0;
        int ver2   = 0;
        int weight = 0;

        in >> ver1 >> ver2 >> weight;

        graph1.add_edge(ver1 - 1, ver2 - 1, weight);
    }

    std::sort(
        graph1.input_edge_list.begin(),
        graph1.input_edge_list.end(),
        [](const edge &edge1, const edge &edge2) {
            return edge1.weight > edge2.weight;
        }
    );

    dsu sets(n);

    int edge_count = 0;

    for (auto edge : graph1.input_edge_list) {
        if (sets.merge(edge.ver1, edge.ver2)) {
            graph2.add_edge(edge.ver1, edge.ver2, edge.weight);

            edge_count++;

            if (edge_count == n - 1) break;
        }
    }

    graph2.tree = true;

    hang(graph2, 0);

    while (k--) {
        int ver1 = 0;
        int ver2 = 0;

        in >> ver1 >> ver2;

        out << min_edge(graph2, ver1 - 1, ver2 - 1) << "\n";
    }
}
