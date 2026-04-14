class bipartite_matching {
private:
    int n, m;
    max_flow mf;

public:
    bipartite_matching(int n, int m) : n(n), m(m), mf(n + m + 2) {
        for (int i = 0; i < n; ++i) {
            mf.add_edge(n + m, i, 1);
        }
        for (int i = 0; i < m; ++i) {
            mf.add_edge(i + n, n + m + 1, 1);
        }
    }

    void add_edge(int u, int v) {
        mf.add_edge(u, v + n, 1);
    }

    int max_matching() {
        return mf.flow(n + m, n + m + 1);
    }

    vector<array<int, 2>> get_matching() {
        max_matching();
        vector<array<int, 2>> res;
        for (auto [u, v, c, f] : mf.edges()) {
            if (f && u != n + m && v != n + m + 1) res.push_back({u, v - n});
        }
        return res;
    }

    pair<vector<int>, vector<int>> min_vertex_cover() {
        int S = n + m;     // source
        int T = n + m + 1; // sink
        max_matching();
        vector<bool> cut = mf.min_cut();

        vector<int> L, R;
        for (int i = 0; i < n; i++) {
            if (!cut[i]) L.push_back(i); // unvisited left -> in cover
        }
        for (int j = 0; j < m; j++) {
            if (cut[n + j]) R.push_back(j); // visited right -> in cover
        }
        return {L, R};
    }
};
