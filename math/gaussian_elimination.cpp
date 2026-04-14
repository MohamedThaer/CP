
template <class Z>
vector<vector<Z>> gaussian_elimination(vector<vector<Z>> v) {
    if (v.empty()) return v;
    int n = v.size(), m = v[0].size();

    auto apply = [&](int r1, Z m1, int r2, Z m2) {
        for (int i = 0; i < m; ++i) {
            v[r2][i] = m2 * v[r2][i] - m1 * v[r1][i];
        }
    };

    for (int i = 0; i < n; ++i) {
        int idx = m;
        for (int j = m - 1; ~j; --j) {
            if (v[i][j] != 0) idx = j;
        }

        if (idx != m) {
            for (int j = 0; j < n; ++j) {
                if (i != j && v[j][idx] != 0) {
                    apply(i, v[j][idx], j, v[i][idx]);
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        int idx = m;
        for (int j = m - 1; ~j; --j) {
            if (v[i][j] != 0) idx = j;
        }

        for (int j = m - 1; ~j && idx < m; --j) {
            v[i][j] /= v[i][idx];
        }
    }

    return v;
}
