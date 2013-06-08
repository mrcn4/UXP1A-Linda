#include <iostream>
#include "Tuple.hpp"
using namespace std;
int main(...) {
	linda::Tuple t;
	
	t.values.push_back(999);
	string s = "qwerty";
	t.values.push_back(s);
	t.values.push_back(1.5f);
	
	//////////////////////
	
	int a, d;
	string b, e;
	float c, f;
	t.values[0].loadTo(a);
	t.values[1].loadTo(b);
	t.values[2].loadTo(c);
	
	cout << a << b << c << endl;
	linda::SerializedTuple st = t.serialize();
	cout << st.length << endl;
	for(int i = 0; i < st.length; ++i) cout << st.data[i];
	cout << endl;
	linda::Tuple newT;
	try
	{
		newT.deserialize(st.length, st.data);
		newT.values[0].loadTo(d);
		newT.values[1].loadTo(e);
		newT.values[2].loadTo(f);
		cout << d << e << f << endl;		
	}
	catch(std::exception &e)
	{
		cerr << e.what() << endl;
	}
	delete st.data;
	//t[0].loadTo(c); //invalid_argument exception
	
	return 0;
}
