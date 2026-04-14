
struct Dinic {
    /*
        general:                                        O(V*V*E)
        cap = 1:                                        O(E*sqrt(E))
    */
    struct FlowEdge {
        int from, to;
        int64_t cap, flow = 0;

        FlowEdge(int from, int to, int64_t cap) : from(from), to(to), cap(cap) {}
    };

    const int64_t flow_inf = 1e18;
    vector<FlowEdge> edges;
    vector<vector<int>> adj;
    int n, m = 0;
    int s, t;
    vector<int> level, ptr;
    queue<int> q;

    Dinic(int n, int s, int t) : n(n), s(s), t(t) {
        adj.resize(n);
        level.resize(n);
        ptr.resize(n);
    }

    void add_edge(int from, int to, int64_t cap) {
        edges.emplace_back(from, to, cap);
        edges.emplace_back(to, from, 0);
        adj[from].push_back(m);
        adj[to].push_back(m + 1);
        m += 2;
    }

    bool bfs() {
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int id : adj[v]) {
                if (edges[id].cap - edges[id].flow < 1)
                    continue;
                if (level[edges[id].to] != -1)
                    continue;
                level[edges[id].to] = level[v] + 1;
                q.push(edges[id].to);
            }
        }
        return level[t] != -1;
    }

    int64_t dfs(int v, int64_t pushed) {
        if (pushed == 0)
            return 0;
        if (v == t)
            return pushed;
        for (int &cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
            int id = adj[v][cid];
            int u = edges[id].to;
            if (level[v] + 1 != level[u] || edges[id].cap - edges[id].flow < 1)
                continue;
            int64_t tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
            if (tr == 0)
                continue;
            edges[id].flow += tr;
            edges[id ^ 1].flow -= tr;
            return tr;
        }
        return 0;
    }
    int64_t flow() {
        int64_t f = 0;
        while (true) {
            fill(level.begin(), level.end(), -1);
            level[s] = 0;
            q.push(s);
            if (!bfs())
                break;
            fill(ptr.begin(), ptr.end(), 0);
            while (int64_t pushed = dfs(s, flow_inf)) {
                f += pushed;
            }
        }
        return f;
    }
    // get any set of edges to achieve the min cut (max flow)
    vector<array<int, 2>> getEdges() {
        vector<int> srcSide(n + 1);
        queue<int> q;
        q.push(s), srcSide[s] = 1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto &i : adj[u]) {
                auto &[_, v, f, c] = edges[i];
                if (!srcSide[v] && f != c) q.push(v), srcSide[v] = 1;
            }
        }
        vector<array<int, 2>> res;
        for (int i = 0; i < edges.size(); i += 2) {
            auto &[u, v, f, c] = edges[i];
            if (srcSide[u] && !srcSide[v]) res.push_back({u, v});
        }
        return res;
    }
};
