template <class M>
struct SparseTable {
    vector<vector<M>> dp;

    SparseTable() {}

    SparseTable(const vector<M> &a) {
        int n = a.size(), lg = __lg(n);
        dp.assign(lg + 1, a);

        for (int j = 1; j <= lg; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                dp[j][i] = merge(dp[j - 1][i],
                                 dp[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    M merge(M a, M b) {
        return min(a, b);
    }

    M query(int l, int r) {
        int k = __lg(r - l + 1);
        return merge(dp[k][l],
                     dp[k][r - (1 << k) + 1]);
    }
};
