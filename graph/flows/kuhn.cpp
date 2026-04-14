class Kuhn {
private:
    int n, m;
    vector<vector<int>> adj;
    vector<int> match;
    vector<bool> used;

    bool try_kuhn(int v) {
        for (int to : adj[v]) {
            if (!used[to]) {
                used[to] = true;
                if (match[to] == -1 || try_kuhn(match[to])) {
                    match[to] = v;
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * @brief Finds all nodes reachable from any unmatched left node in the alternating graph.
     *
     * This is a common utility function required for both
     * min_vertex_cover and max_antichain.
     *
     * @return A boolean vector `vis` of size (n + m).
     * vis[0...n-1] is true if the corresponding left node is reachable.
     * vis[n...n+m-1] is true if the corresponding right node (v-n) is reachable.
     */
    vector<bool> get_reachable_nodes() {
        // We assume max_matching() has been called by the public function
        // that uses this helper, so 'match' is populated.

        // 1. Build the alternating graph (directed)
        vector<vector<int>> g(n + m);
        for (int u = 0; u < n; u++) {
            for (int v : adj[u]) {
                if (match[v] == u) {
                    // Matched edge (u, v): Add directed edge R_v -> L_u
                    g[n + v].push_back(u);
                }
                else {
                    // Unmatched edge (u, v): Add directed edge L_u -> R_v
                    g[u].push_back(n + v);
                }
            }
        }

        // 2. Find all unmatched left nodes
        vector<bool> left_is_matched(n, false);
        for (int v = 0; v < m; v++) {
            if (match[v] != -1) {
                // match[v] is the left node (0 to n-1) matched to v
                left_is_matched[match[v]] = true;
            }
        }

        // 3. Run BFS from all unmatched left nodes
        vector<bool> vis(n + m, false);
        queue<int> q;
        for (int u = 0; u < n; u++) {
            if (!left_is_matched[u]) {
                q.push(u);
                vis[u] = true; // Mark as visited
            }
        }

        while (!q.empty()) {
            int u_or_v = q.front();
            q.pop();

            // u_or_v is an index in the combined graph (0 to n+m-1)
            for (int neighbor : g[u_or_v]) {
                if (!vis[neighbor]) {
                    vis[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        return vis; // Z (set of reachable nodes)
    }

public:
    Kuhn(int nn, int mm) : n(nn), m(mm) {
        adj.resize(n);
        match.assign(m, -1);
        used.assign(m, 0);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    int max_matching() {
        fill(match.begin(), match.end(), -1);
        int mx = 0;
        for (int i = 0; i < n; i++) {
            fill(used.begin(), used.end(), false);
            if (try_kuhn(i)) mx++;
        }
        return mx;
    }

    vector<pair<int, int>> get_matching() {
        max_matching();
        vector<pair<int, int>> res;
        for (int v = 0; v < m; v++) {
            if (match[v] != -1) {
                res.emplace_back(match[v], v);
            }
        }
        return res;
    }

    // returns {L, R} for min vertex cover
    pair<vector<int>, vector<int>> min_vertex_cover() {
        max_matching();

        // Build alternating graph
        vector<vector<int>> g(n + m);
        for (int u = 0; u < n; u++) {
            for (int v : adj[u]) {
                if (match[v] == u) {
                    // matched edge -> right -> left
                    g[n + v].push_back(u);
                }
                else {
                    // unmatched edge -> left -> right
                    g[u].push_back(n + v);
                }
            }
        }

        // BFS from all unmatched left nodes
        vector<bool> vis(n + m, false);
        queue<int> q;
        for (int u = 0; u < n; u++) {
            bool matched = false;
            for (int v : adj[u]) {
                if (match[v] == u) {
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                q.push(u);
                vis[u] = true;
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) {
                if (!vis[v]) {
                    vis[v] = true;
                    q.push(v);
                }
            }
        }

        // Build cover: unvisited left + visited right
        vector<int> L, R;
        for (int u = 0; u < n; u++) {
            if (!vis[u]) L.push_back(u);
        }
        for (int v = 0; v < m; v++) {
            if (vis[n + v]) R.push_back(v);
        }

        return {L, R};
    }

    vector<int> get_dag_max_antichain() {
        // 1. Ensure max_matching() has been run and 'match' is populated
        max_matching();

        // 2. Find the set Z of reachable nodes
        vector<bool> vis = get_reachable_nodes(); // vis[0...n-1] is Z_L, vis[n...n+m-1] is Z_R

        // 3. Build the antichain set
        vector<int> antichain;
        for (int v = 0; v < n; v++) { // 'n' is the number of DAG nodes
            bool L_v_is_reachable = vis[v];
            bool R_v_is_reachable = vis[n + v];

            if (L_v_is_reachable && !R_v_is_reachable) {
                antichain.push_back(v);
            }
        }
        return antichain;
    }
};
