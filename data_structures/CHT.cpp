
struct Line {
    mutable int64_t m, c, p;

    bool operator<(const Line &o) const {
        return m < o.m;
    }

    bool operator<(int64_t x) const {
        return p < x;
    }

    int64_t val(int64_t x) const {
        return m * x + c;
    }
};

class CHT : multiset<Line, less<>> {
public:
    enum Mode {
        Min = -1,
        Max = 1,
    };

private:
    static constexpr int64_t LLimit = LLONG_MIN, RLimit = LLONG_MAX;
    Mode mode;

    int64_t div(int64_t a, int64_t b) {
        return a / b - ((a ^ b) < 0 && a % b);
    }

    bool is_intersect(iterator x, iterator y) {
        if (y == end()) {
            return x->p = RLimit, 0;
        }
        if (x->m == y->m) {
            x->p = (x->c > y->c ? RLimit : LLimit);
        }
        else {
            x->p = div(y->c - x->c, x->m - y->m);
        }
        return x->p >= y->p;
    }

public:
    CHT(Mode mode) : mode(mode) {}

    void add(int64_t m, int64_t c) {
        m *= mode, c *= mode;
        auto z = insert({m, c, 0}), y = z++, x = y;
        while (is_intersect(y, z))
            z = erase(z);
        if (x != begin() && is_intersect(--x, y))
            is_intersect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p)
            is_intersect(x, erase(y));
    }

    int64_t query(int64_t x) {
        auto l = *lower_bound(x);
        return mode * l.val(x);
    }
};
