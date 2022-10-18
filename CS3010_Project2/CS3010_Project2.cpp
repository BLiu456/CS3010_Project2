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
void getUserInput();
bool getFileInput();
void checkDiagDom();
vector<double> jacobiMethod();
vector<double> gaussSeidel();

int main()
{
    int numE = 0, menuOp = 0;
    double stopErr= 0.0, temp;
    vector<vector<double>> coef;
    vector<double> bVal;
    vector<double> startSolution;
    vector<double> answers;

    cout << "Enter the number of linear equations to solve: ";
    cin >> numE;

    while (true)
    {
        cout << "\nChoose from the following options:\n"
            "1. Enter numbers manually from command line\n"
            "2. Load a file\n";
        cin >> menuOp;

        if (menuOp == 1)
        {
            getUserInput();
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
