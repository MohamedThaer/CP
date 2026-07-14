dr_string h(vector<int>(2e5));
struct node {
        int priority;
        int size = 1;
        node *left = nullptr, *right = nullptr;

        int val;
        int64_t hash, hash_rev;
        
        bool lazy_rev = false;
        int64_t lazy_add = 0;

        node(int v) : priority(rng()), val(v), hash(v), hash_rev(v) {}

        // edit in reverse function 
        void apply_rev() {
            lazy_rev = !lazy_rev;
            swap(hash, hash_rev);
        }

        // edit in update function
        void apply_add(int64_t v) {
            val += v;
            lazy_add += v;
        }

        void pull() {
            size = 1;
            hash = val;
            hash_rev = val;

            if (left) {
                hash = h.merge(left->hash, val, 1);
                hash_rev = h.merge(val, left->hash_rev, left->size);
                size += left->size;
            }
            if (right) {
                hash = h.merge(hash, right->hash, right->size);
                hash_rev = h.merge(right->hash_rev, hash_rev, size); 
                size += right->size;
            }
        }

        void push() {
            if (lazy_rev) {
                swap(left, right);
                if (left) left->apply_rev();
                if (right) right->apply_rev();
                lazy_rev = false;
            }
            if (lazy_add != 0) {
                if (left) left->apply_add(lazy_add);
                if (right) right->apply_add(lazy_add);
                lazy_add = 0;
            }
        }
    };
