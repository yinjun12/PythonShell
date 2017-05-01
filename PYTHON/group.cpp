#include"python.h"
//*************************Group���ʵ��*************
void Group::GetDataFromString(const string& str)//str=(2,4,5,'string',(3,6,'sdf'),4)
{
	int i = 1;//��str�ĵڶ����ַ���ʼ�������ڶ�������,Ϊ��ͳһ������Ҫ���������һ�����ѱ������һ��ʱ����ch��Ϊ������
	vector<char>bracket;
	bool flag = true;//���������Ƿ�����﷨����
	string element;
	int bracket_maxnum = 0;
	while (flag&&i < str.size())
	{
		char ch = str[i];
		if (i == str.size() - 1)//�����һ��Ԫ��Ҳ����
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
			if (element == "")//������",,"�����������ڵ����
			{
				flag = false;
				break;
			}
			if (bracket.empty())//����ջΪ�գ���ʱelementΪһ��TypeԪ�أ����Խ�ջ
			{
				Type temp_t;
				if (bracket_maxnum == 0)//û�����ŵ�Ԫ��
				{
					int a = judgeValueType(element);
					if (a == INTEGER || a == FLOAT)//��û�����ŵ�Ԫ�ظ��ݲ�ͬ���ͽ�����Ӵ���
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
				else//�������ŵĵ���Ԫ�ؽ��д���
				{
					temp_t.val_str = element;
					temp_t.type = bracket_maxnum + OFFSET;
					bracket_maxnum = 0;//��¼��һ��������Ԫ�ص����Ų���
				}
				m_Data.push_back(temp_t);
				element = "";//��ո�Ԫ�أ���¼��һ��Ԫ��
			}
			else//����ջ��Ϊ�գ���,�������е�һ��Ԫ�ط��뵽element
				element += ',';
			break;
		default:
			element += ch;
			break;
		}//switch����			
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
	int start = -1, end = -1, offset = -1;//��㣬�յ㣬ƫ��
	vector<int>temp_index(3, INT_MAX);//��ʱ���
	string temp_str = str;
	removeSymbol(temp_str, "[");
	removeSymbol(temp_str, "]");
	stringstream ss(temp_str);
	string line = "";
	int i = 0;
	while (getline(ss, line, ':'))//��������֮ǰ�����֣�û�а���Ĭ��ֵ����
	{
		if (line != "")
			temp_index[i] = stoi(line);
		i++;
	}
	bool flag = true;//������¼�Ƿ����﷨����
	if (temp_index[2] == INT_MAX || temp_index[2] >= 0)//��ƫ��Ϊ��������¡�ƫ��Ĭ��Ϊ1
	{
		if (temp_index[2] == 0)
			flag = false;
		else if (temp_index[2] == INT_MAX)
			offset = 1;
		else
			offset = temp_index[2];
		//��ƫ��Ϊ��������£�startĬ��Ϊ0��endĬ��Ϊ���һ��Ԫ�ء�������ڣ�С��0�Ļ��ǵ������㣬����0ֱ�������
		temp_index[0] == INT_MAX ? start = 0 : temp_index[0] < 0 ? start = m_Data.size() + temp_index[0] : start = temp_index[0];
		temp_index[1] == INT_MAX ? end = m_Data.size() - 1 : temp_index[1] < 0 ? end = m_Data.size() + temp_index[1] : end = temp_index[1];
	}
	else//ƫ��Ϊ�������
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
		if (start < 0)//ȷ��i��startIndex����m_vData���ȵķ�Χ��
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
		if (start > m_Data.size() - 1)//ȷ��i��startIndex����Ч��
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
