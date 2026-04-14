
class permutation {
public:
    vector<int> p; // 0-index: from 0 to n-1
    permutation(const vector<int> &p) : p(p) {}
    permutation(const vector<vector<int>> &cycles) {
        int n = 0;
        for (auto c : cycles) {
            n += c.size();
        }
        *this = restore(n, cycles);
    }

    static vector<int> kth_permutation(int n, int64_t k) { // k is 0-index
        vector<int64_t> fact(n + 1, 1);
        for (int i = 1; i <= n; i++) {
            fact[i] = fact[i - 1] * i;
        }

        vector<int> nums(n), result;
        iota(nums.begin(), nums.end(), 0);

        for (int i = n; i >= 1; i--) {
            int64_t idx = k / fact[i - 1];
            k %= fact[i - 1];
            result.push_back(nums[idx]);
            nums.erase(nums.begin() + idx);
        }
        return result;
    }

    static int64_t permutation_index(const vector<int> &perm) { // return 0-index
        int n = perm.size();
        vector<int64_t> fact(n + 1, 1);
        for (int i = 1; i <= n; i++) {
            fact[i] = fact[i - 1] * i;
        }

        vector<int> used(n, 0);
        int64_t k = 0;
        for (int i = 0; i < n; i++) {
            int smaller = 0;
            for (int j = 0; j < perm[i]; j++) {
                if (!used[j]) smaller++;
            }
            k += smaller * fact[n - i - 1];
            used[perm[i]] = 1;
        }
        return k;
    }

    permutation inverse() const {
        int n = p.size();
        vector<int> inv(n);
        for (int i = 0; i < n; i++) {
            inv[p[i]] = i;
        }
        return inv;
    }

    vector<int> apply_on(const vector<int> &v) const { // element at p[i] becomes at i, p[i] = q[q[i]]
        assert(p.size() == v.size());
        int n = v.size();
        vector<int> ret(n);
        for (int i = 0; i < n; ++i) {
            ret[i] = v[p[i]];
        }
        return ret;
    }

    permutation power(int64_t k) const {
        vector<int> ret(p.size());
        iota(ret.begin(), ret.end(), 0);
        permutation base(p);
        while (k) {
            if (k & 1) {
                ret = base.apply_on(ret);
            }
            base = base.apply_on(base.p);
            k /= 2;
        }
        return ret;
    }

    // decompose into cycles
    vector<vector<int>> get_cycles() const {
        int n = p.size();
        vector<vector<int>> cycles;
        vector<bool> vis(n, 0);
        for (int i = 0; i < n; i++) {
            if (!vis[i]) {
                vector<int> v;
                int x = i;
                while (!vis[x]) {
                    v.push_back(x);
                    vis[x] = 1;
                    x = p[x];
                }
                cycles.push_back(v);
            }
        }
        return cycles;
    }

    // restore the permutation from the cycles
    static permutation restore(int n, const vector<vector<int>> &cycles) {
        vector<int> p(n);
        for (auto cyc : cycles) {
            for (int i = 0, m = cyc.size(); i < m; i++) {
                p[cyc[i]] = cyc[(i + 1) % m];
            }
        }
        return p;
    }

    // cycle decomposition of the k-th power of p
    vector<vector<int>> decompose_kth_power(int64_t k) const {
        int n = p.size();
        vector<vector<int>> ans;
        for (auto cyc : get_cycles()) {
            int64_t len = cyc.size(), g = gcd(k, len); // g cycles of len / g length
            for (int i = 0; i < g; i++) {
                vector<int> w;
                for (int j = i, cnt = 0; cnt < len / g; cnt++, j = (j + k) % len) {
                    w.push_back(cyc[j]);
                }
                ans.push_back(w);
            }
        }
        return ans;
    }

    // k-th root of p with minimum disjoint cycles
    // returns empty vector if no solution exists
    // if toggle = 1, then the parity of number of cycles will be different from the other(toggle = 0) if possible
    permutation root(int k, int toggle = 0) {
        int n = p.size();
        vector<vector<int>> cycles[n + 1];
        auto d = get_cycles();
        for (auto v : d)
            cycles[(int)v.size()].push_back(v);
        vector<vector<int>> ans;
        for (int len = 1; len <= n; len++) {
            if (cycles[len].empty()) continue;
            int tmp = k, t = 1, x = __gcd(len, tmp);
            while (x != 1) {
                t *= x;
                tmp /= x;
                x = __gcd(len, tmp);
            }
            if ((int)cycles[len].size() % t != 0) {
                ans.clear();
                return ans; // no solution exists
            }
            int id = 0;
            // we can merge t * z cycles iff tmp % z === 0
            if (toggle && tmp % 2 == 0 && (int)cycles[len].size() >= 2 * t) {
                int m = 2 * t * len;
                vector<int> cycle(m);
                for (int x = 0; x < 2 * t; x++) { // merging 2t cycles to perform the toggle
                    for (int y = 0; y < len; y++) {
                        cycle[(x + 1LL * y * k) % m] = cycles[len][x][y];
                    }
                }
                ans.push_back(cycle);
                id = 2 * t;
                toggle = 0;
            }
            for (int i = id; i < (int)cycles[len].size(); i += t) {
                int m = t * len;
                vector<int> cycle(m);
                for (int x = 0; x < t; x++) { // merging t cycles
                    for (int y = 0; y < len; y++) {
                        cycle[(x + 1LL * y * k) % m] = cycles[len][i + x][y];
                    }
                }
                ans.push_back(cycle);
            }
        }
        return ans;
    }

    // minimum swaps to obtain this perm from unit perm
    vector<pair<int, int>> transpositions() {
        vector<pair<int, int>> ans;
        for (auto v : get_cycles()) {
            int m = v.size();
            for (int i = m - 1; i > 0; i--)
                ans.push_back({v[0], v[i]});
        }
        return ans;
    }
};
