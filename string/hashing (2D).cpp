class dr_string2D {
private:
    using i128 = __int128_t;
    static const int64_t mod = (1LL << 61) - 1;
    static int64_t base1, base2;
    vector<int64_t> pow1, pow2;
    vector<vector<int64_t>> hash;

public:
    dr_string2D(const vector<string> &mat) {
        int n = mat.size(), m = mat[0].size();
        pow1.assign(n + 1, 1), pow2.assign(m + 1, 1);
        hash.assign(n + 1, vector<int64_t>(m + 1, 0));

        for (int i = 0; i < n; i++) {
            pow1[i + 1] = (i128)pow1[i] * base1 % mod;
        }
        for (int i = 0; i < m; i++) {
            pow2[i + 1] = (i128)pow2[i] * base2 % mod;
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                hash[i + 1][j + 1] = mat[i][j];
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= m; j++) {
                (hash[i + 1][j] += (i128)hash[i][j] * base1 % mod) %= mod;
            }
        }
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j < m; j++) {
                (hash[i][j + 1] += (i128)hash[i][j] * base2 % mod) %= mod;
            }
        }
    }

    int64_t substr(int x1, int y1, int x2, int y2) { // 1-indexed
        x1--, y1--;
        int dx = x2 - x1, dy = y2 - y1;
        i128 val = (hash[x2][y2] - (i128)hash[x2][y1] * pow2[dy]) - (hash[x1][y2] - (i128)hash[x1][y1] * pow2[dy] % mod) * pow1[dx];
        return (val % mod + mod) % mod;
    }
};
int64_t dr_string2D::base1 = (mod >> 2) + rand() % (mod >> 1);
int64_t dr_string2D::base2 = (mod >> 2) + rand() % (mod >> 1);
