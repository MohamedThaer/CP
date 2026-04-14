

class EdmondsKarp { // O(V*E*E)
private:
    int n;
    const int64_t INF = 1e18;
    vector<vector<int64_t>> res; // residual capacities
    vector<vector<int>> adj;     // adjacency list

    int64_t bfs(int s, int t, vector<int> &parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        queue<pair<int, int64_t>> q;
        q.push({s, INF});

        while (!q.empty()) {
            auto [cur, flow] = q.front();
            q.pop();
            for (int next : adj[cur]) {
                if (parent[next] == -1 && res[cur][next] > 0) {
                    parent[next] = cur;
                    int64_t new_flow = min(flow, res[cur][next]);
                    if (next == t) return new_flow;
                    q.push({next, new_flow});
                }
            }
        }
        return 0;
    }

public:
    EdmondsKarp(int n) : n(n) {
        res.assign(n, vector<int64_t>(n, 0));
        adj.assign(n, {});
    }

    void add_edge(int u, int v, int64_t cap) {
        if (u == v) return; // no self loops
        res[u][v] += cap;   // allow multiple edges
        adj[u].push_back(v);
        adj[v].push_back(u); // reverse edge for residual graph
    }

    int64_t maxflow(int s, int t) {
        int64_t flow = 0, new_flow;
        vector<int> parent(n);
        while ((new_flow = bfs(s, t, parent))) {
            flow += new_flow;
            int cur = t;
            while (cur != s) {
                int prev = parent[cur];
                res[prev][cur] -= new_flow;
                res[cur][prev] += new_flow;
                cur = prev;
            }
        }
        return flow;
    }
};
