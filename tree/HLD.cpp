template <class node, bool val_on_edge = false, bool reverse = false>
class HLD {
private:
    int n, cur_pos = 0;
    const vector<vector<int>> &g;
    segment_tree<node> forward, backward;
    vector<int> par, sz, depth, pos, top;

    vector<pair<int, int>> get_path(int l, int r) {
        vector<pair<int, int>> a, b;
        while (top[l] != top[r]) {
            if (depth[top[r]] > depth[top[l]]) {
                b.push_back({pos[top[r]], pos[r]});
                r = par[top[r]];
            }
            else {
                a.push_back({pos[l], pos[top[l]]});
                l = par[top[l]];
            }
        }

        if (!val_on_edge) {
            a.push_back({pos[l], pos[r]});
        }
        else if (l != r) {
            a.push_back(pos[l] > pos[r] ? pair{pos[l], pos[r] + 1} : pair{pos[l] + 1, pos[r]});
        }

        a.insert(a.end(), b.rbegin(), b.rend());
        return a;
    }

    void dfs1(int nd, int pr, int dep) {
        par[nd] = pr;
        sz[nd] = 1;
        depth[nd] = dep;
        for (auto it : g[nd]) {
            if (it != pr) {
                dfs1(it, nd, dep + 1);
                sz[nd] += sz[it];
            }
        }
    }

    void dfs2(int nd, bool is_top) {
        pos[nd] = cur_pos++;
        top[nd] = (is_top ? nd : top[par[nd]]);

        int heavy = -1;
        for (auto it : g[nd]) {
            if (it != par[nd] && (heavy == -1 || sz[heavy] < sz[it])) {
                heavy = it;
            }
        }
        if (heavy != -1) {
            dfs2(heavy, false);
        }
        for (auto it : g[nd]) {
            if (it != par[nd] && it != heavy) {
                dfs2(it, true);
            }
        }
    }

public:
    HLD(const vector<vector<int>> &_g, int root = -1) : g(_g), n(_g.size()), forward(n), backward(reverse ? n : 1) {
        par = sz = depth = pos = top = vector<int>(n);
        if (~root) dfs1(root, -1, 0), dfs2(root, true);
        for (int i = 0; i < n; ++i) {
            if (!sz[i]) {
                dfs1(i, -1, 0);
                dfs2(i, true);
            }
        }
    }

    template <class... M>
    void update_subtree(int v, const M &...value) {
        int L = pos[v] + (val_on_edge), R = pos[v] + sz[v] - 1;
        if (L <= R) {
            forward.update(L, R, value...);
            if (reverse) backward.update(n - R - 1, n - L - 1, value...);
        }
    }

    template <class... M>
    void update_path(int u, int v, const M &...value) {
        for (auto [L, R] : get_path(u, v)) {
            if (L > R) swap(L, R);
            forward.update(L, R, value...);
            if (reverse) backward.update(n - R - 1, n - L - 1, value...);
        }
    }

    node query_subtree(int v) {
        if (val_on_edge && sz[v] == 1) return node();
        return forward.query(pos[v] + (val_on_edge), pos[v] + sz[v] - 1);
    }

    node query_path(int u, int v) {
        node ret, mrg, combiner;
        int i = 0;
        for (auto [L, R] : get_path(u, v)) {
            mrg = (L <= R or !reverse ? forward.query(min(L, R), max(L, R)) : backward.query(n - L - 1, n - R - 1));
            i++ == 0 ? void(combiner = mrg) : combiner.merge(ret, mrg), ret = combiner;
        }
        return ret;
    }
};
