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
bool checkDiagDom(vector<vector<double>>);
bool convertDiagDom(vector<vector<double>>&);
//vector<double> jacobiMethod();
//vector<double> gaussSeidel();
void printVector(vector<vector<double>>);
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
            bool fileObtain = getFileInput(coef, bVals, numE);

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

    printVector(coef);
    cout << endl;
    if (!checkDiagDom(coef))
    {
        if (convertDiagDom(coef))
        {
            cout << "Your coefficients were rearranged to be diagonally dominant." << endl;
        }
        else
        {
            cout << "Your system of linear euqations could not be rearranged to be diagonally dominant. Results may not be accurate because of this." << endl;
        }
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

        if (tempVect.size() != numE)
        {
            for (int j = tempVect.size(); j < numE; j++)
            {
                tempVect.push_back(0);
            }
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

        if (tempVect.size() != numE)
        {
            for (int j = tempVect.size(); j < numE; j++)
            {
                tempVect.push_back(0);
            }
        }

        coef.push_back(tempVect);
    }

    cout << "Contents of " << filename << ":" << endl;
    printMatrix(coef, bVals);
    matFile.close();
    return true;
}

bool checkDiagDom(vector<vector<double>> coef)
{
    double sum;
    for (int i = 0; i < coef.size(); i++)
    {
        sum = 0.0;
        for (int j = 0; j < coef.at(i).size(); j++)
        {
            if (j == i)
            {
                continue;
            }

            sum += abs(coef.at(i).at(j));
        }

        if (coef.at(i).at(i) < sum)
        {
            //Not strictly diagonly dominant
            return false;
        }
    }
    //At this point all rows were checked, and have satisfied the condition of being diagonally dominant
    return true;
}

bool convertDiagDom(vector<vector<double>>& coef)
{
    //Matrix m will be a copy of coef matrix. Will need to keep original matrix until program can confirm it is possible to convert it to diagonally dominant.
    vector<vector<double>> m(coef); 
    //rowIndex is used to keep track of which row should be at which index to make the matrix diagonally dominant
    vector<int> rowIndex(m.size(), -1); //-1 is used as a flag to make sure a spot is available at a certain index
    double rowMax, sum;
    int indexMax;

    for (int i = 0; i < m.size(); i++)
    {
        sum = 0.0;
        rowMax = 0.0;
        indexMax = 0;
        for (int j = 0; j < m.at(i).size(); j++)
        {
            if (abs(m.at(i).at(j)) > rowMax)
            {
                rowMax = abs(m.at(i).at(j));
                indexMax = j;
            }
            sum += abs(m.at(i).at(j));
        }

        sum -= rowMax; //Subtract the rowMax from the sum so that it is actually comparing the sum of everything else in that row with the rowMax.
        if (sum < rowMax)
        {
            if (rowIndex.at(indexMax) == -1) //If there is a -1 then it means that this index is available to have a value stored in it.
            {
                rowIndex.at(indexMax) = i; 
            }
            else
            {
                //Two rows will need to occupy the same spot for the matrix to become diagonally dominant, so it is not possible to convert the matrix.
                return false;
            }
        }
        else
        {
            return false;
        }
    }
   
    for (int i = 0; i < rowIndex.size(); i++) //Swapping the rows based on rowIndex
    {
        coef.at(i) = m.at(rowIndex.at(i));
    }

    return true;
}

void printVector(vector<vector<double>> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v.at(i).size(); j++)
        {
            cout << v.at(i).at(j) << " ";
        }
        cout << endl;
    }
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

