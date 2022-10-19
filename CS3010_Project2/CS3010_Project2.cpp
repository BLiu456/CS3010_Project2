/*
Name: Benjamin Liu
Class: CS3010
Assignment: Programming Project 2 
Due Date: 10/23/2022
Coded in C++ with Visual Studios 2022
*/

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>
using namespace std;

//Function Prototypes here
void getUserInput(vector<vector<double>>&, vector<double>&, int);
bool getFileInput(vector<vector<double>>&, vector<double>&, int);
//void checkDiagDom();
//vector<double> jacobiMethod();
//vector<double> gaussSeidel();
void printVector(vector<double>);
void printMatrix(vector<vector<double>>, vector<double>);

int main()
{
    int numE = 0, menuOp = 0;
    double stopErr= 0.0, temp;
    vector<vector<double>> coef;
    vector<double> bVals;
    vector<double> startSolution;
    vector<double> answers;

    cout << "Enter the number of linear equations to solve: ";
    cin >> numE;

    while (true)
    {
        cout << "\nChoose from the following options:\n"
            "1. Enter numbers manually from command line\n"
            "2. Load a file\n"
            "Enter option number: ";
        cin >> menuOp;

        if (menuOp == 1)
        {
            getUserInput(coef, bVals, numE);
            break;
        }
        else if (menuOp == 2)
        {
            bool fileObtain = getFileInput();

            if (!fileObtain)
            {
                return 0;
            }
            break;
        }
        else if (!cin)
        {
            cout << "Invalid option. Please enter a 1 or 2.\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
        else
        {
            cout << "Invalid option. Please enter a 1 or 2.\n";
        }
    }

    cout << "Please enter the desired stopping error: ";
    cin >> stopErr;

    cout << "Please enter starting solution: ";
    for (int i = 0; i < numE; i++)
    {
        cin >> temp;
        startSolution.push_back(temp);
    }

}

void getUserInput(vector<vector<double>>& coef, vector<double>& bVals, int numE)
{
    string rowInput = "", holder = "";

    cin.clear();
    cin.ignore(INT_MAX, '\n');

    cout << "Enter the coefficients and b values of the equations row by row:\n";
    for (int i = 0; i < numE; i++)
    {
        getline(cin, rowInput);
        istringstream ss(rowInput);

        vector<double> tempVect;
        while (ss >> holder)
        {
            if (ss.eof())
            {
                bVals.push_back(stod(holder));
                continue;
            }

            tempVect.push_back(stod(holder));
        }
        coef.push_back(tempVect);
    }
}

bool getFileInput(vector<vector<double>>& coef, vector<double>& bVals, int numE)
{
    string filename = "", rowInput = "", holder = "";
    
    cout << "Enter file name: ";
    cin >> filename;

    ifstream matFile;
    matFile.open(filename);
    if (!matFile)
    {
        cout << "Could not open file, ending program." << endl;
        return false;
    }

    for (int i = 0; i < numE; i++)
    {
        getline(matFile, rowInput);
        istringstream ss(rowInput);
        vector<double> tempVect;

        while (ss >> holder)
        {
            if (ss.eof())
            {
                bVals.push_back(stod(holder));
                continue;
            }

            tempVect.push_back(stod(holder));
        }
        coef.push_back(tempVect);
    }

    cout << "Contents of " << filename << ":" << endl;
    printMatrix(coef, bVals);
    matFile.close();
    return true;
}
void printVector(vector<double> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << v.at(i) << " ";
    }
    cout << endl;
}

void printMatrix(vector<vector<double>> m, vector<double> n)
{
    for (int i = 0; i < m.size(); i++)
    {
        for (int j = 0; j < m.at(i).size(); j++)
        {
            cout << m.at(i).at(j) << " ";
        }
        cout << n.at(i) << endl;
    }
}