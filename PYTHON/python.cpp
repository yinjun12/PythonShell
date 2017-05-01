#include"python.h"

//*******************************class Python*********************
Type Python::OperatorType(const Type &a, const Type &b, char ch) const//两个类型进行运算符操作，并且判断操作后的类型
{
	Type result;
	if (a.type == FLOAT || b.type == FLOAT)
		result.type = FLOAT;
	else
		result.type = INTEGER;
	switch (ch)
	{
	case '+':
		result.value = a.value + b.value;
		break;
	case '*':
		result.value = a.value*b.value;
		break;
	case '-':
		result.value = a.value - b.value;
		break;
	case '/':
		result.value = a.value / b.value;
		break;
	case '%':
		if (a.type == INTEGER&&b.type == INTEGER)//求余要求两边都要是int型
		{
			result.value = static_cast<int>(a.value) % static_cast<int>(b.value);
		}
		else
			result.type = -1;
		break;
	case '^':
		result.value = pow(a.value,b.value);
		break;
	default:
		break;
	}
	if (result.type == INTEGER)
	{
		result.value = (int)result.value;
	}
	return result;

}
Type Python::solveInfixExpress(const string &str) const//计算中缀表达式,表达式异常返回一个错误
{
	string exp_str = str;
	exp_str += '#';
	vector<Type>num;
	vector<char>opera;
	opera.push_back('#');
	int i = 0;		
	string temp_str = "";
	bool flag = true;//判断表达式中是否有错误的符号
	string str_op = "+-*/()%^#";
	while (flag&&opera.size()!=0)
	{
		 if(isdigit(exp_str[i])||exp_str[i]=='.')//提取字符串里的整数和小数
		{
			 temp_str += exp_str[i];
			 i++;
			 continue;
		}
		 if (temp_str.size() != 0)//有运算数的情况下入运算数栈
		 {
			 stringstream ss(temp_str);
			 float flo_temp;
			 ss >> flo_temp;
			 Type ty = Type(flo_temp,"", judgeValueType(temp_str));
			 num.push_back(ty);
			 temp_str = "";
		 }
		 //操作符进入操作符栈
		else if (str_op.find(exp_str[i])!=-1)
		{
			char ch = exp_str[i];
			char top = opera.back();
			int i_temp = priority[mp[top]][mp[ch]];
			switch (i_temp)
			{
			case 0:
				opera.push_back(ch);
				i++;
				break;
			case 1:
				if (num.size() >= 2)//判断是否能从运算数栈里提出两个数
				{
					Type first = num.back();
					num.pop_back();
					Type second = num.back();
					num.pop_back();
					char op = opera.back();
					opera.pop_back();
					Type result = OperatorType(second, first, op);
					num.push_back(result);
				}
				else
					flag = false;
				break;
			case 2:
				opera.pop_back();
				i++;
				break;
			default:
				break;
			}
		}
		else if (isspace(exp_str[i]))//为空格则跳过
		{
			i++;
		}
		else//即不是操作符也不是运算符则此表达式出现错误
		{
			flag = false;
			cout << "operator error!" << endl;
			break;
		}
	}
	if (flag&&num.size() == 1&&opera.empty())
	{
		return num.back();
	}
	else
	{
		return Type(0,"",-1);//若类型为-1代表表达错误！
	}
}

Type Python::addString(const string &str)const
{
	Type result;
	result.type = 2;
	stringstream ss(str);
	string temp;
	while (getline(ss, temp, '+'))
	{
		if (judgeValueType(temp) != STRING)//判断+的是否有非字符字符串
		{
			result.type = -1;
			break;
		}
		else
		{
			removeSymbol(temp, "\"");
			removeSymbol(temp, "'");
			result.val_str += temp;
		}
	}
	result.val_str = "'" + result.val_str + "'";
	return result;
}


Type Python::transferToDecimal(const string &str)const
{
	string temp_str = str;
	int fac = 0;
	if (temp_str.find("0x") != -1)//16进制
	{
		temp_str.erase(0, 2);//delete 0x
		fac = 16;
	}
	else//8进制
	{
		temp_str.erase(0, 1);
		fac = 8;
	}
	int point_pos = temp_str.find('.');
	Type result, int_res, flo_res;
	if (point_pos == -1)//没有小数
	{
		int_res = transferToDecimal_int(temp_str,0,temp_str.size(),fac);
		result = int_res;
	}
	else
	{
		int_res = transferToDecimal_int(temp_str,0,point_pos,fac);//转换整数部分
		flo_res = transferToDecimal_float(temp_str,temp_str.size()-1,point_pos,1.0/fac);//转换小数部分
		result = OperatorType(int_res,flo_res,'+');
 	}
	return result;
}
void Python::notHaveKeyword(const string &str) const
{
	string temp_str = str;
	char ch1 = '"', ch2 = '\'';
	if (temp_str.find(ch1) != -1|| temp_str.find(ch2) != -1)//处理字符串问题
	{
		if (judgeValueType(temp_str)==STRING)//一个字符串
		{
			string str_t;
			str_t.assign(temp_str.begin()+1,temp_str.end()-1);
			cout << str_t << endl;
		}
		else if (temp_str.find('+') != -1)//字符串相加,字符串只有加，没有减
		{
			Type res = addString(temp_str);
			cout << res;
		}
		else
		{
			cout << "\tSyntax error!" << endl;
		}
	}//处理整数和浮点数问题，输出字符串类型必须加""
	else if (judgeValueType(temp_str) == INTEGER || judgeValueType(temp_str) == FLOAT)
	{
		cout << temp_str << endl;
	}
	else if (temp_str.find_first_of("+-*/^%") != -1)//输出计算表达式结果
	{
		Type res = solveInfixExpress(temp_str);
		cout << res;
	}
	else if (temp_str.find("0x") == 0 || temp_str.find("0") == 0)
	{
		Type res = transferToDecimal(temp_str);
		cout << res;
	}
}

