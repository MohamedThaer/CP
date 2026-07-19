// BASE 0
template<typename T>
struct OfflineBIT2D {
    int n;
    vector<vector<int> > vals;
    vector<vector<T> > bit;

    int ind(const vector<int> &v, int x) {
        return upper_bound(v.begin(), v.end(), x) - v.begin() - 1;
    }

    OfflineBIT2D() : n(0) {}

    OfflineBIT2D(int n, vector<vector<int> > &todo) : n(n), vals(n + 1), bit(n + 1) {
        sort(todo.begin(), todo.end(),
             [](const vector<int> &a, const vector<int> &b) {
                 return a[1] < b[1];
             });

        for (int i = 1; i <= n; i++)vals[i].push_back(0);

        for (auto &p: todo) {
            int r = p[0] + 1, c = p[1] + 1;
            for (; r <= n; r += r & -r) {
                if (vals[r].back() != c)
                    vals[r].push_back(c);
            }
        }

        for (int i = 1; i <= n; i++)
            bit[i].resize(vals[i].size());
    }

    void add(int r, int c, T val) {
        r++, c++;
        for (; r <= n; r += r & -r) {
            int i = ind(vals[r], c);
            for (; i < (int) bit[r].size(); i += i & -i)
                bit[r][i] += val;
        }
    }

    T sum(int r, int c) {
        r++, c++;
        T sum = 0;
        for (; r > 0; r -= r & -r) {
            int i = ind(vals[r], c);
            for (; i > 0; i -= i & -i)
                sum += bit[r][i];
        }
        return sum;
    }

    T query(int r1, int c1, int r2, int c2) {
        return sum(r2, c2)
               - sum(r2, c1 - 1)
               - sum(r1 - 1, c2)
               + sum(r1 - 1, c1 - 1);
    }
};
