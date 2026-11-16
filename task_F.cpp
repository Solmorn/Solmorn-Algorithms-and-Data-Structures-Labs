#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>


class graph {
    public:

    graph(int n, bool oriented) :
        ver(n),
        edge_list(n, std::vector<int>()),
        input_edge_list(),
        color(n, 0),
        mask(n, -1),
        parent(n, -1),
        depth(n, 0),
        tin(n, -1),
        low(n, INT_MAX),
        oriented(oriented),
        tree(false) {}

    void add_edge(int ver1, int ver2) {
        input_edge_list.push_back(std::make_pair(ver1, ver2));
        edge_list[ver1].push_back(ver2);

        if (!oriented) {
            edge_list[ver2].push_back(ver1);
        }
    }


    int ver;
    std::vector<std::vector<int>> edge_list;
    std::vector<std::pair<int, int>> input_edge_list;

    std::vector<int> color;
    std::vector<int> mask;
    std::vector<int> parent;
    std::vector<int> depth;
    std::vector<int> tin;
    std::vector<int> low;

    bool oriented;
    bool tree;
};


void dfs_comp(graph &g, int start, int counter, std::vector<int> &mask) {
    g.color[start] = 1;
    mask[start] = counter;

    for (auto j : g.edge_list[start]) {
        if (!g.color[j]) {
            dfs_comp(g, j, counter, mask);
        }
    }
}


void dfs_bridge(graph &g, int start, int &counter) {
    g.color[start] = 1;
    g.tin[start] = counter++;
    g.low[start] = g.tin[start];

    for (auto j : g.edge_list[start]) {
        if (j == g.parent[start]) continue;

        if (!g.color[j]) {
            g.parent[j] = start;

            dfs_bridge(g, j, counter);

            g.low[start] = std::min(g.low[start], g.low[j]);

        } else {
            g.low[start] = std::min(g.low[start], g.tin[j]);
        }
    }
}


std::vector<int> dfs_find_bridge(graph &g) {
    int counter = 0;

    for (int i = 0; i < g.ver; i++) {
        if (!g.color[i]) {
            dfs_bridge(g, i, counter);
        }
    }

    std::vector<int> bridges;

    for (size_t i = 0; i < g.input_edge_list.size(); i++) {
        auto [ver1, ver2] = g.input_edge_list[i];

        if (g.parent[ver1] == ver2 && g.low[ver1] > g.tin[ver2]) {
            bridges.push_back(i);
        }

        if (g.parent[ver2] == ver1 && g.low[ver2] > g.tin[ver1]) {
            bridges.push_back(i);
        }
    }

    return bridges;
}


int dfs_ignore_bridge(graph &g, const std::vector<int> &bridges) {
    std::vector<bool> is_bridge(g.input_edge_list.size(), false);

    for (auto i : bridges) {
        is_bridge[i] = true;
    }

    graph new_g(g.ver, false);

    for (size_t i = 0; i < g.input_edge_list.size(); i++) {
        if (!is_bridge[i]) {
            auto [ver1, ver2] = g.input_edge_list[i];

            new_g.add_edge(ver1, ver2);
        }
    }

    int counter = 0;

    std::fill(g.mask.begin(), g.mask.end(), -1);

    for (int i = 0; i < g.ver; i++) {
        if (!new_g.color[i]) {
            dfs_comp(new_g, i, counter, g.mask);

            counter++;
        }
    }

    return counter;
}


void dfs_hang(graph &g, int start) {
    for (auto j : g.edge_list[start]) {
        if (j == g.parent[start]) continue;

        g.parent[j] = start;
        g.depth[j] = g.depth[start] + 1;

        dfs_hang(g, j);
    }
}


void hang(graph &g, int root) {
    if (!g.tree) return;

    std::fill(g.parent.begin(), g.parent.end(), -1);
    std::fill(g.depth.begin(), g.depth.end(), 0);

    dfs_hang(g, root);
}


int lca(graph &g, int ver1, int ver2) {
    static std::vector<std::vector<int>> up;
    static int log = 0;

    if (up.empty()) {
        log = 1;

        while ((1 << log) <= g.ver) {
            log++;
        }

        up.resize(log, std::vector<int>(g.ver));

        for (int v = 0; v < g.ver; v++) {
            if (g.parent[v] == -1) {
                up[0][v] = v;
            } else {
                up[0][v] = g.parent[v];
            }
        }

        for (int i = 1; i < log; i++) {
            for (int v = 0; v < g.ver; v++) {
                up[i][v] = up[i - 1][up[i - 1][v]];
            }
        }
    }

    if (g.depth[ver1] < g.depth[ver2]) {
        std::swap(ver1, ver2);
    }

    int diff = g.depth[ver1] - g.depth[ver2];

    for (int i = log - 1; i >= 0; i--) {
        if (diff >= (1 << i)) {
            ver1 = up[i][ver1];
            diff -= (1 << i);
        }
    }

    if (ver1 == ver2) return ver1;

    for (int i = log - 1; i >= 0; i--) {
        if (up[i][ver1] != up[i][ver2]) {
            ver1 = up[i][ver1];
            ver2 = up[i][ver2];
        }
    }

    return g.parent[ver1];
}


int main() {
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");

    int n = 0;
    int m = 0;

    in >> n >> m;

    int finish = 0;

    in >> finish;
    finish--;

    graph graph1(n, false);

    for (int i = 0; i < m; i++) {
        int ver1 = 0;
        int ver2 = 0;

        in >> ver1 >> ver2;

        graph1.add_edge(ver1 - 1, ver2 - 1);
    }

    std::vector<int> bridges = dfs_find_bridge(graph1);

    int comp_count = dfs_ignore_bridge(graph1, bridges);

    graph bridge_tree(comp_count, false);

    for (auto i : bridges) {
        auto [ver1, ver2] = graph1.input_edge_list[i];

        bridge_tree.add_edge(
            graph1.mask[ver1],
            graph1.mask[ver2]
        );
    }

    bridge_tree.tree = true;

    hang(bridge_tree, graph1.mask[finish]);

    int q = 0;

    in >> q;

    while (q--) {
        int ver1 = 0;
        int ver2 = 0;

        in >> ver1 >> ver2;

        ver1 = graph1.mask[ver1 - 1];
        ver2 = graph1.mask[ver2 - 1];

        int ancestor = lca(bridge_tree, ver1, ver2);

        out << bridge_tree.depth[ancestor] << "\n";
    }
}
