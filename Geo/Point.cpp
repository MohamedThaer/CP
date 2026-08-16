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

bool half(point p) { return p.y > 0.0 || (p.y == 0.0 && p.x < 0.0); }
void polar_sort(vector<point>& v, point p = {0, 0}) { // sort points in counterclockwise with respect to point p
    sort(v.begin(), v.end(), [&] (point a, point b) {
        a -= p, b -= p;
        if (half(a) != half(b)) return half(a) < half(b);
        int o = orient({0, 0}, a, b);
        if (!o) {
            return a.dot_product(a) < b.dot_product(b);
        }
        return 0 < o;
    });
}

// point p has image fp, point q has image fq then what is image of point r
point linearTransform(point p, point q, point r, point fp, point fq) {
    point pq = q - p, num{pq.cross_product(fq - fp), pq.dot_product(fq - fp)};
    return fp + point{(r - p).cross_product(num), (r - p).dot_product(num)} / pq.dot_product(pq);
}

// angle with positive x axis
Type angle(point p) {
    Type angle = atan2l(p.y, p.x);
    if (angle < 0) angle += 2 * PI;
    return angle;
}

Type min_angle(point v, point w) {
    return acosl(clamp(v.dot_product(w) / magnitude(v) / magnitude(w), (Type)-1.0, (Type)1.0));
}

// calc BAC angle
Type orientedAngle(point a, point b, point c) {
    if (orient(a, b, c) >= 0)
        return min_angle(b - a, c - a);
    else
        return 2 * PI - min_angle(b - a, c - a);
}

// amplitude travelled around point A, from P to Q
Type angleTravelled(point a, point p, point q) {
    Type ampli = min_angle(p - a, q - a);
    if (orient(a, p, q) > 0)
        return ampli;
    else
        return -ampli;
}

// check p in between angle(bac) counter clockwise
bool inAngle(point a, point b, point c, point p) {
    Type abp = orient(a, b, p), acp = orient(a, c, p), abc = orient(a, b, c);
    if (abc < 0) swap(abp, acp);
    return (abp >= 0 && acp <= 0) ^ (abc < 0);
}

Type rad_to_deg(Type x) {
    if (x < 0) x += 2 * PI;
    return x * 180 / PI;
}

Type deg_to_rad(Type x) { return x * PI / 180; }
