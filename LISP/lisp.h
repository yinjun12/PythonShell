#pragma once
#include<iostream>
#include<algorithm>
#include<vector>
#include<string>
#include<regex>
using namespace std;
class Lisp
{
private:
	string mem_exp;//cin expression
public:
	//judge a string type and use it by c++ type,there is only three types, 0 represent integer
	//1 respresent float, 2 respresent string 
	int judgeValueType(const string &str) const;
};
