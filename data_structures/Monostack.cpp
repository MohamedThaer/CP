template <class T, class Compare = less<T>>
class Monostack {
private:
    /*
     * next/prev greater       ---> Monostack<int, greater<int>>
     * next/prev less          ---> Monostack<int, less<int>>
     * next/prev greater_equal ---> Monostack<int, greater_equal<int>>
     * next/prev less_equal    ---> Monostack<int, less_equal<int>>
     */
public:
    vector<int> next, prev;

    Monostack(const vector<T> &v) {
        int n = v.size();
        next.assign(n, n);
        prev.assign(n, -1);

        stack<int> a, b;
        Compare cmp = Compare();
        for (int i = 0; i < n; i++) {
            while (!a.empty() && cmp(v[i], v[a.top()])) {
                next[a.top()] = i;
                a.pop();
            }
            a.push(i);
        }

        for (int i = n - 1; i >= 0; i--) {
            while (!b.empty() && cmp(v[i], v[b.top()])) {
                prev[b.top()] = i;
                b.pop();
            }
            b.push(i);
        }
    }
};
