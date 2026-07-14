
struct DAC {
    static const int LOG = 20;
    vector<string> bucket[LOG];
    AC ac[LOG];

    void clear(AC &ac_temp) {
        ac_temp.N = 0, ac_temp.P = 0;
        ac_temp.next.clear(), ac_temp.link.clear();
        ac_temp.out_link.clear(), ac_temp.out.clear();
        ac_temp.node();
    }

    void insert(const string &s) {
        vector<string> cur = {s};
        for (int i = 0; i < LOG; i++) {
            if (bucket[i].empty()) {
                bucket[i] = move(cur);
                clear(ac[i]);
                for (auto &s: bucket[i]) ac[i].add(s);
                ac[i].compute();
                return;
            } else {
                for (auto &x: bucket[i]) cur.push_back(x);
                bucket[i].clear();
                clear(ac[i]);
            }
        }
    }

    long long query(const string &s) {
        long long ans = 0;
        for (int i = 0; i < LOG; i++) {
            if (!bucket[i].empty()) {
                int u = 0;
                for (char c: s) {
                    u = ac[i].advance(u, c);
                    int tmp = u;
                    while (tmp)
                        ans += ac[i].out[tmp].size(), tmp = ac[i].out_link[tmp];
                }
            }
        }
        return ans;
    }
};
// another way 
struct DynamicAC {
    AC &ac;
    vector<int> term;
    vector<vector<int> > tree;
    vector<int> tin, tout;
    int timer = 0;
    Fenwick<int> bit;

    DynamicAC(AC &automaton, const vector<string> &pat) : ac(automaton) {
        int k = pat.size();
        term.resize(k);
        for (int i = 0; i < k; i++) {
            int u = 0;
            for (char c: pat[i]) {
                u = ac.next[u][ac.get(c)];
            }
            term[i] = u;
        }

        build_tree();
        bit.init(ac.N + 2);
    }

    void build_tree() {
        tree.assign(ac.N, {});
        for (int v = 1; v < ac.N; v++) {
            tree[ac.link[v]].push_back(v);
        }
        tin.assign(ac.N, 0);
        tout.assign(ac.N, 0);
        timer = 0;
        dfs(0);
    }

    void dfs(int u) {
        tin[u] = ++timer;
        for (int v: tree[u]) dfs(v);
        tout[u] = timer;
    }

    void activate(int id) {
        int u = term[id];
        bit.range_add(tin[u], tout[u], 1);
    }

    void deactivate(int id) {
        int u = term[id];
        bit.range_add(tin[u], tout[u], -1);
    }

    long long query(const string &s) {
        long long ans = 0;
        int u = 0;
        for (int i = 0; i < (int) s.size(); i++) {
            u = ac.advance(u, s[i]);
            ans += bit.range_sum(tin[u], tin[u]);
        }
        return ans;
    }
};
