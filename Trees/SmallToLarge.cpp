struct SmallToLarge {
    int n;
    vector<vector<int>> g;
    vector<int> v, tree, in, sz, ans;
    int res = 0;

    SmallToLarge(int n, vector<vector<int>>& adj, vector<int>& values) 
        : n(n), g(adj), v(values), in(n + 1), sz(n + 1), ans(n + 1) {
        
        dfs_sz(1, 0);
        dfs_sack(1, 0, false);
    }

    void dfs_sz(int u, int p) {
        in[u] = tree.size();
        tree.push_back(u);
        sz[u] = 1;
        for (int child : g[u]) {
            if (child != p) {
                dfs_sz(child, u);
                sz[u] += sz[child];
            }
        }
    }

    void add(int u) {
        // e.g., freq[v[u]]++;
    }

    void remove(int u) {
        // e.g., freq[v[u]]--;
    }

    void dfs_sack(int u, int p, bool keep) {
        int heavy = -1;
        for (int v : g[u]) {
            if (v != p && (heavy == -1 || sz[v] > sz[heavy])) {
                heavy = v;
            }
        }

        for (int v : g[u]) {
            if (v != p && v != heavy) {
                dfs_sack(v, u, false);
            }
        }

        if (heavy != -1)
            dfs_sack(heavy, u, true);

        for (int v : g[u]) {
            if (v != p && v != heavy) {
                for (int i = 0; i < sz[v]; ++i) {
                    add(tree[in[v] + i]);
                }
            }
        }

        // Add node itself before calc
        add(u);
        // calc
        ans[u] = res;

        if (!keep) {
            for (int i = 0; i < sz[u]; ++i) {
                remove(tree[in[u] + i]);
            }
        }
    }
};

void dfs(int u, int p) {
    cnt[u][v[u]]++;
    for (int child : g[u]) {
        if (child == p) continue;
        dfs(child, u);

        if (cnt[child].size() > cnt[u].size())
            swap(cnt[u], cnt[child]);

        for (auto & [val, freq] : cnt[child])
            cnt[u][val] += freq;

        cnt[child].clear();
    }
    ans[u] = cnt[u].size();
}
