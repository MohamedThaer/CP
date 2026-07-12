struct DSU {
    int disjoint_sets;
    vector<int> par, sz;

    DSU(int n) : disjoint_sets(n), par(n + 1), sz(n + 1, 1) {
        iota(par.begin(), par.end(), 0);
    }

    int find_par(int node) {
        if (node == par[node]) {
            return node;
        }
        return par[node] = find_par(par[node]);
    }

    bool same_set(int a, int b) {
        return find_par(a) == find_par(b);
    }

    bool merge(int a, int b) {
        a = find_par(a);
        b = find_par(b);
        if (a == b) return false;

        if (sz[a] < sz[b]) swap(a, b);
        
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

    void reset() {
        iota(par.begin(), par.end(), 0);
        fill(sz.begin(), sz.end(), 1);
        disjoint_sets = par.size() - 1; 
    }

    int get_max_size() {
        int max_sz = 0;
        for (int i = 1; i < par.size(); i++) {
            if (par[i] == i) { 
                max_sz = max(max_sz, sz[i]);
            }
        }
        return max_sz;
    }

    vector<int> get_roots() {
        vector<int> roots;
        for (int i = 1; i < par.size(); i++) {
            if (par[i] == i) {
                roots.push_back(i);
            }
        }
        return roots;
    }

    vector<vector<int>> get_components() {
        vector<vector<int>> comps(par.size());
        for (int i = 1; i < par.size(); i++) {
            comps[find_par(i)].push_back(i);
        }
        
        vector<vector<int>> result;
        for (int i = 1; i < par.size(); i++) {
            if (!comps[i].empty()) {
                result.push_back(move(comps[i]));
            }
        }
        return result;
    }
};
