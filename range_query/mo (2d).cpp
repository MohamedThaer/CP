template <class M>
class MO {
private:
    struct query {
        int l, r, q_indx;
    };

    vector<M> v;
    vector<query> Q;

    int64_t res = 0;
    void add(int ind) {
    }

    void remove(int ind) {
    }

    int64_t calc(...) {
        return res;
    }

public:
    MO(const vector<M> &v) : v(v) {}

    void add_query(int l, int r) {
        Q.push_back({l, r, Q.size()});
    }

    void mo_process() {
        vector<int64_t> ans(Q.size());
        int SQ = sqrt(v.size());
        sort(Q.begin(), Q.end(), [&](query a, query b) {
            return pair{a.l / SQ, a.r} < pair{b.l / SQ, b.r};
        });

        int l = 0, r = -1;
        for (auto [L, R, q_indx] : Q) {
            while (r < R)
                add(++r);
            while (l > L)
                add(--l);
            while (r > R)
                remove(r--);
            while (l < L)
                remove(l++);
            ans[q_indx] = calc();
        }

        for (auto it : ans) {
            cout << it << '\n';
        }
    }
};
