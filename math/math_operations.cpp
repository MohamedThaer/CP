namespace math_operations {
    int base = 10;

    string add(string a, string b) {
        reverse(a.begin(), a.end());
        reverse(b.begin(), b.end());
        while (a.size() < b.size()) {
            a += '0';
        }
        while (a.size() > b.size()) {
            b += '0';
        }
        string ret((int)a.size(), '0');

        int carry = 0;
        for (int i = 0; i < a.size(); ++i) {
            int res = (a[i] - '0') + (b[i] - '0') + carry;
            ret[i] = (res % base + '0');
            carry = res / base;
        }
        if (carry) {
            ret.push_back('1');
        }
        while (ret.size() > 1 && ret.back() == '0') {
            ret.pop_back();
        }
        reverse(ret.begin(), ret.end());
        return ret;
    }

    string sub(string a, string b) {
        reverse(a.begin(), a.end());
        reverse(b.begin(), b.end());
        while (a.size() < b.size()) {
            a += '0';
        }
        while (a.size() > b.size()) {
            b += '0';
        }

        string sign, ret((int)a.size(), '0');
        for (int i = (int)a.size() - 1; ~i; --i) {
            if (b[i] > a[i]) {
                sign = "-";
                swap(a, b);
                break;
            }
            if (b[i] < a[i]) {
                break;
            }
        }

        for (int i = 0; i < a.size(); ++i) {
            ret[i] = (a[i] - b[i] + '0');
            if (a[i] < b[i]) {
                int p = i + 1;
                while (a[p] == '0') {
                    a[p] = base - 1 + '0';
                    ++p;
                }
                --a[p];
                ret[i] += base;
            }
        }
        while (ret.size() > 1 && ret.back() == '0') {
            ret.pop_back();
        }
        reverse(ret.begin(), ret.end());
        return sign + ret;
    }

    string mul(string a, string b) {
        reverse(a.begin(), a.end());
        reverse(b.begin(), b.end());
        string ret = "0";
        for (int i = 0; i < b.size(); ++i) {
            int carry = 0;
            string res((int)a.size(), '0');
            for (int j = 0; j < a.size(); ++j) {
                int x = (int)(a[j] - '0') * (b[i] - '0') + carry;
                res[j] = (x % base + '0');
                carry = x / base;
            }
            if (carry) {
                res.push_back(carry + '0');
            }
            reverse(res.begin(), res.end());
            ret = add(ret, res + string(i, '0'));
        }
        return ret;
    }

    string divide(const string &dividend, const string &divisor) {
        string quotient, current;
        int n = dividend.size();
        for (int i = 0; i < n; ++i) {
            current.push_back(dividend[i]);
            current.erase(0, min(current.find_first_not_of('0'), current.size() - 1));

            int count = 0, low = 0, high = base - 1;
            while (low <= high) {
                int mid = (low + high) / 2;
                string product = mul(divisor, to_string(mid));
                if (sub(current, product)[0] != '-') {
                    count = mid;
                    low = mid + 1;
                }
                else {
                    high = mid - 1;
                }
            }

            current = sub(current, mul(divisor, to_string(count)));
            quotient.push_back(count + '0');
        }

        quotient.erase(0, min(quotient.find_first_not_of('0'), quotient.size() - 1));
        return quotient;
    }

    string modulo(string a, string b) {
        return sub(a, mul(b, divide(a, b)));
    }
}
using namespace math_operations;
