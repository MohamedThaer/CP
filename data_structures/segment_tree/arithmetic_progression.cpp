struct node {
    long long sum = 0, lazy_start = 0, lazy_end = 0, lazy_step = 0;
 
    void apply(int lx, int rx, int val) {
        apply(lx, rx, val, val, 1);
    }
 
    void apply(int lx, int rx, int l, int r) {
        apply(lx, rx, lx - l + 1, rx - l + 1, 1);
    }
 
    void apply(int lx, int rx, long long start, long long end, long long step) {
        lazy_start += start;
        lazy_end += end;
        lazy_step += step;
        sum += get(start, end, step);
    }
 
    void merge(const node &a, const node &b) {
        sum = a.sum + b.sum;
    }
 
    void propagate(node &left, node &right, int lx, int rx, int mid) {
        left.apply(lx, mid, lazy_start, lazy_start + (mid - lx) * lazy_step, lazy_step);
        right.apply(mid + 1, rx, lazy_start + (mid - lx + 1) * lazy_step, lazy_end, lazy_step);
        lazy_start = 0, lazy_end = 0, lazy_step = 0;
    }
};
