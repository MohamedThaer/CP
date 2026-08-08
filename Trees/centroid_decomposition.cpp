struct centroid_decomposition {
    int n;
    vector<int> sz, par, vis;
    vector<vector<int> > adj;

    centroid_decomposition(int n) {
        this->n = n;
        adj.resize(n + 1);
        sz = vis = par = vector<int>(n + 1);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int get_subtree_size(int u, int p) {
        sz[u] = 1;
        for (int &v: adj[u]) {
            if (v == p || vis[v])
                continue;
            sz[u] += get_subtree_size(v, u);
        }
        return sz[u];
    }

    int get_centroid(int u, int p, int tree_size) {
        for (int &v: adj[u]) {
            if (v == p || vis[v])
                continue;
            if (sz[v] * 2 > tree_size)
                return get_centroid(v, u, tree_size);
        }
        return u;
    }

    void collect(int u, int p, int64_t mx, int64_t mn, vector<pair<int64_t, int64_t> > &path) {
        // base case
        // add in the path
        mx = max(mx, val[u]);
        mn = min(mn, val[u]);
        path.push_back({mx, mn});
        for (int &v: adj[u]) {
            if (v != p && !vis[v])
                collect(v, u, mx, mn, path);
        }
    }

    int64_t count(vector<pair<int64_t, int64_t> > &path) {
        if (path.empty()) return 0;
        sort(path.begin(), path.end());
        // solve
        vector < int64_t > cc;
        for (auto &[mx, mn]: path) cc.push_back(mn);
        sort(cc.begin(), cc.end());
        cc.erase(unique(cc.begin(), cc.end()), cc.end());
        auto get = [&](int64_t x) {
            return (int) (lower_bound(cc.begin(), cc.end(), x) - cc.begin()) + 1;
        };
        BIT bit(cc.size() + 1);
        int64_t c = 0;
        for (auto &[mx, mn]: path) {
            int64_t mnn = (mx + k - 1) / k;
            int idx = get(mnn);
            if (mn >= mnn && idx <= cc.size())
                c += bit.query(idx, (int) cc.size());
            bit.add(get(mn), 1);
        }
        return c;
    }

    void decompose(int u, int p) {
        int tree_size = get_subtree_size(u, p);
        int cent = get_centroid(u, p, tree_size);
        vis[cent] = true;
        par[cent] = p;
        // Do
        vector<pair<int64_t, int64_t> > path;
        for (int &v: adj[cent]) {
            if (vis[v])
                continue;
            vector<pair<int64_t, int64_t> > sub_path;
            collect(v, cent, val[cent], val[cent], sub_path);
            ans -= count(sub_path);
            for (auto &p: sub_path)path.push_back(p);
        }
        ans += count(path);
        // path pass with cent
        for (auto &[mx, mn]: path)if (mx <= k * mn) ans++;


        for (int &v: adj[cent]) {
            if (vis[v])
                continue;
            decompose(v, cent);
        }
    }
};
