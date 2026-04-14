class range_fenwick_2d {
private:
    fenwick_2d t1, t2, t3, t4;

    void add(int x, int y, int64_t val) {
        t1.add(x, y, val);
        t2.add(x, y, val * (x - 1));
        t3.add(x, y, val * (y - 1));
        t4.add(x, y, val * (x - 1) * (y - 1));
    }

public:
    range_fenwick_2d(int n, int m) : t1(n, m), t2(n, m), t3(n, m), t4(n, m) {}

    void add(int x1, int y1, int x2, int y2, int64_t val) {
        add(x1, y1, val);
        add(x1, y2 + 1, -val);
        add(x2 + 1, y1, -val);
        add(x2 + 1, y2 + 1, val);
    }

    int64_t get(int x, int y) {
        int64_t res = 0;
        res += t1.get(x, y) * x * y;
        res -= t2.get(x, y) * y;
        res -= t3.get(x, y) * x;
        res += t4.get(x, y);
        return res;
    }

    int64_t query(int x1, int y1, int x2, int y2) {
        return get(x2, y2) - get(x1 - 1, y2) - get(x2, y1 - 1) + get(x1 - 1, y1 - 1);
    }
};
