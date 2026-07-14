struct LCA {
    vector<vector<int>> anc, dp ,&g;
    vector<int> in, depth, euler;

    void dfs(int node, int par, int d) {
        in[node] = euler.size();
        depth[node] = d;
        anc[0][node] = par;
        euler.push_back(node);

        for (int to : g[node]) {
            if (to != par) {
                dfs( to, node, d + 1);
                euler.push_back(node); 
            }
        }
    }

    void build_rmq() {
        int m = euler.size(),k = __lg(m) + 1;
        dp.assign(k, vector<int>(m));

        for (int i = 0; i < m; i++) {
            dp[0][i] = euler[i];
        }
        for (int i = 1; i < k; i++) {
            for (int j = 0; j + (1 << i) <= m; j++) {
                int l = dp[i - 1][j];
                int r = dp[i - 1][j + (1 << (i - 1))];
                dp[i][j] = (depth[l] < depth[r]) ? l : r;
            }
        }
    }

    LCA(vector<vector<int>> &_g, int root = 1) : g(_g) {
        int n = g.size();
        in.assign(n, -1);
        depth.assign(n, 0);
        anc.assign(__lg(n) + 1, vector<int>(n, -1));
        euler.reserve(2 * n);
        dfs(root, -1, 0);
        build_rmq();

        for (int i = 1; i < anc.size(); ++i) {
            for (int j = 0; j < n; ++j) {
                if (anc[i - 1][j] != -1) {
                    anc[i][j] = anc[i - 1][anc[i - 1][j]];
                }
            }
        }
    }

    int kth_anc(int node, int k) {
        if (depth[node] < k) return -1;
        while (k) {
            int lsb = k & -k;
            node = anc[__lg(lsb)][node];
            k -= lsb;
        }
        return node;
    }

    int lca(int u, int v) {
        int l = in[u], r = in[v];
        if (l > r) swap(l, r);
        int i = __lg(r - l + 1);
        int L = dp[i][l];
        int R = dp[i][r - (1 << i) + 1];
        return depth[L] < depth[R] ? L : R;
    }

    int distance(int a, int b) { 
        return depth[a] + depth[b] - 2 * depth[lca(a, b)]; 
    }
};
