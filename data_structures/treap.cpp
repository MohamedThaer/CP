class Treap {
private:
    struct node {
        int priority;
        int size = 1;
        node *left = nullptr, *right = nullptr;
        int key = 0;
        int val = 0;

        int64_t sum = 0;
        int64_t lazy_add = 0;

        node(int k, int v) : priority(rand()), key(k), val(v), sum(v) {}

        void apply(int64_t v) {
            val += v;
            sum += size * v;
            lazy_add += v;
        }

        void pull() {
            size = 1;
            sum = val;
            if (left) {
                size += left->size;
                sum += left->sum;
            }
            if (right) {
                size += right->size;
                sum += right->sum;
            }
        }

        void push() {
            if (lazy_add != 0) {
                if (left) left->apply(lazy_add);
                if (right) right->apply(lazy_add);
                lazy_add = 0;
            }
        }
    };

    node *root = nullptr;

    node *merge(node *left, node *right) {
        if (!left) return right;
        if (!right) return left;

        left->push();
        right->push();

        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            left->pull();
            return left;
        }
        else {
            right->left = merge(left, right->left);
            right->pull();
            return right;
        }
    }

    void split(node *t, int key, node *&left, node *&right) {
        if (!t) {
            left = right = nullptr;
            return;
        }

        t->push();

        if (t->key <= key) {
            left = t;
            split(t->right, key, left->right, right);
            left->pull();
        }
        else {
            right = t;
            split(t->left, key, left, right->left);
            right->pull();
        }
    }

    // FIXED: Simple and correct merge_treap that handles duplicates properly
    node *merge_treap(node *left, node *right) {
        if (!left) return right;
        if (!right) return left;

        left->push();
        right->push();

        // Always ensure right tree values overwrite left tree values for duplicate keys
        if (left->priority > right->priority) {
            // Remove any node in right with the same key as left
            node *right_low, *right_high, *dup;
            split(right, left->key - 1, right_low, right_high);
            split(right_high, left->key, dup, right_high);

            // If duplicate exists, update left's value and delete duplicate
            if (dup) {
                left->val = dup->val;
                left->sum = left->val; // Will be updated in pull()
                delete dup;
            }

            // Recursively merge subtrees
            left->left = merge_treap(left->left, right_low);
            left->right = merge_treap(left->right, right_high);
            left->pull();
            return left;
        }
        else {
            // Remove any node in left with the same key as right
            node *left_low, *left_high, *dup;
            split(left, right->key - 1, left_low, left_high);
            split(left_high, right->key, dup, left_high);

            // Delete duplicate from left if it exists
            if (dup) {
                delete dup;
            }

            // Recursively merge subtrees
            right->left = merge_treap(left_low, right->left);
            right->right = merge_treap(left_high, right->right);
            right->pull();
            return right;
        }
    }

    void dfs(node *t, vector<pair<int, int>> &result) const {
        if (!t) return;
        t->push();
        dfs(t->left, result);
        result.push_back({t->key, t->val});
        dfs(t->right, result);
    }

    Treap(node *r) : root(r) {}

public:
    Treap() { srand(time(0)); }

    void insert(int key, int val) {
        node *left, *mid, *right;
        split(root, key - 1, left, mid);
        split(mid, key, mid, right);
        if (mid) {
            delete mid;
        }
        root = merge(merge(left, new node(key, val)), right);
    }

    Treap erase(int l, int r) {
        node *left, *mid, *right;
        split(root, l - 1, left, mid);
        split(mid, r, mid, right);
        root = merge(left, right);
        return Treap(mid);
    }

    void update(int l, int r, int64_t val) {
        node *left, *mid, *right;
        split(root, l - 1, left, mid);
        split(mid, r, mid, right);
        if (mid) {
            mid->apply(val);
        }
        root = merge(merge(left, mid), right);
    }

    node query(int l, int r) {
        node *left, *mid, *right;
        split(root, l - 1, left, mid);
        split(mid, r, mid, right);

        node res(0, 0);
        if (mid) {
            mid->push();
            res = *mid;
        }

        root = merge(merge(left, mid), right);
        return res;
    }

    vector<pair<int, int>> get() {
        vector<pair<int, int>> result;
        dfs(root, result);
        return result;
    }

    void combine(Treap x) {
        root = merge_treap(root, x.root);
        x.root = nullptr;
    }

    int size() const {
        return root ? root->size : 0;
    }

    bool empty() const {
        return root == nullptr;
    }
};
