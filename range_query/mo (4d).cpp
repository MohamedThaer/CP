template <typename T>
class Mo4D {
private:
    struct Query {
        int l1, r1, l2, r2, id;
    };

    vector<Query> Q;
    vector<T> v;

    void Add1(int idx) {
    }

    void Remove1(int idx) {
    }

    void Add2(int idx) {
    }

    void Remove2(int idx) {
    }

    int64_t Calculate(Query q) {
        int res = 0;
        return res;
    }

public:
    Mo4D(const vector<T> &v) : v(v) {}

    void add_query(int l1, int r1, int l2, int r2) {
        Q.push_back({l1, r1, l2, r2, Q.size()});
    }

    vector<int64_t> mo_process() {
        int B = pow(v.size(), 3.0 / 4.0);
        sort(Q.begin(), Q.end(), [&](Query a, Query b) {
            return make_tuple(a.l1 / B, a.r1 / B, a.l2 / B, a.r2, a.l1, a.r1, a.l2) //
                   < make_tuple(b.l1 / B, b.r1 / B, b.l2 / B, b.r2, b.l1, b.r1, b.l2);
        });

        int l1 = 0, r1 = -1, l2 = 0, r2 = -1;
        vector<int64_t> ans(Q.size());
        for (auto q : Q) {
            // Adjust first interval [l1, r1]
            while (r1 < q.r1)
                Add1(++r1);
            while (l1 > q.l1)
                Add1(--l1);
            while (r1 > q.r1)
                Remove1(r1--);
            while (l1 < q.l1)
                Remove1(l1++);

            // Adjust second interval [l2, r2]
            while (r2 < q.r2)
                Add2(++r2);
            while (l2 > q.l2)
                Add2(--l2);
            while (r2 > q.r2)
                Remove2(r2--);
            while (l2 < q.l2)
                Remove2(l2++);

            ans[q.id] = Calculate(q);
        }

        return ans;
    }
};
