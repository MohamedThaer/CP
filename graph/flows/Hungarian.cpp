
/*
    Complexity: O(n^3), optimized
    0-indexed
*/
template <class M = int64_t>
class Hungarian {
public:
    enum Mode {
        Min = 1,
        Max = -1,
    };

private:
    vector<vector<M>> c;
    vector<M> fx, fy, d;
    vector<int> l, r, arg, trace;
    queue<int> q;
    int start, finish, n;
    const M inf = numeric_limits<M>::max() / 2;

    inline M getC(int u, int v) {
        return c[u][v] - fx[u] - fy[v];
    }

    void initBFS() {
        while (!q.empty())
            q.pop();
        q.push(start);
        for (int i = 0; i <= n; ++i)
            trace[i] = 0;
        for (int v = 1; v <= n; ++v) {
            d[v] = getC(start, v);
            arg[v] = start;
        }
        finish = 0;
    }

    void findAugPath() {
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v = 1; v <= n; ++v)
                if (!trace[v]) {
                    M w = getC(u, v);
                    if (!w) {
                        trace[v] = u;
                        if (!r[v]) {
                            finish = v;
                            return;
                        }
                        q.push(r[v]);
                    }
                    if (d[v] > w) {
                        d[v] = w;
                        arg[v] = u;
                    }
                }
        }
    }

    void subX_addY() {
        M delta = inf;
        for (int v = 1; v <= n; ++v)
            if (trace[v] == 0 && d[v] < delta) {
                delta = d[v];
            }
        // Rotate
        fx[start] += delta;
        for (int v = 1; v <= n; ++v)
            if (trace[v]) {
                int u = r[v];
                fy[v] -= delta;
                fx[u] += delta;
            }
            else
                d[v] -= delta;
        for (int v = 1; v <= n; ++v)
            if (!trace[v] && !d[v]) {
                trace[v] = arg[v];
                if (!r[v]) {
                    finish = v;
                    return;
                }
                q.push(r[v]);
            }
    }

    void Enlarge() {
        do {
            int u = trace[finish];
            int nxt = l[u];
            l[u] = finish;
            r[finish] = u;
            finish = nxt;
        } while (finish);
    }

    Mode mode;

public:
    Hungarian() {}
    Hungarian(int n1, int n2, Mode mode) : mode(mode) {
        n = (max(n1, n2));
        l = r = arg = trace = vector<int>(n + 1, 0);
        fx = fy = d = vector<M>(n + 1, 0);
        c = vector<vector<M>>(n + 1, vector<M>(n + 1, 0));

        for (int i = 1; i <= n; ++i) {
            fy[i] = l[i] = r[i] = 0;
            for (int j = 1; j <= n; ++j)
                c[i][j] = mode * inf; // make it 0 for maximum cost matching (not necessarily with max count of matching)
        }
    }

    void add_edge(int u, int v, M cost) {
        ++u, ++v;
        c[u][v] = min(c[u][v], mode * cost);
    }

    M BestMatching() {
        for (int u = 1; u <= n; ++u) {
            fx[u] = c[u][1];
            for (int v = 1; v <= n; ++v) {
                fx[u] = min(fx[u], c[u][v]);
            }
        }
        for (int v = 1; v <= n; ++v) {
            fy[v] = c[1][v] - fx[1];
            for (int u = 1; u <= n; ++u) {
                fy[v] = min(fy[v], c[u][v] - fx[u]);
            }
        }
        for (int u = 1; u <= n; ++u) {
            start = u;
            initBFS();
            while (!finish) {
                findAugPath();
                if (!finish) subX_addY();
            }
            Enlarge();
        }
        M ans = 0;
        for (int i = 1; i <= n; ++i) {
            if (c[i][l[i]] != inf)
                ans += c[i][l[i]];
            else
                l[i] = 0;
        }
        return mode * ans;
    }

    vector<pair<int, int>> get_matching() {
        vector<pair<int, int>> matching;
        for (int i = 1; i <= n; ++i) {
            if (l[i] != 0) {
                matching.push_back({i - 1, l[i] - 1});
            }
        }
        return matching;
    }
};
