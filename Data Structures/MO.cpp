struct MO {
    struct Query {
        int l, r, id;
    };

    vector<int> v;
    vector<Query> Q;
    int64_t res = 0;

    // vector<int> freq;
    MO(const vector<int> &v) : v(v) {
        //freq.assign(MAX_VALUE, 0);
    }

    void add_query(int l, int r) {
        Q.push_back({l, r, (int) Q.size()});
    }

    void add(int ind) {
        int num = v[ind];
    }

    void remove(int ind) {
        int num = v[ind];
    }

    vector<int64_t> mo_process() {
        vector<int64_t> ans(Q.size());
        int SQ = max(1, (int) sqrt(v.size()));
        sort(Q.begin(), Q.end(), [&](query a, query b) {
            return pair{a.l / SQ, a.r} < pair{b.l / SQ, b.r};
        });

        int l = 0, r = -1;
        for (const auto &q: Q) {
            while (r < q.r) add(++r);
            while (l > q.l) add(--l);
            while (r > q.r) remove(r--);
            while (l < q.l) remove(l++);
            ans[q.id] = res;
        }

        return ans;
    }
};
