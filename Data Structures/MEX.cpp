template <int N = (int)1e6>
struct MEX {
    bitset<N + 1> mex;
    vector<int> cnt;
    
    MEX() {
        mex.set();
        cnt.assign(N + 1, 0);
    }

    void add(int x) {
        if (++cnt[x] == 1) {
            mex[x] = false;
        }
    }

    void remove(int x) {
        if (--cnt[x] == 0) {
            mex[x] = true;
        }
    }

    int get_mex() {
        return mex._Find_first();
    }
};
