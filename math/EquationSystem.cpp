

template <class Z>
class EquationSystem {
private:
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

    int n, m;
    vector<int> where;
    vector<vector<Z>> mat;

public:
    EquationSystem(vector<vector<Z>> Equations, vector<Z> res) {
        n = Equations.size(), m = (n ? Equations[0].size() : 0);
        // Augment A with b
        mat.assign(n, vector<Z>(m + 1));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                mat[i][j] = Equations[i][j];
            }
            mat[i][m] = res[i];
        }
        mat = gaussian_elimination(mat);

        where.assign(m, -1);
        for (int i = 0; i < n; i++) {
            int pivot = -1;
            for (int j = 0; j < m; j++) {
                if (mat[i][j] != 0) {
                    pivot = j;
                    break;
                }
            }
            if (pivot != -1) {
                where[pivot] = i;
            }
        }
    }

    EquationSystem(vector<vector<Z>> v) {
        n = v.size(), m = (n ? v[0].size() - 1 : 0);
        mat = gaussian_elimination(v);

        where.assign(m, -1);
        for (int i = 0; i < n; i++) {
            int pivot = -1;
            for (int j = 0; j < m; j++) {
                if (mat[i][j] != 0) {
                    pivot = j;
                    break;
                }
            }
            if (pivot != -1) {
                where[pivot] = i;
            }
        }
    }

    bool is_consistent() {
        int n = mat.size(), m = (int)mat[0].size() - 1;
        for (int i = 0; i < n; i++) {
            bool all_zero = true;
            for (int j = 0; j < m; j++) {
                if (mat[i][j] != 0) {
                    all_zero = false;
                }
            }
            if (all_zero && mat[i][m] != 0) {
                return false;
            }
        }
        return true;
    }

    int get_rank() {
        int ans = 0;
        for (auto it : mat) {
            ans += (it != vector<Z>(it.size(), 0));
        }
        return ans;
    }

    vector<Z> get_solution() {
        vector<Z> sol(m, 0);
        // One particular solution
        for (int j = 0; j < m; j++) {
            if (where[j] != -1) {
                sol[j] = mat[where[j]][m];
            }
        }
        return sol;
    }

    vector<vector<Z>> get_basis() {
        vector<vector<Z>> basis;
        // Build nullspace basis
        for (int freeVar = 0; freeVar < m; freeVar++) {
            if (where[freeVar] != -1) continue;
            vector<Z> v(m, 0);
            v[freeVar] = 1;
            for (int j = 0; j < m; j++)
                if (where[j] != -1)
                    v[j] = -1 * mat[where[j]][freeVar];
            basis.push_back(v);
        }
        return basis;
    }
};
