class PairRelation {
private:
    suffix_array<string> sa;
    vector<suffix_array<string>> sa_s;
    vector<int> sz;
    vector<string> v;

public:
    PairRelation() {}
    PairRelation(const vector<string> &v) : v(v) {
        string s;
        sz = {0};
        for (auto it : v) {
            s += it;
            sa_s.emplace_back(it);
            sz.push_back(sz.back() + it.size());
        }
        sa = suffix_array(s);
    }

    int cmp(int a, int la, int ra, int b, int lb, int rb) {
        return sa.cmp({la + sz[a], ra + sz[a]}, {lb + sz[b], rb + sz[b]});
    }

    int count(int ida, int idb) { // occurence of v[ida] in v[idb]
        int l = 0, r = sa_s[idb].suf.size() - 1, left = -1, right;

        while (l <= r) {
            int mid = (l + r) / 2;
            if (cmp(idb, sa_s[idb].suf[mid], min<int>(v[idb].size(), sa_s[idb].suf[mid] + v[ida].size()) - 1, ida, 0, v[ida].size() - 1) == -1) {
                l = mid + 1;
            }
            else {
                r = mid - 1;
                left = mid;
            }
        }

        if (left == -1 || cmp(idb, sa_s[idb].suf[left], min<int>(v[idb].size(), sa_s[idb].suf[left] + v[ida].size()) - 1, ida, 0, v[ida].size() - 1) != 0) {
            return 0;
        }

        l = 0, r = sa_s[idb].suf.size() - 1;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (cmp(idb, sa_s[idb].suf[mid], min<int>(v[idb].size(), sa_s[idb].suf[mid] + v[ida].size()) - 1, ida, 0, v[ida].size() - 1) != 1) {
                l = mid + 1;
                right = mid;
            }
            else {
                r = mid - 1;
            }
        }

        return right - left + 1;
    }
};
