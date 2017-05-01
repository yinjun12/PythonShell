#include"python.h"
//索引优先级，0-6依次代表+ - * / ( ) #
map<char, int>mp{ { '+',0 },{ '-',1 },{ '*',2 },{ '/',3 },{ '(',4, },{ ')',5 },{ '#',6 },{ '%',7 },{ '^',8 } };
//1代表大于，0代表小于，2代表等于
const vector<vector<int>>priority{ { 1,1,0,0,0,1,1,0,0 },{ 1,1,0,0,0,1,1,0,0 },{ 1,1,1,1,0,1,1 ,0,0 },
{ 1,1,1,1,0,1,1,0,0 },{ 0,0,0,0,0,2,1,0,0 },{ 1,1,1,1,2,1,1,1,1 },{ 0,0,0,0,0,0,2,0,0 },{ 1,1,1,1,1,0,1,1,0 },{ 1,1,1,1,1,0,1,1,1 } };
//后面所有的str是mem_exp处理过后的字符串
int judgeValueType(const string &str)//判断数据类型
{
	string str_temp = "0123456789";
	if (count(str.begin(), str.end(), '"') == 2 && str.front() == '"'&&str.back() == '"'
		|| count(str.begin(), str.end(), '\'') == 2 && str.front() == '\''&&str.back() == '\'')//find the alpha
	{//表现形式是"dfd",内容是dfd,即双引号用来标志字符串，但不属于字符串内容,python中'dfd'也算字符串
		return STRING;
	}
	else if (str.find_first_not_of(str_temp + '.') == -1)
	{
		if (count(str.begin(), str.end(), '.') == 1)
			return FLOAT;
		else if (count(str.begin(), str.end(), '.') == 0)
			return INTEGER;
		else
			return ERROR;
	}
	else
		return ERROR;
}
void removeSymbol(string &str, const string&symbol)
{
	int lpos = str.find_first_not_of(symbol);
	str.erase(0, lpos);
	int  rpos = str.find_last_not_of(symbol);
	str.erase(rpos + 1);
}
int transCharToInt(char ch)//把十六进制或八进制中的字符转换为整数
{
	string transtable = "abcdefABCDEF";//16进制的字符串表
	int temp_int = 0;
	if (isdigit(ch))
		temp_int = ch - '0';
	else if (transtable.find(ch) != -1)
	{
		if (islower(ch))
			temp_int = ch - 'a' + 10;
		else
			temp_int = ch - 'A' + 10;
	}
	else
		temp_int = -1;
	return temp_int;
}
Type transferToDecimal_int(const string &temp_str, int start, int end, int factor)
{//转换到十进制中的整数部分
	Type res;
	int sum = 0;
	for (int i = start; i < end; i++)
	{
		int temp_int = transCharToInt(temp_str[i]);
		if (temp_int == -1)
		{
			res.type = -1;
			break;
		}
		sum = sum * factor + temp_int;
	}
	res.value = sum;
	res.type = 0;
	return res;
}
Type transferToDecimal_float(const string &temp_str, int start, int end, float factor)
{//转换十六或八进制中的小数部分到十进制
	Type res;
	float sum = 0;
	for (int i = start; i > end; i--)
	{
		int temp_int = transCharToInt(temp_str[i]);
		if (temp_int == -1)
		{
			res.type = -1;
			break;
		}
		sum = sum * factor + temp_int;
	}
	res.value = sum*factor;
	res.type = 1;
	return res;
}
int Len(const List &lst)
{
	return lst.getLength();
}
Type Max(const List &lst)
{
	return lst.getMaxType();
}
Type Min(const List &lst)
{
	return lst.getMinType();
}

