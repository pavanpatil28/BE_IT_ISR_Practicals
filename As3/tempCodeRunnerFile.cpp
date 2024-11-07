
#include <queue>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Structure of Output
struct Pair
{
    string word;
    vector<int> positions;
    int count;

    Pair(string w, vector<int> p, int c) : word(w), positions(p), count(c) {}
};