#include <iostream>
#include <vector>
#include <string>
#include <algorithm>



class graph {
    public:

    graph(int n, bool oriented) :
        ver(n),
        edge_list(n, std::vector<int>()),
        edge_list_rev(n, std::vector<int>()),
        color(n, 0),
        mask(n, -1),
        topsort(n, std::make_pair(-1, 0)),
        comp(n, -1),
        oriented(oriented) {
            for (int i = 0; i < n; i++) {
                topsort[i].second = i;
            }
        }

    void add_edge(int ver1, int ver2) {
        edge_list[ver1].push_back(ver2);
        if (!oriented) {
            edge_list[ver2].push_back(ver1);
        } else {
            edge_list_rev[ver2].push_back(ver1);
        } 
        
    }


    bool oriented;
    int ver;
    std::vector<std::vector<int>> edge_list;
    std::vector<std::vector<int>> edge_list_rev;

    std::vector<int> color;
    std::vector<int> mask;
    std::vector<std::pair<int, int>> topsort;
    std::vector<int> comp;
    int comp_counter = 0;
};


int dfs_cycle(graph &g, int start);
void dfs_cluster(graph &g, int start, std::vector<int> &clust);
int dfs_topsort(graph &g, int start, int &counter);
void dfs_order(graph &g, int start, std::vector<int> &order);
void dfs_comp(graph &g, int start, int counter, std::vector<int> &comp);



void dfs_comp_find(graph &g) {
    std::vector<int> order;

    std::fill(g.color.begin(), g.color.end(), 0);

    for (int i = 0; i < g.ver; i++) {
        if (!g.color[i]) {
            dfs_order(g, i, order);
        }
    }


    graph reverse_g(g.ver, 1);

    for (int i = 0; i < g.ver; i++) {
        for (auto j : g.edge_list[i]) {
            reverse_g.add_edge(j, i);
        }
    }


    int counter = 0;

    for (int i = (int)order.size() - 1; i >= 0; i--) {
        int v = order[i];

        if (!reverse_g.color[v]) {
            dfs_comp(reverse_g, v, counter, g.comp);
            counter++;
            g.comp_counter++;
        }
    }
}



int dfs_topsort_find(graph &g) {
    int counter = 0;
    for (int i = 0; i < g.ver; i++) {
        if (!g.color[i]) {
            int res = dfs_topsort(g, i, counter);
            if (res == -1) return -1;
        }
    }

    return 1;
}

int dfs(graph &g) {
    for (int i = 0; i < g.ver; i++) {
        if (!g.color[i]) {
            int res = dfs_cycle(g, i);

            if (res != -1) return res;
        }
    }

    return -1;
}

void dfs_cluster_find(graph &g, std::vector<std::vector<int>> &clusters) {
    for (int i = 0; i < g.ver; i++) {
        if (!g.color[i]) {
            std::vector<int> clust;
            dfs_cluster(g, i, clust);
            clusters.push_back(clust);
        }
    }

    return;
}


int dfs_cycle(graph &g, int start) {
    g.color[start] = 1;

    for (auto j : g.edge_list[start]) {
        if (!g.oriented && j == g.mask[start]) continue;

        if (!g.color[j]) {
            g.mask[j] = start;

            int res = dfs_cycle(g, j);

            if (res != -1) return res;

        } else if (g.color[j] == 1) {
            g.mask[j] = start;
            return j;
        }
    }

    g.color[start] = 2;

    return -1;
}

void dfs_cluster(graph &g, int start, std::vector<int> &clust) {
    g.color[start] = 1;

    clust.push_back(start);

    for (auto j : g.edge_list[start]) {
        if (j == g.mask[start]) continue;

        if (!g.color[j]) {
            g.mask[j] = start;

            dfs_cluster(g, j, clust);

        }
    }

    return;
}

int dfs_topsort(graph &g, int start, int &counter) {
    g.color[start] = 1;

    for (auto j : g.edge_list[start]) {
        if (!g.oriented && j == g.mask[start]) continue;

        if (!g.color[j]) {
            g.mask[j] = start;

            int res = dfs_topsort(g, j, counter);

            if (res != 1) return res;

        } else if (g.color[j] == 1) {
            g.mask[j] = start;
            return -1;
        }
    }

    g.color[start] = 2;
    g.topsort[start].first = counter++;

    return 1;
}



void dfs_order(graph &g, int start, std::vector<int> &order) {
    g.color[start] = 1;

    for (auto j : g.edge_list[start]) {
        if (!g.color[j]) {
            dfs_order(g, j, order);
        }
    }

    order.push_back(start);
}


void dfs_comp(graph &g, int start, int counter, std::vector<int> &comp) {
    g.color[start] = 1;
    comp[start] = counter;

    for (auto j : g.edge_list[start]) {
        if (!g.color[j]) {
            dfs_comp(g, j, counter, comp);
        }
    }
}






int main() {
    int n = 0;
    int m = 0;
    
    std::cin >> n >> m;

    graph g(n, 1);

    for (size_t i = 0; i < m; i++) {
        int ver1, ver2;
        std::cin >> ver1 >> ver2;
        g.add_edge(ver1 - 1, ver2 - 1);
    }

    dfs_comp_find(g);

    std::cout << g.comp_counter << "\n";

    for (auto v : g.comp) {
        std::cout << v + 1 << " ";
    }

}
