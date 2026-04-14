class MCMF {
private:
    struct Edge {
        int to;       // destination node
        int64_t cost; // edge cost
        int cap, flow, rev;
    };

    int n, source, sink;
    vector<vector<Edge>> g;
    vector<int64_t> h;               // potentials (Johnson's reweighting)
    const int64_t INF = (1LL << 60); // safe INF

    // Dijkstra with reduced costs
    bool dijkstra(int s, int t, vector<int64_t> &d, vector<int> &p, vector<int> &pe) {
        fill(d.begin(), d.end(), INF);
        d[s] = 0;
        priority_queue<pair<int64_t, int>, vector<pair<int64_t, int>>, greater<>> pq;
        pq.push({0, s});
        while (!pq.empty()) {
            auto [dist, v] = pq.top();
            pq.pop();
            if (dist > d[v]) continue;

            for (int i = 0; i < (int)g[v].size(); i++) {
                auto &e = g[v][i];
                int64_t cost = e.cost + h[v] - h[e.to]; // reduced cost
                if (e.cap > e.flow && d[e.to] > d[v] + cost) {
                    d[e.to] = d[v] + cost;
                    p[e.to] = v;
                    pe[e.to] = i;
                    pq.push({d[e.to], e.to});
                }
            }
        }
        return d[t] != INF;
    }

    // Bellman-Ford for initial potentials if negative costs exist
    void calculatePotentials(int s) {
        fill(h.begin(), h.end(), INF);
        h[s] = 0;
        for (int iter = 0; iter < n - 1; iter++) {
            for (int v = 0; v < n; v++) {
                if (h[v] == INF) continue;
                for (auto &e : g[v]) {
                    if (e.cap > 0 && h[e.to] > h[v] + e.cost) {
                        h[e.to] = h[v] + e.cost;
                    }
                }
            }
        }
    }

    // Update potentials after each Dijkstra
    void updatePotentials(const vector<int64_t> &d) {
        for (int i = 0; i < n; i++)
            if (d[i] < INF) h[i] += d[i];
    }

public:
    MCMF(int n) : n(n), source(-1), sink(-1), g(n), h(n) {}

    // Add directed edge u -> v with given cost and capacity
    void addEdge(int u, int v, int64_t cost, int cap) {
        g[u].push_back({v, cost, cap, 0, (int)g[v].size()});
        g[v].push_back({u, -cost, 0, 0, (int)g[u].size() - 1}); // residual edge
    }

    pair<int64_t, int> MinCostMaxFlow(int s, int t, int maxFlow = 1e9, bool has_negative_costs = false) {
        source = s;
        sink = t;

        int64_t cost = 0;
        int flow = 0;
        vector<int64_t> d(n);
        vector<int> p(n), pe(n);

        if (has_negative_costs) calculatePotentials(s);

        while (flow < maxFlow) {
            if (!dijkstra(s, t, d, p, pe)) break;
            if (has_negative_costs) updatePotentials(d);

            int cur = t, add = maxFlow - flow;
            while (cur != s) {
                add = min(add, g[p[cur]][pe[cur]].cap - g[p[cur]][pe[cur]].flow);
                cur = p[cur];
            }

            cur = t;
            while (cur != s) {
                auto &e = g[p[cur]][pe[cur]];
                e.flow += add;
                g[cur][e.rev].flow -= add;
                cost += (int64_t)add * e.cost;
                cur = p[cur];
            }

            flow += add;
        }
        return {cost, flow};
    }

    // Return all edges with positive flow (ignores residuals)
    vector<tuple<int, int, int, int64_t>> getEdgesWithFlow() const {
        vector<tuple<int, int, int, int64_t>> result;
        for (int u = 0; u < n; u++) {
            for (auto &e : g[u]) {
                if (e.flow > 0 && e.cap > 0) {
                    result.push_back({u, e.to, e.flow, e.cost});
                }
            }
        }
        return result;
    }

    vector<vector<int>> get_paths() {
        vector<vector<int>> g(n);
        for (auto [u, v, f, c] : getEdgesWithFlow()) {
            g[u].push_back(v);
        }

        vector<vector<int>> ret;
        while (g[source].size()) {
            vector<int> v;
            int cur = source;
            while (cur != sink) {
                v.push_back(cur);
                int nxt = g[cur].back();
                g[cur].pop_back();
                cur = nxt;
            }
            v.push_back(sink);
            ret.push_back(v);
        }
        return ret;
    }

    // Return min-cut edges after max flow is computed
    vector<pair<int, int>> mincut(int s) const {
        vector<bool> vis(n, false);
        queue<int> q;
        q.push(s);
        vis[s] = true;

        // BFS on residual graph (forward edges with remaining capacity)
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &e : g[u]) {
                if (e.cap > e.flow && !vis[e.to]) {
                    vis[e.to] = true;
                    q.push(e.to);
                }
            }
        }

        vector<pair<int, int>> cutEdges;
        for (int u = 0; u < n; u++) {
            if (vis[u]) {
                for (auto &e : g[u]) {
                    if (!vis[e.to] && e.cap > 0) { // edge from reachable → non-reachable
                        cutEdges.push_back({u, e.to});
                    }
                }
            }
        }
        return cutEdges;
    }
};
