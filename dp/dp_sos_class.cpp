template <const int LG = 20>
class DP_SOS {
private:
    int F1[1 << LG]{}, F2[1 << LG]{};

    inline int inv(int x) const { return ~x & ((1 << LG) - 1); }

public:
    DP_SOS(vector<int> &v) {
        for (int i = 0; i < v.size(); i++) {
            F1[v[i]]++;
            F2[inv(v[i])]++;
        }

        for (int i = 0; i < LG; ++i) {
            for (int mask = 0; mask < (1 << LG); ++mask) {
                if (mask & (1 << i)) {
                    F1[mask] += F1[mask ^ (1 << i)];
                    F2[mask] += F2[mask ^ (1 << i)];
                }
            }
        }
    }

    int64_t submasks_ans(int mask) {
        return F1[mask];
    }

    int64_t zero_and(int mask) {
        return F1[inv(mask)];
    }

    int64_t supermasks_ans(int mask) {
        return F2[inv(mask)];
    }
};
