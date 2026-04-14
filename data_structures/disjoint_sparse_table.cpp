template <class M, class F>
class disjoint_sparse_table {
private:
    F merge;
    vector<vector<M>> pref, suff;

public:
    disjoint_sparse_table(vector<M> &a, const F &merge) : merge(merge) {
        int n = a.size(), k = __lg(n) + 1;
        pref.assign(k, a);
        suff.assign(k, a);
        for (int j = 1; (1 << j) <= n; ++j) {
            int mask = (1 << j) - 1;
            for (int i = 1; i < n; ++i) {
                if (((i - 1) & mask) != mask) {
                    pref[j][i] = merge(pref[j][i - 1], pref[j][i]);
                }
            }
            for (int i = n - 2; i >= 0; --i) {
                if (((i + 1) & mask) != 0) {
                    suff[j][i] = merge(suff[j][i], suff[j][i + 1]);
                }
            }
        }
    }

    M query(int l, int r) {
        if (l == r) return pref[0][r];
        int i = __lg(l ^ r);
        return merge(suff[i][l], pref[i][r]);
    }
};
