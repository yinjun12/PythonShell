#include"lisp.h"
int main()
{
	Lisp lis;
	cout << lis.judgeValueType("2s") << endl;
	tr1::regex re("[0-9]+");
	cmatch res;
	string str = "Ilike34sdfd34dfd55fds32";
	while (regex_search(str.c_str(), res, re))
	{
		cout << res[0] << " ";
		str = res.suffix().str();
	}
	return 0;
}