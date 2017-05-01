#include"lisp.h"
int Lisp::judgeValueType(const string &str) const
{
	string str_temp = "0123456789.";
	if (str.find_first_not_of(str_temp) != -1)//find the alpha
	{
		return 2;
	}
	else if (str.find('.') != -1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}