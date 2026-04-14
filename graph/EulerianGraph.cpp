class EulerianGraph {
private:
    int n;
    struct Edge {
        int to;
        int rev_idx;
        bool used;
    };
    vector<vector<Edge>> adj;

    vector<int> in_degree, out_degree;
    vector<int> ptr;

    bool directed;

    void dfs(int u, vector<bool> &visited) {
        visited[u] = true;
        for (auto &e : adj[u]) {
            if (!visited[e.to]) {
                dfs(e.to, visited);
            }
        }
    }

    void dfsUndirected(int u, const vector<vector<Edge>> &graph, vector<bool> &visited) {
        visited[u] = true;
        for (auto &e : graph[u]) {
            if (!visited[e.to]) {
                dfsUndirected(e.to, graph, visited);
            }
        }
    }

    bool isConnected() {
        if (directed) {
            vector<vector<Edge>> undirected_adj(n);
            for (int u = 0; u < n; u++) {
                for (auto &e : adj[u]) {
                    int v = e.to;
                    undirected_adj[u].push_back({v, (int)undirected_adj[v].size(), false});
                    undirected_adj[v].push_back({u, (int)undirected_adj[u].size() - 1, false});
                }
            }

            vector<bool> visited(n, false);
            int start = -1;
            for (int i = 0; i < n; i++) {
                if (!undirected_adj[i].empty()) {
                    start = i;
                    break;
                }
            }
            if (start == -1) return true;

            dfsUndirected(start, undirected_adj, visited);

            for (int i = 0; i < n; i++) {
                if (!undirected_adj[i].empty() && !visited[i]) {
                    return false;
                }
            }
            return true;
        }
        else {
            vector<bool> visited(n, false);
            int start = -1;
            for (int i = 0; i < n; i++) {
                if (!adj[i].empty()) {
                    start = i;
                    break;
                }
            }
            if (start == -1) return true;

            dfs(start, visited);

            for (int i = 0; i < n; i++) {
                if (!adj[i].empty() && !visited[i]) {
                    return false;
                }
            }
            return true;
        }
    }

    void resetState() {
        ptr.assign(n, 0);
        for (int i = 0; i < n; ++i) {
            for (auto &e : adj[i]) {
                e.used = false;
            }
        }
    }

public:
    EulerianGraph(int num_vertices, bool is_directed = false)
        : n(num_vertices), directed(is_directed) {
        adj.resize(n);
        in_degree.resize(n, 0);
        out_degree.resize(n, 0);
        ptr.resize(n, 0);
    }

    void addEdge(int u, int v) {
        if (directed) {
            adj[u].push_back({v, -1, false});
        }
        else {
            adj[u].push_back({v, (int)adj[v].size(), false});
            adj[v].push_back({u, (int)adj[u].size() - 1, false});
        }

        out_degree[u]++;
        in_degree[v]++;
        if (!directed) {
            out_degree[v]++;
            in_degree[u]++;
        }
    }

    bool hasEulerianCycle() {
        if (!isConnected()) return false;
        if (directed) {
            for (int i = 0; i < n; i++) {
                if (in_degree[i] != out_degree[i]) return false;
            }
        }
        else {
            for (int i = 0; i < n; i++) {
                if (out_degree[i] % 2 != 0) return false;
            }
        }
        return true;
    }

    bool hasEulerianPath() {
        if (!isConnected()) return false;
        if (directed) {
            int start = 0, end = 0;
            for (int i = 0; i < n; i++) {
                if (out_degree[i] - in_degree[i] == 1)
                    start++;
                else if (in_degree[i] - out_degree[i] == 1)
                    end++;
                else if (in_degree[i] != out_degree[i])
                    return false;
            }
            return (start == 0 && end == 0) || (start == 1 && end == 1);
        }
        else {
            int odd_count = 0;
            for (int i = 0; i < n; i++) {
                if (out_degree[i] % 2 != 0) odd_count++;
            }
            return odd_count == 0 || odd_count == 2;
        }
    }

    // Find Eulerian cycle using Hierholzer's algorithm (O(V+E))
    vector<int> findEulerianCycle() {
        resetState();

        vector<int> circuit;
        vector<int> stack;
        int start = 0;
        for (int i = 0; i < n; i++) {
            if (!adj[i].empty()) {
                start = i;
                break;
            }
        }

        stack.push_back(start);

        while (!stack.empty()) {
            int u = stack.back();

            while (ptr[u] < adj[u].size() && adj[u][ptr[u]].used) {
                ptr[u]++;
            }

            if (ptr[u] == adj[u].size()) {
                circuit.push_back(u);
                stack.pop_back();
            }
            else {
                Edge &e = adj[u][ptr[u]];

                e.used = true;
                if (!directed) {
                    adj[e.to][e.rev_idx].used = true;
                }

                stack.push_back(e.to);
            }
        }

        reverse(circuit.begin(), circuit.end());
        return circuit;
    }

    // Find Eulerian path using Hierholzer's algorithm (O(V+E))
    vector<int> findEulerianPath() {
        resetState();

        vector<int> path;
        vector<int> stack;

        int start = -1;
        if (directed) {
            for (int i = 0; i < n; i++) {
                if (out_degree[i] - in_degree[i] == 1) {
                    start = i;
                    break;
                }
            }
        }
        else {
            for (int i = 0; i < n; i++) {
                if (out_degree[i] % 2 != 0) {
                    start = i;
                    break;
                }
            }
        }

        if (start == -1) {
            for (int i = 0; i < n; i++) {
                if (out_degree[i] > 0) {
                    start = i;
                    break;
                }
            }
        }
        if (start == -1) {
            if (n > 0) return {0};
            return {};
        }

        stack.push_back(start);

        while (!stack.empty()) {
            int u = stack.back();
            while (ptr[u] < adj[u].size() && adj[u][ptr[u]].used) {
                ptr[u]++;
            }

            if (ptr[u] == adj[u].size()) {
                path.push_back(u);
                stack.pop_back();
            }
            else {
                Edge &e = adj[u][ptr[u]];

                e.used = true;
                if (!directed) {
                    adj[e.to][e.rev_idx].used = true;
                }

                stack.push_back(e.to);
            }
        }

        reverse(path.begin(), path.end());
        return path;
    }
};
