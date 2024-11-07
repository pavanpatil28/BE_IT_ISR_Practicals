#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include <set>

using namespace std;

// Function prototypes
void countKeywordFrequencies(const vector<string>& keywords, const vector<string>& documents, vector<vector<double>>& matrix);
int extractCount(const string& line, const string& keyword);
void printMatrix(const string& title, const vector<vector<double>>& matrix, const vector<string>& rowLabels, const vector<string>& colLabels);
map<string, vector<string>> performOverlappingClustering(const vector<vector<double>>& matrix, const vector<string>& documents, double threshold);
map<string, vector<string>> performDisjointClustering(const vector<vector<double>>& matrix, const vector<string>& documents, double threshold);
vector<double> calculateCentroid(const vector<vector<double>>& matrix, const vector<string>& documents, const vector<string>& clusterDocs);
double calculateSimilarity(const vector<double>& centroid, const vector<double>& documentVector);
vector<double> extractDocumentVector(const vector<vector<double>>& matrix, int docIndex);
void printClusters(const map<string, vector<string>>& clusters);
vector<string> extractKeywordsFromDocuments(const vector<string>& documents);

int main() {
    // Extract keywords from document files
    vector<string> documents = {"text1.txt", "text2.txt", "text3.txt", "text4.txt", "text5.txt"};
    vector<string> keywords = extractKeywordsFromDocuments(documents);

    double threshold;
    int choice;

    // Ask user for threshold value
    cout << "Enter the threshold value: ";
    cin >> threshold;

    // Ask user for clustering choice
    cout << "Choose clustering type:\n";
    cout << "1. Overlapping Clustering\n";
    cout << "2. Disjoint Clustering\n";
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;

    // Get the frequency for each word in each document
    vector<vector<double>> matrix(keywords.size(), vector<double>(documents.size(), 0));
    countKeywordFrequencies(keywords, documents, matrix);

    // Print the input matrix
    printMatrix("Initial Keyword Frequency Matrix:", matrix, keywords, documents);

    // Perform clustering based on user choice
    if (choice == 1) {
        cout << "Overlapping Clusters:" << endl;
        map<string, vector<string>> overlappingClusters = performOverlappingClustering(matrix, documents, threshold);
        printClusters(overlappingClusters);
    } else if (choice == 2) {
        cout << "\nDisjoint Clusters:" << endl;
        map<string, vector<string>> disjointClusters = performDisjointClustering(matrix, documents, threshold);
        printClusters(disjointClusters);
    } else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}

vector<string> extractKeywordsFromDocuments(const vector<string>& documents) {
    set<string> keywordsSet;
    for (const string& fileName : documents) {
        ifstream file(fileName);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                size_t colonPos = line.find(':');
                if (colonPos != string::npos) {
                    string keyword = line.substr(0, colonPos);
                    keywordsSet.insert(keyword);
                }
            }
            file.close();
        } else {
            cerr << "Unable to open file " << fileName << endl;
        }
    }
    return vector<string>(keywordsSet.begin(), keywordsSet.end());
}

void countKeywordFrequencies(const vector<string>& keywords, const vector<string>& documents, vector<vector<double>>& matrix) {
    for (int docIndex = 0; docIndex < documents.size(); docIndex++) {
        string fileName = documents[docIndex];
        map<string, int> keywordCounts;
        for (const string& keyword : keywords) {
            keywordCounts[keyword] = 0;
        }

        ifstream file(fileName);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                for (const string& keyword : keywords) {
                    int count = extractCount(line, keyword);
                    keywordCounts[keyword] += count;
                }
            }
            file.close();
        } else {
            cerr << "Unable to open file " << fileName << endl;
        }

        for (int keywordIndex = 0; keywordIndex < keywords.size(); keywordIndex++) {
            string keyword = keywords[keywordIndex];
            matrix[keywordIndex][docIndex] = keywordCounts[keyword];
        }
    }
}

int extractCount(const string& line, const string& keyword) {
    if (line.find(keyword + ":") == 0) {
        size_t pos = line.find(' ');
        if (pos != string::npos) {
            try {
                return stoi(line.substr(pos + 1));
            } catch (const exception& e) {
                return 0;
            }
        }
    }
    return 0;
}

void printMatrix(const string& title, const vector<vector<double>>& matrix, const vector<string>& rowLabels, const vector<string>& colLabels) {
    cout << title << endl;
    cout << "      ";
    for (const string& label : colLabels) {
        cout.width(12);
        cout << label;
    }
    cout << endl;
    for (int i = 0; i < matrix.size(); i++) {
        cout.width(12);
        cout << rowLabels[i];
        for (double val : matrix[i]) {
            cout.width(12);
            cout << val;
        }
        cout << endl;
    }
    cout << endl;
}

map<string, vector<string>> performOverlappingClustering(const vector<vector<double>>& matrix, const vector<string>& documents, double threshold) {
    map<string, vector<string>> clusters;
    
    for (int docIndex = 0; docIndex < documents.size(); docIndex++) {
        string doc = documents[docIndex];
        bool addedToAnyCluster = false;

        for (auto& cluster : clusters) {
            const string& clusterName = cluster.first;
            vector<string>& clusterDocs = cluster.second;
            vector<double> centroid = calculateCentroid(matrix, documents, clusterDocs);
            double similarity = calculateSimilarity(centroid, extractDocumentVector(matrix, docIndex));

            if (similarity >= threshold) {
                clusterDocs.push_back(doc);
                addedToAnyCluster = true;
            } 
        }

        if (!addedToAnyCluster) {
            string newClusterName = "C" + to_string(clusters.size() + 1);
            clusters[newClusterName] = vector<string>{doc};
        }
    }
    return clusters;
}

map<string, vector<string>> performDisjointClustering(const vector<vector<double>>& matrix, const vector<string>& documents, double threshold) {
    map<string, vector<string>> clusters;
    for (int docIndex = 0; docIndex < documents.size(); docIndex++) {
        string doc = documents[docIndex];
        bool addedToCluster = false;

        string bestClusterName;
        double bestSimilarity = 0;

        for (auto& cluster : clusters) {
            const string& clusterName = cluster.first;
            vector<string>& clusterDocs = cluster.second;
            vector<double> centroid = calculateCentroid(matrix, documents, clusterDocs);
            double similarity = calculateSimilarity(centroid, extractDocumentVector(matrix, docIndex));

            if (similarity >= threshold && similarity > bestSimilarity) {
                bestClusterName = clusterName;
                bestSimilarity = similarity;
            }
        }

        if (!bestClusterName.empty()) {
            clusters[bestClusterName].push_back(doc);
            addedToCluster = true;
        }

        if (!addedToCluster) {
            clusters["C" + to_string(clusters.size() + 1)] = vector<string>{doc};
        }
    }
    return clusters;
}

vector<double> calculateCentroid(const vector<vector<double>>& matrix, const vector<string>& documents, const vector<string>& clusterDocs) {
    vector<double> centroid(matrix.size(), 0);
    for (const string& doc : clusterDocs) {
        int docIndex = find(documents.begin(), documents.end(), doc) - documents.begin();
        for (int i = 0; i < matrix.size(); i++) {
            centroid[i] += matrix[i][docIndex];
        }
    }
    for (int i = 0; i < centroid.size(); i++) {
        centroid[i] /= clusterDocs.size();
    }
    return centroid;
}

double calculateSimilarity(const vector<double>& centroid, const vector<double>& documentVector) {
    double similarity = 0;
    for (int i = 0; i < centroid.size(); i++) {
        similarity += centroid[i] * documentVector[i];
    }
    return similarity;
}

vector<double> extractDocumentVector(const vector<vector<double>>& matrix, int docIndex) {
    vector<double> documentVector(matrix.size());
    for (int i = 0; i < matrix.size(); i++) {
        documentVector[i] = matrix[i][docIndex];
    }
    return documentVector;
}

void printClusters(const map<string, vector<string>>& clusters) {
    for (const auto& entry : clusters) {
        cout << entry.first << " = ";
        for (const string& doc : entry.second) {
            cout << doc << " ";
        }
        cout << endl;
    }
}
