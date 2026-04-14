pair<bool, vector<int>> bipartite_graph(const vector<vector<int>> &g) {
    int n = g.size();
    vector<int> side(n, -1);
    bool ok = true; // stores if graph is bipartite or not

    function<void(int)> dfs = [&](int node) {
        for (auto to : g[node]) {
            if (side[to] == -1) {
                side[to] = side[node] ^ 1;
                dfs(to);
            }
            else if (side[node] == side[to]) {
                ok = false;
            }
        }
    };

    for (int i = 0; i < n; ++i) {
        if (side[i] == -1) {
            side[i] = 0;
            dfs(i);
        }
    }
    return {ok, side};
}
