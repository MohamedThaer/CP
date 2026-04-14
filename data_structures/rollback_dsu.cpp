class rollback_dsu {
private:
    int disjoint_sets;
    vector<int> par, sz;
    vector<pair<int &, int>> history;

    int find_par(int node) {
        if (node == par[node]) {
            return node;
        }
        return find_par(par[node]);
    }

public:
    rollback_dsu(int n) : disjoint_sets(n), par(n + 1), sz(n + 1, 1) {
        iota(par.begin(), par.end(), 0);
    }

    bool same_set(int a, int b) {
        return find_par(a) == find_par(b);
    }

    void merge(int a, int b) {
        a = find_par(a);
        b = find_par(b);
        if (sz[a] < sz[b]) swap(a, b);

        history.push_back({disjoint_sets, disjoint_sets});
        history.push_back({par[b], par[b]});
        history.push_back({sz[a], sz[a]});

        if (a != b) {
            disjoint_sets--;
            par[b] = a;
            sz[a] += sz[b];
        }
    }

    int get_size(int u) {
        return sz[find_par(u)];
    }

    int comp_cnt() {
        return disjoint_sets;
    }

    int snapshot() {
        return history.size();
    }

    void rollback(int k) {
        while (k-- > 0 && history.size()) {
            for (int itr = 3; itr--;) {
                auto [i, v] = history.back();
                history.pop_back();
                i = v;
            }
        }
    }

    void rollback_to(int to) {
        rollback(((int)history.size() - to) / 3);
    }
};
