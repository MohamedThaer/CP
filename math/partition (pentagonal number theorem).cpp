
// ans[i] = number of partitions of i
// O(n sqrt(n))
// takes 1.5s for n = 5e5
vector<Z> partition(int n) {
    vector<Z> ans(n + 1);
    vector<pair<int, int>> gp; // (sign, generalized pentagonal numbers)
    gp.emplace_back(0, 0);
    for (int i = 1; gp.back().second <= n; i++) {
        gp.emplace_back(i % 2 ? 1 : -1, i * (3 * i - 1) / 2);
        gp.emplace_back(i % 2 ? 1 : -1, i * (3 * i + 1) / 2);
    }
    ans[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (auto it : gp) {
            if (i >= it.second) {
                ans[i] += ans[i - it.second] * it.first;
            }
            else {
                break;
            }
        }
    }
    return ans;
}
