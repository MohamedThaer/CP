struct BipartiteDSU {
    vector<int> sz,bipartite;
    vector<pair<int, int>>par;
    BipartiteDSU(int n) : par(n), sz(n, 1),bipartite(n) {
        for (int i = 0; i < n; ++i) {
            par[i] = {i,0};
        }
    }
    pair<int, int> find(int u) {
        if (u == par[u].fi)return {u, 0};
        int parity = par[u].se;
        par[u] = find(par[u].first);
        par[u].se ^= parity;
        return par[u];
    }
    bool same(int x, int y) { return find(x).first == find(y).first; }
    bool join(int u, int v) {
        pair<int,int>pu = find(u);
        pair<int,int>pv = find(v);
        u = pu.first;
        v = pv.first;
        int x = pu.second,y = pv.second;
        if (u == v) {
            if(x == y)
                bipartite[u] = false;
            return false;
        }
        if (sz[u] < sz[v])
            swap(u, v);
        par[v] = {u, x ^ y ^ 1};
        bipartite[u] &= bipartite[v];
        sz[u] += sz[v];
        return true;
    }
    int size(int x) { return sz[find(x).first]; }
};

/**
 * Offline Parallel DSU
 * Constraints: N, Q <= 1e6
 * Time: O(N log N) total
 * Space: O(N log N)
 * MUST run Build after merging all
 */
class ParallelDSU {
    int n;
    vector<vector<int>> par;
    int find(int k, int x) { return par[k][x] == x ? x : par[k][x] = find(k, par[k][x]); }

    void unite(int k, int u, int v) {
        u = find(k, u), v = find(k, v);
        if (u != v) par[k][u] = v;
    }

public:
    int cnt;
    vector<int> sz;

    ParallelDSU(int n)
        : n(n), cnt(n), sz(n, 1) {
        int LOG = 32 - __builtin_clz(n);
        par.assign(LOG, vector<int>(n));
        for (int k = 0; k < LOG; ++k) {
            iota(par[k].begin(), par[k].end(), 0);
        }
    }

    // Merge arbitrary ranges [u, u + len) and [v, v + len)
    void merge(int u, int v, int len) {
        if (len <= 0) return;
        int k = 31 - __builtin_clz(len);
        unite(k, u, v);
        unite(k, u + len - (1 << k), v + len - (1 << k));
    }

    void build() {
        for (int k = par.size() - 1; k > 0; --k) {
            for (int i = 0; i + (1 << k) <= n; ++i) {
                int r = find(k, i);
                if (i != r) {
                    unite(k - 1, i, r);
                    unite(k - 1, i + (1 << (k - 1)), r + (1 << (k - 1)));
                }
            }
        }

        vector<int> fp(n);
        iota(fp.begin(), fp.end(), 0);

        auto get = [&](auto &&s, int x) -> int {
            return fp[x] == x ? x : fp[x] = s(s, fp[x]);
        };

        for (int i = 0; i < n; ++i) {
            int target = find(0, i);
            int u = get(get, i);
            int v = get(get, target);

            if (u != v) {
                --cnt;
                fp[u] = v;
                sz[v] += sz[u];
            }
        }
        par[0] = fp;
    }

    int count() const { return cnt; }
    int find(int x) { return par[0][x] == x ? x : par[0][x] = find(par[0][x]); }
    int size(int u) { return sz[find(u)]; }
};
