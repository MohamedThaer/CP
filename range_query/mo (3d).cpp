
class MO_update {
private:
    struct Query {
        int l, r, t, id;
    };

    struct Modify {
        int pos, original_val, new_val;
    };

    struct Operation {
        int type, x, y;
    };

    vector<int> v;
    vector<Query> queries;
    vector<Modify> modifications;
    vector<Operation> operations;

    void AddElement(int x) {
    }

    void RemoveElement(int x) {
    }

    int Calculate(Query q) {
        int answer = 1;
        return answer;
    }

    void ApplyModification(int t, int l, int r) {
        if (l <= modifications[t].pos && modifications[t].pos <= r) {
            RemoveElement(v[modifications[t].pos]);
            AddElement(modifications[t].new_val);
        }
        v[modifications[t].pos] = modifications[t].new_val;
    }

    void RevertModification(int t, int l, int r) {
        if (l <= modifications[t].pos && modifications[t].pos <= r) {
            RemoveElement(v[modifications[t].pos]);
            AddElement(modifications[t].original_val);
        }
        v[modifications[t].pos] = modifications[t].original_val;
    }

public:
    MO_update(const vector<int> &v) : v(v) {}

    void add_query(int type, int x, int y) {
        operations.push_back({type, x, y});
    }

    void mo_process() {
        int n = v.size(), q = operations.size(), block_size = pow(n, 2.0 / 3.0);

        int t = -1;
        for (int i = 0; i < q; ++i) {
            auto [type, x, y] = operations[i];
            if (type == 1) {
                queries.push_back({x, y, t, i});
            }
            else if (type == 2) {
                modifications.push_back({x, v[x], y});
                t++;
                v[x] = y;
            }
        }

        sort(queries.begin(), queries.end(), [&](Query a, Query b) {
            return make_tuple(a.l / block_size, a.r / block_size, a.t) < make_tuple(b.l / block_size, b.r / block_size, b.t);
        });

        vector<int> result(q, -1);
        int l = 0, r = -1;
        for (int i = 0; i < queries.size(); ++i) {
            while (queries[i].r > r)
                AddElement(v[++r]);
            while (queries[i].l < l)
                AddElement(v[--l]);
            while (queries[i].r < r)
                RemoveElement(v[r--]);
            while (queries[i].l > l)
                RemoveElement(v[l++]);
            while (queries[i].t > t)
                ApplyModification(++t, l, r);
            while (queries[i].t < t)
                RevertModification(t--, l, r);

            result[queries[i].id] = Calculate(queries[i]);
        }

        for (auto it : result) {
            if (it != -1) {
                cout << it << '\n';
            }
        }
    }
};
