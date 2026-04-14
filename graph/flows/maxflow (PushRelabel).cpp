


/**
 * Description: Push-relabel using the highest label selection rule and the gap heuristic. Quite fast in practice.
 * To obtain the actual flow, look at positive values only.
 * Time: \$O(V^2\sqrt E)\$
 */
class PushRelabel {
private:
    struct Edge {
        int dest, back;
        int64_t f, c;
    };
    vector<vector<Edge>> g;
    vector<int64_t> ec;
    vector<Edge *> cur;
    vector<vector<int>> hs;
    vector<int> H;

    void addFlow(Edge &e, int64_t f) {
        Edge &back = g[e.dest][e.back];
        if (!ec[e.dest] && f) hs[H[e.dest]].push_back(e.dest);
        e.f += f;
        e.c -= f;
        ec[e.dest] += f;
        back.f -= f;
        back.c += f;
        ec[back.dest] -= f;
    }

public:
    PushRelabel(int n) : g(n), ec(n), cur(n), hs(2 * n), H(n) {}

    void addEdge(int s, int t, int64_t cap, int64_t rcap = 0) {
        if (s == t) return;
        g[s].push_back({t, (int)g[t].size(), 0, cap});
        g[t].push_back({s, (int)g[s].size() - 1, 0, rcap});
    }

    int64_t maxflow(int s, int t) {
        int v = g.size();
        H[s] = v;
        ec[t] = 1;
        vector<int> co(2 * v);
        co[0] = v - 1;
        for (int i = 0; i < v; ++i) {
            cur[i] = g[i].data();
        }
        for (Edge &e : g[s]) {
            addFlow(e, e.c);
        }

        for (int hi = 0;;) {
            while (hs[hi].empty()) {
                if (!hi--) return -ec[s];
            }
            int u = hs[hi].back();
            hs[hi].pop_back();
            while (ec[u] > 0) // discharge u
                if (cur[u] == g[u].data() + g[u].size()) {
                    H[u] = 1e9;
                    for (Edge &e : g[u]) {
                        if (e.c && H[u] > H[e.dest] + 1) {
                            H[u] = H[e.dest] + 1, cur[u] = &e;
                        }
                    }
                    if (++co[H[u]], !--co[hi] && hi < v) {
                        for (int i = 0; i < v; ++i) {
                            if (hi < H[i] && H[i] < v) {
                                --co[H[i]], H[i] = v + 1;
                            }
                        }
                    }
                    hi = H[u];
                }
                else if (cur[u]->c && H[u] == H[cur[u]->dest] + 1) {
                    addFlow(*cur[u], min(ec[u], cur[u]->c));
                }
                else {
                    ++cur[u];
                }
        }
    }

    bool leftOfMinCut(int a) { return H[a] >= g.size(); }
};
