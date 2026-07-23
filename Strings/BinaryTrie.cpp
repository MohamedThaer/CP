template <int LOG = 62>
struct BinaryTrie {
    struct Node {
        int child[2];
        int cnt;
        Node() {
            child[0] = child[1] = -1;
            cnt = 0;
        }
    };
    vector<Node> tree;
    BinaryTrie() {
        tree.emplace_back();
    }
    bool valid(int u) {
        return u != -1 && tree[u].cnt > 0;
    }
    void insert(long long x, int delta = 1) {
        int cur = 0;
        tree[cur].cnt += delta;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            int b = (x >> bit) & 1;
            if (tree[cur].child[b] == -1) {
                tree[cur].child[b] = tree.size();
                tree.emplace_back();
            }
            cur = tree[cur].child[b];
            tree[cur].cnt += delta;
        }
    }

    int count(long long x) {
        int cur = 0;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            int b = (x >> bit) & 1;
            if (!valid(tree[cur].child[b]))
                return 0;
            cur = tree[cur].child[b];
        }
        return tree[cur].cnt;
    }
    // {maximum xor value, element giving it}
    pair<long long, long long> max_xor(long long x) {
        int cur = 0;
        long long xr = 0, val = 0;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            int b = (x >> bit) & 1;
            if (valid(tree[cur].child[b ^ 1])) {
                cur = tree[cur].child[b ^ 1];
                xr |= 1LL << bit;
                val |= 1LL * (b ^ 1) << bit;
            }
            else {
                cur = tree[cur].child[b];
                val |= 1LL * b << bit;
            }
        }
        return {xr, val};
    }
    // {minimum xor value, element giving it}
    pair<long long, long long> min_xor(long long x) {
        int cur = 0;
        long long xr = 0, val = 0;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            int b = (x >> bit) & 1;
            if (valid(tree[cur].child[b])) {
                cur = tree[cur].child[b];
                val |= 1LL * b << bit;
            }
            else {
                cur = tree[cur].child[b ^ 1];
                xr |= 1LL << bit;
                val |= 1LL * (b ^ 1) << bit;
            }
        }

        return {xr, val};
    }

    // Count (x xor value) < k
    long long xor_less_than(long long x, long long k) {
        int cur = 0;
        long long ans = 0;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            if (!valid(cur))
                break;
            int xb = (x >> bit) & 1;
            int kb = (k >> bit) & 1;
            if (kb) {
                if (valid(tree[cur].child[xb]))
                    ans += tree[tree[cur].child[xb]].cnt;

                cur = tree[cur].child[xb ^ 1];
            }
            else {
                cur = tree[cur].child[xb];
            }
        }
        return ans;
    }
    long long xor_greater_equal(long long x, long long k) {
        return tree[0].cnt - xor_less_than(x, k);
    }
    // k-th smallest (1-indexed)
    long long kth(int k) {
        int cur = 0;
        long long ans = 0;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            int left = tree[cur].child[0];
            int cntLeft = valid(left) ? tree[left].cnt : 0;
            if (k <= cntLeft) {
                cur = left;
            }
            else {
                k -= cntLeft;
                cur = tree[cur].child[1];
                ans |= 1LL << bit;
            }
        }
        return ans;
    }

    long long count_less(long long x) {
        int cur = 0;
        long long ans = 0;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            if (!valid(cur))
                break;
            int b = (x >> bit) & 1;
            if (b) {
                if (valid(tree[cur].child[0]))
                    ans += tree[tree[cur].child[0]].cnt;
                cur = tree[cur].child[1];
            }
            else {
                cur = tree[cur].child[0];
            }
        }
        return ans;
    }
    long long count_less_equal(long long x) {
        return count_less(x + 1);
    }
    long long count_greater(long long x) {
        return size() - count_less_equal(x);
    }
    long long count_greater_equal(long long x) {
        return size() - count_less(x);
    }
     int64_t get_mex() {
        int cur = 0, mex = 0;
        for (int bit = LOG - 1; ~bit; bit--) {
            int full = (1LL << bit);
            int left = tree[cur].child[0];
            if (valid(left) && tree[left].cnt == full) {
                mex |= full;
                cur = tree[cur].child[1];
            } else cur = left;
            if (!valid(cur)) break;
        }
        return mex;
    }
 
    int64_t get_mex_xor(int x) {
        int cur = 0, mex = 0;
        for (int bit = LOG - 1; ~bit; bit--) {
            int xb = (x >> bit) & 1, full = (1LL << bit);
            int sub = tree[cur].child[xb];
            if (valid(sub) && tree[sub].cnt == full) {
                mex |= full;
                cur = tree[cur].child[xb ^ 1];
            } else cur = sub;
            if (!valid(cur)) break;
        }
        return mex;
    }
    int size() {
        return tree[0].cnt;
    }

};
