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
const int INTEGER = 0, FLOAT = 1, STRING = 2, ERROR = -1, OFFSET = 2;//OFFSETΪƫ������������string������
//***************Type��*******************
class Type//Python�е����к��������������
{
private:
	string *plast;//����operator[]�дﵽ���õ�Ч����ָ�򸸴����ڴ棻��opertor[]������ǿգ����򷵻ؿ�
public:
	string val_str;//���string���� 
	float value;//���float��int����
	int type;
    Type(float v=0,string vs="", int t=0) :value(v), type(t) {//��python�а��ַ��������ϡ���,�ڹ��캯����ʵ��
		if (t == STRING)
		{
			vs = "'" + vs + "'";
			val_str = vs;
		}
		plast = NULL;
	}
	/*Type(const Type& ty);*/
	Type operator[](int);//���ܷ������ã���plast������operator=���ﵽ���õ�Ч��
	Type& operator=(const Type&ty);//���õ�Ч������ʵ��
	Type& operator=(const List&lst);//�����б������Ԫ��
	//����һ�����⣺��copy constructorʱ���string *plast������ȥ�ˣ�����Ҳ���Ըı�ԭ����ֵ��
	//��ʶ���ʱ�����&�ģ�ֱ�ӿ�����ȥ��û��&�Ŀ������plast����ΪNULL;�ڷ���������������
	bool operator==(const Type& ty)const;
};
//python���ַ�����������Ұ��б����б�������
extern bool operator<(Type lty,Type rty);//����operatro<��ʱ���õ�operator[]
extern ostream& operator<<(ostream &os, const Type &ty);
//**************list�б���****************
class List//�б���
{
private:
	vector<Type>m_vData;
	vector<Type>*plast_Data;
	int startIndex;
public:
	List() { plast_Data = NULL; startIndex = -1; }
	void addType(const Type&ty) { m_vData.push_back(ty); }
	void getDataFromString(const string &str);//��string�ж����б����ݣ�"[1,2,[1,2]]"���뵽list��
	Type& operator[](int);//��Ϊ���ﷵ�ص������ã������Type[]���ܸı�ֵ
	List operator[](const string &str);//lst[1:4:1],�޷����ز������ã�����
	List& operator=(const List& lst);//ͨ��*plast_Data,startIndex��operator=��ʵ�ֲ������õ�Ч��
	List& operator=(const Type& ty);//��Ԫ�����͸�ֵ���б�����
	List operator+(const List &lst) const;
	friend Group Tuple(const List&content);
	friend ostream& operator<<(ostream&os, const List &lst);
	friend Type& Type::operator=(const List&lst);
	bool in(const Type&ty);//��Ա�ʸ������
	friend List operator*(const List &lst, int cnt);
	void Sort();//����ͨ������Type��operator<��STL�е�sort()��ʵ������
	int getLength()const;//����Ԫ�صĸ���
	Type getMaxType()const;//�������Ԫ�ص�ֵ
	Type getMinType()const;//������СԪ�ص�ֵ
	void Del(int index);//��������ɾ��Ԫ��
	void Append(const Type& ty);//���һ��Ԫ��
	int Count() const;//ͳ���ж��ٸ�Ԫ�ظ���
	void Extend(const List& lst);
	int Index(const Type& ty) const;
	void Insert(int index,const Type&ty);
	Type Pop(int index=-1);//Ĭ��ɾ�����һ��Ԫ�أ�����ɾ���Լ�ָ����Ԫ��
	void Remove(const Type& ty);
	void Reverse();
};
extern List list(const string &str);//list('Hello')=['H','e','l','l','o']
extern ostream& operator<<(ostream&os, const List&lst);
extern List operator*(const List &lst, int cnt);
extern int Len(const List &lst);
extern Type Max(const List &lst);//�����Ҫ����operator>,operator<,operator=
extern Type Min(const List &lst);
//*******************Ԫ���࣬û��һϵ�еķ�����ʵ�ֱȽϼ�,��list�е�����*******
class Group//Ԫ���࣬Ԫ�����Ԫ�ز����޸ģ�û��һЩ������ʵ�������Ƚϼ�
{
private:
	vector<Type>m_Data;
public:
	friend ostream& operator<<(ostream&os, const Group&gro);
	friend Group operator*( const Group& gro,int cnt);
	void GetDataFromString(const string& str);
	Type operator[](int index)const; //�����޸�Ԫ�أ����Բ��÷������ã�����������Type�ı䲻��ֵ��
	Group operator[](const string& str);
	void addType(const Type& ty) { m_Data.push_back(ty); };
};
extern ostream& operator<<(ostream&os, const Group&gro);
extern Group operator*(const Group& gro,int cnt);
extern Group Tuple(const List&content);
extern Group Tuple(const string&str);
//******************String��*******************
class String//������string����ߵı�����String
{
private:
	string m_sData;//��ŵ����ַ�������
public:
	void StringFormt(const Group&gr);//m_sData:"sfdf%ssdfd,%d,%f",Group gr:('sd',44,5,5)
	int Find(const string&str)const;//��m_sData����str
	String Join(const List&lst);//m_sData���lst���Ԫ��
	void Lower();//��m_sData���Ԫ�ر�ΪСд
	void Replace(const string&lst, const string&rstr);//��m_sData���lst�滻Ϊrstr
	List Split(const string &str)const;
	void Strip(const string&str);//��m_sData��ǰ���strɾ��
};
//***************Dict�ֵ���********************
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
//******************�ܵ�ʵ��**********************
class Python
{
private:
	string mem_exp;//�����û�������ַ������������Ž��з���
	Type OperatorType(const Type &a, const Type &b, char ch) const;//Ϊʲô��const?���ӱ��벻��
public:
	//judge a string type and use it by c++ type,there is only three types, 0 represent integer
	//1 respresent float, 2 respresent string 
	Type solveInfixExpress(const string &str) const;//solve infix expression vlaue;
	Type addString(const string &str)const;
	void notHaveKeyword(const string &temp_str) const;//��û�йؼ��ֵ������д���������print����
	Type transferToDecimal(const string &str)const;//��16���ƻ�8����ת����ʮ����
};

extern int transCharToInt(char ch);//��ʮ�����ƻ�˽����е��ַ�ת��Ϊ����
//ת��ʮ����˽����е��������ֵ�ʮ���ƣ�factor��8��16
extern Type transferToDecimal_int(const string &temp_str, int start, int end, int factor);
//ת��ʮ����˽����е�С�����ֵ�ʮ���ƣ�factor��1/8��1/16
extern Type transferToDecimal_float(const string &temp_str, int start, int end, float factor);
extern 	int judgeValueType(const string &str);//�ж�string������
extern void removeSymbol(string &str, const string &symbol);//ɾ������ǰ��ָ�����ţ�����ʶ��ؼ��ֵĺ�����