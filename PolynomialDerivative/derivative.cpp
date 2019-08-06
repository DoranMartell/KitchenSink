#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cassert>

using namespace std;

/* Interface of this function was provided by 'THEM',
	evaluate derivative of given polynomial and return it as a string,
	where its addends ordered by powers of x in descending order */
string derivative(string polynomial);

/* Helper function which is called in derivative(), as each addend will be parsed deeply into coefficients and exponents,
 	so, natuarally, we split parsing into two steps:
 	parsing input string into addends and parsing each addend into coefficients and exponents
 	This function performs first step */
vector<string> ParseIntoSeparateAddends(string polynomial);

/* Helper function used in derivative() to parse each addend, to change their coefficients and exponents (derivative evaluation)
		and then to combine derivatives of each addend into one polynomial */
map<int, int> ParseIntoExponentCoeffMap(const vector<string> & addends_vector);

int main(int argc, char const *argv[])
{
	string pol;
	string output;

	pol = "x^2+x";
	cout << "Another test polynomial: " << pol << endl;
	output = derivative(pol);
	assert("2*x+1" == output);

	pol = "120+x^2+x";
	cout << "Another test polynomial: " << pol << endl;
	output = derivative(pol);
	assert("2*x+1" == output);

	pol = "-999*x^3-99*x^4+8*x^5-7*x^2-235+x";
	cout << "Another test polynomial: " << pol << endl;
	output = derivative(pol);
	assert("40*x^4-396*x^3-2997*x^2-14*x+1" == output);

	pol = "2*x^100+100*x^2";
	cout << "Another test polynomial: " << pol << endl;
	output = derivative(pol);
	assert("200*x^99+200*x" == output);

	pol = "x^10000+x+1";
	cout << "Another test polynomial: " << pol << endl;
	output = derivative(pol);
	assert("10000*x^9999+1" == output);

	pol = "-x^2-x^3";
	cout << "Another test polynomial: " << pol << endl;
	output = derivative(pol);
	assert("-3*x^2-2*x" == output);

	pol = "x+x+x+x+x+x+x+x+x+x";
	cout << "Another test polynomial: " << pol << endl;
	output = derivative(pol);
	assert("10" == output);


	return 0;
}

vector<string> ParseIntoSeparateAddends(string polynomial)
{
	vector<string> addends_vector;

	while (!polynomial.empty())
	{
		string another_addend;

		if (polynomial[0] == '-')
		{
			another_addend += '-';
			polynomial.erase(0, 1);
		}

		int pos = polynomial.find_first_of("+-");
		// cout << "Pos of + or - (delimiters between addends) " << pos << endl;

		if (pos != -1)
			another_addend += polynomial.substr(0, pos);
		else
			another_addend += polynomial;

		// cout << "Another addend: " << another_addend << endl;
		addends_vector.push_back(another_addend);

		if (pos != -1)
		{	if (polynomial[pos] == '+')
				polynomial.erase(0, pos+1);
			else
				polynomial.erase(0, pos);
		}
		else
			polynomial.clear();

		// cout << "What is left of polynomial " << polynomial << endl;

	}

	// for (int i = 0; i < addends_vector.size(); ++i)
	// {
	// 	cout << "Another addend in vector of addends: " << addends_vector[i] << endl;
	// }

	return addends_vector;
}

map<int, int> ParseIntoExponentCoeffMap(const vector<string> & addends_vector)
{
	map<int, int> parsed_polynom;

	for (auto separate_addend : addends_vector)
	{
		int pos = separate_addend.find('x');

		if (pos == -1)
			continue;

		string coeff_str = separate_addend.substr(0, pos);
		int coeff_int = (pos == 0) ? 1
								   : (pos == 1 && coeff_str == "-") ? (-1)
								   								    :stoi(coeff_str);

		// cout << "Coefficient " << coeff_int << endl;

		pos = separate_addend.find('^');

		if (pos != -1)
		{
			string exponent_str = separate_addend.substr(pos + 1);
			int exponent_int = stoi(exponent_str);

			// cout << "Exponent (before taking derivative) " << exponent_int << endl;

			parsed_polynom[exponent_int-1] += coeff_int * exponent_int;
		}
		else
		{
			parsed_polynom[0] += coeff_int;
		}
	}

	return parsed_polynom;
}

string derivative(string polynomial)
{
	string result;
	vector<string> addends_vector = ParseIntoSeparateAddends(polynomial);
	map<int, int> parsed_polynom = ParseIntoExponentCoeffMap(addends_vector);

	ostringstream os;


	for (auto it = parsed_polynom.rbegin(); it != parsed_polynom.rend(); it++)
	{
		// cout << "another value in map: " << it->second << "*x^" << it->first << endl;
		
		if (it != parsed_polynom.rbegin() && it->second > 0)
		{
			os << "+";
		}

		os << it->second;

		if (it->first > 1)
		{
			os << "*x^" << it->first;
		}
		else if (it->first == 1)
		{
			os << "*x";
		}
		/* if it->first == 0 - no need to print anything */

	}

	result = os.str();
	// cout << "Result: " << result << endl;

    return result;
}
