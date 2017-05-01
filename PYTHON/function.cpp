#include"python.h"
//�������ȼ���0-6���δ���+ - * / ( ) #
map<char, int>mp{ { '+',0 },{ '-',1 },{ '*',2 },{ '/',3 },{ '(',4, },{ ')',5 },{ '#',6 },{ '%',7 },{ '^',8 } };
//1������ڣ�0����С�ڣ�2�������
const vector<vector<int>>priority{ { 1,1,0,0,0,1,1,0,0 },{ 1,1,0,0,0,1,1,0,0 },{ 1,1,1,1,0,1,1 ,0,0 },
{ 1,1,1,1,0,1,1,0,0 },{ 0,0,0,0,0,2,1,0,0 },{ 1,1,1,1,2,1,1,1,1 },{ 0,0,0,0,0,0,2,0,0 },{ 1,1,1,1,1,0,1,1,0 },{ 1,1,1,1,1,0,1,1,1 } };
//�������е�str��mem_exp���������ַ���
int judgeValueType(const string &str)//�ж���������
{
	string str_temp = "0123456789";
	if (count(str.begin(), str.end(), '"') == 2 && str.front() == '"'&&str.back() == '"'
		|| count(str.begin(), str.end(), '\'') == 2 && str.front() == '\''&&str.back() == '\'')//find the alpha
	{//������ʽ��"dfd",������dfd,��˫����������־�ַ��������������ַ�������,python��'dfd'Ҳ���ַ���
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
int transCharToInt(char ch)//��ʮ�����ƻ�˽����е��ַ�ת��Ϊ����
{
	string transtable = "abcdefABCDEF";//16���Ƶ��ַ�����
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
{//ת����ʮ�����е���������
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
{//ת��ʮ����˽����е�С�����ֵ�ʮ����
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

