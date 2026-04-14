
// j == -1 ? (k == 0 ? 0 : -INF)
// dp(i, k) = min[0 <= j <= i] (dp(j - 1, k - 1) + C(j, i))
// cost is a cost function satisfying the quadrangle inequality
class DP_DAC {
private:
    function<int64_t(int64_t, int64_t)> cost;
    vector<vector<int64_t>> dp;

    // compute dp[i][l..r]
    void compute(int i, int l, int r, int optl, int optr) {
        if (l > r) return;

        int mid = (l + r) >> 1;
        pair<int64_t, int> best = {LLONG_MAX, -1};

        for (int k = optl; k <= min(mid, optr); k++) {
            best = min(best, {(k ? dp[i - 1][k - 1] : 0) + cost(k, mid), k});
        }

        dp[i][mid] = best.first;
        int opt = best.second;

        compute(i, l, mid - 1, optl, opt);
        compute(i, mid + 1, r, opt, optr);
    }

public:
    // n: length of 0-indexed array
    // k: max number of divisions
    DP_DAC(int n, int k, const auto &cost) : cost(cost) {
        dp.assign(k, vector<int64_t>(n, 0));
        for (int i = 0; i < k; i++) {
            if (!i) {
                for (int j = 0; j < n; ++j) {
                    dp[0][j] = cost(0, j);
                }
            }
            else {
                compute(i, 0, n - 1, 0, n - 1);
            }
        }
    }

    int64_t solve(int i, int k) { // divide prefix i into k divisions
        return dp[k - 1][i];
    }
};
