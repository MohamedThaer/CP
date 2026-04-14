
class fwht {
public:
    enum Mode {
        XOR,
        AND,
        OR,
    };

private:
    template <typename T>
    static void hadamard(vector<T> &a, int inv, Mode mode) {
        int n = a.size();
        for (int k = 1; k < n; k <<= 1) {
            for (int i = 0; i < n; i += 2 * k) {
                for (int j = 0; j < k; j++) {
                    T x = a[i + j];
                    T y = a[i + j + k];

                    if (mode == XOR) {
                        a[i + j] = x + y;
                        a[i + j + k] = x - y;
                    }
                    else if (mode == AND) {
                        if (!inv) {
                            a[i + j] = y, a[i + j + k] = x + y;
                        }
                        else {
                            a[i + j] = y - x, a[i + j + k] = x;
                        }
                    }
                    else if (mode == OR) {
                        if (!inv) {
                            a[i + j + k] = x + y;
                        }
                        else {
                            a[i + j + k] = y - x;
                        }
                    }
                }
            }
        }
    }

public:
    template <typename T>
    static vector<T> multiply(vector<T> a, vector<T> b, Mode mode) {
        int n = 1;
        while (n < (int)max(a.size(), b.size())) {
            n <<= 1;
        }
        a.resize(n);
        b.resize(n);

        hadamard(a, 0, mode);
        hadamard(b, 0, mode);
        for (int i = 0; i < n; i++) {
            a[i] *= b[i];
        }
        hadamard(a, 1, mode);
        if (mode == XOR) {
            for (int i = 0; i < n; i++) {
                a[i] /= n;
            }
        }
        return a;
    }
};
