auto MST(auto &g) {
    int n = (int)g.size() - 1;
    vector<bool> added(n + 1, false);
    vector<vector<pair<int, int64_t>>> mst(n + 1);
    priority_queue<tuple<int64_t, int, int>> pq;

    auto add = [&](int node) {
        for (auto &[next, cost] : g[node]) {
            if (!added[next]) {
                pq.push({-cost, node, next});
            }
        }
        added[node] = true;
    };

    add(1);
    for (int i = 1; i < n; ++i) {
        while (added[get<2>(pq.top())]) {
            pq.pop();
        }
        auto [cost, from, to] = pq.top();
        pq.pop();
        mst[from].push_back({to, -cost});
        add(to);
    }
    return mst;
}
