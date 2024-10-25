#include <bits/stdc++.h>

using namespace std;


struct Position {
    int64_t vertexTo, distTo;
};
struct Node {
    int64_t l_, r_, link_, parent_;
    map<char, int64_t> children_;
};
struct SuffixTree {
    string text_;
    vector<Node> nodes_;
    Position currentPosition;
    SuffixTree()
            : text_("")
            , nodes_({{-1,-1,-1,-1, {}}})
            , currentPosition({0, 0}) {}
    int64_t& L(int x) {
        return nodes_[x].l_;
    }
    int64_t& R(int x) {
        return nodes_[x].r_;
    }
    map<char, int64_t>& Children(int x) {
        return nodes_[x].children_;
    }
    int64_t& Parent(int x) {
        return nodes_[x].parent_;
    }
    bool canGo(Position& position, char symbol) {
        if(position.distTo == 0) {
            return Children(position.vertexTo).count(symbol);
        }
        return (symbol == text_[R(position.vertexTo) - position.distTo + 1]);
    }
    int64_t createNodeIfNeed(Position& pos) {
        int64_t curr = pos.vertexTo;
        if(pos.distTo == 0) {
            return curr;
        }
        int p = Parent(curr);
        nodes_[p].children_[text_[L(curr)]] = nodes_.size();
        nodes_.push_back
                (
                        {
                                L(curr)
                                , R(curr) - pos.distTo
                                , -1
                                , Parent(curr)
                                , {{text_[R(curr) - pos.distTo + 1], curr}}
                        }
                );
        L(curr) = R(curr) - pos.distTo + 1;
        Parent(curr) = nodes_.size() - 1;
        return nodes_.size() - 1;
    }
    Position go(Position& pos, char symbol) {
        if(pos.distTo == 0) {
            int64_t node = Children(pos.vertexTo)[symbol];
            if(L(node) == R(node)) {
                return {node, 0};
            }
            return {node, R(node) - L(node)};
        }
        return {pos.vertexTo, pos.distTo - 1};
    }
    Position descent(int64_t u, int l, int r) {
        while(l <= r) {
            int64_t node = Children(u)[text_[l]];
            int64_t m = R(node) - L(node);
            if(m == (r - l)) {
                return {node, 0};
            }
            if(m > (r - l)) {
                return {node, (R(node) - L(node)) - (r - l)};
            }
            l += (R(node) - L(node) + 1);
            u = node;
        }
    }
    int64_t getSuff(int v) {
        if(nodes_[v].link_ != -1) {
            return nodes_[v].link_;
        }
        int64_t parent = Parent(v);
        int64_t l = L(v);
        int64_t r = R(v);
        int64_t u;
        if(parent == 0) {
            ++l;
            if(l > r) {
                nodes_[v].link_ = 0;
                return 0;
            }
            u = 0;
        } else {
            u = nodes_[parent].link_;
        }
        Position ans = descent(u, l, r);
        int64_t w = createNodeIfNeed(ans);
        nodes_[v].link_ = w;
        return w;
    }
    void push_back(string& text) {
        std::swap(text, text_); text_ += '}';
        int64_t n = text_.size();
        for(int64_t i = 0; i < n; ++i) {
            char insertedSymbol = text_[i];
            while(!canGo(currentPosition, insertedSymbol)) {
                int64_t v = createNodeIfNeed(currentPosition);
                Children(v).insert({insertedSymbol, nodes_.size()});
                nodes_.push_back({i, n - 1, -1, v, {}});
                if(v == 0) {
                    goto to;
                }
                currentPosition = {getSuff(v), 0};
            }
            currentPosition = go(currentPosition, insertedSymbol);
            to:
            continue;
        }
    }
    string getAnswer() {
        int64_t sz = (text_.size() - 1)/2;
        string ans = "";
        int64_t curr = 0;
        while(sz > 0) {
            int64_t node = Children(curr).begin()->second;
            int64_t ln = (R(node) - L(node) + 1);
            for(int64_t i = 0; i < ln; ++i) {
                char c = text_[L(node) + i];
                ans += c;
                --sz;
                if(sz == 0) {
                    break;
                }
            }
            curr = node;
        }
        return ans;
    }
};


int main() {
    string s; cin>>s;
    s += s;

    SuffixTree tree;
    tree.push_back(s);
    string ans = tree.getAnswer();
    cout<<ans<<endl;

}
