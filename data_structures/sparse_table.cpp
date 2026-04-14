template <class M, class Func>
class sparse_table {
private:
    Func merge;
    vector<vector<M>> dp;

public:
    sparse_table() {}
    sparse_table(const vector<M> &a, const Func &merge) : merge(merge) {
        int n = a.size(), lg = __lg(n);
        dp.resize(lg + 1, a);

        for (int j = 1; j <= lg; j++) {
            for (int i = 0; i + (1 << (j - 1)) < n; i++) {
                dp[j][i] = merge(dp[j - 1][i], dp[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    M query(int l, int r) {
        int lg = 32 - __builtin_clz(r - l + 1) - 1;
        return merge(dp[lg][l], dp[lg][r - (1 << lg) + 1]);
    }
};
// sparse_table sp(vector<int64_t>(10, 1), [](int64_t a, int64_t b) { return min(a, b); });
