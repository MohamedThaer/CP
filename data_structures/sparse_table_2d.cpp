
template <class M>
class sparse_table_2D {
private:
    vector<vector<vector<vector<M>>>> dp;
    function<M(M, M)> merge;

public:
    sparse_table_2D(const vector<vector<M>> &a, function<M(M, M)> merge) : merge(merge) {
        int n = a.size(), m = a[0].size();
        int LGN = __lg(n), LGM = __lg(m);

        dp.assign(LGN + 1, vector<vector<vector<M>>>(
                               LGM + 1, vector<vector<M>>(
                                            n, vector<M>(m))));

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                dp[0][0][i][j] = a[i][j];
            }
        }

        for (int x = 0; x <= LGN; ++x) {
            for (int y = 0; y <= LGM; ++y) {
                if (x + y == 0) continue;
                for (int i = 0; i + (1 << x) <= n; ++i) {
                    for (int j = 0; j + (1 << y) <= m; ++j) {
                        if (x == 0) {
                            dp[x][y][i][j] = merge(dp[x][y - 1][i][j], dp[x][y - 1][i][j + (1 << (y - 1))]);
                        }
                        else {
                            dp[x][y][i][j] = merge(dp[x - 1][y][i][j], dp[x - 1][y][i + (1 << (x - 1))][j]);
                        }
                    }
                }
            }
        }
    }

    M query(int x1, int y1, int x2, int y2) {
        ++x2, ++y2;
        int lgx = 32 - __builtin_clz(x2 - x1) - 1;
        int lgy = 32 - __builtin_clz(y2 - y1) - 1;
        return merge(
            merge(dp[lgx][lgy][x1][y1], dp[lgx][lgy][x2 - (1 << lgx)][y1]),
            merge(dp[lgx][lgy][x1][y2 - (1 << lgy)], dp[lgx][lgy][x2 - (1 << lgx)][y2 - (1 << lgy)]));
    }
};
// sparse_table_2D<int> sp(vector<vector<int>>(10, vector<int>(10)), [](int a, int b) { return max(a, b); });
