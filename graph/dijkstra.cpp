template <class T = int64_t, int base = 1>
vector<T> dijkstra(vector<vector<pair<int, T>>> &g, const vector<int> &src = {base}) {
    const T INF = numeric_limits<T>::max();
    vector<T> dis(g.size(), INF);

    set<pair<T, int>> s;
    for (auto it : src) {
        s.insert({0, it});
    }

    while (s.size()) {
        auto [cost, node] = *s.begin();
        s.erase(s.begin());
        if (dis[node] != INF) continue;
        dis[node] = cost;

        for (auto [next, c] : g[node]) {
            if (dis[next] == INF) {
                s.insert({cost + c, next});
            }
        }
    }

    return dis;
}
