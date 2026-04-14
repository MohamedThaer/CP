
/*
    add logical statements using n variables:

    Assert(f_a, a)      ====> (f_a ? a : !a) = true;
    Or(f_a, a, f_b, b)  ====> (f_a ? a : !a) or (f_b ? b : !b) = true;
    And(f_a, a, f_b, b) ====> (f_a ? a : !a) and (f_b ? b : !b) = true;
    etc...

    get answer that satisfies all logical statements.
*/
class TwoSat {
private:
    int n;
    vector<vector<int>> e;
    vector<bool> ans;
    void addClause(int u, bool f, int v, bool g) {
        e[2 * u + !f].push_back(2 * v + g);
        e[2 * v + !g].push_back(2 * u + f);
    }

public:
    TwoSat(int n) : n(n), e(2 * n), ans(n) {}
    bool satisfiable() {
        vector<int> id(2 * n, -1), dfn(2 * n, -1), low(2 * n, -1);
        vector<int> stk;
        int now = 0, cnt = 0;
        function<void(int)> tarjan = [&](int u) {
            stk.push_back(u);
            dfn[u] = low[u] = now++;
            for (auto v : e[u]) {
                if (dfn[v] == -1) {
                    tarjan(v);
                    low[u] = min(low[u], low[v]);
                }
                else if (id[v] == -1) {
                    low[u] = min(low[u], dfn[v]);
                }
            }
            if (dfn[u] == low[u]) {
                int v;
                do {
                    v = stk.back();
                    stk.pop_back();
                    id[v] = cnt;
                } while (v != u);
                ++cnt;
            }
        };
        for (int i = 0; i < 2 * n; ++i) {
            if (dfn[i] == -1) tarjan(i);
        }
        for (int i = 0; i < n; ++i) {
            if (id[2 * i] == id[2 * i + 1]) return false;
            ans[i] = id[2 * i] > id[2 * i + 1];
        }
        return true;
    }
    vector<bool> answer() { return ans; }

    void Or(bool f_a, int a, bool f_b, int b) {
        addClause(a, f_a, b, f_b);
    }

    void Implies(bool f_a, int a, bool f_b, int b) {
        Or(!f_a, a, f_b, b);
    }

    void Assert(bool f_a, int a) {
        Or(f_a, a, f_a, a);
    }

    void And(bool f_a, int a, bool f_b, int b) {
        Assert(f_a, a);
        Assert(f_b, b);
    }

    void Xor(bool f_a, int a, bool f_b, int b) {
        Or(f_a, a, f_b, b);
        Or(!f_a, a, !f_b, b);
    }

    void Nor(bool f_a, int a, bool f_b, int b) {
        And(!f_a, a, !f_b, b);
    }

    void Nand(bool f_a, int a, bool f_b, int b) {
        Or(!f_a, a, !f_b, b);
    }

    void Xnor(bool f_a, int a, bool f_b, int b) {
        Or(!f_a, a, f_b, b);
        Or(f_a, a, !f_b, b);
    }

    int new_var() {
        e.push_back({});
        e.push_back({});
        ans.push_back(false);
        return n++;
    }

    void at_most_one(const vector<pair<bool, int>> &vars) {
        if (vars.size() <= 1) return;
        int prev = -1;
        for (int i = 0; i < vars.size(); i++) {
            int cur = new_var();
            auto [f_a, a] = vars[i];
            // (¬xi ∨ cur)
            Or(!f_a, a, true, cur);
            if (i) {
                // (¬prev ∨ cur)
                Or(false, prev, true, cur);
                // (¬xi ∨ ¬prev)
                Or(!f_a, a, false, prev);
            }
            prev = cur;
        }
    }
};
