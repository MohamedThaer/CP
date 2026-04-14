
/**
 * @brief A robust multiset hashing template.
 *
 * This structure computes a hash for a multiset of 64-bit integers.
 * It is order-independent and handles duplicates.
 *
 * How it works:
 * 1. Each element 'val' is passed through `hash_int` to get a
 * well-distributed 64-bit hash value.
 * 2. The `current_hash` is the sum of all these individual hashes.
 * 3. Addition is commutative, so order doesn't matter.
 * 4. Adding a duplicate `val` adds its hash again, changing the sum,
 * which correctly handles duplicates.
 * 5. All arithmetic is done modulo 2^64 via uint64_t overflow.
 */
class MultisetHash {
private:
    uint64_t current_hash;

    /**
     * @brief A high-quality 64-bit hash function (splitmix64).
     * It scrambles the input 'x' to produce a well-distributed hash.
     */
    static uint64_t hash_int(uint64_t x) {
        static const uint64_t FIXED_SEED = chrono::steady_clock::now().time_since_epoch().count();
        x += 0x9e3779b97f4a7c15 + FIXED_SEED;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

public:
    MultisetHash() : current_hash(0) {}
    void add(int64_t val) { current_hash += hash_int(val); }
    void remove(int64_t val) { current_hash -= hash_int(val); }
    uint64_t get() const { return current_hash; }
    bool operator==(const MultisetHash &other) const { return current_hash == other.current_hash; }
};

class MultisetHashArray : MultisetHash {
private:
    vector<uint64_t> h = {0};

public:
    MultisetHashArray(const auto &a) {
        for (auto it : a) {
            add(it);
            h.push_back(get());
        }
    }

    uint64_t get_subarray(int l, int r) { return h[r + 1] - h[l]; }
};
