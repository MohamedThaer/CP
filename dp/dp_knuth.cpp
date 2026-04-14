

// dp(i, j) = min[ i <= k < j ](dp(i, k) + dp(k+1, j) + C(i, j))
// cost is a cost function satisfying the quadrangle inequality
class DP_Knuth {
private:
    vector<vector<int64_t>> dp;

public:
    DP_Knuth(int n, const auto &cost) {
        dp.assign(n, vector<int64_t>(n, 0));
        vector<vector<int>> opt(n, vector<int>(n, 0));

        for (int i = 0; i < n; i++) {
            opt[i][i] = i;
        }

        for (int len = 1; len < n; len++) {
            for (int i = 0; i + len < n; i++) {
                int j = i + len;

                pair<int64_t, int> best = {LLONG_MAX, -1};

                // Knuth’s optimization: search only between opt[i][j-1] and opt[i+1][j]
                for (int k = opt[i][j - 1]; k <= min(opt[i + 1][j], j - 1); k++) {
                    int64_t val = dp[i][k] + dp[k + 1][j] + cost(i, j);
                    best = min(best, {val, k});
                }

                dp[i][j] = best.first;
                opt[i][j] = best.second;
            }
        }
    }

    int64_t solve(int i, int j) {
        return dp[i][j];
    }
};
