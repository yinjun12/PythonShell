#include"python.h"
//**********************struct Type***********************
Type Type::operator[](int index)//不能返回引用
{
	if (type < 2)//int float and string has no operator[]
	{
		cout << "\tthis type has no attritute" << endl;
		exit(-1);
	}
	if (type == 2)
	{
		string temp_s = "";
		temp_s = "'" + val_str[index];
		temp_s += "'";
		Type ty = Type(0.0, temp_s, STRING);
		ty.plast = &val_str;
	}
	//type>=3时，val_str不为空
	vector<char>bracket;//括号栈，当括号栈为空时索引才加1
	int i = 1;//从第一个中括号开始
	bool flag = true;//判断是否有语法错误
	int cur_index = -1;//当前索引
	string ele = "";//记录当前的元素
	int bracket_maxnum = 0;//括号层数
	while (flag&&i < val_str.size() && cur_index<index)
	{
		char ch = val_str[i];
		if (i == val_str.size() - 1)
			ch = ',';
		switch (ch)
		{
		case '[':
			bracket.push_back('[');
			if (bracket.size() > bracket_maxnum)
				bracket_maxnum = bracket.size();
			ele += '[';
			break;
		case ']':
			if (!bracket.empty() && bracket.back() == '[')
				bracket.pop_back();
			else
				flag = false;
			ele += ']';
			break;
		case ',':
			if (ele == "")
			{
				flag = false;
				break;
			}
			if (bracket.empty())
			{
				cur_index++;
				if (cur_index < index)
				{
					bracket_maxnum = 0;
					ele = "";
				}
			}
			else
				ele += ',';
			break;
		default:
			ele += ch;
			break;
		}
		i++;
	}
	if (!flag)//出现语法错误
	{
		cout << "\terror syntax!" << endl;
		exit(-1);
	}
	else if (cur_index == index)//找到指定索引元素
	{
		Type temp_t;
		temp_t.plast = &val_str;
		if (judgeValueType(ele) == INTEGER || judgeValueType(ele) == FLOAT)
		{
			stringstream ss(ele);
			ss >> temp_t.value;
			temp_t.type = judgeValueType(ele);
		}
		else if (judgeValueType(ele) == STRING)
		{
			//removeSymbol(ele,"\"");
			//removeSymbol(ele,"'");
			temp_t.val_str = ele;
			temp_t.type = STRING;
		}
		else
		{
			temp_t.val_str = ele;
			temp_t.type = OFFSET + bracket_maxnum;
		}
		return temp_t;
	}
	else
	{
		cout << "\terror index!" << endl;
		exit(-1);
	}
}
Type& Type::operator=(const Type& ty)
{
	if (plast)//指针不为空则是operator[]返回的，要改变原来的内存；若为空则不是operator[]返回，不改变
	{
		(*plast).replace((*plast).find(val_str), val_str.size(), ty.val_str);
		plast = NULL;
	}
	value = ty.value;
	val_str = ty.val_str;
	type = ty.type;
	return *this;
}
Type& Type::operator=(const List&lst)
{
	Type ty_temp;
	ty_temp.plast = NULL;
	int max_type = 0;
	ty_temp.val_str += "[";
	for (auto it : lst.m_vData)
	{
		if (it.type == INTEGER)
			ty_temp.val_str += to_string(static_cast<int>(it.value));
		else if (it.type == FLOAT)
			ty_temp.val_str += to_string(it.value);
		else
			ty_temp.val_str += it.val_str;
		ty_temp.val_str += ",";
		if (it.type > max_type)
			max_type = it.type;
	}
	ty_temp.val_str.erase(ty_temp.val_str.size() - 1);//删除最后一个，
	ty_temp.val_str += "]";
	if (max_type == 1 || max_type == 2)
		ty_temp.type = 3;//OFFSET+中括号的个数
	else
		ty_temp.type = max_type + 1;
	return operator=(ty_temp);
}
bool Type::operator==(const Type&ty)const
{
	if (type == ty.type&&ty.value == value&&ty.val_str == val_str)
		return true;
	else
		return false;
}
bool operator<(Type lty, Type rty)
{
	bool res = false;
	if (lty.type == -1 || rty.type == -1)
		return res;
	if (lty.type == rty.type&&lty.type == STRING)//two string
		lty.val_str < rty.val_str ? res = true : res = false;
	else if (lty.type < STRING&&rty.type < STRING)//int or float compares with int or float
		lty.value < rty.value ? res = true : res = false;
	else if (lty.type == rty.type&&lty.type > STRING)//get result by comparing first element
		lty[0] < rty[0] ? res = true : res = false;
	else
		lty.type < rty.type ? res = true : res = false;//different type,by comparing type
	return res;
}
ostream& operator<<(ostream&os, const Type &ty)
{
	if (ty.type == INTEGER)
		os << static_cast<int>(ty.value);
	else if (ty.type == FLOAT)
		os << ty.value;
	else if (ty.type == STRING)
		os << ty.val_str;
	else if (ty.type == -1)
		os << "\terror data by operator<<const Type &ty!" << endl;
	else//是输出[[1,2],[3,4]]这种带有中括号的情况
		os << ty.val_str;
	return os;
}