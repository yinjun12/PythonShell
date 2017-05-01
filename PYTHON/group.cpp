#include"python.h"
//*************************Group类的实现*************
void Group::GetDataFromString(const string& str)//str=(2,4,5,'string',(3,6,'sdf'),4)
{
	int i = 1;//从str的第二个字符开始到倒数第二个结束,为了统一操作需要遍历到最后一个并把遍历最后一个时，把ch设为‘，’
	vector<char>bracket;
	bool flag = true;//标记语句中是否出现语法问题
	string element;
	int bracket_maxnum = 0;
	while (flag&&i < str.size())
	{
		char ch = str[i];
		if (i == str.size() - 1)//让最后一个元素也进来
			ch = ',';
		switch (ch)
		{
		case '(':
			bracket.push_back(ch);
			if (bracket.size() > bracket_maxnum)
				bracket_maxnum = bracket.size();
			element += ch;
			break;
		case ')':
			if (!bracket.empty() && bracket.back() == '(')
			{
				bracket.pop_back();
				element += ch;
			}
			else
				flag = false;
			break;
		case ',':
			if (element == "")//遇到了",,"两个逗号相邻的情况
			{
				flag = false;
				break;
			}
			if (bracket.empty())//括号栈为空，此时element为一个Type元素，可以进栈
			{
				Type temp_t;
				if (bracket_maxnum == 0)//没有括号的元素
				{
					int a = judgeValueType(element);
					if (a == INTEGER || a == FLOAT)//对没有括号的元素根据不同类型进行入队处理
					{
						stringstream ss(element);
						ss >> temp_t.value;
						temp_t.type = a;
					}
					else if (a == STRING)
					{
						/*removeSymbol(element, "\"");
						removeSymbol(element,"'");*/
						temp_t.val_str = element;
						temp_t.type = STRING;
					}
					else
						flag = false;
				}
				else//对有括号的单个元素进行处理
				{
					temp_t.val_str = element;
					temp_t.type = bracket_maxnum + OFFSET;
					bracket_maxnum = 0;//记录下一个带括号元素的括号层数
				}
				m_Data.push_back(temp_t);
				element = "";//清空该元素，记录下一个元素
			}
			else//括号栈不为空，则,属于其中的一个元素放入到element
				element += ',';
			break;
		default:
			element += ch;
			break;
		}//switch结束			
		i++;
	}
	if (!flag)
	{
		m_Data.clear();
		cout << "\tvalid syntax get data from string in Group!" << endl;
	}
}
Type Group::operator[](int index)const
{
	if (abs(index) >= m_Data.size())
	{
		cout << "\tvalid index in Group::operator[]!" << endl;
		exit(-1);
	}
	else if (index < 0)
	{
		return m_Data[m_Data.size() - index];
	}
	else
	{
		return m_Data[index];
	}
}
Group Group::operator[](const string & str)//gr["1:3"]
{
	int start = -1, end = -1, offset = -1;//起点，终点，偏移
	vector<int>temp_index(3, INT_MAX);//暂时存放
	string temp_str = str;
	removeSymbol(temp_str, "[");
	removeSymbol(temp_str, "]");
	stringstream ss(temp_str);
	string line = "";
	int i = 0;
	while (getline(ss, line, ':'))//读出：：之前的数字，没有按照默认值来算
	{
		if (line != "")
			temp_index[i] = stoi(line);
		i++;
	}
	bool flag = true;//用来记录是否有语法错误
	if (temp_index[2] == INT_MAX || temp_index[2] >= 0)//在偏移为正的情况下。偏移默认为1
	{
		if (temp_index[2] == 0)
			flag = false;
		else if (temp_index[2] == INT_MAX)
			offset = 1;
		else
			offset = temp_index[2];
		//在偏移为正的情况下：start默认为0，end默认为最后一个元素。如果存在：小于0的话是倒数的算，大于0直接是这个
		temp_index[0] == INT_MAX ? start = 0 : temp_index[0] < 0 ? start = m_Data.size() + temp_index[0] : start = temp_index[0];
		temp_index[1] == INT_MAX ? end = m_Data.size() - 1 : temp_index[1] < 0 ? end = m_Data.size() + temp_index[1] : end = temp_index[1];
	}
	else//偏移为负的情况
	{
		offset = temp_index[2];
		temp_index[0] == INT_MAX ? start = m_Data.size() - 1 : temp_index[0] < 0 ? start = m_Data.size() + temp_index[0] : start = temp_index[0];
		temp_index[1] == INT_MAX ? end = 0 : temp_index[1] < 0 ? end = m_Data.size() + temp_index[1] : end = temp_index[1];
	}
	Group gr;
	if (!flag)
		return gr;
	if (offset > 0)
	{
		if (start < 0)//确保i和startIndex是在m_vData长度的范围内
		{
			i = 0;
		}
		else
		{
			i = start;
		}
		for (; i < end&&i<m_Data.size(); i += offset)
			gr.addType(m_Data[i]);
	}
	else
	{
		if (start > m_Data.size() - 1)//确保i和startIndex是有效的
			i = m_Data.size() - 1;
		else
			i = start;
		for (; i > end&&i >= 0; i += offset)
			gr.addType(m_Data[i]);
	}
	return gr;
}
ostream& operator<<(ostream&os, const Group&gro)
{
	os << "(";
	for (int i = 0; i < gro.m_Data.size(); i++)
	{
		os << gro.m_Data[i];
		if (i == gro.m_Data.size() - 1)
			os << ")";
		else
			os << ",";
	}
	return os;
}

Group operator*(const Group& gro, int cnt)
{
	Group res;
	for (int i = 0; i < cnt; i++)
		for (int j = 0; j < gro.m_Data.size(); j++)
			res.addType(gro.m_Data[j]);
	return res;
}
Group Tuple(const List&content)
{
	Group res;
	for (auto it : content.m_vData)
		res.addType(it);
	return res;
}
Group Tuple(const string &str)
{
	Group res;
	for (char ch : str)
	{
		string temp = "";
		temp += ch;
		res.addType(Type(0, temp, 2));
	}
	return res;
}
