#include "TupleDatabase.hpp"

using namespace linda;



Tuple linda::TupleDatabase::read(string query) const {
}

Tuple linda::TupleDatabase::input(string query) {
}

void linda::TupleDatabase::output(const Tuple& tup) {
}

bool linda::TupleQuery::operator ==(const Tuple& t) {
	if(t.size() != this->conditions.size()) return false;

	auto cond_it = conditions.begin();
	auto val_it = values.begin(); //values.size() always == conditions.size()

	for(auto tuple_it : t) {

		switch(*cond_it) {
			case LOWER:
				//if(!tuple_it.compare(cond_it, less<> ))
				break;
			case GREATER:
				break;
			case EQUAL:
				break;
			case LOWER_EQUAL:
				break;
			case GREATER_EQUAL:
				break;
		}

		++cond_it;
		++val_it;
	}

	return true;
}

linda::TupleQuery::TupleQuery(string query) {
	vector<string> tokens;

	Tokenize(query, tokens);
	string* type;
	ECondition condition;

	auto it = tokens.begin();
	while(it != tokens.end()) {

		//type prefix
		type = *it;

		//condition
		++it; //TODO: error handle
		if(it == "<") 		condition = LOWER;
		else if(it == ">") 	condition = GREATER;
		else if(it == "==") condition = EQUAL;
		else if(it == "<=") condition = LOWER_EQUAL;
		else if(it == ">=") condition = GREATER_EQUAL;
		else 				condition = UNKNOWN; //TODO: error handle

		conditions.push_back(condition);

		//value
		++it; //TODO: error handle
		if(*type=="INT") {
			if(it=="*") {
				values.push_back(0);
				condition = NONE;
			}
			else
				values.push_back(stoi(it));
		}
		else if(*type =="FLOAT") {
			if(it=="*") {
				values.push_back(0.0F);
				condition = NONE;
			}
			else
				values.push_back(stof(it));
		}
		else if(*type =="STR") {
			if(it=="*")
				condition = NONE; //TODO: possible bug, cannot search for "*"
			values.push_back(it);
		}
	}

	//TODO check conditions.size() == values.size()

}
