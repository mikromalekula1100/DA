#include <iostream>
#include <cstring>
#include <chrono>


using std::cout;
using std::cin;

const int MAX_SIZE_KEY = 33;
const int MAX_SIZE_VALUE = 65;
const int ASCII_CODE_ZERO = 48;
const int ASCII_LETTER_A = 97;
const int SHIFT = 10;

int charToInt(char s) {
    if(std::isdigit(s)) {
        return s - ASCII_CODE_ZERO;
    }
    return s - ASCII_LETTER_A + SHIFT;
}

struct Pair {
    char key[MAX_SIZE_KEY] {};
    char value[MAX_SIZE_VALUE] {};
};

std::istream& operator>>(std::istream &in, Pair &obj) {
    cin>>obj.key>>obj.value;
    return in;
}

std::ostream& operator<<(std::ostream &out, Pair &obj) {
    out<<obj.key<<'\t'<<obj.value;
    return out;
}

struct Vector {
    size_t _size = 0, _capacity = 0;
    Pair* arr = nullptr;
    Vector() = default;
    Vector(size_t);
    Vector(Vector&&);
    Vector& operator=(Vector&&);
    size_t size() {return _size;}
    size_t capacity() {return _capacity;}
    void push_back(const Pair&);
    ~Vector();
    Pair& operator[](size_t);
};

Vector::Vector(size_t size)
        : _size(size)
        , _capacity(_size)
        , arr(new Pair[_size]{}) {}

Vector::Vector(Vector&& other)
            : _size(other._size)
            , _capacity(other._capacity)
            , arr(other.arr) {
        other.arr = nullptr;
        other._size = 0;
        other._capacity = 0;
}

Vector& Vector::operator=(Vector&& other) {
        if(this != &other) {
            if(arr != nullptr) {
                delete[] arr;
            }
            Vector newVector(std::move(other));
            arr = newVector.arr;
            _size = newVector._size;
            _capacity = newVector._capacity;
            newVector.arr = nullptr;
        }
        return *this;
}

Vector::~Vector() {
        if(arr != nullptr){
            delete [] arr;
        }
}

Pair& Vector::operator[](size_t index) {
    return arr[index];
}

void Vector::push_back(const Pair& obj) {
    if(_size == _capacity) {
        _capacity = (_capacity > 0) ? (_capacity * 2) : 1;
        Pair* newArr = new Pair[_capacity];
        std::memcpy(newArr, arr, _size * sizeof(Pair));
        if(arr != nullptr) {
            delete[] arr;
        }
        arr = newArr;
    }
    arr[_size] = obj;
    ++_size;
}

int main() {
	std::ios::sync_with_stdio(0);
    cin.tie(0);
    Vector listPairs;
    Pair pair;
    while(cin>>pair) {
        listPairs.push_back(pair);
    }
    auto start = std::chrono::steady_clock::now();
    size_t listPairsSize = listPairs.size();
    Vector copyListPairs(listPairs.size());
    for(int i = 0; i < 32; ++i) {
        int count[16]{};
        for(int j = 0; j < listPairsSize; ++j) {
            char currentChar = (listPairs[j].key)[31 - i];
            ++count[charToInt(currentChar)];
        }
        for(int j = 1; j < 16; ++j) {
           count[j] += count[j-1];
        }
        for(int j = listPairsSize - 1; j >= 0; --j) {
            char currentChar = (listPairs[j].key)[31 - i];
            copyListPairs[--count[ charToInt(currentChar)]] = listPairs[j];
        }
        std::swap(listPairs, copyListPairs);
    }
    auto end = std::chrono::steady_clock::now();
    cout << "Elapsed time in microseconds: "
    << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
    << " µs" <<'\n';
}
