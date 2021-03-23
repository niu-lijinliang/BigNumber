#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

vector<int> str_to_reverse_vec(string& s) {
    vector<int> ans(s.size());
    for (int i = 0; i < ans.size(); i++) {
        ans[i] = s[s.size() - 1 - i] - '0';
    }
    return ans;
}

string vec_to_real(vector<int>& vec) {
    string ans;
    int i = vec.size() - 1;
    while (vec[i] == 0) { //去前导零
        i--;
    }
    for (; i >= 0; i--) {
        ans.push_back(vec[i] + '0');
    }
    return ans;
}

bool smaller(string a, string b) {
    if (a.size() < b.size()) //首先比较长度
        return true;         //长度大的肯定大
    if (a.size() > b.size())
        return false;
    return a < b; //若长度相等可直接比较
}

string add(string&, string&);
string subtract(string& str1, string& str2) {
    if (str1[0] == '-' && str2[0] == '-') {
        str1.erase(str1.begin());
        str2.erase(str2.begin());
        return subtract(str2, str1);
    }
    else if (str1[0] == '-') {
        str2.insert(str2.begin(), '-');
        return add(str2, str1);
    }
    else if (str2[0] == '-'){
        str2.erase(str2.begin());
        return add(str1, str2);
    }
    if (str1 == str2) {
        return "0";
    }
    bool positive = true;
    if (smaller(str1, str2)) {//始终保持a > b，并记录初始a，b的关系以便确定结果的符号
        swap(str1, str2);
        positive = false;
    }
    vector<int> a = str_to_reverse_vec(str1);
    vector<int> b = str_to_reverse_vec(str2);
    b.insert(b.end(), a.size() - b.size(), 0);
    vector<int> ans(a.size(), 0);
    for (int i = 0; i < a.size(); i++) {
        if (a[i] < b[i]) {//不够则借数
            for (int j = i + 1; j < a.size(); j++) {
                if (a[j] > 0) { // 往前找第一个不为0的位
                    a[j]--;
                    for (int t = j - 1; t > i; t--) { // 为0的都变成9
                        a[t] = 9;
                    }
                    a[i] += 10; // 借位的位加10
                    break;
                }
            }
        }
        ans[i] += (a[i] - b[i]);
    }
    string ans_str = vec_to_real(ans);
    if (!positive) {
        ans_str.insert(ans_str.begin(), '-');
    }
    return ans_str;
}

string add(string& str1, string& str2) {
    bool positive = true;
    if (str1[0] == '-' && str2[0] == '-') {
        str1.erase(str1.begin());
        str2.erase(str2.begin());
        positive = false;
    }
    else if(str1[0] == '-'){
        str1.erase(str1.begin());
        return subtract(str2, str1);
    }
    else if (str2[0] == '-') {
        str2.erase(str2.begin());
        return subtract(str1, str2);
    }
    vector<int> a = str_to_reverse_vec(str1);
    vector<int> b = str_to_reverse_vec(str2);
    vector<int> ans(max(a.size(), b.size()) + 1, 0);
    //补零，使两串长度相等
    if (a.size() < b.size()) {
        a.insert(a.end(), b.size() - a.size(), 0);
    }
    else if (b.size() < a.size()) {
        b.insert(b.end(), a.size() - b.size(), 0);
    }
    for (int i = 0; i < a.size(); i++) {
        ans[i] += (a[i] + b[i]);
        if (ans[i] >= 10) {
            ans[i + 1] = 1;
            ans[i] -= 10;
        }
    }
    string ans_str = vec_to_real(ans);
    if (!positive) {
        ans_str.insert(ans_str.begin(), '-');
    }
    return ans_str;
}

string multiply(string& str1, string& str2) {  
    bool positive = true;
    if (str1[0] == '-' && str2[0] == '-') {
        str1.erase(str1.begin());
        str2.erase(str2.begin());
    }
    else if (str1[0] == '-') {
        str1.erase(str1.begin());
        positive = false;
    }
    else if (str2[0] == '-') {
        str2.erase(str2.begin());
        positive = false;
    }
    if (smaller(str1, str2)) { // 用小的做被乘数提高效率
        swap(str1, str2);
    }
    vector<int> a = str_to_reverse_vec(str1);
    vector<int> b = str_to_reverse_vec(str2);
    vector<int> ans(a.size() + b.size() + 1, 0);
    for (int i = 0; i < b.size(); i++) {// 一串的一位与另一串的所有位依次相乘
        for (int j = 0; j < a.size(); j++) {
            int sum = b[i] * a[j];
            ans[i + j] += sum;
            if (ans[i + j] > 9) {
                ans[i + j + 1] += ans[i + j] / 10;
                ans[i + j] = ans[i + j] % 10;
            }
        }
    }
    string ans_str = vec_to_real(ans);
    if (!positive) {
        ans_str.insert(ans_str.begin(), '-');
    }
    return ans_str;
}

string small_divide_big(string a, string b, int n) {
    string ans("0.");
    if (a == "0") {
        ans.insert(ans.end(), n, '0');
        return ans;
    }
    int pos = -1; 
    while (pos != n && a != "00") {
        int sum = 0;
        if (smaller(a, b)) {// 被除数除不动时，商和被除数都补0
            a += '0';
            if (pos >= 0) {
                ans.push_back('0');
            }
            pos++;
        }
        while (!smaller(a, b)) { // 转化为减法
            a = subtract(a, b);
            sum++;
        }
        if (sum != 0) { // 如果有商
            a += '0';  
            ans += (sum + '0'); 
            pos++;         
        }
    }
    if (ans.size() - 2 < n) { //如果不能得到需要的位数，例如1/2 = 0.5，要求三位小数的话达不到
        ans.insert(ans.end(), n - (ans.size() - 2), '0');
    }
    return ans;
}

string big_divide_small(string& a, string& b, int n) {
    int moved = 0;;
    while (!smaller(a, b)) {
        b.push_back('0');
        moved++;
    }
    string ans = small_divide_big(a, b, n + moved);
    // 右移
    ans.erase(ans.begin());
    for (int i = 0; i < moved; i++) {
        swap(ans[i], ans[i + 1]);
    }
    return ans;
}

string divide(string& a, string& b, int n) {//用来判断是大除小还是小除大
    if (a[0] == 0) {
        return "0";
    }
    if (b[0] == 0) {
        return "ERROR";
    }    
    if (a == b) {
        return "1";
    }
    bool positive = true;
    if (a[0] == '-' && b[0] == '-') {
        a.erase(a.begin());
        b.erase(b.begin());
    }
    else if (a[0] == '-') {
        a.erase(a.begin());
        positive = false;
    }
    else if (b[0] == '-') {
        b.erase(b.begin());
        positive = false;
    }

    string ans;
    if (smaller(a, b)) {
        ans = small_divide_big(a, b, n);
    }
    else {
        ans = big_divide_small(a, b, n);
    }
    if (!positive) {
        ans.insert(ans.begin(), '-');
    }
    return ans;
}

string module(string& a, string& b) {
    if (!smaller(a, b)) {
        while (!smaller(a, b)) {
            a = subtract(a, b);
        }
        return a;
    }
    else {
        return a;
    }
}

int main(int argc, char *argv[]) {
    string str1, str2;
    char a;
    int n = 0;
    while (true) {
        cin >> str1;
        cin >> a;
        cin >> str2;
        if (a == '/')
            cin >> n;
        switch (a) {
        case '+':
            cout << "相加等于：" << endl
                << add(str1, str2) << endl;
            break;
        case '-':
            cout << "相减等于：" << endl
                << subtract(str1, str2) << endl;
            break;
        case '*':
            cout << "相乘等于：" << endl
                << multiply(str1, str2) << endl;
            break;
        case '/':
            cout << "相除等于：" << endl
                << divide(str1, str2, n);
            break;
        case '%':
            cout << "求模等于：" << endl
                << module(str1, str2) << endl;
            break;
        default:
            break;
        }
    }
    return 0;
}