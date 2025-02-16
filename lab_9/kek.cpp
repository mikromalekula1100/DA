#include <iostream>
#include <set>
#include <vector>

using namespace std;


set<pair<int, pair<int, int>>> s;
vector<int64_t> d;
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, m, start, finish; cin>>n>>m>>start>>finish;
    d = vector<int64_t>(n + 1, INT64_MAX); d[start] = 0;
    int64_t u, v, weigh;
    for(int i = 0; i < m; ++i) {
        cin>>u>>v>>weigh;
        s.insert({u, {v, weigh}});
    }
    if(start == finish) {
        cout<<0;
        return 0;
    }
    for(int i = 0; i < n-1; ++i) {
        for(auto& j : s) {
            if(d[j.first] == INT64_MAX) continue;
            d[j.second.first] = min(d[j.second.first], d[j.first] + j.second.second);
        }
    }
    if(d[finish] == INT64_MAX) {
        cout<<"No solution";
        return 0;
    }
    cout<<d[finish];
}
