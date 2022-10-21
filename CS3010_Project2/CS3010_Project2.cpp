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
bool convertDiagDom(vector<vector<double>>&, vector<double>&);
void jacobiMethod(vector<vector<double>>, vector<double>, vector<double>, double);
void gaussSeidel(vector<vector<double>>, vector<double>, vector<double>, double);
void printVector(vector<double>);
void printMatrix(vector<vector<double>>, vector<double>);

int main()
{
    int numE = 0, menuOp = 0;
    double stopErr= 0.0, temp;
    vector<vector<double>> coef;
    vector<double> bVals;
    vector<double> startSolution;

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

    if (!checkDiagDom(coef))
    {
        if (convertDiagDom(coef, bVals))
        {
            cout << "Your coefficients were rearranged to be diagonally dominant." << endl;
        }
        else
        {
            cout << "Your system of linear euqations could not be rearranged to be diagonally dominant. Results may not be accurate because of this." << endl;
        }
    }

    cout << "\nSolving with Jacobi's Method:\n";
    jacobiMethod(coef, bVals, startSolution, stopErr);
    cout << "\nSolving with Gauss Seidel:\n";
    gaussSeidel(coef, bVals, startSolution, stopErr);
}

void getUserInput(vector<vector<double>>& coef, vector<double>& bVals, int numE)
{
    string rowInput = "", holder = "";

    cin.clear();
    cin.ignore(INT_MAX, '\n');

    cout << "\nEnter the coefficients and b values of the equations row by row:\n";
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
    
    cout << "\nEnter file name: ";
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

bool convertDiagDom(vector<vector<double>>& coef, vector<double>& bVals)
{
    //Matrix m will be a copy of coef matrix. Will need to keep original matrix until program can confirm it is possible to convert it to diagonally dominant.
    vector<vector<double>> m(coef); 
    vector<double> b(bVals);
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
        bVals.at(i) = b.at(rowIndex.at(i));
    }

    return true;
}

void jacobiMethod(vector<vector<double>> coef, vector<double> bVals, vector<double> startSolution, double err)
{
    double sum = 0.0, diag;
    double lsum1 = 0.0, lsum2 = 0.0, l2;
    vector<double> xVect(startSolution), yVect;

    for (int k = 0; k < 50; k++)
    {
        yVect = xVect;
        for (int i = 0; i < coef.size(); i++)
        {
            diag = coef.at(i).at(i);
            sum = bVals.at(i);
            for (int j = 0; j < coef.at(i).size(); j++)
            {
                if (j != i)
                {
                    sum -= coef.at(i).at(j) * yVect.at(j);
                }
            }
            xVect.at(i) = sum / diag;
        }

        //Calculating L2 normal form
        lsum1 = 0.0;
        lsum2 = 0.0;
        for (int i = 0; i < xVect.size(); i++)
        {
            lsum1 += pow(xVect.at(i) - yVect.at(i), 2);
            lsum2 += pow(xVect.at(i), 2);
        }
        l2 = sqrt(lsum1) / sqrt(lsum2);

        if (l2 < err)
        {
            //The method has converged
            cout << "Jaccobi's method converged on iteration " << k + 1 << ". Approximate solution is x = [";
            printVector(xVect);
            cout << "]  ";
            cout << "Error value is: " << l2 << endl;
            return;
        }

        //Display the xVect at current iteration
        cout << "Iteration " << k + 1 << ": x = [";
        printVector(xVect);
        cout << "]  ";
        cout << "Error value is: " << l2 << endl;
    }
    cout << "Maximum iterations has been reached. Refer to iteration 50 for final result" << endl;
}

void gaussSeidel(vector<vector<double>> coef, vector<double> bVals, vector<double> startSolution, double err)
{
    double sum = 0.0, diag;
    double lsum1 = 0.0, lsum2 = 0.0, l2;
    vector<double> xVect(startSolution), yVect;

    for (int k = 0; k < 50; k++)
    {
        yVect = xVect; //yVect here will be used to remember what the xVect was in the previous iteration so we can calculate the l2.
        for (int i = 0; i < coef.size(); i++)
        {
            diag = coef.at(i).at(i);
            sum = bVals.at(i);
            for (int j = 0; j < i; j++)
            {
                sum -= coef.at(i).at(j) * xVect.at(j);
            }

            for (int j = i + 1; j < coef.at(i).size(); j++)
            {
                sum -= coef.at(i).at(j) * xVect.at(j);
            }
            xVect.at(i) = sum / diag;
        }

        //Calculating L2 normal form
        lsum1 = 0.0;
        lsum2 = 0.0;
        for (int i = 0; i < xVect.size(); i++)
        {
            lsum1 += pow(xVect.at(i) - yVect.at(i), 2);
            lsum2 += pow(xVect.at(i), 2);
        }
        l2 = sqrt(lsum1) / sqrt(lsum2);

        if (l2 < err)
        {
            //The method has converged
            cout << "Gauss Seidel converged on iteration " << k + 1 << ". Approximate solution is x = [";
            printVector(xVect);
            cout << "]  ";
            cout << "Error value is: " << l2 << endl;
            return;
        }

        //Display the xVect at current iteration
        cout << "Iteration " << k + 1 << ": x = [";
        printVector(xVect);
        cout << "]  ";
        cout << "Error value is : " << l2 << endl;
    }
    cout << "Maximum iterations has been reached. Refer to iteration 50 for final result" << endl;
}

void printVector(vector<double> v)
{
    for (int i = 0; i < v.size() - 1; i++)
    {
        cout << v.at(i) << ", ";
    }
    cout << v.at(v.size() - 1);
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