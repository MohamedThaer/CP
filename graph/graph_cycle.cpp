template <bool directed = true, bool edge_disjoint = true>
class GraphCycle {
private:
    int id = 0;
    vector<int> vis, par, e_id;
    vector<array<int, 2>> edges;
    vector<int> cycle_e, cycle_v; // edge ids, vertices
    vector<vector<pair<int, int>>> g;
    map<pair<int, int>, vector<int>> mp;

    bool dfs(int u) {
        if (!cycle_e.empty()) return 1;
        vis[u] = 1;
        for (auto [v, id] : g[u]) {
            if ((!edge_disjoint && directed) || v != par[u]) {
                if (vis[v] == 0) {
                    par[v] = u;
                    e_id[v] = id;
                    if (dfs(v)) return 1;
                }
                else if (vis[v] == 1) {
                    cycle_e.push_back(id);
                    cycle_v.push_back(v);
                    for (int x = u; x != v; x = par[x]) {
                        cycle_v.push_back(x);
                        cycle_e.push_back(e_id[x]);
                    }
                    cycle_v.push_back(v);
                    return 1;
                }
            }
        }
        vis[u] = 2;
        return 0;
    }

public:
    GraphCycle() {}
    GraphCycle(int n) : vis(n), par(n, -1), e_id(n), g(n) {}

    void add_edge(int u, int v) {
        g[u].push_back({v, id});
        if (!directed) {
            g[v].push_back({u, id});
        }
        if (!directed && !edge_disjoint && u != v) {
            mp[{min(u, v), max(u, v)}].push_back(id);
        }
        id++;
    }

    pair<vector<int>, vector<int>> get_cycle() { // {edge_id, vertices}
        if (!directed && !edge_disjoint) {
            for (auto &[edge, ids] : mp) {
                auto [u, v] = edge;
                if (ids.size() > 1) {
                    return make_pair(vector<int>{ids[0], ids[1]}, vector<int>{u, v, u});
                }
            }
        }
        for (int u = 0; u < g.size(); u++) {
            if (vis[u] == 0 and dfs(u)) {
                reverse(cycle_e.begin(), cycle_e.end());
                reverse(cycle_v.begin(), cycle_v.end());
                return {cycle_e, cycle_v};
            }
        }
        return {};
    }
};
