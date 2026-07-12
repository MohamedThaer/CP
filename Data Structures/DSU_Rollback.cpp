struct RollbackDSU {
    int disjoint_sets;
    vector<int> par, sz;
    vector<pair<int&, int>> history;

    RollbackDSU(int n)
        : disjoint_sets(n), par(n + 1), sz(n + 1, 1) {
        iota(par.begin(), par.end(), 0);
    }

    int find_par(int node) {
        if (node == par[node]) return node;
        return find_par(par[node]);
    }

    bool same_set(int a, int b) {
        return find_par(a) == find_par(b);
    }

    bool merge(int a, int b) {
        a = find_par(a);
        b = find_par(b);

        if (sz[a] < sz[b]) swap(a, b);

        history.push_back({disjoint_sets, disjoint_sets});
        history.push_back({par[b], par[b]});
        history.push_back({sz[a], sz[a]});

        if (a == b) return false;

        disjoint_sets--;
        par[b] = a;
        sz[a] += sz[b];

        return true;
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

    void rollback(int merges = 1) {
        while (merges-- && !history.empty()) {
            for (int i = 0; i < 3; i++) {
                auto [ref, val] = history.back();
                history.pop_back();
                ref = val;
            }
        }
    }

    void rollback_to(int snap) {
        rollback((history.size() - snap) / 3);
    }

    void reset() {
        history.clear();
        iota(par.begin(), par.end(), 0);
        fill(sz.begin(), sz.end(), 1);
        disjoint_sets = par.size() - 1;
    }
};
