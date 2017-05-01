#include"python.h"
//**************class LIst*********************
void List::getDataFromString(const string &str)//��[],[[],[]]������һ��Type,����value��ֵ��type�����ŵĲ���
											   //str="[1,2,"string",[1,2],[[1,2],[3,4],'string'],5]"
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
		case '[':
			bracket.push_back(ch);
			if (bracket.size() > bracket_maxnum)
				bracket_maxnum = bracket.size();
			element += '[';
			break;
		case ']':
			if (!bracket.empty() && bracket.back() == '[')
			{
				bracket.pop_back();
				element += ']';
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
				m_vData.push_back(temp_t);
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
		m_vData.clear();
		cout << "\tvalid syntax get data from string in list!" << endl;
	}
}
Type& List::operator[](int index)
{
	if (abs(index) >= m_vData.size())
	{
		cout << "\tvalid index in List::operator[]!" << endl;
		exit(-1);
	}
	else if (index < 0)
	{
		return m_vData[m_vData.size() - index];
	}
	else
	{
		return m_vData[index];
	}
}
List List::operator[](const string &str)//lst[1:4:1],str="[1:4:1]"
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
		temp_index[0] == INT_MAX ? start = 0 : temp_index[0] < 0 ? start = m_vData.size() + temp_index[0] : start = temp_index[0];
		temp_index[1] == INT_MAX ? end = m_vData.size() - 1 : temp_index[1] < 0 ? end = m_vData.size() + temp_index[1] : end = temp_index[1];
	}
	else//ƫ��Ϊ�������
	{
		offset = temp_index[2];
		temp_index[0] == INT_MAX ? start = m_vData.size() - 1 : temp_index[0] < 0 ? start = m_vData.size() + temp_index[0] : start = temp_index[0];
		temp_index[1] == INT_MAX ? end = 0 : temp_index[1] < 0 ? end = m_vData.size() + temp_index[1] : end = temp_index[1];
	}
	List lst;
	if (!flag)
		return lst;
	if (offset > 0)
	{
		if (start < 0)//ȷ��i��startIndex����m_vData���ȵķ�Χ��
		{
			lst.startIndex = 0;
			i = 0;
		}
		else
		{
			lst.startIndex = start;
			i = start;
		}
		for (; i < end&&i<m_vData.size(); i += offset)
			lst.addType(m_vData[i]);
	}
	else
	{
		if (start > m_vData.size() - 1)//ȷ��i��startIndex����Ч��
			i = m_vData.size() - 1;
		else
			i = start;
		lst.startIndex = end;
		if (end < 0)
			lst.startIndex = 0;
		for (; i > end&&i >= 0; i += offset)
			lst.addType(m_vData[i]);
	}
	if (lst.m_vData.empty())//��ʹ��ʼ������λ�÷�����������û��Ԫ�أ���startIndex��Ϊ��
		lst.startIndex = -1;
	lst.plast_Data = &m_vData;
	return lst;
}
List& List::operator=(const List& lst)
{
	if (plast_Data&&startIndex != -1)//��Ϊ������Operator[]���صģ�ɾ��������
	{
		auto it = (*plast_Data).erase((*plast_Data).begin() + startIndex, (*plast_Data).begin() + startIndex + m_vData.size());
		(*plast_Data).insert(it, lst.m_vData.begin(), lst.m_vData.end());
		plast_Data = NULL;
		startIndex = -1;
	}
	m_vData = lst.m_vData;
	return *this;
}
List& List::operator=(const Type&ty)
{
	List lst_temp;
	lst_temp.addType(ty);
	lst_temp.plast_Data = NULL;
	lst_temp.startIndex = -1;
	return operator=(lst_temp);
}
List List::operator+(const List &lst)const
{
	List reslst;
	for (auto it : m_vData)
		reslst.addType(it);
	for (auto it : lst.m_vData)
		reslst.addType(it);
	return reslst;
}
List operator*(const List &lst, int cnt)
{
	List reslst;
	for (int i = 0; i < cnt; i++)
	{
		for (int j = 0; j < lst.m_vData.size(); j++)
			reslst.addType(lst.m_vData[j]);
	}
	return reslst;
}
ostream& operator<<(ostream&os, const List&lst)
{
	os << "[";
	for (int i = 0; i < lst.m_vData.size(); i++)
	{
		os << lst.m_vData[i];
		if (i == lst.m_vData.size() - 1)
			os << "]";
		else
			os << ",";
	}
	return os;
}
bool List::in(const Type &ty)
{
	if (find(m_vData.begin(), m_vData.end(), ty) != m_vData.end())
		return true;
	else
		return false;
}
void List::Sort()
{
	sort(m_vData.begin(), m_vData.end());
}
int List::getLength()const
{
	return m_vData.size();
}
Type List::getMaxType()const//STL�е�max_element��min_element���õ���operator<
{
	auto it = max_element(m_vData.begin(), m_vData.end());
	return *it;
}
Type List::getMinType()const
{
	auto it = min_element(m_vData.begin(), m_vData.end());
	return *it;
}
void List::Del(int index)
{
	if (index < 0 || index >= m_vData.size())
	{
		cout << "error index in Del!" << endl;
		exit(-1);
	}
	m_vData.erase(m_vData.begin() + index);
}
void List::Append(const Type& ty)
{
	m_vData.push_back(ty);
}
int List::Count() const
{
	return m_vData.size();
}
void List::Extend(const List& lst)
{
	for (auto it : lst.m_vData)
		m_vData.push_back(it);
}
int List::Index(const Type& ty) const
{
	auto it = find(m_vData.begin(), m_vData.end(), ty);
	if (it == m_vData.end())
	{
		cout << "\tthis element is not in list!" << endl;
		exit(-1);
	}
	else
	{
		return it - m_vData.begin();
	}
}
void List::Insert(int index, const Type&ty)
{
	if (index<0 || index>m_vData.size())
	{
		cout << "\terror index in list::insert!" << endl;
		exit(-1);
	}
	m_vData.insert(m_vData.begin() + index, ty);
}
Type List::Pop(int index)
{
	if (index == -1)
		index = m_vData.size() - 1;
	if (m_vData.empty())
	{
		cout << "\terror!list is empty!" << endl;
		exit(-1);
	}
	if (index < 0 || index >= m_vData.size())
	{
		cout << "\tindex error in list::pop()!" << endl;
		exit(-1);
	}
	Type ty_temp = m_vData[index];
	m_vData.erase(m_vData.begin() + index);
	cout << ty_temp << endl;
	return ty_temp;
}
void List::Remove(const Type& ty)
{
	auto it = find(m_vData.begin(), m_vData.end(), ty);
	if (it == m_vData.end())
	{
		cout << "\tvalue error! element is not in list!" << endl;
		exit(-1);
	}
	else
		m_vData.erase(it);
}
void List::Reverse()
{
	reverse(m_vData.begin(), m_vData.end());
}
List list(const string& str)
{
	List res;
	for (char ch : str)
	{
		string temp = "";
		temp += ch;
		res.addType(Type(0, temp, 2));
	}
	return res;
}