
// 0-indexed
class HopcroftKarp {
private:
    static const int inf = 1E9;

    int n;
    vector<int> l, r, d;
    vector<vector<int>> g;

    bool bfs() {
        queue<int> q;
        for (int u = 1; u <= n; u++) {
            if (!l[u])
                d[u] = 0, q.push(u);
            else
                d[u] = inf;
        }
        d[0] = inf;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto v : g[u]) {
                if (d[r[v]] == inf) {
                    d[r[v]] = d[u] + 1;
                    q.push(r[v]);
                }
            }
        }
        return d[0] != inf;
    }

    bool dfs(int u) {
        if (!u) return true;
        for (auto v : g[u]) {
            if (d[r[v]] == d[u] + 1 && dfs(r[v])) {
                l[u] = v;
                r[v] = u;
                return true;
            }
        }
        d[u] = inf;
        return false;
    }

public:
    HopcroftKarp(int _n, int _m) {
        n = _n;
        int p = _n + _m + 1;
        g.resize(p);
        l.resize(p, 0);
        r.resize(p, 0);
        d.resize(p, 0);
    }

    void add_edge(int u, int v) {
        g[u + 1].push_back(v + 1 + n);
    }

    int max_matching() {
        int ans = 0;
        while (bfs()) {
            for (int u = 1; u <= n; u++)
                if (!l[u] && dfs(u)) ans++;
        }
        return ans;
    }

    vector<pair<int, int>> get_matching() {
        max_matching();
        vector<pair<int, int>> matches;
        for (int u = 1; u <= n; u++) {
            if (l[u] != 0) {
                matches.push_back({u - 1, l[u] - n - 1});
            }
        }
        return matches;
    }
};
