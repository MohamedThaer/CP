template <int base = 1>
class ZeroOneBFS {
private:
    vector<int> par;

public:
    vector<int64_t> dis;
    const int64_t INF = 1e18;

    // g[node] = vector of {neighbor, edge_weight} where edge_weight belongs to {0, 1}
    ZeroOneBFS(vector<vector<pair<int, int>>> &g, vector<int> src = {base}) {
        par = vector<int>(g.size(), -1);
        dis = vector<int64_t>(g.size(), INF);
        deque<int> dq;

        for (auto it : src) {
            dis[it] = 0;
            dq.push_back(it);
        }

        while (!dq.empty()) {
            int node = dq.front();
            dq.pop_front();

            for (auto [to, w] : g[node]) {
                if (dis[to] > dis[node] + w) {
                    dis[to] = dis[node] + w;
                    par[to] = node;
                    if (w == 0)
                        dq.push_front(to);
                    else
                        dq.push_back(to);
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
        return vector<int>(ret.rbegin(), ret.rend());
    }
};
