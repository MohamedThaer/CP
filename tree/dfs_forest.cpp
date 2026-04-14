class dfs_forest {
private:
    vector<vector<int>> &g;
    void dfs(int node) {
        tree.push_back(node);
        st[node] = tree.size() - 1;
        sz[node] = 1;
        for (auto it : g[node]) {
            if (it != par[node]) {
                par[it] = node;
                depth[it] = depth[node] + 1;
                dfs(it);
                sz[node] += sz[it];
                if (heavy[node] == -1 || sz[it] > sz[heavy[node]]) {
                    heavy[node] = it;
                }
            }
        }
        en[node] = tree.size() - 1;
    }

public:
    dfs_forest(vector<vector<int>> &g, int root = -1) : g(g) {
        int n = g.size();
        st = en = sz = depth = vector<int>(n, 0);
        heavy = par = vector<int>(n, -1);
        if (~root) dfs(root);
        for (int i = 0; i < n; ++i) {
            if (!sz[i]) {
                dfs(i);
            }
        }
    }

    vector<int> tree, st, en, par, sz, depth, heavy;

    vector<int> get_path(int u, int v) {
        vector<int> a, b;
        while (depth[u] > depth[v]) {
            a.push_back(u), u = par[u];
        }
        while (depth[u] < depth[v]) {
            b.push_back(v), v = par[v];
        }
        while (u != v) {
            a.push_back(u), u = par[u];
            b.push_back(v), v = par[v];
        }
        a.push_back(u);
        a.insert(a.end(), b.rbegin(), b.rend());
        return a;
    }

    vector<int> get_subtree(int u) {
        return vector<int>(tree.begin() + st[u], tree.begin() + en[u] + 1);
    }
};
