struct hash_subtree {
    vector<int> in, out, order;
    dr_string ds;
    int timer;

    hash_subtree(int n, int root, const vector<vector<int>>& adj, const vector<int>& vals) {
        in.resize(n);
        out.resize(n);
        order.resize(n);
        timer = 0;

        function<void(int, int)> dfs = [&](int u, int p) {
            in[u] = timer;
            order[timer++] = vals[u];
            for (int v : adj[u]) {
                if (v != p) dfs(v, u);
            }
            out[u] = timer - 1;
        };
        
        dfs(root, -1);
        ds = dr_string(order);
    }

  
    int64_t query(int u) {
        return ds.substr(in[u], out[u]);
    }
};
