vector<int> vis(n + 1);
function<void(int)> dfs = [&](int node) {
    if (vis[node]) {
        return;
    }
    vis[node] = true;
    for (auto &it : g[node]) {
        dfs(it);
    }
};
