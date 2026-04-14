class LCA {
private:
    vector<vector<int>> anc;
    vector<int> flat, st, depth;
    sparse_table<int, function<int(int, int)>> sp;

public:
    LCA() {}
    LCA(const vector<vector<int>> &g, int root = 1) {
        int n = g.size();
        st = depth = vector<int>(n);
        anc.assign(__lg(n) + 1, vector<int>(n, -1));

        function<void(int, int, int)> dfs = [&](int node, int par, int d) {
            depth[node] = d;
            anc[0][node] = par;
            st[node] = flat.size();
            flat.push_back(node);

            for (auto to : g[node]) {
                if (to != par) {
                    dfs(to, node, d + 1);
                    flat.push_back(node);
                }
            }
        };
        dfs(root, -1, 0);

        for (int i = 1; i < anc.size(); ++i) {
            for (int j = 0; j < n; ++j) {
                if (~anc[i - 1][j]) {
                    anc[i][j] = anc[i - 1][anc[i - 1][j]];
                }
            }
        }

        sp = sparse_table<int, function<int(int, int)>>(flat, [&](int a, int b) {
            return depth[a] < depth[b] ? a : b;
        });
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

    int lca(int u, int v) { return sp.query(min(st[u], st[v]), max(st[u], st[v])); }
    int distance(int a, int b) { return depth[a] + depth[b] - 2 * depth[lca(a, b)]; }
};
