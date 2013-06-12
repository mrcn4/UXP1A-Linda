#include "TupleDatabase.hpp"

using namespace linda;

#include <iostream>
using namespace std;

Tuple linda::TupleDatabase::read(const TupleQuery &tq) const {
	//auto q = TupleQuery(query);
	auto it = find(db.begin(), db.end(), tq);

	if(it == db.end())
		return Tuple();

	return *it;
}

Tuple linda::TupleDatabase::input(const TupleQuery &tq) {
	auto it = find(db.begin(), db.end(), tq);

	if(it == db.end())
			return Tuple();

    Tuple result = *it;
    db.erase(it);
	return result;
}

void linda::TupleDatabase::output(const Tuple& tuple) {
	db.push_back(tuple);
}

bool linda::operator==(const ParsedClientRequest& req, const Tuple& t) {
	return req.tq == t;
}

list<ParsedClientRequest>::iterator searchParsedClientRequests(list<ParsedClientRequest>& pcr_db, const Tuple& t) {
	auto it = find(pcr_db.begin(), pcr_db.end(), t);

	return it;
}
