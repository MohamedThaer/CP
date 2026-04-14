
template <typename T>
struct PrimeBasis {
    T gcd(T a, T b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    void reduce_pair(T &x, T &y) {
        bool to_swap = 0;
        if (x > y) {
            to_swap ^= 1;
            swap(x, y);
        }
        while (x > 1 && y % x == 0) {
            y /= x;
            if (x > y) {
                to_swap ^= 1;
                swap(x, y);
            }
        }
        if (to_swap) swap(x, y);
    }
    vector<T> basis;
    void solve_inner(int pos, T &val) {
        while (basis[pos] % val == 0)
            basis[pos] /= val;
        vector<T> curr_basis = {basis[pos], val};
        int c_ptr = 1;
        while (c_ptr < curr_basis.size()) {
            for (int i = 0; i < c_ptr; i++) {
                reduce_pair(curr_basis[i], curr_basis[c_ptr]);
                if (curr_basis[c_ptr] == 1) break;
                if (curr_basis[i] == 1) continue;
                T g = gcd(curr_basis[c_ptr], curr_basis[i]);
                if (g > 1) {
                    curr_basis[c_ptr] /= g;
                    curr_basis[i] /= g;
                    curr_basis.push_back(g);
                }
            }
            ++c_ptr;
        }
        basis[pos] = curr_basis[0];
        val = curr_basis[1];
        for (int i = 2; i < curr_basis.size(); i++)
            if (curr_basis[i] > 1) basis.push_back(curr_basis[i]);
        if (basis[pos] == 1) {
            swap(basis[pos], basis.back());
            basis.pop_back();
        }
    }
    void add_element(T val) {
        for (int i = 0; i < basis.size(); i++) {
            reduce_pair(val, basis[i]);
            if (basis[i] == 1) {
                swap(basis[i], basis.back());
                basis.pop_back();
                continue;
            }
            if (val == 1) return;
            if (gcd(basis[i], val) > 1) solve_inner(i, val);
        }
        if (val > 1) basis.push_back(val);
    }
    void verify_basis() {
        for (int i = 0; i < basis.size(); i++) {
            for (int j = i + 1; j < basis.size(); j++) {
                assert(gcd(basis[i], basis[j]) == 1);
            }
        }
    }
    void verify_element(T ele) {
        for (auto &x : basis) {
            while (ele % x == 0)
                ele /= x;
        }
        assert(ele == 1);
    }
    auto factorization(T ele) {
        vector<int> factors(basis.size());
        for (int i = 0; i < basis.size(); i++) {
            while (ele % basis[i] == 0) {
                factors[i]++;
                ele /= basis[i];
            }
        }
        return factors;
    }
};
