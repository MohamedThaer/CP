template <const int LG>
class subset_basis {
private:
    int dp[LG][1 << LG]{};

    void add(int mask, int i) {
        if (i == -1) {
            dp[LG - 1][mask] = 1;
            return;
        }
        if (dp[i][mask]) return;
        dp[i][mask] = 1;

        add(mask, i - 1);
        if (mask & (1 << i)) {
            add(mask ^ (1 << i), i - 1);
        }
    }

public:
    void add(int x) {
        add(x, LG - 1);
    }

    int max_and(int x) {
        int ans = 0;
        for (int i = LG - 1; ~i; --i) {
            if (((x >> i) & 1) && dp[LG - 1][ans | (1 << i)]) {
                ans |= (1 << i);
            }
        }
        return ans;
    }

    int max_or(int x) {
        int ans = 0;
        for (int i = LG - 1; ~i; --i) {
            if (!((x >> i) & 1) && dp[LG - 1][ans | (1 << i)]) {
                ans |= (1 << i);
            }
        }
        return ans | x;
    }
};
