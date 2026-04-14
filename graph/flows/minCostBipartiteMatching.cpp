class minCostBipartiteMatching {
private:
    int n, m;
    MCMF mf;

public:
    minCostBipartiteMatching(int n, int m) : n(n), m(m), mf(n + m + 2) {
        for (int i = 0; i < n; ++i) {
            mf.addEdge(n + m, i, 0, 1);
        }
        for (int i = 0; i < m; ++i) {
            mf.addEdge(i + n, n + m + 1, 0, 1);
        }
    }

    void add_edge(int u, int v, int c) {
        mf.addEdge(u, v + n, c, 1);
    }

    auto max_matching(int k = 1e9) {
        return mf.MinCostMaxFlow(n + m, n + m + 1, k);
    }

    // first requires calling max_matching
    vector<array<int, 2>> get_matching() {
        vector<array<int, 2>> res;
        for (auto [u, v, flow, cost] : mf.getEdgesWithFlow()) {
            if (u != n + m && v != n + m + 1) res.push_back({u, v - n});
        }
        return res;
    }
};
