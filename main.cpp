#include <iostream>

//#include "TupleDatabase.hpp"
//#include "Message.hpp"
#include "HelperFunctions.hpp"
#include "ParsedRequest.hpp"

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

	Message m_Msg;
	string pattern = "STR == * STR == * INT == * STR == *";
	m_Msg.id = EMessageType::READ;
	m_Msg.tag = 0;
	m_Msg.length = pattern.length()+1;
	cout << m_Msg.length << endl;
	strcpy(m_Msg.data,pattern.c_str());
	char buffer[1024];
	memcpy(buffer, &m_Msg, sizeof(MessageHeader) + m_Msg.length);
	ParsedClientRequest pr = deserializeClientRequest(buffer);
	cout << pr.id << endl;
	cout << pr.tag << endl;
	Tuple res0 = db.read (pr.tq);
	//Tuple res0 = db.read (TupleQuery("STR == * STR == * INT == * STR == *"));

	Tuple res1 = db.read(TupleQuery("INT == 5 INT == * INT >= 7 STR == *"));
	Tuple res2 = db.input(TupleQuery("INT == 5 INT == * INT < 10 STR == *"));

	Tuple res3 = db.input(TupleQuery("STR == * STR == \"bbb\" INT == 5 STR == *"));
	Tuple res4 = db.input(TupleQuery("STR == * STR ==   bbb   INT == * STR == *"));

	Tuple res5 = db.input(TupleQuery("STR == * STR == * INT == * STR == *"));


	//show tuple names (last field)
	string r0, r1,r2,r3,r4,r5;

	res0[3].loadTo(r0);
	res1[3].loadTo(r1);
	res2[3].loadTo(r2);
	res3[3].loadTo(r3);
	res4[3].loadTo(r4);
	res5[3].loadTo(r5);


	cout << r0 << endl;
	cout << r1 << endl;
	cout << r2 << endl;
	cout << r3 << endl;
	cout << r4 << endl;
	cout << r5 << endl;

	cout << db.read(TupleQuery("STR == * STR == * INT == * STR == *")).size() << endl;
	cout << db.read(TupleQuery("STR == alfa")).size() << endl;
}
