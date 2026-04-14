#include <ext/rope>
using namespace __gnu_cxx;
/*
rope<int> v(n, 0);
for (int i = 0; i < n; ++i) {
    v.mutable_reference_at(i) = i + 1;
}

int l = 0, r = n - 1;
rope <int> cur = v.substr(l, r - l + 1);
v.erase(l, r - l + 1);
v.insert(v.mutable_begin(), cur);

for (auto it = v.mutable_begin(); it != v.mutable_end(); ++it)
    cout << *it << " ";

for (int i = 0; i < n; ++i)
    cout << v[i] << " ";

*/
