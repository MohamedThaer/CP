template <class M = int>
class LCA {
private:
    int n;
    vector<vector<int>> anc;
    vector<int> flat, st, depth;
    sparse_table<int, function<int(int, int)>> sp;

public:
    LCA() {}
    LCA(const vector<vector<int>> &g, int root = 1) {
        n = g.size();
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
    int first_on_path(int u, int v) { return u == lca(u, v) ? kth_anc(v, distance(u, v) - 1) : kth_anc(u, 1); }

    vector<vector<M>> ans_up, ans_down;
    M merge(M a, M b) { return a + b; }

    void build(const vector<M> &v) {
        ans_up = ans_down = vector<vector<M>>(__lg(n) + 1, v);

        for (int i = 1; i < anc.size(); ++i) {
            for (int j = 0; j < n; ++j) {
                if (~anc[i][j]) {
                    ans_up[i][j] = merge(ans_up[i - 1][j], ans_up[i - 1][anc[i - 1][j]]);
                    ans_down[i][j] = merge(ans_down[i - 1][anc[i - 1][j]], ans_down[i - 1][j]);
                }
            }
        }
    }

    M straight_line_query(int u, int v) {
        int k = distance(u, v);
        if (lca(u, v) == u) {
            int node = v;
            M ret;
            while (k) {
                int lsb = k & -k;
                ret = (node == v ? ans_down[__lg(lsb)][node] : merge(ans_down[__lg(lsb)][node], ret));
                node = anc[__lg(lsb)][node];
                k -= lsb;
            }
            return ret;
        }
        else {
            int node = u;
            M ret;
            while (k) {
                int lsb = k & -k;
                ret = (node == u ? ans_up[__lg(lsb)][node] : merge(ret, ans_up[__lg(lsb)][node]));
                node = anc[__lg(lsb)][node];
                k -= lsb;
            }
            return ret;
        }
    }

    M query(int u, int v) {
        int lc = lca(u, v);
        if (u == lc or v == lc) return straight_line_query(u, v);
        return merge(straight_line_query(u, lc), straight_line_query(first_on_path(lc, v), v));
    }
};
