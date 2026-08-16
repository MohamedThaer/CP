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

    line(Type a, Type b, Type _c) {
        v = {b, -a};
        c = _c;
    }

    line(point p, point q) {
        v = q - p;
        c = v.cross_product(p);
    }

    Type side(point p) { return v.cross_product(p) - c; }
    Type dist(point p) { return fabsl(side(p)) / magnitude(v); }
    bool cmpProj(point p, point q) { return v.dot_product(p) < v.dot_product(q); }

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

    int64_t get_lattice() {
        point v = vec(a, b);
        return gcd((int64_t)fabsl(v.x), (int64_t)fabsl(v.y)) + 1;
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
};

struct polygon {
    vector<point> v;
    polygon(vector<point> v) : v(v) {}

    /*
 REGULAR POLYGON + BASIC GEOMETRY TEMPLATE

 Regular Polygon Definition:
 - All sides equal
 - All interior angles equal
 - All vertices lie on a circumcircle

 Important formulas:

 1) Sum of interior angles
     (n - 2) * 180

 2) Each interior angle
     ((n - 2) * 180) / n

 3) Central angle between two vertices
     360 / n   degrees
     2π / n    radians

 If polygon center = (cx, cy) and radius = r

     xi = cx + r * cos(2πi / n)
     yi = cy + r * sin(2πi / n)

  r=s/2sin(π/n)


long double polygonregular_area(long double s, int n){
    return (n * s * s) / (4 * tan(M_PI / n));
}
    ​


     This generates all vertices.

      */
    polygon(vector<Type> sides, vector<Type> angles) : v(1, point(0, 0)) {
        /* angles[i] is between sides[i] and sides[i + 1] */
        /* angles are in rad */
        Type dir = 0;
        for (int i = 1; i < sides.size(); ++i) {
            v.push_back(v.back() + point(cosl(dir), sinl(dir)) * sides[i - 1]);
            dir += PI - angles[i - 1];
        }
    }

    Type area() {
        __float128 area = 0;
        for (int i = 0, n = v.size(); i < n; ++i) {
            area += v[i].cross_product(v[(i + 1) % n]);
        }
        return fabsl(area / 2);
    }

    bool is_counter() {
        __float128 dir = 0;
        for (int i = 0, n = v.size(); i < n; ++i) {
            dir += v[i].cross_product(v[(i + 1) % n]);
        }
        return (eps)dir >= 0;
    }

    // 1: inside
    // 0: on
    // -1: outside
    int polygon_orient(point p) {
        int n = v.size(), cnt = 0;
        for (int i = 0; i < n; ++i) {
            if (segment(v[i], v[(i + 1) % n]).on_segment(p)) return 0;
            point mn = min(v[i], v[(i + 1) % n]), mx = max(v[i], v[(i + 1) % n]);
            cnt += (mn.x <= p.x && p.x < mx.x && orient(p, mn, mx) == -1);
        }
        return (cnt & 1) ? 1 : -1;
    }

    pair<int64_t, int64_t> polygon_lattice() {
        int64_t boundary = 0;
        for (int i = 0, n = v.size(); i < n; ++i) {
            boundary += segment(v[i], v[(i + 1) % n]).get_lattice() - 1;
        }
        int64_t interior = (2 * area() - boundary) / 2 + 1;
        return {interior, boundary};
    }

    // 0: not convex
    // 1: counter clockwise
    // -1: clockwise
    int is_convex() {
        if (v.size() <= 2) return 1;
        int pos = 0, neg = 0;
        for (int i = 0, n = v.size(); i < n; i++) {
            int o = orient(v[i], v[(i + 1) % n], v[(i + 2) % n]);
            pos += (o > 0);
            neg += (o < 0);
        }
        if (pos && neg) return 0;
        return neg ? -1 : 1;
    }

    // p is sorted counter clockwise
    int convex_orient(point a) {
        int n = v.size();
        if (n <= 3) return polygon_orient(a);

        int l = 1, r = n - 2, ans = 1;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (orient(v[0], v[mid], a) == 1) {
                l = mid + 1;
                ans = mid;
            }
            else {
                r = mid - 1;
            }
        }

        vector<int> t{0, ans, ans + 1, 0};
        for (int i = 0; i < 3; ++i) {
            if ((t[i] + 1) % n == t[i + 1] && segment(v[t[i]], v[t[i + 1]]).on_segment(a)) {
                return 0;
            }
        }
        if (a == v[0]) return 0;

        return polygon({v[0], v[ans], v[ans + 1]}).polygon_orient(a) >= 0 ? 1 : -1;
    }

    // return hull is ccw
    polygon convex_hull(int include_colinear = 1) {
        vector<point> p = v;
        sort(p.begin(), p.end(), [] (point a, point b) { return pair{a.y, a.x} < pair{b.y, b.x}; });
        p.erase(unique(p.begin(), p.end()), p.end());
        if (include_colinear && (eps)area() == 0) return p;

        sort(p.begin() + 1, p.end(), [&] (point a, point b) {
            int o = orient(p[0], a, b);
            if (!o) {
                a -= p[0], b -= p[0];
                return a.dot_product(a) < b.dot_product(b);
            }
            return o > 0;
        });

        vector<point> hull;
        for (auto cur : p) {
            while (hull.size() > 1 && orient(hull.end()[-2], hull.end()[-1], cur) < !include_colinear) {
                hull.pop_back();
            }
            hull.push_back(cur);
        }

        if (include_colinear) {
            point last = hull.back();
            for (int i = 1; i < p.size(); ++i) {
                if (p[i] != last && is_colinear(p[0], last, p[i])) {
                    hull.push_back(p[i]);
                }
            }
        }

        return hull;
    }

    // p, q must be counter clockwise O(n + m)
    static polygon minkowski(polygon P, polygon Q) {
        auto reorder_convex = [&] (vector<point>& v) {
            // the first vertex must be the lowest
            int pos = 0;
            for (int i = 1; i < v.size(); i++) {
                if (v[i].y < v[pos].y || (v[i].y == v[pos].y && v[i].x < v[pos].x))
                    pos = i;
            }
            rotate(v.begin(), v.begin() + pos, v.end());
            // we must ensure cyclic indexing
            v.push_back(v[0]);
            v.push_back(v[1]);
        };
        reorder_convex(P.v);
        reorder_convex(Q.v);
        // main part
        vector<point> res;
        int i = 0, j = 0;
        while (i < P.v.size() - 2 || j < Q.v.size() - 2) {
            res.push_back(P.v[i] + Q.v[j]);
            Type cp = (P.v[i + 1] - P.v[i]).cross_product(Q.v[j + 1] - Q.v[j]);
            if (cp >= 0 && i < P.v.size() - 2) ++i;
            if (cp <= 0 && j < Q.v.size() - 2) ++j;
        }
        return res;
    }

    // maximum distance from a convex polygon to another convex polygon
    // both must be counter clockwise
    Type polygon_max_dis(polygon& b) { // O(n)
        int n = v.size(), m = b.v.size();
        Type ans = 0;
        if (n < 3 || m < 3) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++)
                    ans = max(ans, (v[i] - b.v[j]).dot_product(v[i] - b.v[j]));
            }
            return sqrtl(ans);
        }
        if (v[0].x > b.v[0].x) return b.polygon_max_dis(*this);
        int i = 0, j = 0, step = n + m + 10;
        while (j + 1 < m && b.v[j].x < b.v[j + 1].x)
            j++;
        while (step--) {
            if ((v[(i + 1) % n] - v[i]).cross_product(b.v[(j + 1) % m] - b.v[j]) >= 0)
                j = (j + 1) % m;
            else
                i = (i + 1) % n;
            ans = max(ans, (v[i] - b.v[j]).dot_product(v[i] - b.v[j]));
        }
        return sqrtl(ans);
    }
    // Minimum distance between two convex polygons in O(N + M)
    static Type min_dis_convex_polygons(polygon P, polygon Q) {
        vector<point> negQ_v;
        for (int i = 0; i < Q.v.size(); ++i) {
            // Negate all points of Q to create -Q
            negQ_v.push_back(point(-Q.v[i].x, -Q.v[i].y));
        }
        polygon negQ(negQ_v);

        // Minkowski Difference is essentially P + (-Q)
        polygon MD = minkowski(P, negQ);

        point origin(0, 0);

        // If origin is inside or on the boundary of MD, the polygons intersect
        if (MD.polygon_orient(origin) >= 0) {
            return 0.0;
        }

        // If origin is outside, find the minimum distance to any edge of MD
        Type min_dist = numeric_limits<Type>::max();
        for (int i = 0, n = MD.v.size(); i < n; ++i) {
            segment edge(MD.v[i], MD.v[(i + 1) % n]);
            min_dist = min(min_dist, edge.point_dis(origin));
        }

        return min_dist;
    }
    // tuple<a, b, c, d>
    // from a to b ---antipodal with--- from c to d
    vector<tuple<int, int, int, int>> all_anti_podal() {
        int n = v.size();
        vector<tuple<int, int, int, int>> result;

        auto nx = [&] (int i) { return (i + 1) % n; };
        auto pv = [&] (int i) { return (i - 1 + n) % n; };

        vector<bool> vis(n, false);

        for (int p1 = 0, p2 = 0; p1 < n; ++p1) {
            point base = v[nx(p1)] - v[p1];

            while (p2 == p1 || p2 == nx(p1) ||
                   sgn(base.cross_product(v[nx(p2)] - v[p2])) ==
                       sgn(base.cross_product(v[p2] - v[pv(p2)]))) {
                p2 = nx(p2);
            }

            if (vis[p1]) continue;
            vis[p1] = true;

            // edge (p1->nx(p1)) vs vertex (p2)
            result.push_back({p1, nx(p1), p2, p2});

            // also mark from other endpoint of edge
            result.push_back({nx(p1), p1, p2, p2});

            // if edges parallel: edge-edge pair
            if (sgn(base.cross_product(v[nx(p2)] - v[p2])) == 0) {
                result.push_back({p1, nx(p1), p2, nx(p2)});
                result.push_back({nx(p1), p1, nx(p2), p2});
                vis[p2] = true;
            }
        }

        return result;
    }

    // maximum distance from any point on the perimeter to another point on the perimeter
    Type diameter() {
        int n = v.size();
        if (n == 1) return 0;
        if (n == 2) return dis(v[0], v[1]);
        Type ans = 0;
        int i = 0, j = 1;
        while (i < n) {
            while ((v[(i + 1) % n] - v[i]).cross_product(v[(j + 1) % n] - v[j]) >= 0) {
                ans = max(ans, dis_sq(v[i], v[j]));
                j = (j + 1) % n;
            }
            ans = max(ans, dis_sq(v[i], v[j]));
            i++;
        }
        return sqrtl(ans);
    }

    // minimum distance between two parallel lines (non necessarily axis parallel)
    // such that the polygon can be put between the lines
    Type width() {
        int n = v.size();
        if (n <= 2) return 0;
        Type ans = numeric_limits<Type>::max();
        int i = 0, j = 1;
        while (i < n) {
            while ((v[(i + 1) % n] - v[i]).cross_product(v[(j + 1) % n] - v[j]) >= 0)
                j = (j + 1) % n;
            ans = min(ans, line(v[i], v[(i + 1) % n]).dist(v[j]));
            i++;
        }
        return ans;
    }

    // minimum perimeter
    Type minimum_enclosing_rectangle() {
        int n = v.size();
        if (n <= 2) return 2 * dis(v.front(), v.back());
        int mndot = 0;
        Type tmp = (v[1] - v[0]).dot_product(v[0]);
        for (int i = 1; i < n; i++) {
            if ((v[1] - v[0]).dot_product(v[i]) <= tmp) {
                tmp = (v[1] - v[0]).dot_product(v[i]);
                mndot = i;
            }
        }
        Type ans = numeric_limits<Type>::max();
        int i = 0, j = 1, mxdot = 1;
        while (i < n) {
            point cur = v[(i + 1) % n] - v[i];
            while (cur.cross_product(v[(j + 1) % n] - v[j]) >= 0)
                j = (j + 1) % n;
            while (v[(mxdot + 1) % n].dot_product(cur) >= v[mxdot].dot_product(cur))
                mxdot = (mxdot + 1) % n;
            while (v[(mndot + 1) % n].dot_product(cur) <= v[mndot].dot_product(cur))
                mndot = (mndot + 1) % n;
            ans = min(ans, (Type)2 * ((v[mxdot].dot_product(cur) / magnitude(cur) - v[mndot].dot_product(cur) / magnitude(cur)) + line(v[i], v[(i + 1) % n]).dist(v[j])));
            i++;
        }
        return ans;
    }
};

// max manhattan distance ----> +x+y +x-y -x+y -x-y
// max euclidean distance ----> polygon(convex(a)).polygon_max_dis(convex(b))
// min manhattan distance ---->
// min euclidean distance ---->
Type min_euclidean_dis_2(vector<point> a, vector<point> b) {
    vector<pair<point, int>> v;
    for (auto it : a) {
        v.push_back({it, 0});
    }
    for (auto it : b) {
        v.push_back({it, 1});
    }
    int n = v.size();
    sort(v.begin(), v.end());

    auto cmp = [&] (point a, point b) { return pair{a.y, a.x} < pair{b.y, b.x}; };
    vector<set<point, decltype(cmp)>> useful(2, set<point, decltype(cmp)>(cmp));

    Type ans = numeric_limits<Type>::max() - INT_MAX;
    int j[2]{}, i = 0;
    for (auto [p, t] : v) { // [point, type]
        int& cur = j[!t];
        while (cur < i && (v[cur].second != !t || (p.x - v[cur].first.x) * (p.x - v[cur].first.x) > ans)) {
            if (v[cur].second == !t) useful[!t].erase(v[cur].first);
            cur++;
        }

        int64_t sq = sqrtl(ans);
        auto s = useful[!t].lower_bound({0, p.y - sq});
        auto e = useful[!t].upper_bound({0, p.y + sq});
        for (auto it = s; it != e; it++) {
            ans = min(ans, vec(*it, p).dot_product(vec(*it, p)));
        }

        useful[t].insert(p);
        i++;
    }
    return ans;
}

Type min_euclidean_dis_2(vector<point> v) {
    sort(v.begin(), v.end());

    auto cmp = [&] (point a, point b) {
        if (a.y != b.y) return a.y < b.y;
        return a.x < b.x;
    };
    set<point, decltype(cmp)> useful(cmp);

    Type ans = 8e18;
    int p = 0;
    for (int i = 0; i < v.size(); ++i) {
        while (p < i && (v[i].x - v[p].x) * (v[i].x - v[p].x) > ans) {
            useful.erase(v[p++]);
        }

        auto s = useful.lower_bound({-2e9, v[i].y - ans});
        auto e = useful.upper_bound({2e9, v[i].y + ans});
        for (auto it = s; it != e; it++) {
            ans = min(ans, vec(*it, v[i]).dot_product(vec(*it, v[i])));
        }

        useful.insert(v[i]);
    }
    return ans;
}
Type max_euclidean_distance(vector<point> v) {
    polygon poly(v);

    // get convex hull excluding collinear points
    polygon hull = poly.convex_hull(0);

    // get maximum distance using rotating calipers
    return hull.diameter();
}

Type min_manhattan_distance(vector<point> v) {
    sort(v.begin(), v.end(), [](point a, point b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });

    auto cmp = [](point a, point b) {
        if (a.y != b.y) return a.y < b.y;
        return a.x < b.x;
    };
    set<point, decltype(cmp)> useful(cmp);

    Type ans = 8e18;
    int p = 0;

    for (int i = 0; i < v.size(); ++i) {
        // remove points that are too far on the x-axis
        while (p < i && (v[i].x - v[p].x) >= ans) {
            useful.erase(v[p++]);
        }

        // search in the active window on the y-axis
        auto s = useful.lower_bound({-2e9, v[i].y - ans});
        auto e = useful.upper_bound({2e9, v[i].y + ans});

        for (auto it = s; it != e; it++) {
            // calculate manhattan distance
            Type current_dist = abs(it->x - v[i].x) + abs(it->y - v[i].y);
            ans = min(ans, current_dist);
        }

        useful.insert(v[i]);
    }
    return ans;
}

Type max_manhattan_distance(const vector<point>& v) {
    if (v.empty()) return 0;

    Type max_sum = -8e18, min_sum = 8e18;
    Type max_diff = -8e18, min_diff = 8e18;
    
    for (const auto& p : v) {
        // calculate x + y
        max_sum = max(max_sum, p.x + p.y);
        min_sum = min(min_sum, p.x + p.y);

        // calculate x - y
        max_diff = max(max_diff, p.x - p.y);
        min_diff = min(min_diff, p.x - p.y);
    }

    // the max distance is the maximum of the absolute differences
    return max(max_sum - min_sum, max_diff - min_diff);
}