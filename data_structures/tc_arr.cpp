template <class M>
class tc_arr {
private:
    M *arr, reset_val;
    int *used, last = 0;
    bool *vis;

public:
    tc_arr(int N, M reset_val = 0) : reset_val(reset_val) {
        arr = new M[N + 5]{};
        used = new int[N + 5]{};
        vis = new bool[N + 5]{};
        fill(arr, arr + N + 5, reset_val);
    }

    void reset() {
        while (last) {
            last--;
            vis[used[last]] = false;
            arr[used[last]] = reset_val;
        }
    }

    M &operator[](int index) {
        if (!vis[index]) {
            vis[index] = true;
            used[last++] = index;
        }
        return arr[index];
    }
};
