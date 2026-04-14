
template <int base = 1>
class dijkstra {
private:
    vector<int> par;

public:
    const int64_t INF = 1e18;
    vector<int64_t> dis;

    dijkstra(vector<vector<pair<int, int>>> &g, vector<int> src = {base}) {
        par = vector<int>(g.size(), -1);
        dis = vector<int64_t>(g.size(), INF);
        priority_queue<pair<int64_t, int>, vector<pair<int64_t, int>>, greater<>> pq;
        for (auto it : src) {
            dis[it] = 0;
            pq.push({0, it});
        }

        while (pq.size()) {
            auto [cost, node] = pq.top();
            pq.pop();
            if (dis[node] < cost) continue;

            for (auto [next, c] : g[node]) {
                if (dis[node] + c < dis[next]) {
                    par[next] = node;
                    dis[next] = dis[node] + c;
                    pq.push({dis[next], next});
                }
            }
        }
    }

    vector<int> get_path(int node) {
        if (dis[node] == INF) return {};
        vector<int> ret;
        for (int cur = node; ~cur; cur = par[cur]) {
            ret.push_back(cur);
        }
        return vector(ret.rbegin(), ret.rend());
    }
};
