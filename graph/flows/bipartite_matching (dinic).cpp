class bipartite_matching {
private:
    Dinic d;
    int n, m;

public:
    bipartite_matching(int n, int m) : n(n), m(m), d(n + m + 2, n + m, n + m + 1) {
        for (int i = 0; i < n; ++i) {
            d.add_edge(n + m, i, 1);
        }
        for (int i = 0; i < m; ++i) {
            d.add_edge(i + n, n + m + 1, 1);
        }
    }

    void add_edge(int u, int v) {
        d.add_edge(u, v + n, 1);
    }

    int max_matching() {
        return d.flow();
    }

    vector<array<int, 2>> get_matching() {
        d.flow();
        vector<array<int, 2>> res;
        for (int i = 1; i < d.edges.size(); i += 2) {
            auto [u, v, c, f] = d.edges[i];
            if (f && v != n + m && u != n + m + 1) res.push_back({v, u - n});
        }
        return res;
    }
};
