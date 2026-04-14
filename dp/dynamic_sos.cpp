
class dynamic_sos {
private:
    int l_size, r_size;                      // sizes of parts
    vector<vector<int64_t>> a;               // we split vector of size 2^k in 2^(k/2) vectors of size 2^(k/2)
    vector<vector<pair<int, int64_t>>> push_val; // temporary vector for pushing values
    // Main idea of optimization of memory to O(2^(k/2)) is to resize a[left_part] only when we make a query
    // When we insert we push value to some temporary vector push_val[left_part]

public:
    dynamic_sos(int k) {
        l_size = k / 2, r_size = k - l_size;
        // split bits in half
        a.resize(1 << l_size);
        push_val.resize(1 << l_size);
        // resize both vectors
    }
    void add(int x, int64_t val = 1) {
        int left_part = x & ((1 << l_size) - 1), right_part = ((x - left_part) >> l_size);
        for (int mask = 0; mask < (1 << l_size); mask++)
            if ((mask & left_part) == left_part)
                push_val[mask].push_back({right_part, val});
        // for each UPmask of the left part, add right part
        // Note that instead of just adding to the vector of size 2^k, we push it in some temporary vector
    }
    int64_t submask_sum(int x) {
        int64_t ans = 0;
        int left_part = x & ((1 << l_size) - 1), right_part = ((x - left_part) >> l_size);
        if (a[left_part].size() == 0) a[left_part].resize(1 << r_size); // if we have not queried this left part before
        for (auto &x : push_val[left_part])
            a[left_part][x.first] += x.second; // push everything that we have not added
        push_val[left_part].clear();
        for (int mask = 0; mask < (1 << r_size); mask++)
            if ((mask & right_part) == mask)
                ans += a[left_part][mask];
        // iterate over right part
        return ans;
    }
};
