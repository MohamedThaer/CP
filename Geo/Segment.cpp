#pragma once

#include <bits/stdc++.h>
using namespace std;
using Type = long double;
const Type EPS = 1e-9, PI = acosl(-1.0l), E = expl(1.0l);

// istream &operator>>(istream &in, __float128 &x) { long double y; return in >> y, x = y, in; }
// ostream &operator<<(ostream &out, __float128 x) { return out << (long double)x; }

struct eps {
    Type a;
    eps(Type a) : a(a) {}

    static int cmp(Type a, Type b) { return fabsl(a - b) <= EPS ? 0 : (a > b ? 1 : -1); }

    bool operator>(Type b) { return cmp(a, b) == 1; }
    bool operator<=(Type b) { return cmp(a, b) != 1; }
    bool operator<(Type b) { return cmp(a, b) == -1; }
    bool operator>=(Type b) { return cmp(a, b) != -1; }
    bool operator==(Type b) { return cmp(a, b) == 0; }
    bool operator!=(Type b) { return cmp(a, b) != 0; }
};

struct point {
    Type x, y;

    point() {}
    point(Type x, Type y) : x(x), y(y) {
        // normalize();
    }

    void normalize(int ignore = 9) {
        Type t = pow(10, ignore);
        x = __float128(__int128_t((__float128)x * t)) / t;
        y = __float128(__int128_t((__float128)y * t)) / t;
    }

    friend point operator+(point a, point b) { return point(a.x + b.x, a.y + b.y); }
    friend point operator-(point a, point b) { return point(a.x - b.x, a.y - b.y); }
    friend point operator*(point a, Type b) { return point(a.x * b, a.y * b); }
    friend point operator/(point a, Type b) { return point(a.x / b, a.y / b); }

    Type cross_product(point p) { return x * p.y - p.x * y; }
    Type dot_product(point p) { return x * p.x + y * p.y; }

    bool operator==(point p) { return (eps)x == p.x && (eps)y == p.y; }
    bool operator!=(point p) { return !(*this == p); }

    point translate(point v) { return *this + v; }
    point translate(Type mag, Type theta) { return *this + point(cosl(theta), sinl(theta)) * mag; }
    point scale_around(point c, Type factor) { return c + (*this - c) * factor; }
    point rotate_around(point c, Type theta) {
        point p = *this - c;
        Type cos_t = cosl(theta), sin_t = sinl(theta);
        return c + point(p.x * cos_t - p.y * sin_t, p.x * sin_t + p.y * cos_t);
    }

    friend bool operator<(const point& a, const point& b) { return (eps)a.x < b.x || ((eps)a.x == b.x && a.y < b.y); }

};

int sgn(Type a) { return eps::cmp(a, 0); }

point vec(point a, point b) { return b - a; }

point perpendicular(point vec) { return {-vec.y, vec.x}; }

Type magnitude(point v) { return sqrtl(v.dot_product(v)); }
Type dis_sq(point a, point b) { return vec(a, b).dot_product(vec(a, b)); }
Type dis(point a, point b) { return magnitude(vec(a, b)); }

// 1: point on the left
// 0: point on the line
// -1: point on the right
int orient(point a, point b, point c) { return sgn(vec(a, b).cross_product(vec(a, c))); }
bool is_colinear(point a, point b, point c) { return orient(a, b, c) == 0; }

struct line {
    point v;
    Type c;

    line(point v, Type c) : v(v), c(c) {}

    // from equation ax+by = c
    line(Type a, Type b, Type _c) {
        v = {b, -a};
        c = _c;
    }

    // line from two points
    line(point p, point q) {
        v = q - p;
        c = v.cross_product(p);
    }

    pair<point, point> get_points() { // extract any two points from this line
        point p, q;
        Type a = -v.y, b = v.x; // ax + by = c
        if (sgn(a) == 0) {
            p = point(0, c / b);
            q = point(1, c / b);
        }
        else if (sgn(b) == 0) {
            p = point(c / a, 0);
            q = point(c / a, 1);
        }
        else {
            p = point(0, c / b);
            q = point(1, (c - a) / b);
        }
        return {p, q};
    }

    Type slope() { return v.y / v.x; }
    Type intercept() { return c / v.x; }
    Type side(point p) { return v.cross_product(p) - c; }
    Type dist(point p) { return fabsl(side(p)) / magnitude(v); }
    Type sqDist(point p) { return side(p) * side(p) / v.dot_product(v); }
    bool cmpProj(point p, point q) { return v.dot_product(p) < v.dot_product(q); }
    static line prepThrought(point p) { return {p, p + perpendicular(p)}; }
    line translate(point t) { return {v, c + v.cross_product(t)}; }
    line shiftLeft(Type dist) { return {v, c + dist * magnitude(v)}; }
    point proj(point p) { return p - perpendicular(v) * side(p) / v.dot_product(v); }
    point refl(point p) { return p - perpendicular(v) * (Type)2.0 * side(p) / v.dot_product(v); }

    pair<bool, point> inter(line l) {
        Type d = v.cross_product(l.v);
        if ((eps)d == 0) return {false, point()};
        return {true, (l.v * c - v * l.c) / d};
    }

    static line bisector(line l1, line l2, bool interior) {
        assert(l1.v.cross_product(l2.v) != 0); // l1 and l2 cannot be parallel!
        Type sign = interior ? 1 : -1;
        return {l2.v / magnitude(l2.v) + l1.v / magnitude(l1.v) * sign,
            l2.c / magnitude(l2.v) + l1.c / magnitude(l1.v) * sign};
    }

    // bac angle, from side ab to ac counter clockwise
    static line angle_bisector(point a, point b, point c) {
        return bisector(line(a, b), line(a, c), 1);
    }
};

struct segment {
    point a, b;
    segment(point a, point b) : a(a), b(b) {}

    bool in_disk(point c) {
        return (eps)vec(c, a).dot_product(vec(c, b)) <= 0;
    }

    bool on_segment(point c) {
        return is_colinear(a, b, c) && in_disk(c);
    }

    bool is_perpendicular(segment s) {
        return (eps)vec(a, b).dot_product(vec(s.a, s.b)) == 0;
    }

    bool is_parallel(segment s) {
        return (eps)vec(a, b).cross_product(vec(s.a, s.b)) == 0;
    }

    bool is_intersect(segment s) {
        return on_segment(s.a) || on_segment(s.b) || s.on_segment(a) || s.on_segment(b) || (orient(a, b, s.a) != orient(a, b, s.b) && orient(s.a, s.b, a) != orient(s.a, s.b, b));
    }

    int64_t get_lattice() {
        point v = vec(a, b);
        return gcd((int64_t)fabsl(v.x), (int64_t)fabsl(v.y)) + 1;
    }

    // 0 : not intersect
    // 1 : strictly intersect
    // 2 : overlap
    // 3 : intersect at endpoint
    tuple<int, point, point> segmentIntersection(segment s) {
        if (!is_intersect(s)) {

            return {0, point(), point()};
        }

        if (is_parallel(s)) {
            if (on_segment(s.a) && on_segment(s.b)) {
                return {2 + (s.a == s.b), s.a, s.b};
            }
            if (s.on_segment(a) && s.on_segment(b)) {
                return {2 + (a == b), a, b};
            }
            point p1 = s.on_segment(a) ? a : b;
            point p2 = on_segment(s.a) ? s.a : s.b;
            return {2 + (p1 == p2), p1, p2};
        }

        point p = line(a, b).inter(line(s.a, s.b)).second;
        if (p != a && p != b && p != s.a && p != s.b) {
            return {1, p, p};
        }
        else {
            return {3, p, p};
        }
    }

    Type point_dis(point p) {
        if (a != b) {
            line l(a, b);
            if (l.cmpProj(a, p) && l.cmpProj(p, b)) { // if closest to projection
                return l.dist(p);                     // output distance to line
            }
        }
        return min(magnitude(p - a), magnitude(p - b)); // otherwise distance to A or B
    }

    Type seg_dis(segment s) {
        if (is_intersect(s)) return 0;
        return min({point_dis(s.a), point_dis(s.b), s.point_dis(a), s.point_dis(b)});
    }
};