
class max_flow {
private:
    struct edge {
        int to;
        int64_t cap;
        edge(int to, int64_t cap) : to(to), cap(cap) {}
    };

    struct Edge {
        int from, to;
        int64_t cap, flow;
    };

    bool bfs(int s, int t) {
        h.assign(n, -1);
        queue<int> que;
        h[s] = 0;
        que.push(s);
        while (!que.empty()) {
            const int u = que.front();
            que.pop();
            for (int i : g[u]) {
                auto [v, c] = e[i];
                if (c > 0 && h[v] == -1) {
                    h[v] = h[u] + 1;
                    if (v == t) {
                        return true;
                    }
                    que.push(v);
                }
            }
        }
        return false;
    }

    int64_t dfs(int u, int t, int64_t f) {
        if (u == t) {
            return f;
        }
        auto r = f;
        for (int &i = cur[u]; i < int(g[u].size()); ++i) {
            const int j = g[u][i];
            auto [v, c] = e[j];
            if (c > 0 && h[v] == h[u] + 1) {
                auto a = dfs(v, t, min(r, c));
                e[j].cap -= a;
                e[j ^ 1].cap += a;
                r -= a;
                if (r == 0) {
                    return f;
                }
            }
        }
        return f - r;
    }

    int n;
    vector<edge> e;
    vector<vector<int>> g;
    vector<int> cur, h;

public:
    max_flow(int n) : n(n), g(n), h(n) {}

    void add_edge(int u, int v, int64_t c) {
        g[u].push_back(e.size());
        e.emplace_back(v, c);
        g[v].push_back(e.size());
        e.emplace_back(u, 0);
    }

    int64_t flow(int s, int t) {
        for (int i = 0; i < e.size(); i += 2) {
            e[i].cap += e[i + 1].cap;
            e[i + 1].cap = 0;
        }
        int64_t ans = 0;
        while (bfs(s, t)) {
            cur.assign(n, 0);
            ans += dfs(s, t, numeric_limits<int64_t>::max());
        }
        return ans;
    }

    vector<bool> min_cut() {
        vector<bool> c(n);
        for (int i = 0; i < n; i++) {
            c[i] = (h[i] != -1);
        }
        return c;
    }

    vector<Edge> edges() {
        vector<Edge> a;
        for (int i = 0; i < e.size(); i += 2) {
            Edge x;
            x.from = e[i + 1].to;
            x.to = e[i].to;
            x.cap = e[i].cap + e[i + 1].cap;
            x.flow = e[i + 1].cap;
            a.push_back(x);
        }
        return a;
    }

    vector<Edge> mincut_edges() {
        vector<bool> cut = min_cut();

        vector<Edge> res;
        for (auto [u, v, c, f] : edges()) {
            if (cut[u] && !cut[v] && c > 0) {
                res.push_back({u, v, c, f});
            }
        }
        return res;
    }

    vector<vector<int>> get_paths(int s, int t) {
        flow(s, t);
        vector<vector<int>> g(n);
        for (auto [u, v, c, f] : edges()) {
            if (f) g[u].push_back(v);
        }

        vector<vector<int>> ret;
        while (g[s].size()) {
            vector<int> v;
            int cur = s;
            while (cur != t) {
                v.push_back(cur);
                int nxt = g[cur].back();
                g[cur].pop_back();
                cur = nxt;
            }
            v.push_back(t);
            ret.push_back(v);
        }
        return ret;
    }
};
