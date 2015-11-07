// USE 'LOG' AND 'RT' CAPITALIZED TO WORK CORRECTLY
// PREV ITERATOR COMMANDS IN TOKENIZE METHOD ONLY WORK IN C++ 11 OR ABOVE

//Working Examples:
//[1]: 2 + 3 * 4 - 5 ^ 2 = -11
//[2]: 2 ^ 3 + 4 * 5 - 2 = 26
//[3]: ( 2 + 3 ) * ( 4 - 5 ) ^ 2  = 5
//[4]: 8 ^ ( -4 / 3 ) * 4 = 1 / 4
//[5]: 108 LOG 3 = 4

//[6]: 3 RT 108 * 3 RT 16 = 12

//[7]: (e ^ 3) LOG e = 3

//[8]: (7 - 3) LOG (2 RT 2) = 4
//[9]: 2 ^ (5 / 3 + pi) / ((3 RT 4) * 2 ^ pi) = 2

#include "ExpressionProject.h"

using namespace std;

double string_to_double( const string& s )
{
	istringstream i(s);
	double x;
	if (!(i >> x))
		return 0;
	return x;
}

long gcd(long a, long b)
{
    if (a == 0)
        return b;
    else if (b == 0)
        return a;

    if (a < b)
        return gcd(a, b % a);
    else
        return gcd(b, a % b);
}

void fractionize(double input)
{
	double integral = floor(input);
    double frac = input - integral;

    const long precision = 1000; // This is the accuracy.

    long gcd_ = gcd(round(frac * precision), precision);

    long denominator = precision / gcd_;
    long numerator = round(frac * precision) / gcd_;

    long add = integral * denominator;
    numerator = add + numerator;
    if(denominator != 1)
    	cout << numerator << " / " << denominator << endl;
    else
    	cout << numerator << endl;
}

int main(int argc, char** argv)
{
	vector<string> previous;
	int menuChoice = 0;

	cout << "----------------------------------------------------------" << endl;
	cout << "Expression calculator with fraction, root, and log support" << endl;
	cout << "----------------------------------------------------------" << endl << endl;

	while(menuChoice != 5)
	{
		cout << "[1] Compute a new expression." << endl;
		cout << "[2] Help." << endl;
		cout << "[3] Review previous expressions." << endl;
		cout << "[4] Convert floating point to fraction." << endl;
		cout << "[5] Quit." << endl;

		cout << endl << "Please remember to capitalize LOG and RT." << endl << "Please enter a menu option from 1 to 5: ";
		cin >> menuChoice;
		cin.ignore(1000, '\n');

		if(menuChoice == 1)
		{
			cout << endl << "Infix: ";
			string s;
			getline(cin, s);
			s.append(" ");

			ExpressionProject parser(s);

			vector<string> postfix;

			if (parser.InfixToRPN(postfix))
			{
				cout << "Postfix: ";
				for(int i = 0; i < (int)postfix.size(); i++)
				{
					cout << postfix[i] << " ";
				}

				string str_result;

				if (parser.Evaluate(postfix, str_result))
				{
					double result = string_to_double(str_result);
					cout << "\nResult = ";
					fractionize(result);

					cout << endl;

					parser.ans = result;
					previous.push_back(s + "= " + str_result);
				}
			}


			else
				cout << "Error. Please run again." << endl;
		}

		else if(menuChoice == 2)
		{
			cout << endl << "Compute a new expression with to receive postfix notation and answer." << endl;
			cout << "Enter in form of 'a + b' or 'a * b' or 'a / b' or any other permutation of the sort." << endl;
			cout << "To do a root, write in the form of 'ath root of b : a RT b', To do a logarithm, do 'log base a of b a LOG b'." << endl;
			cout << "For root and logarithm to work, make sure RT and LOG are capitalized." << endl << endl;
			cout << "If you wish to use the previous answer entered, use term 'ans' during computation." << endl;
		}
		else if(menuChoice == 3)
		{
			cout << endl << endl;
			for(int i = 0; i < (int)previous.size(); i++)
			{
				cout << previous[i] << endl;
			}
			cout << endl;
		}
		else if(menuChoice == 4)
		{
			cout << "Enter a decimal number: ";
			double input;
			cin >> input;

			cout << "Fractional form: ";
			fractionize(input);
			cout << endl;
		}
		else
		{
			break;
		}
	}



	return 0;
}
