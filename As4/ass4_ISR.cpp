#include <iostream>
#include <vector>
#include <set>
#include <iomanip>

using namespace std;

// Function to calculate precision and recall
void calculatePrecisionRecall(const vector<string> &retrieved, const set<string> &relevant)
{
    int Ra = 0; // Number of relevant documents retrieved
    int A = 0;  // Number of documents retrieved
    double precision, recall;

    // Header for the output table
    cout << setw(45) << left << "| Documents"
         << setw(10) << "| Ra |"
         << setw(10) << "| A |"
         << setw(20) << "| Precision (%) |"
         << setw(20) << "| Recall (%) |" << endl;

    cout << "---------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < retrieved.size(); i++)
    {
        A++;
        if (relevant.find(retrieved[i]) != relevant.end())
        {
            Ra++;
        }

        precision = (double)Ra / A * 100;
        recall = (double)Ra / relevant.size() * 100;

        // Output row for current retrieved documents, Ra, A, precision, and recall
        for (int j = 0; j <= i; j++)
        {
            if (j != i)
            {
                cout << retrieved[j] << ", ";
            }
            else
            {
                cout << retrieved[j];
            }
        }

        cout << setw(35 - (int)(i * 4)) << " "; // Adjust the padding for the retrieved documents list
        cout << setw(10) << right << "| " + to_string(Ra) + " |";
        cout << setw(10) << right << "| " + to_string(A) + " |";
        cout << setw(20) << right << "| " + to_string(precision) + " |";
        cout << setw(20) << right << "| " + to_string(recall) + " |" << endl;
    }
}

// Function to calculate Harmonic mean (F-measure) and E-value
void calculateFandEValue(int j, const vector<string> &retrieved, const set<string> &relevant, double b = 1.0)
{
    int Ra = 0, A = 0;
    double precision, recall, F, E;

    for (int i = 0; i <= j; i++)
    {
        A++;
        if (relevant.find(retrieved[i]) != relevant.end())
        {
            Ra++;
        }
    }

    precision = (double)Ra / A;
    recall = (double)Ra / relevant.size();

    // Harmonic Mean (F-measure)
    if (precision + recall != 0)
    {
        F = 2 * ((precision * recall) / (precision + recall));
    }
    else
    {
        F = 0;
    }

    // E-value
    if (precision + recall != 0)
    {
        E = 1 - (1 / ((b * b * (1 / recall)) + (1 / precision)));
    }
    else
    {
        E = 1;
    }

    cout << "\nHarmonic mean (F" << j << ") is: " << fixed << setprecision(2) << F << endl;

    // E-value for different values of b
    cout << "\nE-value table (b > 1, b = 1, b < 1):" << endl;
    cout << "b > 1   | b = 1   | b < 1" << endl;
    cout << "--------------------------" << endl;

    double E_b_greater_1 = 1 - (1 / ((1.5 * 1.5 * (1 / recall)) + (1 / precision)));
    double E_b_equal_1 = 1 - (1 / ((1 * 1 * (1 / recall)) + (1 / precision)));
    double E_b_less_1 = 1 - (1 / ((0.5 * 0.5 * (1 / recall)) + (1 / precision)));

    cout << fixed << setprecision(2);
    cout << E_b_greater_1 << "   | " << E_b_equal_1 << "   | " << E_b_less_1 << endl;
}

int main()
{
    vector<string> retrieved = {"d123", "d84", "d56", "d6", "d8", "d9", "d511", "d129", "d187", "d25", "d38", "d48", "d250", "d113", "d3"};
    set<string> relevant = {"d3", "d5", "d9", "d25", "d39", "d44", "d56", "d71", "d89", "d123"};

    // Stepwise calculation of Precision and Recall
    calculatePrecisionRecall(retrieved, relevant);

    // Calculate Harmonic Mean (F-measure) and E-value for a user-specified j6
    int j;
    cout << "\nEnter value of j (0 - " << retrieved.size() - 1 << ") to find F(j) and E(j): ";
    cin >> j;

    if (j >= 0 && j < retrieved.size())
    {
        calculateFandEValue(j, retrieved, relevant);
    }
    else
    {
        cout << "Invalid value of j!" << endl;
    }

    return 0;
}
