
auto euler_tour(vector<vector<int>> &g) {
    int n = (int)g.size() - 1;
    vector<int> flat, st(n + 1), en(n + 1);

    function<void(int, int)> euler = [&](int node, int par) {
        st[node] = flat.size();
        flat.push_back(node);
        for (auto &it : g[node]) {
            if (it != par) {
                euler(it, node);
            }
        }
        en[node] = flat.size() - 1;
    };
    euler(1, -1);
    return tuple{flat, st, en};
}
