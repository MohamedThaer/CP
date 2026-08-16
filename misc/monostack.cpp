vector<int> MonoStack(const vector<int>& v, int step, const char ch,bool eq=0) {
    int n = v.size();
    int l = step > 0 ? 0 : n - 1;
    int r = step > 0 ? n : -1;
    vector<int> res(n, (step > 0 ? n : -1));
    stack<int> st;
    auto cmp = [&](int curr, int top) {
        if (ch == '>') return (eq ? curr >= top : curr > top);
        else return (eq ? curr <= top : curr < top);
    };
    for (int i = l; i != r; i += step) {
        while (!st.empty() && cmp(v[i], v[st.top()])) {
            res[st.top()] = i;
            st.pop();
        }
        st.push(i);
    }
    return res;
}
