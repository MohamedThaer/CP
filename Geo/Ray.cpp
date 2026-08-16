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

    pair<bool, point> inter(line l) {
        Type d = v.cross_product(l.v);
        if ((eps)d == 0) return {false, point()};
        return {true, (l.v * c - v * l.c) / d};
    }

    Type side(point p) { return v.cross_product(p) - c; }
    Type dist(point p) { return fabsl(side(p)) / magnitude(v); }
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
};

struct ray {
    point a, b;

    ray(point a, point b) : a(a), b(b) {}

    bool on_ray(point p) {
        return segment(a, b).on_segment(p) || segment(a, p).on_segment(b);
    }

    Type point_dis(point p) {
        if ((eps)vec(a, b).dot_product(vec(a, p)) <= 0) {
            return dis(a, p);
        }
        else {
            return line(a, b).dist(p);
        }
    }

    pair<int, point> ray_intersect(ray r) {
        if (on_ray(r.a)) return {1, r.a};
        if (r.on_ray(a)) return {1, a};

        point p = line(a, b).inter(line(r.a, r.b)).second;
        if (!on_ray(p) || !r.on_ray(p)) {
            return {0, point()};
        }
        return {1, p}; 
    }

    Type ray_dis(ray r) {
        if (ray_intersect(r).first) {
            return 0;
        }

        return min(point_dis(r.a), r.point_dis(a));
    }
};