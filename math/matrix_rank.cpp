template <class Z>
int matrix_rank(vector<vector<Z>> v) {
    v = gaussian_elimination(v);
    int ans = 0;
    for (auto it : v) {
        ans += (it != vector<Z>(it.size(), 0));
    }
    return ans;
}
