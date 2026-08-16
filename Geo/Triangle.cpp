#pragma once

#include <bits/stdc++.h>
using namespace std;
using Type = long double;
const Type EPS = 1e-9, PI = acosl(-1.0l), E = expl(1.0l);

// istream &operator>>(istream &in, __float128 &x) { long double y; return in >> y, x = y, in; }
// ostream &operator<<(ostream &out, __float128 x) { return out << (long double)x; }

struct Triangle {
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
    static Type triangle_area_3points(Type x1, Type y1, Type x2, Type y2, Type x3, Type y3) {
        // Calculate the area using the 2D coordinates of the 3 points
        return abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0;
    }
    static bool is_valid_triangle_sides(Type a, Type b, Type c) {
        // A valid triangle must have positive side lengths
        // and satisfy the triangle inequality theorem
        return (a > 0 && b > 0 && c > 0) && (a + b > c) && (a + c > b) && (b + c > a);
    }
    static bool is_right_triangle(pair<int,int> A, pair<int,int> B, pair<int,int> C) {
        pair<int,int> AB = {B.first - A.first, B.second - A.second};
        pair<int,int> BC = {C.first - B.first, C.second - B.second};
        pair<int,int> CA = {A.first - C.first, A.second - C.second};

        return dot_product(AB, BC) == 0 || dot_product(BC, CA) == 0 || dot_product(CA, AB) == 0;
    }
    static bool is_valid_triangle_points(Type x1, Type y1, Type x2, Type y2, Type x3, Type y3) {
        // A triangle is valid if the points are not collinear
        // This is true when the doubled area is not equal to zero
        Type area_doubled = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
        return area_doubled != 0;
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