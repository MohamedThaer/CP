class WaveletTree {
private:
    int64_t lo, hi;
    unique_ptr<WaveletTree> left, right;
    vector<int> b;
    vector<int64_t> c;

    int kth(int l, int r, int k) {
        if (l > r) return 0;
        if (lo == hi) return lo;
        int lb = b[l - 1];
        int rb = b[r];
        int inLeft = rb - lb;
        if (k <= inLeft) {
            return left->kth(lb + 1, rb, k);
        }
        else {
            return right->kth(l - lb, r - rb, k - inLeft);
        }
    }

    int LTE(int l, int r, int64_t k) {
        if (l > r || k < lo) return 0;
        if (hi <= k) return r - l + 1;
        int lb = b[l - 1];
        int rb = b[r];
        return (left ? left->LTE(lb + 1, rb, k) : 0) + (right ? right->LTE(l - lb, r - rb, k) : 0);
    }

    int count(int l, int r, int64_t k) {
        if (l > r || k < lo || k > hi) return 0;
        if (lo == hi) return r - l + 1;
        int64_t mid = lo + hi >> 1;
        int lb = b[l - 1];
        int rb = b[r];
        if (k <= mid) {
            return left ? left->count(lb + 1, rb, k) : 0;
        }
        else {
            return right ? right->count(l - lb, r - rb, k) : 0;
        }
    }

    int64_t sum(int l, int r, int k) {
        if (l > r || k < lo) return 0;
        if (hi <= k) return c[r] - c[l - 1];
        int lb = b[l - 1];
        int rb = b[r];
        return (left ? left->sum(lb + 1, rb, k) : 0) + (right ? right->sum(l - lb, r - rb, k) : 0);
    }

public:
    template <class T>
    WaveletTree(const vector<T> &v, int64_t low, int64_t high) : lo(low), hi(high) {
        int n = v.size();
        b.reserve(n + 1);
        c.reserve(n + 1);
        b.push_back(0);
        c.push_back(0);
        int64_t mid = lo + hi >> 1;
        vector<T> left_arr, right_arr;
        for (auto val : v) {
            b.push_back(b.back() + (val <= mid));
            c.push_back(c.back() + val);
            if (val <= mid) {
                left_arr.push_back(val);
            }
            else {
                right_arr.push_back(val);
            }
        }
        if (lo != hi) {
            if (!left_arr.empty()) {
                left = make_unique<WaveletTree>(left_arr, lo, mid);
            }
            else {
                left = nullptr;
            }
            if (!right_arr.empty()) {
                right = make_unique<WaveletTree>(right_arr, mid + 1, hi);
            }
            else {
                right = nullptr;
            }
        }
    }

    int kth_order(int l, int r, int k) {
        return kth(++l, ++r, k);
    }

    int count_range(int l, int r, int64_t a, int64_t b) {
        return LTE(l, r, b) - LTE(l, r, a - 1);
    }

    int countVal(int l, int r, int64_t k) {
        return count(++l, ++r, k);
    }

    int64_t sum_k(int l, int r, int k) {
        return sum(++l, ++r, k);
    }
};
