#include <iostream>
#include <map>
#include <vector>
#include <algorithm>


using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

class SuffixTree {
public:
    struct Node;
    int currCount = 0;
    string text;
    vector<Node> vertexes;
    int textSize;
    struct Node {
        int prevNode;
        int left, len;
        map<char, int> go;
        int number = -1;
    };
    struct Position {
        int node;
        int distance;
    };
    bool canGo(Position& pos, char ch) {
        if(pos.distance == 0) {
            return (vertexes[pos.node].go.find(ch) != vertexes[pos.node].go.end());
        }
        Node currNode = vertexes[pos.node];
        char nextSymbol = text[currNode.left + currNode.len - pos.distance + 1];
        return nextSymbol == ch;
    }
    Position go(Position& pos, char ch) {
        if(pos.distance == 0) {
            int nextNode = vertexes[pos.node].go[ch];
            if(vertexes[nextNode].len == 0) {
                return {nextNode, 0};
            }
            return {nextNode, vertexes[nextNode].len};
        }
        return {pos.node, --pos.distance};
    }
    void buildNewNode(Position& pos, int indexSymbol) {
        Node& currentNode = vertexes[pos.node];
        if(pos.distance == 0) {
            currentNode.go.insert({text[indexSymbol], vertexes.size()});
            vertexes.push_back({pos.node, indexSymbol, (textSize - indexSymbol - 1), {}, currCount});
        } else {
            Node& prevNode = vertexes[currentNode.prevNode];
            prevNode.go[text[currentNode.left]] = vertexes.size();
            vertexes.push_back(
                    {
                        currentNode.prevNode
                        , currentNode.left
                        , (currentNode.len - pos.distance)
                        , {
                            {
                                    text[currentNode.left + currentNode.len - pos.distance + 1]
                                    , pos.node
                                }
                                }
                    }
                    );
            vertexes[vertexes.size() - 1].go.insert({text[indexSymbol], vertexes.size()});
            int newParent = vertexes.size() - 1;
            vertexes.push_back({newParent, indexSymbol, textSize - indexSymbol - 1, {}, currCount});
            Node& currNode = vertexes[pos.node];
            int copy = currNode.left;
            currNode.left = currNode.left + currNode.len - pos.distance + 1;
            currNode.len = copy + currNode.len - currNode.left;
            currNode.prevNode = vertexes.size() - 2;
        }
        ++currCount;
    }
    void insertWord(size_t index) {
        Position currentPos{0, 0};
        for(; index < textSize; ++index) {
            char currentSymbol = text[index];
            if(canGo(currentPos, currentSymbol)) {
                currentPos = go(currentPos, currentSymbol);
                continue;
            }
            buildNewNode(currentPos, index);
            return;
        }
    }
public:
    SuffixTree(string& s) : text(std::move(s + "$")), vertexes({{-1, -1, -1, {}}}), textSize(text.size()) {
        for(size_t indexLeadingLetter = 0; indexLeadingLetter < textSize; ++indexLeadingLetter) {
            insertWord(indexLeadingLetter);
        }
    }
    void findHelper (int node, vector<int>& vec) {
        auto d = vertexes[node].go;
        if(d.empty()) {
            vec.push_back(vertexes[node].number);
            return;
        }
        for(auto i : vertexes[node].go) {
            findHelper(i.second, vec);
        }
    }
    vector<int> find(string& patter) {
        vector<int> indexes;
        Position pos{0, 0};
        for(char c : patter) {
            if(canGo(pos, c)) {
                pos = go(pos, c);
            } else {
                return {};
            }
        }
        findHelper(pos.node, indexes);
        return indexes;
    }
};

int main () {
    string str; cin>>str;
    SuffixTree tree(str);
    string patter;
    int c = 0;
    while(cin>>patter) {
        ++c;
        auto a= tree.find(patter);
        if(a.empty()) {
            continue;
        }
        std::sort(a.begin(), a.end());
        cout<<c<<": ";
        for(int i = 0; i < a.size(); ++i) {
            if(i == a.size() - 1) {
                cout<<a[i] + 1;
                continue;
            }
            cout<<a[i] + 1<<", ";
        }
        cout<<"\n";
    }
}