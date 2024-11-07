#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
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

// Removing the stopwords
void removeStopWords(unordered_map<string, vector<int>> &wordPositionMap)
{
    ifstream stopWordsFile("stopwords.txt");
    unordered_set<string> stopWords;

    if (!stopWordsFile)
    {
        cerr << "Error opening stopwords.txt file!" << endl;
        return;
    }

    string word;
    while (stopWordsFile >> word)
    {
        stopWords.insert(word);
    }

    for (const auto &stopWord : stopWords)
    {
        wordPositionMap.erase(stopWord);
    }
}

// Remove common suffixes 
string stripSuffixes(const string &word)
{
    string lowerWord = word;
    transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);

    if (lowerWord.length() >= 3 && lowerWord.substr(lowerWord.length() - 3) == "ing")
    {
        return lowerWord.substr(0, lowerWord.length() - 3);
    }
    else if (lowerWord.length() >= 2 && lowerWord.substr(lowerWord.length() - 2) == "ed")
    {
        return lowerWord.substr(0, lowerWord.length() - 2);
    }
    else if (lowerWord.length() >= 3 && lowerWord.substr(lowerWord.length() - 3) == "ion")
    {
        return lowerWord.substr(0, lowerWord.length() - 3);
    }
    else if (lowerWord.length() >= 2 && lowerWord.substr(lowerWord.length() - 2) == "ly")
    {
        return lowerWord.substr(0, lowerWord.length() - 2);
    }
    else if (lowerWord.length() >= 4 && lowerWord.substr(lowerWord.length() - 4) == "ness")
    {
        return lowerWord.substr(0, lowerWord.length() - 4);
    }
    else if (lowerWord.length() >= 3 && lowerWord.substr(lowerWord.length() - 3) == "ies")
    {
        return lowerWord.substr(0, lowerWord.length() - 3) + "y";
    }
    else if (lowerWord.length() >= 2 && lowerWord.substr(lowerWord.length() - 2) == "es")
    {
        return lowerWord.substr(0, lowerWord.length() - 2);
    }
    else if (lowerWord.back() == 's')
    {
        return lowerWord.substr(0, lowerWord.length() - 1);
    }

    return lowerWord;
}

// Remove punctuation marks, split text into words, and track positions of each word
void countWordsWithPositions(const string &text, unordered_map<string, vector<int>> &wordPositionMap)
{
    string tempText = text;
    vector<string> punctuationMarks = {".", ",", ":", ";", "'", "\"", "-", "(", ")"};

    for (const auto &mark : punctuationMarks)
    {
        replace(tempText.begin(), tempText.end(), mark[0], ' ');
    }

    stringstream ss(tempText);
    string word;
    int position = 1;
    while (ss >> word)
    {
        if (!word.empty())
        {
            string stemmedWord = stripSuffixes(word);
            wordPositionMap[stemmedWord].push_back(position);
        }
        position++;
    }
}

// Function to print and save inverted index
void saveInvertedIndex(const unordered_map<string, vector<int>> &wordPositionMap)
{
    ofstream invertedIndexFile("Inverted_index_file.txt");

    if (!invertedIndexFile)
    {
        cerr << "Error creating Inverted_index_file.txt file!" << endl;
        return;
    }

    // Create a vector to store pairs of words and their first position
    vector<Pair> sortedEntries;

    // Fill sortedEntries with word and their positions
    for (const auto &entry : wordPositionMap)
    {
        sortedEntries.emplace_back(entry.first, entry.second, entry.second.size());
    }

    // Sort the entries based on the first position of each word
    sort(sortedEntries.begin(), sortedEntries.end(), [](const Pair &a, const Pair &b) {
        return a.positions.front() < b.positions.front();
    });

    // Print and save sorted entries
    for (const auto &pair : sortedEntries)
    {
        invertedIndexFile << pair.word << " : ";
        cout << pair.word << " : ";
        for (int i = 0; i < pair.positions.size(); ++i)
        {
            invertedIndexFile << pair.positions[i];
            cout << pair.positions[i];
            if (i < pair.positions.size() - 1)
            {
                invertedIndexFile << ", ";
                cout << ", ";
            }
        }
        invertedIndexFile << " >> count: " << pair.count << endl;
        cout << " >> count: " << pair.count << endl;
    }
}

int main()
{
    cout << "\n***********OUTPUT************\n\n";

    cout << "\n**INVERTED FILE**\n";

    ifstream inputFile("input.txt");
    stringstream textBuilder;

    if (!inputFile)
    {
        cerr << "Error opening input.txt file!" << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line))
    {
        textBuilder << line << " ";
    }

    string text = textBuilder.str();
    unordered_map<string, vector<int>> wordPositionMap;
    countWordsWithPositions(text, wordPositionMap);

    removeStopWords(wordPositionMap);

    saveInvertedIndex(wordPositionMap);

    // Query for a word
    string queryWord;
    cout << "\nEnter a word to search: ";
    cin >> queryWord;

    queryWord = stripSuffixes(queryWord);
    if (wordPositionMap.find(queryWord) != wordPositionMap.end())
    {
        const auto &positions = wordPositionMap[queryWord];
        cout << "Word \"" << queryWord << "\" is found " << positions.size() << " times at positions: ";
        for (int i = 0; i < positions.size(); ++i)
        {
            cout << positions[i];
            if (i < positions.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
    else
    {
        cout << "Word \"" << queryWord << "\" not found." << endl;
    }

    return 0;
}
