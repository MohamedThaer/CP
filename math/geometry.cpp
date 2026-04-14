


namespace geometry {
    using Type = long double;
    const Type EPS = 1e-9, PI = acosl(-1.0l), E = expl(1.0l);

    // istream &operator>>(istream &in, __float128 &x) { long double y; return in >> y, x = y, in; }
    // ostream &operator<<(ostream &out, __float128 x) { return out << (long double)x; }

    struct eps {
        Type a;
        eps(auto a) : a(a) {}

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
        point(auto x, auto y) : x(x), y(y) {
            // normalize();
        }

        void normalize(int ignore = 9) {
            Type t = pow(10, ignore);
            x = __float128(__int128_t((__float128)x * t)) / t;
            y = __float128(__int128_t((__float128)y * t)) / t;
        }

        point operator+=(point p) { return *this = *this + p; }
        point operator-=(point p) { return *this = *this - p; }
        point operator*=(Type x) { return *this = *this * x; }
        point operator/=(Type x) { return *this = *this / x; }
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

        friend bool operator<(const point &a, const point &b) { return (eps)a.x < b.x || ((eps)a.x == b.x && a.y < b.y); }

        friend istream &operator>>(istream &in, point &p) { return in >> p.x >> p.y; }
        friend ostream &operator<<(ostream &out, const point &p) { return out << p.x << " " << p.y << '\n'; }
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
    void polar_sort(vector<point> &v, point p = {0, 0}) { // sort points in counterclockwise with respect to point p
        sort(v.begin(), v.end(), [&](point a, point b) {
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
    point linearTransfo(point p, point q, point r, point fp, point fq) {
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

    struct triangle {
        static Type triangle_area(Type b, Type h) {
            return b * h / 2;
        }
        static Type triangle_area_3sides(Type a, Type b, Type c) {
            Type s = (a + b + c) / 2;
            return sqrtl(s * (s - a) * (s - b) * (s - c));
        }
        static Type triangle_area_2sides_angle(Type a, Type b, Type C) {
            return a * b * sinl(C) / 2;
        }
        static Type triangle_area_medians(Type a, Type b, Type c) {
            return 4 * triangle_area_3sides(a, b, c) / 3;
        }

        /*
            angles: ABC
            sides: abc
            sin rule: sin(A)/a = sin(B)/b = sin(C)/c
            cos rule: a^2 = b^2 + c^2 - 2*b*c*cos(A)
        */

        /*
            three cases:
            - 1 side, 2 angle
            - 2 side, 1 angle
            - 3 side, 0 angle
            angles are in rad
            put known parameters and for unknown put 0
        */
        static auto triangle_solver(Type a, Type b, Type c, Type A, Type B, Type C) {
        L:;
            if (a && b && c) {
                A = acosl((b * b + c * c - a * a) / (2. * b * c));
                B = acosl((a * a + c * c - b * b) / (2. * a * c));
                C = acosl((a * a + b * b - c * c) / (2. * a * b));
            }
            if (!A + !B + !C == 1) {
                Type third = PI - A - B - C;
                if (!A) A = third;
                if (!B) B = third;
                if (!C) C = third;
            }

            if (A && B && C) {
                if (!a) {
                    if (b) a = b * sinl(A) / sinl(B);
                    if (c) a = c * sinl(A) / sinl(C);
                }
                if (!b) {
                    if (a) b = a * sinl(B) / sinl(A);
                    if (c) b = c * sinl(B) / sinl(C);
                }
                if (!c) {
                    if (a) c = a * sinl(C) / sinl(A);
                    if (b) c = b * sinl(C) / sinl(B);
                }
            }

            if (!A + !B + !C == 2) {
                if (A) {
                    if (!a) a = sqrtl(b * b + c * c - 2 * b * c * cosl(A));
                    if (!b) C = asinl(c * sinl(A) / a);
                    if (!c) B = asinl(b * sinl(A) / a);
                }
                if (B) {
                    if (!b) b = sqrtl(a * a + c * c - 2 * a * c * cosl(B));
                    if (!a) C = asinl(c * sinl(B) / b);
                    if (!c) A = asinl(a * sinl(B) / b);
                }
                if (C) {
                    if (!c) c = sqrtl(a * a + b * b - 2 * a * b * cosl(C));
                    if (!a) B = asinl(b * sinl(C) / c);
                    if (!b) A = asinl(a * sinl(C) / c);
                }
                goto L;
            }
            return tuple{a, b, c, A, B, C};
        }
    };

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

    struct polygon {
        vector<point> v;
        polygon(vector<point> v) : v(v) {}

        polygon(vector<Type> sides, vector<Type> angles) : v(1, point(0, 0)) {
            /* angles[i] is between sides[i] and sides[i + 1] */
            /* angles are in rad */
            Type dir = 0;
            for (int i = 1; i < sides.size(); ++i) {
                v.push_back(v.back() + point(cosl(dir), sinl(dir)) * sides[i - 1]);
                dir += PI - angles[i - 1];
            }
        }

        __float128 area() {
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
            sort(p.begin(), p.end(), [](point a, point b) { return pair{a.y, a.x} < pair{b.y, b.x}; });
            p.erase(unique(p.begin(), p.end()), p.end());
            if (include_colinear && (eps)area() == 0) return p;

            sort(p.begin() + 1, p.end(), [&](point a, point b) {
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
            auto reorder_convex = [&](vector<point> &v) {
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
        Type polygon_max_dis(polygon &b) { // O(n)
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

        // tuple<a, b, c, d>
        // from a to b ---antipodal with--- from c to d
        vector<tuple<int, int, int, int>> all_anti_podal() {
            int n = v.size();
            vector<tuple<int, int, int, int>> result;

            auto nx = [&](int i) { return (i + 1) % n; };
            auto pv = [&](int i) { return (i - 1 + n) % n; };

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

        auto cmp = [&](point a, point b) { return pair{a.y, a.x} < pair{b.y, b.x}; };
        vector<set<point, decltype(cmp)>> useful(2, set<point, decltype(cmp)>(cmp));

        Type ans = numeric_limits<Type>::max() - INT_MAX;
        int j[2]{}, i = 0;
        for (auto [p, t] : v) { // [point, type]
            int &cur = j[!t];
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

        auto cmp = [&](point a, point b) {
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

    struct circle {
        point c;
        Type r;

        circle() {}
        circle(point c, Type r) : c(c), r(r) {}

        circle(point a, point b, point c) {  // circumCircle
            b -= a, c -= a;                  // consider coordinates relative to A
            assert(b.cross_product(c) != 0); // no circumcircle if A,B,C aligned
            this->c = a + perpendicular(b * c.dot_product(c) - c * b.dot_product(b)) / b.cross_product(c) / (Type)2;
            r = magnitude(perpendicular(b * c.dot_product(c) - c * b.dot_product(b)) / b.cross_product(c) / (Type)2);
        }

        static circle inscribed_circle(point a, point b, point c) {
            Type m = atan2l(b.y - a.y, b.x - a.x), n = atan2l(c.y - a.y, c.x - a.x);
            line u(a, a + point(cosl((n + m) / 2.0), sinl((n + m) / 2.0)));

            m = atan2l(a.y - b.y, a.x - b.x), n = atan2l(c.y - b.y, c.x - b.x);
            line v(b, b + point(cosl((n + m) / 2.0), sinl((n + m) / 2.0)));

            point cen = u.inter(v).second;
            Type r = segment(a, b).point_dis(cen);
            return circle(cen, r);
        }

        // given n points, find the minimum enclosing circle of the points
        // call convex_hull() before this for faster solution
        // expected O(n)
        static circle minimum_enclosing_circle(vector<point> p) {
            random_shuffle(p.begin(), p.end());
            int n = p.size();
            circle c(p[0], 0);
            for (int i = 1; i < n; i++) {
                if (sgn(dis(c.c, p[i]) - c.r) > 0) {
                    c = circle(p[i], 0);
                    for (int j = 0; j < i; j++) {
                        if (sgn(dis(c.c, p[j]) - c.r) > 0) {
                            c = circle((p[i] + p[j]) / 2, dis(p[i], p[j]) / 2);
                            for (int k = 0; k < j; k++) {
                                if (sgn(dis(c.c, p[k]) - c.r) > 0) {
                                    c = circle(p[i], p[j], p[k]);
                                }
                            }
                        }
                    }
                }
            }
            return c;
        }

        Type area() { return r * r * PI; }
        Type perimeter() { return 2 * PI * r; }
        Type sector_area(Type angle) { return r * r * angle / 2; }
        Type arc_perimeter(Type angle) { return angle * r; }

        vector<point> line_intersect(line l) {
            vector<point> ret;
            Type h2 = r * r - l.sqDist(c);
            if (h2 >= 0) {                                          // the line touches the circle
                point p = l.proj(c);                                // point P
                point h = l.v * (Type)(sqrtl(h2) / magnitude(l.v)); // vector parallel to l, of length h
                if ((eps)h2 != 0) {
                    ret = {p - h, p + h};
                }
                else {
                    ret = {p};
                }
            }
            return ret;
        }

        vector<point> circle_intersect(circle cr) {
            vector<point> ret;
            point d = cr.c - c;
            Type d2 = d.dot_product(d);
            if (d2 == 0) {
                assert(r != cr.r);
                return ret;
            } // concentric crcles
            Type pd = (d2 + r * r - cr.r * cr.r) / 2; // = |O_1P| * d
            Type h2 = r * r - pd * pd / d2;           // = hˆ2
            if (h2 >= 0) {
                point p = c + d * pd / d2, h = perpendicular(d) * sqrtl(h2 / d2);
                if ((eps)h2 != 0) {
                    ret = {p - h, p + h};
                }
                else {
                    ret = {p};
                }
            }
            return ret;
        }

        Type intersection_area(circle cr) {
            Type d = dis(c, cr.c);
            if (d >= r + cr.r) {
                return 0;
            }
            if (d <= fabsl(r - cr.r)) {
                return min(area(), cr.area());
            }
            Type alpha = 2 * acosl(clamp((d * d + r * r - cr.r * cr.r) / (2 * d * r), (Type)-1, (Type)1));
            Type beta = 2 * acosl(clamp((d * d + cr.r * cr.r - r * r) / (2 * d * cr.r), (Type)-1, (Type)1));
            return (r * r * (alpha - sinl(alpha)) + cr.r * cr.r * (beta - sinl(beta))) / 2;
        }

        vector<pair<point, point>> tangents(circle cr, bool inner) {
            vector<pair<point, point>> ret;
            if (inner) cr.r = -cr.r;
            point d = cr.c - c;
            Type dr = r - cr.r, d2 = d.dot_product(d), h2 = d2 - dr * dr;
            if (d2 == 0 || h2 < 0) {
                assert(h2 != 0);
                return ret;
            }
            for (Type sign : {-1, 1}) {
                point v = (d * dr + perpendicular(d) * sqrtl(h2) * sign) / d2;
                ret.push_back({c + v * r, cr.c + v * cr.r});
                if ((eps)h2 == 0) break;
            }
            return ret;
        }
    };

    struct Halfplane {
        // 'p' is a passing point of the line and 'pq' is the direction vector of the line.
        point p, pq;
        Type angle;

        Halfplane() {}
        Halfplane(const point &a, const point &b) : p(a), pq(b - a) {
            angle = atan2l(pq.y, pq.x);
        }

        // Check if point 'r' is outside this half-plane.
        // Every half-plane allows the region to the LEFT of its line.
        bool out(const point &r) {
            return pq.cross_product(r - p) < -EPS;
        }

        // Comparator for sorting.
        bool operator<(const Halfplane &e) const {
            return angle < e.angle;
        }

        // Intersection point of the lines of two half-planes. It is assumed they're never parallel.
        friend point inter(Halfplane s, Halfplane t) {
            Type alpha = (t.p - s.p).cross_product(t.pq) / s.pq.cross_product(t.pq);
            return s.p + (s.pq * alpha);
        }

        static polygon hp_intersect(vector<Halfplane> &H) {
            const Type inf = 1e18;
            point box[4] = {// Bounding box in CCW order
                            point(inf, inf),
                            point(-inf, inf),
                            point(-inf, -inf),
                            point(inf, -inf)};

            for (int i = 0; i < 4; i++) { // Add bounding box half-planes.
                Halfplane aux(box[i], box[(i + 1) % 4]);
                H.push_back(aux);
            }

            // Sort by angle and start algorithm
            sort(H.begin(), H.end());
            deque<Halfplane> dq;
            int len = 0;
            for (int i = 0; i < H.size(); i++) {

                // Remove from the back of the deque while last half-plane is redundant
                while (len > 1 && H[i].out(inter(dq[len - 1], dq[len - 2]))) {
                    dq.pop_back();
                    --len;
                }

                // Remove from the front of the deque while first half-plane is redundant
                while (len > 1 && H[i].out(inter(dq[0], dq[1]))) {
                    dq.pop_front();
                    --len;
                }

                // Special case check: Parallel half-planes
                if (len > 0 && fabsl(H[i].pq.cross_product(dq[len - 1].pq)) < EPS) {
                    // Opposite parallel half-planes that ended up checked against each other.
                    if (H[i].pq.dot_product(dq[len - 1].pq) < 0.0)
                        return vector<point>();

                    // Same direction half-plane: keep only the leftmost half-plane.
                    if (H[i].out(dq[len - 1].p)) {
                        dq.pop_back();
                        --len;
                    }
                    else
                        continue;
                }

                // Add new half-plane
                dq.push_back(H[i]);
                ++len;
            }

            // Final cleanup: Check half-planes at the front against the back and vice-versa
            while (len > 2 && dq[0].out(inter(dq[len - 1], dq[len - 2]))) {
                dq.pop_back();
                --len;
            }

            while (len > 2 && dq[len - 1].out(inter(dq[0], dq[1]))) {
                dq.pop_front();
                --len;
            }

            // Report empty intersection if necessary
            if (len < 3) return vector<point>();

            // Reconstruct the convex polygon from the remaining half-planes.
            vector<point> ret(len);
            for (int i = 0; i + 1 < len; i++) {
                ret[i] = inter(dq[i], dq[i + 1]);
            }
            ret.back() = inter(dq[len - 1], dq[0]);
            return ret;
        }
    };
}
using namespace geometry;
