#include <bits/stdc++.h>

using namespace std;
const int POWER_ALPH  = 256;
char lowerSymbol = '`';


struct SuffixArray {
    string text;
    vector<int> indexes;
    vector<int> EC;
    int numberOfAdded = 1;
    int textSize = 0;
    int step = 1;
    SuffixArray (string& text) : text(std::move(text + '`')) {
        completeDegree();
        build();
        removeUnnecessary();
    }
    void removeUnnecessary() {
        vector<int> ind;
        for(size_t i = numberOfAdded; i < indexes.size(); ++i) {
            ind.emplace_back(indexes[i]);
        }
        string t;
        for(int i = 0; i < (textSize - numberOfAdded); ++i) {
            t.push_back(text[i]);
        }
        swap(ind, indexes);
        swap(text, t);
        textSize = text.size();
    }
    void completeDegree() {
        int degree = 0;
        while(text.size() > std::pow(2, degree)) {
           ++degree;
        }
        int c = text.size();
        for(int i = 0; i < (std::pow(2, degree) - c); ++i) {
            text.push_back(lowerSymbol); ++numberOfAdded;
        }
        textSize = text.size();
    }
    bool start() {
        vector<int> helper(POWER_ALPH, 0);
        vector<int> ind(textSize, 0);
        vector<int> ec(textSize, 0);
        for(char c : text) {
            ++helper[c - lowerSymbol];
        }
        for(int i = 1; i < POWER_ALPH; ++i) {
            helper[i] += helper[i-1];
        }
        for(int i = textSize - 1; i >= 0; --i) {
            ind[--helper[text[i] - lowerSymbol]] = i;
        }
        ec[ind[0]] = 0; bool flag = true;
        for(int i = 1; i < textSize; ++i) {
            if(text[ind[i]] == text[ind[i-1]]) {
                ec[ind[i]] = ec[ind[i-1]]; flag = false;
            } else {
                ec[ind[i]] = ec[ind[i-1]] + 1;
            }
        }
        swap(ind, indexes);
        swap(ec, EC);
        return flag;
    }
    void build() {
        if(start())
            return;
        while(step < textSize) {
            vector<int> ind(textSize, 0);
            vector<int> ec(textSize, 0);
            for(int i = 0; i < textSize; ++i) {
                ind[i] = (indexes[i] - step + textSize) % textSize;
            }
            for(int i = 0; i < textSize; ++i) {
                ec[i] = EC[ind[i]];
            }
            vector<int> helper(textSize, 0);
            for(int i = 0; i < textSize; ++i) {
                ++helper[ec[i]];
            }
            for(int i = 1; i < textSize; ++i) {
                helper[i] += helper[i-1];
            }
            vector<int> indHelper(textSize, 0);
            for(int i = textSize - 1; i >= 0; --i){
                indHelper[--helper[ec[i]]] = ind[i];
            }
            vector<pair<int, int>> ecDoubleHelper(textSize, {0, 0});
            for(int i = 0; i < textSize; ++i) {
                ecDoubleHelper[i].first = EC[indHelper[i]];
                ecDoubleHelper[i].second = EC[(indHelper[i] + step) % textSize];
            }
            EC[indHelper[0]] = 0; bool flag = true;
            for(int i = 1; i < textSize; ++i) {
                if(ecDoubleHelper[i] == ecDoubleHelper[i-1]) {
                    EC[indHelper[i]] = EC[indHelper[i-1]]; flag = false;
                } else {
                    EC[indHelper[i]] = EC[indHelper[i-1]] + 1;
                }
            }
            swap(indHelper, indexes);
            step *= 2;
            if(flag)
                return;
        }
    }
    bool equal(const string& pattern, int x) {
        for(int i = 0; i < pattern.size(); ++i) {
            if(x + i > textSize - 1) return false;
            if(pattern[i] != text[x + i]) {
                return false;
            }
        }
        return true;
    }
    bool more(const string& pattern, int x) {
        for(int i = 0; i < pattern.size(); ++i) {
            if(x + i > textSize - 1) {
                return true;
            }
            if(pattern[i] == text[x + i]) continue;
            if(pattern[i] > text[x + i]) {
                return true;
            }
            return false;
        }
        return false;
    }
    bool less(const string& pattern, int x) {
        return !(equal(pattern, x) || more(pattern, x));
    }
    int lowerBound(const string& pattern) {
        vector<int> ans;
        int l = 0, r = indexes.size() - 1;
        if(less(pattern, indexes[l]) || more(pattern, indexes[r])) {
            return -1;
        }
        if(equal(pattern, indexes[l])) return l;
        int m;
        while(r - l > 1) {
            m = (r + l) / 2;
            if(more(pattern, indexes[m])) {
                l = m;
            } else {
                r = m;
            }
        }
        if(equal(pattern, indexes[r])) {
            return r;
        } else {
            return -1;
        }
    }
};

int main() {
    string text;
    cin>>text;
    SuffixArray suffixArray(text);
    string pattern;
    int iteration = 0;
    while(cin>>pattern) {
        ++iteration;
        int l = suffixArray.lowerBound(pattern);
        if(l == -1) continue;
        set<int> ans;
        for(; l < suffixArray.textSize; ++l) {
            if(!suffixArray.equal(pattern, suffixArray.indexes[l])) break;
            ans.insert(suffixArray.indexes[l]);
        }
        cout<<iteration<<": ";
        for(auto iter = ans.begin(); iter != ans.end(); ++iter) {
            auto copy = iter;
            if(++copy == ans.end()) {
                cout<<*iter + 1;
                continue;
            }
            cout<<*iter + 1<<", ";
        }
        cout<<endl;
    }
}