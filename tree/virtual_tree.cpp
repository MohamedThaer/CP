class virtual_tree {
private:
    LCA l;
    vector<int> st, used;

    void clear_v_tree() {
        for (auto it : used) {
            v_tree[it].clear();
        }
        used.clear();
    }

public:
    virtual_tree(vector<vector<int>> g) : l(g) {
        int n = g.size();
        st = vector<int>(n);
        v_tree = vector<vector<int>>(n);

        int time = 0;
        function<void(int, int)> dfs = [&](int node, int par) {
            st[node] = time++;
            for (auto it : g[node]) {
                if (it != par) {
                    dfs(it, node);
                }
            }
        };
        dfs(1, -1);
    }

    vector<vector<int>> v_tree;

    vector<int> buildtree(vector<int> v) {
        clear_v_tree();
        sort(v.begin(), v.end(), [&](int x, int y) {
            return st[x] < st[y];
        });
        int s = v.size();
        for (int i = 0; i < s - 1; i++) {
            int lc = l.lca(v[i], v[i + 1]);
            v.push_back(lc);
        }
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());
        sort(v.begin(), v.end(), [&](int x, int y) {
            return st[x] < st[y];
        });
        stack<int> st;
        st.push(v[0]);
        for (int i = 1; i < v.size(); i++) {
            while (l.lca(st.top(), v[i]) != st.top()) {
                st.pop();
            }
            v_tree[st.top()].push_back(v[i]), used.push_back(st.top());
            st.push(v[i]);
        }
        return v; // v[0] is the root
    }
};
