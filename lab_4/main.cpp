#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <stack>
#include <set>

using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;
using std::unordered_set;
using std::stack;
using std::set;
using std::endl;

vector<int64_t> termNodes;
//------------------------------------------------------------------------------------------------------------------

struct Node {
    bool isTerminal {false};
    unordered_map<int64_t, int64_t> to;
    unordered_set<int64_t> in;
    int64_t len = 0;
};

//------------------------------------------------------------------------------------------------------------------

struct Trie {
    size_t countNodes = 1;
    vector<Node> trie;
    void add(vector<int64_t>&);
    void filling();
    vector<int64_t> links {};
    Trie();
};

void Trie::filling() {
    if(countNodes == 1) {
        return;
    }
    links.resize(countNodes);
    links[0] = -1;
    stack<std::pair<int64_t, std::pair<int64_t, int64_t>>> stack;
    for(auto& pair : trie[0].to) {
        stack.push({0, pair});
    }
    while(!stack.empty()) {
        auto currentNode = stack.top();
        stack.pop();
        int64_t parent = currentNode.first;
        int64_t currentNumber = currentNode.second.first;
        int64_t index = currentNode.second.second;
        int64_t parentLink = links[parent];
        bool flag = true;
        while(parentLink != -1) {
            auto findElem = trie[parentLink].to.find(currentNumber);
            if(findElem != trie[parentLink].to.end()) {
                links[index] = findElem->second;
                flag = false;
                break;
            }
            parentLink = links[parentLink];
        }
        if(flag) {
            links[index] = 0;
        }
        for(auto& pair : trie[index].to) {
            stack.push({index, pair});
        }
    }
}

void Trie::add(vector<int64_t>& insertNumbers) {
    if(insertNumbers.empty()) {
        return;
    }
    int64_t indexCurrentNode = 0;
    for(auto number : insertNumbers) {
        auto findElem = trie[indexCurrentNode].to.find(number);
        if(findElem == trie[indexCurrentNode].to.end()) {
            trie[indexCurrentNode].to.insert({number, trie.size()});
            int64_t lenHelper = trie[indexCurrentNode].len;
            indexCurrentNode = trie.size();
            trie.push_back(Node());
            trie[indexCurrentNode].len += lenHelper + 1;
            ++countNodes;
            continue;
        }
        indexCurrentNode =  findElem->second;
    }
    trie[indexCurrentNode].isTerminal = true;
    termNodes.push_back(indexCurrentNode);
}

Trie::Trie() {
    trie.push_back(Node());
}

//------------------------------------------------------------------------------------------------------------------

Trie trie;

//------------------------------------------------------------------------------------------------------------------

void travel(int64_t numberNode, int64_t numberWord){
    while(numberNode != 0) {
        Node& currentNode = trie.trie[numberNode];
        if(currentNode.isTerminal) {
            currentNode.in.insert(numberWord - currentNode.len + 1);
        }
        numberNode = trie.links[numberNode];
    }
}

//------------------------------------------------------------------------------------------------------------------



int main() {
    string stringPatterns;
    std::getline(cin, stringPatterns);
    std::istringstream streamStringPatterns(stringPatterns);
    string helpString;
    vector<int64_t> partPattern;
    vector<int64_t> jokerVector;
    int64_t jokerHelper = 0;
    while(streamStringPatterns >> helpString) {
        int64_t value;
        if(isdigit(helpString[0])){
            if(jokerHelper != 0) {
                jokerVector.push_back(jokerHelper);
                jokerHelper = 0;
            }
            value = std::stoll(helpString);
            partPattern.push_back(value);
        } else {
            ++jokerHelper;
            trie.add(partPattern);
            partPattern.clear();
        }
    }
    trie.add(partPattern);
    trie.filling();

//-----------------------------------

    int64_t currentNode = 0, numberWord, countWordInString;
    string text;
    vector<int64_t> table;
    numberWord = 0;
    while(std::getline(cin, text)) {
        countWordInString = 0;
        std::istringstream helperString(text);
        int64_t value;
        while(helperString >> value){
            ++countWordInString;
            ++numberWord;
            if(termNodes.size() == 0){
                continue;
            }
            auto findElem =  trie.trie[currentNode].to.find(value);
            int64_t link = trie.links[currentNode];
            while(findElem == trie.trie[currentNode].to.end() && link != -1)  {
                currentNode = link;
                link = trie.links[currentNode];
                findElem =  trie.trie[currentNode].to.find(value);
            }
            if(findElem != trie.trie[currentNode].to.end()) {
                currentNode = findElem->second;
            } else {
                currentNode = 0;
            }
            travel(currentNode, numberWord);
        }
        table.push_back(countWordInString);
    }

//----------------------------------------

    if(termNodes.size() == 0) {
        int64_t sum = 0;
        for (int i = 0; i < table.size(); ++i) {
            for(int j = 1; j <= table[i]; ++j) {
                ++sum;
                if(numberWord - jokerHelper + 1 < sum) return 0;
                cout<<i+1<< ", "<<j<<'\n';
            }
        }
        return 0;
    }

    if(jokerVector.size() == termNodes.size() && jokerHelper != 0) {
        for(int64_t time : trie.trie[termNodes[0]].in) {
            bool flag = true;
            if(time <= jokerVector[0]) {
                continue;
            }
            int64_t sum = 0;
            for(int i = 1; i < termNodes.size(); ++i) {
                auto findELem = trie.trie[termNodes[i]].in.find(time + trie.trie[termNodes[i-1]].len + jokerVector[i] + sum);
                if(findELem == trie.trie[termNodes[i]].in.end()) {
                    flag = false;
                    break;
                }
                if(numberWord - ((*findELem) - 1 + trie.trie[termNodes[i]].len) < jokerHelper){
                    flag = false;
                    break;
                }
                sum += trie.trie[termNodes[i-1]].len + jokerVector[i];
            }
            if(flag) {
                int64_t sum = 0;
                for(int i = 0; i < table.size(); ++i) {
                    if(sum + table[i] >= (time - jokerVector[0])) {
                        int64_t h = 0;
                        while(h + sum != (time - jokerVector[0])) {
                            ++h;
                        }
                        cout<<i+1<<", "<<h<<'\n';
                        break;
                    }
                    sum += table[i];
                }
            }

        }

    } else if(jokerVector.size() == termNodes.size()) {
        for(int64_t time : trie.trie[termNodes[0]].in) {
            bool flag = true;
            if (time <= jokerVector[0]) {
                flag = false;
                continue;
            }
            int64_t sum = 0;
            for (int i = 1; i < termNodes.size(); ++i) {
                auto findELem = trie.trie[termNodes[i]].in.find(time + trie.trie[termNodes[i-1]].len + jokerVector[i] + sum);
                if (findELem == trie.trie[termNodes[i]].in.end()) {
                    flag = false;
                    break;
                }
                sum += trie.trie[termNodes[i-1]].len + jokerVector[i];
            }
            if (flag) {
                int64_t sum = 0;
                for (int i = 0; i < table.size(); ++i) {
                    if(sum + table[i] >= (time - jokerVector[0])) {
                        int64_t h = 0;
                        while(h + sum != (time - jokerVector[0])) {
                            ++h;
                        }
                        cout << i + 1 << ", " << h << '\n';
                        break;
                    }
                    sum += table[i];
                }
            }
        }
    } else if(jokerHelper != 0) {
            for(int64_t time : trie.trie[termNodes[0]].in) {
                bool flag = true;
                int64_t sum = 0;
                for(int i = 1; i < termNodes.size(); ++i) {
                    auto findELem = trie.trie[termNodes[i]].in.find(time + trie.trie[termNodes[i-1]].len + jokerVector[i-1] + sum);
                    if(findELem == trie.trie[termNodes[i]].in.end()) {
                        flag = false;
                        break;
                    }
                    if(numberWord - ((*findELem) - 1 + trie.trie[termNodes[i]].len) < jokerHelper){

                        flag = false;
                        break;
                    }
                    sum += trie.trie[termNodes[i-1]].len + jokerVector[i-1];
                }
                if(flag) {
                    int64_t sum = 0;
                    for(int i = 0; i < table.size(); ++i) {
                        if(sum + table[i] >= time) {
                            int64_t h = 0;
                            while(h + sum != time) {
                                ++h;
                            }
                            cout<<i+1<<", "<<h<<'\n';
                            break;
                        }
                        sum += table[i];
                    }
                }
            }
    } else {
        for(int64_t time : trie.trie[termNodes[0]].in) {
                bool flag = true;
                int64_t sum = 0;
                for(int i = 1; i < termNodes.size(); ++i) {
                    auto findELem = trie.trie[termNodes[i]].in.find(time + trie.trie[termNodes[i-1]].len + jokerVector[i-1] + sum);
                    if(findELem == trie.trie[termNodes[i]].in.end()) {
                        flag = false;
                        break;
                    }
                    sum += trie.trie[termNodes[i-1]].len + jokerVector[i-1];
                }
                if(flag) {
                    int64_t sum = 0;
                    for(int i = 0; i < table.size(); ++i) {
                        if(sum + table[i] >= time) {
                            int64_t h = 0;
                            while(h + sum != time) {
                                ++h;
                            }
                            cout<<i+1<<", "<<h<<'\n';
                            break;
                        }
                        sum += table[i];
                    }
                }
            }
    }
}

