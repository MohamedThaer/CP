

template <int base = 1>
class BFS {
private:
    vector<int> par;

public:
    vector<int64_t> dis;
    const int64_t INF = 1e18;

    BFS(vector<vector<int>> &g, vector<int> src = {base}) {
        par = vector<int>(g.size(), -1);
        dis = vector<int64_t>(g.size(), INF);
        queue<int> q;
        for (auto it : src) {
            dis[it] = 0;
            q.push(it);
        }
        while (q.size()) {
            int node = q.front();
            q.pop();
            for (auto it : g[node]) {
                if (dis[it] == INF) {
                    par[it] = node;
                    dis[it] = dis[node] + 1;
                    q.push(it);
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
