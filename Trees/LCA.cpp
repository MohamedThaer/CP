struct LCA {
    int S, LOG;
    vector<vector<int>> adj, up;
    vector<int> depth;

    LCA(vector<vector<int>> &g, int root = 0) {
        adj = g;
        S = adj.size();
        LOG = 32 - __builtin_clz(S);

        up.assign(S, vector<int>(LOG));
        depth.assign(S, 0);

        dfs(root, root);
    }

    void dfs(int u, int p) {
        up[u][0] = p;

        for (int i = 1; i < LOG; i++)
            up[u][i] = up[up[u][i - 1]][i - 1];

        for (int v : adj[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }

    int kth_ancestor(int u, int k) {
        for (int i = 0; i < LOG; i++)
            if (k & (1 << i))
                u = up[u][i];
        return u;
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v])
            swap(u, v);

        u = kth_ancestor(u, depth[u] - depth[v]);

        if (u == v)
            return u;

        for (int i = LOG - 1; i >= 0; i--) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }

        return up[u][0];
    }

    int dist(int u, int v) {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }
};
