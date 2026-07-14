struct hash_path {
    LCA* lca;
    dr_string* ds;
    vector<vector<int64_t>> h_up, h_down;
    hash_path(LCA& lca_obj, const vector<int>& vals) {
        lca = &lca_obj;
        int n = lca->S,LOG = lca->LOG;
        ds = new dr_string(vector<int>(n, 0));

        h_up.assign(n, vector<int64_t>(LOG, 0));
        h_down.assign(n, vector<int64_t>(LOG, 0));

        for (int u = 0; u < n; u++) {
            h_up[u][0] = vals[u];
            h_down[u][0] = vals[u];
        }
        for (int i = 1; i < LOG; i++) {
            for (int u = 0; u < n; u++) {
                int p = lca->up[u][i - 1];
                // UP: Concatenate (u -> p) with (p -> p's ancestor)
                h_up[u][i] = ds->merge(h_up[u][i - 1], h_up[p][i - 1], 1 << (i - 1));
                // DOWN: Concatenate (p's ancestor -> p) with (p -> u)
                h_down[u][i] = ds->merge(h_down[p][i - 1], h_down[u][i - 1], 1 << (i - 1));
            }
        }
    }
    // start from u to u+d
    int64_t query_up(int u, int d) {
        if (d <= 0) return 0;
        int64_t ans = 0;
        for (int i = 0; i < lca->LOG; i++) {
            if ((d >> i) & 1) {
                ans = ds->merge(ans, h_up[u][i], 1 << i);
                u = lca->up[u][i];
            }
        }
        return ans;
    }

    // start from v-d to v
    int64_t query_down(int v, int d) {
        if (d <= 0) return 0;
        int64_t ans = 0;
        int len = 0;
        for (int i = 0; i < lca->LOG; i++) {
            if ((d >> i) & 1) {
                ans = ds->merge(h_down[v][i], ans, len);
                len += (1 << i);
                v = lca->up[v][i];
            }
        }
        return ans;
    }

    int64_t query_path(int u, int v) {
        int lca_node = lca->lca(u, v);
        int dist_u = lca->depth[u] - lca->depth[lca_node];
        int dist_v = lca->depth[v] - lca->depth[lca_node] + 1;

        int64_t left_hash = query_up(u, dist_u);
        int64_t right_hash = query_down(v, dist_v);

        if (dist_u == 0) return right_hash;
        if (dist_v == 0) return left_hash;

        return ds->merge(left_hash, right_hash, dist_v);
    }
};
