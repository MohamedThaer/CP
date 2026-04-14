template <int Nodes>
class reachability {
private:
    bitset<Nodes + 1> can[Nodes + 1];

public:
    reachability(vector<vector<int>> g) {
        SCC scc(g);
        auto &dag = scc.compress();
        auto &comps = scc.get_components();

        vector<int> vis(scc.components_cnt());
        function<void(int)> dfs = [&](int node) {
            if (vis[node]) return;
            vis[node] = true;

            for (auto i : comps[node]) {
                can[comps[node][0]][i] = 1;
            }
            for (auto it : dag[node]) {
                dfs(it);
                can[comps[node][0]] |= can[comps[it][0]];
            }
            for (auto i : comps[node]) {
                can[i] = can[comps[node][0]];
            }
        };

        for (int i = 0; i < scc.components_cnt(); ++i) {
            dfs(i);
        }
    }

    bool can_reach(int a, int b) {
        return can[a][b];
    }

    int count_reachable(int a) {
        return can[a].count();
    }
};
