#include "ExpressionProject.h"

const string charSet[] = { "(", ")", "+", "-", "*", "/", "^" };

const double pi = 3.1415927;
const double e = 2.71828182846;

ExpressionProject::ExpressionProject(const string& input) { inString = input; }

int OpArgCount(const string& s)
{
    int val = 1;

    if(s == "+" || s == "-" || s == "*" || s == "/" || s == "^" || s == "LOG" || s == "RT")
        val = 2;

    return val;
}

int OpPrecedence(const string& s)
{
    int precedence = 1;

    if(s == "^" || s == "LOG" || s == "RT")
    	precedence = 4;

    else if(s == "*" || s == "/")
        precedence = 3;

    else if(s == "+" || s == "-")
        precedence = 2;

    return precedence;
}

bool OperLeft(const string& s)
{
    bool OperLeft = false;

    if(s == "*" || s == "/" || s == "%" || s == "+" || s == "-")
        OperLeft = true;

    else if(s == "=" || s == "!")\
        OperLeft = false;

    return OperLeft;
}

bool IsOperator(const string& s)
{
    return s == "+" || s == "-" || s == "/" || s == "*" || s == "!" || s == "%" ||  s == "=" || s == "LOG" || s == "RT" || s == "^";
}

string ToUpper(string input)
{
  for(string::iterator it = input.begin();
        it != input.end();
        ++it)
  {
    *it = toupper(*it);
  }

  return input;
}

bool IsPi(const string& s)
{
    bool isPi = false;

    if(ToUpper(s) == "PI")
        isPi = true;

    return isPi;
}

bool IsE(const string& s)
{
    bool isE = false;

    if(ToUpper(s) == "E")
        isE = true;

    return isE;
}

bool IsAns(const string& s)
{
	bool isAns = false;

	if(ToUpper(s) == "ANS")
		isAns = true;

	return isAns;
}

bool IsNumber(const string& s)
{
    string::const_iterator it = s.begin();
    while(it != s.end() && isdigit(*it, locale()))
    {
        ++it;
    }

    return !s.empty() && it == s.end();
}


bool ExpressionProject::MatchingParetheses()
{
    vector<string> inputs;

    const size_t nLeft  = count(inString.begin(), inString.end(), '(');
    const size_t nRight = count(inString.begin(), inString.end(), ')');

	return nLeft == nRight && !inString.empty();
}

void ExpressionProject::Tokenize(list<string>& tokens, const string& delimiter)
{
    size_t size = sizeof(charSet) / sizeof(string);

    for(int i = 0; i < static_cast<int>(size); i++)
    {
        string s = charSet[i];
        ReplaceAll(inString, s, " " + s + " ");
    }

    size_t next_pos = 0;
    size_t init_pos = inString.find_first_not_of(delimiter, next_pos);

    while(next_pos != string::npos &&
            init_pos != string::npos)
    {
        // Get next delimiter position
        next_pos = inString.find(delimiter, init_pos);

        string token = inString.substr(init_pos, next_pos - init_pos);
        tokens.push_back(token);

        init_pos = inString.find_first_not_of(delimiter, next_pos);
    }

    string firstToken = tokens.front();
    if(firstToken == "-")
    {
        list<string>::iterator it = tokens.begin();
        it++;

		if(it == tokens.end())
		{
			return;
		}

        string nextToken = *(it);

        if(IsNumber(nextToken))
        {
            tokens.pop_front();
            tokens.front() = firstToken + nextToken;
        }
		else if(nextToken == "(")
		{
			tokens.front() = firstToken + "1";
			tokens.insert(it, "*");
		}

		else if(nextToken == "-" && firstToken == "-")
		{
			list<string>::iterator nit = it;
			advance(nit, -1);
			tokens.erase(it);
			tokens.erase(nit);
		}
    }

    typedef list<string>::iterator t_iter;
    string prevToken = "";
    for(t_iter it = tokens.begin(); it != prev(tokens.end()); it++)
    {
        string token = *it;

        list<string>::iterator nit = it;
        advance(nit, 1);

        if(nit == tokens.end())
            break;

        string ntoken = *nit;

        if(token == "-" && prevToken == "(")
        {
            if(IsNumber(ntoken))
            {
                tokens.erase(nit);
                *it = "-" + ntoken;
				token = *it;
            }
        }

        else if(token == "-" && (IsOperator(prevToken) || prevToken == "^" || prevToken == "%"))
        {
			if(token == "-" && prevToken == "-")
			{
				list<string>::iterator nit = it;
				list<string>::iterator nnit = nit;
				nnit++;
				advance(nit, -1);
				tokens.erase(it);
				*nit = "+";

				list<string>::iterator pnit = nit;
				advance(pnit, -1);

				if(IsOperator(*pnit) || *pnit == "(")
					tokens.erase(nit);

				token = *nnit;
				it = nnit;

				if(it == prev(tokens.end()))
					break;

			}
            else if(IsNumber(ntoken))
            {
				bool exit = false;
				if(nit == prev(tokens.end()))
					exit = true;

                tokens.erase(nit);
                *it = "-" + ntoken;
				token = *it;

				if(exit)
					break;
            }
			else if(ntoken == "(")
			{
				*it = "-1";
				token = *it;
				tokens.insert(nit, "*");
			}
        }

        prevToken = token;
    }

    prevToken = "";
    t_iter prevIt;

    for(t_iter it = tokens.begin(); it != tokens.end(); it++)
    {
        string token = *it;

        if(token == "(" && prevToken == "-")
        {
			tokens.insert(it, "1");
            tokens.insert(it, "*");
        }

        prevToken = token;
        prevIt = it;
    }
}

void ExpressionProject::ReplaceAll(string& str, const string& from, const string& to)
{
    size_t start_pos = 0;

    while((start_pos = str.find(from, start_pos)) != string::npos)
    {
         str.replace(start_pos, from.length(), to);
         start_pos += to.length();
    }
}

bool ExpressionProject::Evaluate(const vector<string>& rpn, string& result)
{
    typedef vector<string>::const_iterator rpn_iter;
    stack<string> stack;

    for(rpn_iter it = rpn.begin(); it != rpn.end(); it++)
    {
        string token = *it;

        if(IsNumber(token) || IsAns(token)  || IsPi(token) || IsE(token))
        {
            if(IsPi(token))
            {
                stringstream s;
                s << pi;
                token = s.str();
            }
            else if(IsE(token))
            {
                stringstream s;
                s << e;
                token = s.str();
            }
            else if(IsAns(token))
            {
            	stringstream s;
            	s << ans;
            	token = s.str();
            }
            stack.push(token);
        }

        else if(IsOperator(token))
        {
			double result = 0.0;

            unsigned int numArgs = OpArgCount(ToUpper(token));

			bool isUnary = false;
            unsigned int stackArgs = stack.size();
			vector<double> args;

			if(stackArgs < numArgs)
            {
				if(stackArgs == 1 && numArgs == 2 &&(token == "+" || token == "-"))
				{
					string value = stack.top();
					result = strtod(value.c_str(), NULL);
					stack.pop();
					isUnary = true;
				}
				else
					return false;

            }
			else
			{
				while(numArgs > 0)
				{
					string value = stack.top();
					double d = strtod(value.c_str(), NULL);
					args.push_back(d);
					stack.pop();
					numArgs--;
				}
			}

            if(IsOperator(token) && !isUnary)
            {
                double d2 = args[0];
                double d1 = args[1];

                if(token == "+")
                    result = d1 + d2;

                else if(token == "-")
                    result = d1 - d2;

                else if(token == "*")
                    result = d1 * d2;

                else if(token == "/")
                    result = d1 / d2;

                else if(token == "LOG")
                {
                	double d = args[0];
                	double base = args[1];
                	result =(int)(log(base) / log(d));
                }

                else if(token == "EXP")
                	result = exp(args[0]);

                else if(token == "^")
                {
                	double d2 = args[0];
                	double d1 = args[1];
                	result = pow((double) d1, d2);
                }

                else if(token == "RT")
                {
                	double d2 = args[0];
                	double d1 = args[1];
                	result = pow(d2, 1.0/d1);
                }
            }

            if(result ==(long)result)
                result = long(result);

            stringstream s;
            s << result;
            stack.push(s.str());
        }
    }

    if(stack.size() == 1)
    {
		result = stack.top();

		double res = strtod(result.c_str(), NULL);
		if(res ==(long) res)
		{
			long lres =(long) res;
			stringstream s;
			s << lres;
			result = s.str();
		}
		return true;
    }
    return false;
}

bool ExpressionProject::InfixToRPN(vector<string>& inputs)
{
	list<string> infix;
    Tokenize(infix, " ");

    typedef list<string>::const_iterator tok_iter;
    stack<string> stack;
    queue<string> outputQueue;

    bool success = true;

    for(tok_iter it = infix.begin(); it != infix.end(); it++)
    {
        string token = *it;

        if(IsNumber(token) || IsAns(token) || IsPi(token) || IsE(token))
            outputQueue.push(token);

        else if(IsOperator(token))
        {
            while(!stack.empty() && IsOperator(stack.top()) &&
                  ((OperLeft(token) && OpPrecedence(token) == OpPrecedence(stack.top())) ||
                    (OpPrecedence(token) < OpPrecedence(stack.top()))))
            {
                string stackToken = stack.top();
                stack.pop();
                outputQueue.push(stackToken);
            }

            stack.push(token);

        }
        else if(token == "(")
            stack.push(token);

        else if(token == ")")
        {
            while(!stack.empty() && stack.top() != "(")
            {
                outputQueue.push(stack.top());
                stack.pop();
            }

            if(!stack.empty())
            {
                string stackToken = stack.top();

                if(stackToken != "(")
                    success = false;
            }
            else
                return false;

            stack.pop();

            if(!stack.empty())
                string stackToken = stack.top();
        }
    }

    while(!stack.empty())
    {
        outputQueue.push(stack.top());
        stack.pop();
    }

    while(!outputQueue.empty())
    {
      string token = outputQueue.front();
      inputs.push_back(token);
      outputQueue.pop();
    }

    return success;
}
