#include <iostream>
#include <vector>
#include <set>
#include <iomanip>

using namespace std;

// Function to calculate precision and recall
double calculatePrecision(int Ra, int A)
{
    return (A > 0) ? (double)Ra / A : 0.0;
}

double calculateRecall(int Ra, int R)
{
    return (R > 0) ? (double)Ra / R : 0.0;
}

// Function to calculate harmonic mean (F-measure)
double calculateFMeasure(double precision, double recall)
{
    return (precision + recall != 0) ? 2 * (precision * recall) / (precision + recall) : 0.0;
}

// Function to calculate E-measur;e
double calculateEMeasure(double precision, double recall, double b)
{
    return (precision + recall != 0) ? 1 - (1 / ((b * b * (1 / recall)) + (1 / precision))) : 1.0;
}

// Main program to calculate precision, recall, F-measure and E-measure
int main()
{
    vector<string> retrieved = {"d123", "d84", "d56", "d6", "d8", "d9", "d511", "d129", "d187", "d25", "d38", "d48", "d250", "d113", "d3"};
    set<string> relevant = {"d3", "d5", "d9", "d25", "d39", "d44", "d56", "d71", "d89", "d123"};

    int Ra = 0;              // Relevant documents retrieved
    int A = 0;               // Total retrieved
    int R = relevant.size(); // Total relevant documents

    double precision, recall, F, E;

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

        precision = calculatePrecision(Ra, A);
        recall = calculateRecall(Ra, R);

        // Output the table row for precision and recall
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
        cout << setw(20) << right << "| " + to_string(precision * 100) + " |";
        cout << setw(20) << right << "| " + to_string(recall * 100) + " |" << endl;
    }

    // Calculate F-measure for the full set
    precision = calculatePrecision(Ra, A);
    recall = calculateRecall(Ra, R);
    F = calculateFMeasure(precision, recall);

    // Calculate E-measure with different values of b
    double b = 1.0; // E-measure parameter
    double E_b_greater_1 = calculateEMeasure(precision, recall, 1.5);
    double E_b_equal_1 = calculateEMeasure(precision, recall, 1.0);
    double E_b_less_1 = calculateEMeasure(precision, recall, 0.5);

    // Output F-measure and E-measure results
    cout << "\nHarmonic mean (F-measure) for the full set is: " << fixed << setprecision(2) << F << endl;

    // E-value for different values of b
    cout << "\nE-value table (b > 1, b = 1, b < 1):" << endl;
    cout << "b > 1   | b = 1   | b < 1" << endl;
    cout << "--------------------------" << endl;
    cout << fixed << setprecision(2);
    cout << E_b_greater_1 << "   | " << E_b_equal_1 << "   | " << E_b_less_1 << endl;

    return 0;
}
