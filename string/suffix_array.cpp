
template <class M>
class sparse_table {
private:
    function<M(M, M)> merge;
    vector<vector<M>> dp;

public:
    sparse_table() {}
    sparse_table(const vector<M> &a, const function<M(M, M)> &merge) : merge(merge) {
        int n = a.size(), lg = __lg(n);
        dp.resize(lg + 1, a);

        for (int j = 1; j <= lg; j++) {
            for (int i = 0; i + (1 << (j - 1)) < n; i++) {
                dp[j][i] = merge(dp[j - 1][i], dp[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    M query(int l, int r) {
        int lg = 32 - __builtin_clz(r - l + 1) - 1;
        return merge(dp[lg][l], dp[lg][r - (1 << lg) + 1]);
    }
};
// sparse_table sp(vector<int64_t>(10, 1), [](int64_t a, int64_t b) { return min(a, b); });

template <class T>
class suffix_array {
private:
    sparse_table<int> sp;

public:
    T a;
    vector<int> suf, group, lcp_;

    suffix_array() {}
    suffix_array(T a) : a(a) { // a: vector, string, ...
        int n = a.size();

        suf = group = vector<int>(n + 1);
        suf[0] = n, iota(suf.begin() + 1, suf.end(), 0);
        sort(suf.begin() + 1, suf.end(), [&](int i, int j) { return a[i] < a[j]; });
        group[suf[1]] = 1;
        for (int i = 2; i <= n; ++i) {
            group[suf[i]] = group[suf[i - 1]] + (a[suf[i]] != a[suf[i - 1]]);
        }

        vector<int> ptr(n + 1), temp(n + 1);
        int k = 1;
        auto sort = [&](int x) {
            temp.assign(n + 1, 0);
            auto &cnt = temp;
            for (int i = 0; i <= n; ++i) {
                cnt[group[(suf[i] + x) % (n + 1)]]++;
            }

            for (int i = 0, sum = 0; i <= n; ++i) {
                ptr[i] = sum;
                sum += cnt[i];
            }

            for (int i = 0; i <= n; ++i) {
                temp[ptr[group[(suf[i] + x) % (n + 1)]]++] = suf[i];
            }
            suf = temp;
        };

        while (group[suf[n]] != n) {
            sort(k);
            sort(0);
            auto &newG = temp;
            newG = group;
            for (int i = 1; i <= n; ++i) {
                newG[suf[i]] = newG[suf[i - 1]] + (group[suf[i]] != group[suf[i - 1]] || group[suf[i] + k] != group[suf[i - 1] + k]);
            }
            group = newG;
            k <<= 1;
        }

        k = 0;
        lcp_ = vector<int>(n);
        for (int i = 0; i < n; ++i) {
            int pos = group[i], j = suf[pos - 1];
            while (k < n && a[i + k] == a[j + k]) {
                ++k;
            }
            lcp_[pos - 1] = k;
            k = max(k - 1, 0);
        }
        sp = sparse_table<int>(lcp_, [&](int a, int b) { return min(a, b); });
    }

    int count(T sub) {
        int l = 0, r = suf.size() - 1, left = -1, right;

        auto get = [&](int l, int len) {
            return T(a.begin() + l, a.begin() + min((int)a.size(), l + len));
        };

        while (l <= r) {
            int mid = (l + r) / 2;
            if (get(suf[mid], sub.size()) < sub) {
                l = mid + 1;
            }
            else {
                r = mid - 1;
                left = mid;
            }
        }

        if (left == -1 || get(suf[left], sub.size()) != sub) {
            return 0;
        }

        l = 0, r = suf.size() - 1;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (get(suf[mid], sub.size()) <= sub) {
                l = mid + 1;
                right = mid;
            }
            else {
                r = mid - 1;
            }
        }

        return right - left + 1;
    }

    int lcp(int i, int j) {
        i = group[i], j = group[j];
        if (i > j) {
            swap(i, j);
        }
        if (i == j) {
            return (int)lcp_.size() - suf[i];
        }
        if (i + 1 == j) {
            return lcp_[i];
        }
        return sp.query(i, j - 1);
    }

    /*
        cmp (a[l1..r1], a[l2..r2])
        returns
        -1: a[l1..r1] < a[l2..r2]
        0: a[l1..r1] = a[l2..r2]
        1: a[l1..r1] > a[l2..r2]
    */
    int cmp(pair<int, int> sub1, pair<int, int> sub2) {
        auto [l1, r1] = sub1;
        auto [l2, r2] = sub2;

        int lc = lcp(l1, l2), sz1 = r1 - l1 + 1, sz2 = r2 - l2 + 1;
        if (lc >= sz1 || lc >= sz2) {
            if (sz1 == sz2) {
                return 0;
            }
            return (sz1 < sz2 ? -1 : 1);
        }
        return (a[l1 + lc] < a[l2 + lc] ? -1 : 1);
    }

    T kth_distinct_substring(int64_t k) {
        for (int i = 1, n = a.size(); i <= n; ++i) {
            int cnt = (a.size() - suf[i]) - lcp_[i - 1];
            if (k <= cnt) {
                return a.substr(suf[i], lcp_[i - 1] + k);
            }
            k -= cnt;
        }
        return T{};
    }
};
