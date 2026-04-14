vector<int> spf;
vector<int> factors(int n) {
    vector<int> ret;
    int cur = n;
    while (1 < cur) {
        ret.push_back(spf[cur]);
        cur /= spf[cur];
    }
    return ret;
}

vector<int> divisors(int n) {
    auto fact = factors(n);
    vector<array<int, 2>> p;
    for (auto &it : fact) {
        if (p.empty() || p.back()[0] != it) {
            p.push_back({it, 1});
        }
        else {
            p.back()[1]++;
        }
    }
    vector<int> ret;
    function<void(int, int)> build = [&](int i, int res) {
        if (i == p.size()) {
            ret.push_back(res);
            return;
        }
        int x = 1, itr = p[i][1] + 1;
        while (itr--) {
            build(i + 1, res * x);
            x *= p[i][0];
        }
    };
    build(0, 1);
    sort(ret.begin(), ret.end());
    return ret;
}
