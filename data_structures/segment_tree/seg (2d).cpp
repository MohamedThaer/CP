
struct Node {
    int sum = 0;

    void apply(int x1, int x2, int y1, int y2, int val) {
        int area = (x2 - x1 + 1) * (y2 - y1 + 1);
        sum = area * val;
    }

    void merge(const Node &a, const Node &b) {
        sum = a.sum + b.sum;
    }

    void merge(const Node &a, const Node &b, const Node &c, const Node &d) {
        merge(a, b);
        merge(*this, c);
        merge(*this, d);
    }

    void propagate(vector<Node> &seg, int x1, int y1, int x2, int y2) {
        // int x_mid = (x1 + x2) >> 1;
        // int y_mid = (y1 + y2) >> 1;
        // if (lazy != -1) {
        //     if (child[0]) seg[child[0]].apply(x1, x_mid, y1, y_mid, lazy);
        //     if (child[1]) seg[child[1]].apply(x1, x_mid, y_mid + 1, y2, lazy);
        //     if (child[2]) seg[child[2]].apply(x_mid + 1, x2, y1, y_mid, lazy);
        //     if (child[3]) seg[child[3]].apply(x_mid + 1, x2, y_mid + 1, y2, lazy);
        //     lazy = -1;
        // }
    }
};

class SegmentTree2D {
private:
    int n, m;
    vector<Node> seg;

    void build(int x, int x1, int x2, int y1, int y2, const vector<vector<int>> &grid) {
        if (x1 > x2 || y1 > y2) return;

        if (x1 == x2 && y1 == y2) {
            seg[x].apply(x1, x2, y1, y2, grid[x1][y1]);
        }
        else {
            int x_mid = (x1 + x2) >> 1;
            int y_mid = (y1 + y2) >> 1;

            build(4 * x + 1, x1, x_mid, y1, y_mid, grid);
            build(4 * x + 2, x1, x_mid, y_mid + 1, y2, grid);
            build(4 * x + 3, x_mid + 1, x2, y1, y_mid, grid);
            build(4 * x + 4, x_mid + 1, x2, y_mid + 1, y2, grid);
            seg[x].merge(seg[4 * x + 1], seg[4 * x + 2], seg[4 * x + 3], seg[4 * x + 4]);
        }
    }

    void update(int x, int x1, int x2, int y1, int y2,
                int ux1, int ux2, int uy1, int uy2, int val) {
        if (x1 > x2 || y1 > y2 || x2 < ux1 || x1 > ux2 || y2 < uy1 || y1 > uy2) return;

        if (ux1 <= x1 && x2 <= ux2 && uy1 <= y1 && y2 <= uy2) {
            return seg[x].apply(x1, x2, y1, y2, val);
        }
        seg[x].propagate(seg, x1, y1, x2, y2);

        int x_mid = (x1 + x2) >> 1;
        int y_mid = (y1 + y2) >> 1;

        update(4 * x + 1, x1, x_mid, y1, y_mid, ux1, ux2, uy1, uy2, val);
        update(4 * x + 2, x1, x_mid, y_mid + 1, y2, ux1, ux2, uy1, uy2, val);
        update(4 * x + 3, x_mid + 1, x2, y1, y_mid, ux1, ux2, uy1, uy2, val);
        update(4 * x + 4, x_mid + 1, x2, y_mid + 1, y2, ux1, ux2, uy1, uy2, val);
        seg[x].merge(seg[4 * x + 1], seg[4 * x + 2], seg[4 * x + 3], seg[4 * x + 4]);
    }

    Node query(int x, int x1, int x2, int y1, int y2,
               int qx1, int qx2, int qy1, int qy2) {
        if (x1 > x2 || y1 > y2 || x2 < qx1 || x1 > qx2 || y2 < qy1 || y1 > qy2) return Node();

        if (qx1 <= x1 && x2 <= qx2 && qy1 <= y1 && y2 <= qy2) {
            return seg[x];
        }

        seg[x].propagate(seg, x1, y1, x2, y2);
        int x_mid = (x1 + x2) >> 1;
        int y_mid = (y1 + y2) >> 1;

        Node ret;
        ret.merge(query(4 * x + 1, x1, x_mid, y1, y_mid, qx1, qx2, qy1, qy2),
                  query(4 * x + 2, x1, x_mid, y_mid + 1, y2, qx1, qx2, qy1, qy2),
                  query(4 * x + 3, x_mid + 1, x2, y1, y_mid, qx1, qx2, qy1, qy2),
                  query(4 * x + 4, x_mid + 1, x2, y_mid + 1, y2, qx1, qx2, qy1, qy2));
        return ret;
    }

public:
    SegmentTree2D(const vector<vector<int>> &grid) {
        n = grid.size();
        m = grid[0].size();
        seg.resize(16 * n * m);
        build(0, 0, n - 1, 0, m - 1, grid);
    }

    void update(int ux1, int uy1, int ux2, int uy2, int val) {
        update(0, 0, n - 1, 0, m - 1, ux1, ux2, uy1, uy2, val);
    }

    Node query(int qx1, int qy1, int qx2, int qy2) {
        return query(0, 0, n - 1, 0, m - 1, qx1, qx2, qy1, qy2);
    }
};