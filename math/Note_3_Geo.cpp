0) Global types/constants
using Type = long double;
Used for all geometry computations.

Constants
EPS = 1e-9
PI = acosl(-1.0l)
E = expl(1.0l)
1) struct eps (EPS-safe comparisons)
Purpose
Wrap a floating value and compare it to another with tolerance EPS.

Members
Type a;
stored value.

eps(auto a)
Input: numeric value
Output: wrapper object
static int cmp(Type a, Type b)
Input: a, b
Output:
0 if |a-b| <= EPS
1 if a > b
-1 if a < b
Operators with Type b
operator>(b), operator<(b), operator<=(b), operator>=(b), operator==(b), operator!=(b)
Input: b
Output: bool using cmp
Example
cpp
Type x = 0.1L + 0.2L;
if ((geometry::eps)x == 0.3L) { /* treated equal */ }
2) struct point
Purpose
Represents both points and vectors (2D).

Fields
Type x, y
Constructors
point()
Default (does not initialize in your code; be careful if you use it).
point(auto x, auto y)
Input: x, y
Output: point(x,y)
Methods / operators
void normalize(int ignore = 9)
Input: ignore digits
Output: modifies x,y
Idea: rounds to ignore decimal digits using __float128 / integer truncation.
Use when: you want to reduce noise after many operations.
Pitfall: uses truncation (not true rounding). Also, mixing __float128 is unusual but OK.
Compound ops
operator+=(point p), operator-=(point p), operator*=(Type x), operator/=(Type x)
Input: p or scalar
Output: updated point (returns *this)
Friend arithmetic
operator+(point a, point b)
operator-(point a, point b)
operator*(point a, Type b)
operator/(point a, Type b)
Output: new point
Type cross_product(point p)
Input: vector p
Output: cross = x*p.y - p.x*y
Meaning: signed area of parallelogram; orientation test.
Type dot_product(point p)
Input: vector p
Output: dot = x*p.x + y*p.y
Equality
bool operator==(point p) EPS-based on both coordinates
bool operator!=(point p) negation
Transforms
point translate(point v) → *this + v
point translate(Type mag, Type theta) → move by polar vector (mag, angle)
point scale_around(point c, Type factor) → scale around center c
point rotate_around(point c, Type theta) → rotate around center c by theta radians
Sorting
friend bool operator<(const point &a, const point &b)
Orders by x (EPS), then by y (raw compare for y after x equal by EPS)
Used in: convex hull, closest pair, sorting.
I/O
operator>> reads x y
operator<< prints x y\n
Examples
cpp
point a(1,2), b(3,4);
point c = a + b;               // (4,6)
Type dot = a.dot_product(b);   // 11
Type cr  = a.cross_product(b); // -2
point r = point(1,0).rotate_around({0,0}, PI/2); // approx (0,1)
3) Basic helper functions (global inside namespace)
int sgn(Type a)
Input: a
Output: sign of a w.r.t EPS: -1/0/+1
Uses eps::cmp(a,0).
point vec(point a, point b)
Output: b-a
point perpendicular(point vec)
Output: (-y, x) (90° CCW rotation)
Distances
Type magnitude(point v) → |v|
Type dis_sq(point a, point b) → squared distance
Type dis(point a, point b) → distance
Orientation
int orient(point a, point b, point c)
Output:
+1 if c is left of a→b
0 collinear
-1 right
computed by sgn(cross(b-a, c-a))
bool is_colinear(point a, point b, point c)
true if orient == 0
Polar sorting helpers
bool half(point p)
Output: true if p in upper half-plane or on +x side rule:
p.y > 0 OR (p.y == 0 AND p.x < 0)
void polar_sort(vector<point> &v, point p = {0,0})
Input: points v, center p
Output: sorts v CCW around p
Tie-break: closer point first when collinear
Example

cpp
vector<point> v = {{1,0},{0,1},{-1,0},{0,-1}};
polar_sort(v); // should order around origin
Linear transform
point linearTransfo(point p, point q, point r, point fp, point fq)
Meaning: you define a linear transform such that:
image(p) = fp
image(q) = fq
then returns image(r)
Inputs: p,q,r and fp,fq
Output: mapped point
Use-case: geometry mapping / affine tricks.
Assumption: pq.dot_product(pq) not 0 (p != q)
Angles
Type angle(point p)
angle with +x in [0, 2π)
Type min_angle(point v, point w)
Output: smallest angle between v and w in [0, π]
Uses acos(clamped cosine)
Type orientedAngle(point a, point b, point c)
In code: if orient(a,b,c) >= 0 returns min_angle(b-a, c-a)
else returns 2π - min_angle(...)
Be careful: naming suggests angle at A but function uses vectors from a.
Output: oriented angle (0..2π)
Type angleTravelled(point a, point p, point q)
signed smallest angle from (p-a) to (q-a)
positive if orient(a,p,q) > 0 else negative
bool inAngle(point a, point b, point c, point p)
Checks if point p lies inside directed angle ∠BAC (counterclockwise logic).
Output: bool
Type rad_to_deg(Type x), Type deg_to_rad(Type x)
convert units; rad_to_deg also normalizes negative to [0,2π)
4) struct triangle
Purpose
Compute areas and solve triangle given partial data.

static Type triangle_area(Type b, Type h)
area = b*h/2
static Type triangle_area_3sides(Type a, Type b, Type c)
Heron’s formula
Assumption: valid triangle
static Type triangle_area_2sides_angle(Type a, Type b, Type C)
area = absin(C)/2
static Type triangle_area_medians(Type a, Type b, Type c)
Uses relation: area(medians) = 4/3 * area(sides=a,b,c)
static auto triangle_solver(Type a, Type b, Type c, Type A, Type B, Type C)
Inputs: sides a,b,c and angles A,B,C (radians). Unknowns are 0.
Output: tuple{a,b,c,A,B,C} with filled values.
Handles cases:
3 sides
2 angles + 1 angle missing
2 sides + 1 angle
Pitfalls:
Ambiguous SSA case may produce one of possible solutions only.
Uses asin and acos, sensitive to floating errors (clamp not used here).
Example

cpp
auto [a,b,c,A,B,C] = triangle::triangle_solver(3,4,5, 0,0,0);
// returns A≈0.6435, B≈0.9273, C≈1.5708
5) struct line
Representation in your code
direction vector v
constant c such that for point p: side(p) = v.cross(p) - c
Constructed from two points: v=q-p, c=v.cross(p)
Constructors
line(point v, Type c)
direct constructor.

line(Type a, Type b, Type _c) from ax + by = c
sets v = {b, -a}, c=_c
line(point p, point q)
through p and q
Methods
pair<point, point> get_points()
Output: two points on the line (good for debugging).
Type slope()
v.y/v.x (undefined if v.x ~ 0)
Type intercept()
c/v.x (only meaningful depending on representation; be careful)
Type side(point p)
signed value: >0 one side, <0 other side
Type dist(point p)
|side(p)| / |v|
Type sqDist(point p)
side(p)^2 / (v·v)
bool cmpProj(point p, point q)
compares projection along direction v using dot: v·p < v·q
static line prepThrought(point p)
returns line through p with direction perpendicular(p) using:
{p, p + perpendicular(p)}
Weird naming: it’s “perpendicular through p” but uses point as vector too.
line translate(point t)
shifts line by vector t
line shiftLeft(Type dist)
parallel line shifted left by dist
point proj(point p)
projection of point p onto line
point refl(point p)
reflection of p across line
pair<bool, point> inter(line l)
Output:
false if parallel (cross(v,l.v)==0)
else intersection point
static line bisector(line l1, line l2, bool interior)
returns angle bisector line (internal if interior=true else external)
Assumption: l1 and l2 not parallel
static line angle_bisector(point a, point b, point c)
bisector at vertex a between lines (a,b) and (a,c)
Examples

cpp
line l1({0,0},{1,1}); // y=x
line l2({0,1},{1,0}); // y=1-x
auto [ok,P] = l1.inter(l2); // P≈(0.5,0.5)

Type d = l1.dist({1,0}); // ≈ 0.7071
point pr = l1.proj({1,0}); // ≈ (0.5,0.5)
point rf = l1.refl({1,0}); // ≈ (0,1)
6) struct segment
Constructors
segment(point a, point b)
Methods
bool in_disk(point c)
checks if c is between endpoints by dot product:
(c-a)·(c-b) <= 0 (EPS)
used for “within bounding segment” check
bool on_segment(point c)
collinear(a,b,c) AND in_disk(c)
bool is_perpendicular(segment s)
dot( (b-a), (s.b-s.a) ) == 0
bool is_parallel(segment s)
cross( (b-a), (s.b-s.a) ) == 0
bool is_intersect(segment s)
true if:
any endpoint lies on the other segment OR
general proper intersection by orientation checks
int64_t get_lattice()
returns gcd(|dx|,|dy|)+1 (integer lattice points)
Assumption: endpoints are integer-valued (or you accept truncation by cast)
tuple<int, point, point> segmentIntersection(segment s)
Return codes (exactly as your code):

0: not intersect
1: strictly intersect at a point not equal to any endpoint
2: overlap segment (or same point overlap)
3: intersect at endpoint(s)
Output points:

For codes 1 and 3: returns {code, p, p}
For overlap: returns {2 or 3, p1, p2} with p1/p2 overlap endpoints
It returns 2 + (p1==p2) so overlap at a single point becomes code 3.
Type point_dis(point p)
distance from point to segment:
if projection lies between a and b, return distance to line
else min distance to endpoints
Type seg_dis(segment s)
distance between segments (0 if intersect)
else min of distances from endpoints to other segment
Examples

cpp
segment s1({0,0},{2,0});
segment s2({1,-1},{1,1});
auto [code,p,q] = s1.segmentIntersection(s2); // code=1 p=q=(1,0)

segment a({0,0},{4,0});
segment b({2,0},{6,0});
auto [c2,p2,q2] = a.segmentIntersection(b); // code=2, [2,0]-[4,0]

Type d = s1.point_dis({-1,2}); // sqrt(5)
7) struct ray
Constructor
ray(point a, point b) (direction is a→b)
Methods
bool on_ray(point p)
returns true if p is on segment(a,b) OR b lies on segment(a,p)
(i.e., p collinear with a->b and same direction)
Type point_dis(point p)
if p is “behind” start a (dot <= 0) → distance to a
else distance to infinite line(a,b)
pair<int, point> ray_intersect(ray r)
returns {1, intersection_point} if they intersect (including at starts)
else {0, _}
Type ray_dis(ray r)
returns 0 if rays intersect
else min(distance from this ray to r.a, distance from r to this.a)
Examples

cpp
ray r1({0,0},{1,0});  // +x
ray r2({2,1},{2,-1}); // vertical through x=2 (both directions? actually ray is downwards)
auto it = r1.ray_intersect(r2); // likely intersects at (2,0) only if r2 direction includes that

ray r3({2,1},{2,0}); // downwards, includes (2,0)
it = r1.ray_intersect(r3); // {1, (2,0)}
8) struct polygon
Constructors
polygon(vector<point> v)
Stores vertices.

polygon(vector<Type> sides, vector<Type> angles)
Builds polygon from side lengths and internal angles:

starts at (0,0)
walks edges using direction updates dir += PI - angles[i-1]
Assumption: sides/angles represent a valid polygon.
Methods
__float128 area()
returns absolute area using shoelace sum /2
uses __float128 for better precision
bool is_counter()
returns true if signed area sum >= 0
int polygon_orient(point p)
Point in polygon test (your implementation):

returns 0 if on boundary
otherwise uses a ray-crossing-like count cnt
returns 1 inside else -1 outside
Assumption: Works for general simple polygon; relies on x-range checks.

pair<int64_t,int64_t> polygon_lattice()
boundary points = sum over edges (lattice points on edge - 1)
interior points from Pick:
I = (2*area - B)/2 + 1
Assumption: polygon vertices are lattice points (integers).
int is_convex()
returns:
0 not convex
1 convex CCW
-1 convex CW
int convex_orient(point a)
Point in convex polygon using binary search:

returns 0 on boundary, 1 inside, -1 outside
Assumption: vertices are CCW and polygon is convex.
polygon convex_hull(int include_colinear = 1)
returns hull in CCW
Steps:
sort points by (y,x)
remove duplicates
sort by polar around p0
monotonic stack style building
if include_colinear, it appends extra collinear points on last edge
Pitfall in code: if (include_colinear && (eps)area() == 0) return p;
Here area() uses original polygon points, not the unique list p. If you call hull on arbitrary set, this check can be misleading. Usually you’d compute collinearity from the points set directly.
static polygon minkowski(polygon P, polygon Q)
Inputs: P.v and Q.v must be convex and CCW
Reorders each so lowest point first, appends v[0],v[1] for cyclic indexing
Merges edges by cross product sign
Output: resulting convex polygon (as vector of points)
Type polygon_max_dis(polygon &b)
Maximum distance between points on two convex polygons (returns distance, not squared)
If either has <3 vertices, brute O(nm)
Otherwise rotating calipers style heuristic (uses step = n+m+10)
Assumption: both convex, CCW; also expects v[0].x <= b.v[0].x (it swaps otherwise)
vector<tuple<int,int,int,int>> all_anti_podal()
returns antipodal relations used in calipers.
outputs tuples (a,b,c,d) meaning:
edge a→b antipodal with vertex/edge c→d (per your comment)
Type diameter()
returns max distance between any two points on convex polygon boundary
uses rotating calipers; returns sqrt(max squared)
Type width()
minimum width (min distance between two parallel supporting lines)
returns actual distance (not squared)
Type minimum_enclosing_rectangle()
returns minimum perimeter (per your comment)
Assumption: convex polygon
Important: implementation is non-trivial; uses rotating calipers tracking:
farthest point in direction (mxdot)
lowest dot (mndot)
antipodal for height (j)
returns 2*(width_in_direction + height) minimized
Polygon Examples
Example A: point in polygon
cpp
polygon sq({{0,0},{2,0},{2,2},{0,2}});
sq.polygon_orient({1,1}); // 1
sq.polygon_orient({2,1}); // 0
sq.polygon_orient({3,1}); // -1
Example B: convex hull
cpp
vector<point> pts = {{0,0},{2,0},{2,2},{0,2},{1,1}};
polygon hull = polygon(pts).convex_hull();
Example C: diameter/width
cpp
polygon rect({{0,0},{3,0},{3,1},{0,1}});
Type d = rect.diameter(); // sqrt(10)
Type w = rect.width();    // 1
Example D: Minkowski
cpp
polygon P({{0,0},{1,0},{1,1},{0,1}});
polygon Q({{0,0},{2,0},{2,1},{0,1}});
polygon R = polygon::minkowski(P,Q);
9) Closest distance functions (VERY IMPORTANT: both return squared distance)
You have two overloads, both named min_euclidean_dis_2.
Both compute minimum squared Euclidean distance.

Type min_euclidean_dis_2(vector<point> a, vector<point> b)
Input: set A and set B
Output: minimum squared distance between a point in A and a point in B
Algorithm: sweep line with 2 sets (useful[0], useful[1]), removing points too far in x
Complexity: O((n+m) log(n+m)) average
Example

cpp
vector<point> A={{0,0},{10,0}};
vector<point> B={{5,1},{6,10}};
Type sq = min_euclidean_dis_2(A,B); // should be 26
Type d = sqrtl(sq);                // ≈ 5.099
Type min_euclidean_dis_2(vector<point> v)
Input: one set
Output: minimum squared distance among all pairs
Algorithm: sweep line using ordered set by y then x
Complexity: O(n log n) average
IMPORTANT BUG/NOTE IN YOUR CODE
Inside this function you do:

cpp
auto s = useful.lower_bound({-2e9, v[i].y - ans});
auto e = useful.upper_bound({2e9, v[i].y + ans});
But ans is squared distance, so y-window should be sqrt(ans), not ans.
This makes the window too large; it may still work but can degrade badly (and in worst-case it becomes slow).

Correct version usually:

use Type sq = sqrtl(ans);
y range: y ± sq
If you want, I can provide the corrected code snippet.

10) struct circle
Constructors
circle()
circle(point c, Type r)
circle(point a, point b, point c) // circumcircle
Input: triangle points a,b,c
Output: circle passing through all three
Assumption: not collinear (cross != 0 asserted)
Static constructors
static circle inscribed_circle(point a, point b, point c)
Input: triangle
Output: incircle (center + radius)
Uses intersection of two angle bisectors.
static circle minimum_enclosing_circle(vector<point> p)
Input: points
Output: minimum enclosing circle (expected O(n))
Uses randomized incremental:
if point outside current circle, rebuild circle from that point, then pairs, then triples.
Measures
Type area() → πr²
Type perimeter() → 2πr
Type sector_area(Type angle) → r²*angle/2
Type arc_perimeter(Type angle) → r*angle
Intersections
vector<point> line_intersect(line l)
Input: line l
Output: 0/1/2 points
vector<point> circle_intersect(circle cr)
Input: circle cr
Output: 0/1/2 points
Special: if same center, asserts radii differ.
Type intersection_area(circle cr)
Output: overlapping area of two circles
vector<pair<point, point>> tangents(circle cr, bool inner)
Input: other circle, inner for internal tangents
Output: list of tangent point pairs (on this circle, on other circle)
Circle Examples
Example: line-circle intersection
cpp
circle c({0,0}, 1);
line l({-2,0},{2,0});           // y=0
auto pts = c.line_intersect(l); // (-1,0), (1,0)
Example: circle-circle intersection
cpp
circle c1({0,0},1), c2({1,0},1);
auto pts = c1.circle_intersect(c2); // (0.5, ±sqrt(3)/2)
Example: MEC
cpp
vector<point> p={{0,0},{2,0},{0,2}};
circle mec = circle::minimum_enclosing_circle(p);
// center ≈ (1,1), r ≈ sqrt(2)
11) struct Halfplane + hp_intersect
Meaning
Each halfplane keeps the region to the LEFT of directed line p -> p+pq.

Fields
point p, pq
Type angle
Constructors
Halfplane()
Halfplane(const point &a, const point &b)
p=a, pq=b-a, angle=atan2(pq.y,pq.x)
Methods
bool out(const point &r)
Output: true if r is outside (right side) with EPS:
pq.cross(r-p) < -EPS
bool operator<(const Halfplane &e) const
sort by angle
friend point inter(Halfplane s, Halfplane t)
intersection point of boundary lines (assumes not parallel)
static polygon hp_intersect(vector<Halfplane> &H)
Input: vector of halfplanes H
Output: polygon intersection region (convex), or empty vector if infeasible
Internally:
adds huge bounding box (inf=1e18)
sorts halfplanes by angle
maintains deque and removes redundant planes
handles parallel planes (keeps leftmost)
reconstructs polygon from deque intersections
Halfplane Examples
Example: intersection of a rectangle [0,2]×[0,1]
We need 4 constraints; easiest is to define boundary direction so inside is left:

x ≥ 0: line from (0,0) to (0,1) has left side x ≤ 0 (wrong), so use (0,1)->(0,0)
x ≤ 2: use (2,0)->(2,1) left side x ≤ 2 (correct)
y ≥ 0: use (0,0)->(2,0) left side y ≥ 0?
Actually left of (0,0)->(2,0) is y>0, so correct for y ≥ 0.
y ≤ 1: use (2,1)->(0,1) left side y ≤ 1 (correct)
cpp
vector<Halfplane> H;
H.push_back(Halfplane({0,1},{0,0})); // x >= 0
H.push_back(Halfplane({2,0},{2,1})); // x <= 2
H.push_back(Halfplane({0,0},{2,0})); // y >= 0
H.push_back(Halfplane({2,1},{0,1})); // y <= 1

polygon P = Halfplane::hp_intersect(H);
// P.v should be 4 corners (order CCW), area≈2
Example: infeasible
x ≥ 1 and x ≤ 0

cpp
vector<Halfplane> H;
H.push_back(Halfplane({1,1},{1,0})); // x >= 1
H.push_back(Halfplane({0,0},{0,1})); // x <= 0 (direction gives left side x<=0)
polygon P = Halfplane::hp_intersect(H); // empty
Quick “What returns squared vs actual distance” (in your code)
dis_sq(a,b) returns squared
min_euclidean_dis_2(...) returns squared
segment::point_dis, segment::seg_dis return actual distance
polygon::diameter returns actual distance
polygon::polygon_max_dis returns actual distance
circle::intersection_area returns area (actual)
Contest workflow (how to use this template safely)
Use orient, on_segment, segmentIntersection for intersection tasks.
For polygon queries:
If polygon convex: prefer convex_orient.
If not convex: polygon_orient.
For convex hull + farthest pair:
build hull then diameter().
For closest pair:
remember function returns squared distance; take sqrt at end if needed.
consider fixing the single-set version y-window (sqrt(ans)).
For halfplane intersection:
make sure each inequality is converted to a directed line with inside on the left.