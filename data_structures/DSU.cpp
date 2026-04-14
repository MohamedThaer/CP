class DSU {
private:
    int disjoint_sets;
    vector<int> par, sz;

    int find_par(int node) {
        if (node == par[node]) {
            return node;
        }
        return par[node] = find_par(par[node]);
    }

public:
    DSU(int n) : disjoint_sets(n), par(n), sz(n, 1) {
        iota(par.begin(), par.end(), 0);
    }

    bool same_set(int a, int b) {
        return find_par(a) == find_par(b);
    }

    void merge(int a, int b) {
        a = find_par(a);
        b = find_par(b);
        if (sz[a] < sz[b]) swap(a, b);

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
};
