#pragma once
#include<iostream>
#include<algorithm>
#include<vector>
#include<string>
#include<sstream>
#include<regex>
#include<map>
#include<typeinfo>
using namespace std;
class List;
class Python;
class Type;
class Group;
extern map<char, int>mp;
extern const vector<vector<int>>priority;
const int INTEGER = 0, FLOAT = 1, STRING = 2, ERROR = -1, OFFSET = 2;//OFFSET为偏移量，即超过string的类型
//***************Type类*******************
class Type//Python中的所有函数都用这个类型
{
private:
	string *plast;//用在operator[]中达到引用的效果，指向父串的内存；是opertor[]返回则非空，否则返回空
public:
	string val_str;//存放string类型 
	float value;//存放float和int类型
	int type;
    Type(float v=0,string vs="", int t=0) :value(v), type(t) {//在python中把字符串都加上‘’,在构造函数里实现
		if (t == STRING)
		{
			vs = "'" + vs + "'";
			val_str = vs;
		}
		plast = NULL;
	}
	/*Type(const Type& ty);*/
	Type operator[](int);//不能返回引用！用plast和重载operator=来达到引用的效果
	Type& operator=(const Type&ty);//引用的效果由它实现
	Type& operator=(const List&lst);//把用列表来替代元素
	//还有一个问题：在copy constructor时候把string *plast拷贝过去了，那样也可以改变原来的值了
	//在识别的时候把有&的，直接拷贝过去；没有&的拷贝后把plast设置为NULL;在分析语句里进行设置
	bool operator==(const Type& ty)const;
};
//python把字符串放在最后，我把列表中列表放在最后！
extern bool operator<(Type lty,Type rty);//重载operatro<的时候用到operator[]
extern ostream& operator<<(ostream &os, const Type &ty);
//**************list列表类****************
class List//列表类
{
private:
	vector<Type>m_vData;
	vector<Type>*plast_Data;
	int startIndex;
public:
	List() { plast_Data = NULL; startIndex = -1; }
	void addType(const Type&ty) { m_vData.push_back(ty); }
	void getDataFromString(const string &str);//从string中读入列表数据，"[1,2,[1,2]]"读入到list中
	Type& operator[](int);//因为这里返回的是引用，后面的Type[]才能改变值
	List operator[](const string &str);//lst[1:4:1],无法返回部分引用？？？
	List& operator=(const List& lst);//通过*plast_Data,startIndex和operator=来实现部分引用的效果
	List& operator=(const Type& ty);//将元素类型赋值给列表类型
	List operator+(const List &lst) const;
	friend Group Tuple(const List&content);
	friend ostream& operator<<(ostream&os, const List &lst);
	friend Type& Type::operator=(const List&lst);
	bool in(const Type&ty);//成员资格运算符
	friend List operator*(const List &lst, int cnt);
	void Sort();//排序，通过重载Type的operator<和STL中的sort()来实现排序
	int getLength()const;//返回元素的个数
	Type getMaxType()const;//返回最大元素的值
	Type getMinType()const;//返回最小元素的值
	void Del(int index);//根据索引删除元素
	void Append(const Type& ty);//添加一个元素
	int Count() const;//统计有多少个元素个数
	void Extend(const List& lst);
	int Index(const Type& ty) const;
	void Insert(int index,const Type&ty);
	Type Pop(int index=-1);//默认删除最后一个元素，可以删除自己指定的元素
	void Remove(const Type& ty);
	void Reverse();
};
extern List list(const string &str);//list('Hello')=['H','e','l','l','o']
extern ostream& operator<<(ostream&os, const List&lst);
extern List operator*(const List &lst, int cnt);
extern int Len(const List &lst);
extern Type Max(const List &lst);//这个需要重载operator>,operator<,operator=
extern Type Min(const List &lst);
//*******************元组类，没有一系列的方法，实现比较简单,跟list有点类似*******
class Group//元组类，元组里的元素不能修改，没有一些函数，实现起来比较简单
{
private:
	vector<Type>m_Data;
public:
	friend ostream& operator<<(ostream&os, const Group&gro);
	friend Group operator*( const Group& gro,int cnt);
	void GetDataFromString(const string& str);
	Type operator[](int index)const; //不用修改元素，所以不用返回引用，不返回引用Type改变不了值！
	Group operator[](const string& str);
	void addType(const Type& ty) { m_Data.push_back(ty); };
};
extern ostream& operator<<(ostream&os, const Group&gro);
extern Group operator*(const Group& gro,int cnt);
extern Group Tuple(const List&content);
extern Group Tuple(const string&str);
//******************String类*******************
class String//参数用string，左边的变量用String
{
private:
	string m_sData;//存放的是字符串数据
public:
	void StringFormt(const Group&gr);//m_sData:"sfdf%ssdfd,%d,%f",Group gr:('sd',44,5,5)
	int Find(const string&str)const;//在m_sData里找str
	String Join(const List&lst);//m_sData添加lst里的元素
	void Lower();//将m_sData里的元素变为小写
	void Replace(const string&lst, const string&rstr);//将m_sData里的lst替换为rstr
	List Split(const string &str)const;
	void Strip(const string&str);//将m_sData里前后的str删除
};
//***************Dict字典类********************
class Dict
{
	map<Type, Type>m_dData;
public:
	void Clear();
	Type Get(const Type&ty) const;
	bool Has_key(const Type&ty) const;
	List Items()const;
	List Keys()const;
	Type Pop(const Type&ty);
	void Setdefault(const Type&lty, const Type&rty);
	void Update(const Dict&dic);
	List Values()const;
};
extern Dict dict(const List&lst);
extern int len(const Dict &dic);
extern void del(const Type&ty);
//******************总的实现**********************
class Python
{
private:
	string mem_exp;//代表用户输入的字符串，处理过后才进行分析
	Type OperatorType(const Type &a, const Type &b, char ch) const;//为什么加const?不加编译不过
public:
	//judge a string type and use it by c++ type,there is only three types, 0 represent integer
	//1 respresent float, 2 respresent string 
	Type solveInfixExpress(const string &str) const;//solve infix expression vlaue;
	Type addString(const string &str)const;
	void notHaveKeyword(const string &temp_str) const;//对没有关键字的语句进行处理，主用在print后面
	Type transferToDecimal(const string &str)const;//将16进制或8进制转换到十进制
};

extern int transCharToInt(char ch);//把十六进制或八进制中的字符转换为整数
//转换十六或八进制中的整数部分到十进制，factor是8或16
extern Type transferToDecimal_int(const string &temp_str, int start, int end, int factor);
//转换十六或八进制中的小数部分到十进制，factor是1/8或1/16
extern Type transferToDecimal_float(const string &temp_str, int start, int end, float factor);
extern 	int judgeValueType(const string &str);//判断string的类型
extern void removeSymbol(string &str, const string &symbol);//删除语句的前后指定符号，用在识别关键字的函数里