#include <iostream>

#include "TupleDatabase.hpp"
#include "HelperFunctions.hpp"

using namespace std;
using namespace linda;

//int main(...) {
//	linda::Tuple t;
//
//	t.values.push_back(999);
//	string s = "qwerty";
//	t.values.push_back(s);
//	t.values.push_back(1.5f);
//
//	//////////////////////
//
//	int a, d;
//	string b, e;
//	float c, f;
//	t.values[0].loadTo(a);
//	t.values[1].loadTo(b);
//	t.values[2].loadTo(c);
//
//	cout << a << b << c << endl;
//	linda::SerializedTuple st = t.serialize();
//	cout << st.length << endl;
//	for(int i = 0; i < st.length; ++i) cout << st.data[i];
//	cout << endl;
//	linda::Tuple newT;
//	try
//	{
//		newT.deserialize(st.length, st.data);
//		newT.values[0].loadTo(d);
//		newT.values[1].loadTo(e);
//		newT.values[2].loadTo(f);
//		cout << d << e << f << endl;
//	}
//	catch(std::exception &e)
//	{
//		cerr << e.what() << endl;
//	}
//	delete st.data;
//	//t[0].loadTo(c); //invalid_argument exception
//
//	return 0;
//}


int main(...) {
	TupleDatabase db;

	Tuple a,b,c,d,e;

	a.push_back(5);
	a.push_back(3);
	a.push_back(1);
	a.push_back("tuple_a");

	b.push_back(5);
	b.push_back(3);
	b.push_back(11);
	b.push_back("tuple_b");

	c.push_back("aaa");
	c.push_back("bbb");
	c.push_back(3);
	c.push_back("tuple_c");


	d.push_back("ccc");
	d.push_back("bbb");
	d.push_back(5);
	d.push_back("tuple_d");

	e.push_back("ddd");
	e.push_back("eee");
	e.push_back(11);
	e.push_back("tuple_e");


	db.output(a);
	db.output(b);
	db.output(c);
	db.output(d);
	db.output(e);


	Tuple res1 = db.input("INT == 5 INT == * INT < 10 STR == *");
	Tuple res2 = db.read("INT == 5 INT == * INT < 10 STR == *");

	Tuple res3 = db.input("STR == * STR == \"bbb\" INT == 5 STR == *");
	Tuple res4 = db.input("STR == * STR == \"bbb\" INT == * STR == *");

	Tuple res5 = db.input("STR == * STR == * INT == * STR == *");
	Tuple res6 = db.input("STR == * STR == * INT == * STR == *");

	//show tuple names (last field)
	string r1,r2,r3,r4,r5,r6;
	res1[3].loadTo(r1);
	res2[3].loadTo(r2);
	res3[3].loadTo(r3);
	res4[3].loadTo(r4);
	res5[3].loadTo(r5);
	res6[3].loadTo(r6);

	cout << r1 << endl;
	cout << r2 << endl;
	cout << r3 << endl;
	cout << r4 << endl;
	cout << r5 << endl;
	cout << r6 << endl;


	cout << db.read("STR == * STR == * INT == * STR == *").size() << endl;

	//cout << db.read("STR == alfa").size() << endl;
}
