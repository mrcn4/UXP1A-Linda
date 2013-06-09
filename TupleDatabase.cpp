#include "TupleDatabase.hpp"
#include "HelperFunctions.hpp"

using namespace linda;



Tuple linda::TupleDatabase::read(string query) const {
	auto q = TupleQuery(query);
	auto it = find(db.begin(), db.end(), q);

	if(it == db.end())
		return Tuple();

	return *it;
}

Tuple linda::TupleDatabase::input(string query) {
	auto it = find(db.begin(), db.end(), TupleQuery(query));
	Tuple result = *it;

	if(it == db.end())
		return Tuple();

	db.erase(it);
	return result;
}

void linda::TupleDatabase::output(const Tuple& tuple) {
	db.push_back(tuple);
}


bool  linda::operator==(const Tuple& t, const TupleQuery& q) {
	return true;
}


bool linda::TupleQuery::operator ==(const Tuple& t) {
	//assert(conditions.size() == cond_values.size())
	if(t.size() != this->conditions.size()) return false;

	auto cond_it = conditions.begin();
	auto cond_values_element_it = cond_values.begin(); //values.size() always == conditions.size()

	for(auto tuple_element : t) {

		//false compare -> tuple not match to query
		switch(*cond_it) {
			case ECondition::LESS:
				if(!tuple_element.compare<less>(*cond_values_element_it))
					return false;
				break;
			case ECondition::GREATER:
				if(!tuple_element.compare<greater>(*cond_values_element_it))
					return false;
				break;
			case ECondition::EQUAL:
				if(!tuple_element.compare<equal_to>(*cond_values_element_it))
					return false;
				break;
			case ECondition::LESS_EQUAL:
				if(!tuple_element.compare<less_equal>(*cond_values_element_it))
					return false;
				break;
			case ECondition::GREATER_EQUAL:
				if(!tuple_element.compare<greater_equal>(*cond_values_element_it))
					return false;
				break;
			case ECondition::ONLY_EQUAL_TYPE:
				return tuple_element.getType() == cond_values_element_it->getType();
				break;
			case ECondition::NONE:
				return true;
				break;
			default:
				break; //TODO: unknown condition type
		}

		++cond_it;
		++cond_values_element_it;
	}

	//tuple match to query, all conditions fulfilled
	return true;
}

linda::TupleQuery::TupleQuery(string query) {
	vector<string> tokens;

	Tokenize(query, tokens);
	vector<string>::iterator type;
	ECondition condition;

	auto it = tokens.begin();
	while(it != tokens.end()) {

		//type prefix
		type = it;

		//condition type
		++it; //TODO: error handle
		if(*it == "<") 		condition = ECondition::LESS;
		else if(*it == ">") 	condition = ECondition::GREATER;
		else if(*it == "==") condition = ECondition::EQUAL;
		else if(*it == "<=") condition = ECondition::LESS_EQUAL;
		else if(*it == ">=") condition = ECondition::GREATER_EQUAL;
		else 				condition = ECondition::UNKNOWN; //TODO: error handle

		conditions.push_back(condition);

		//condition value
		++it; //TODO: error handle
		if(*type=="INT") {
			if(*it=="*") {
				cond_values.push_back(0);
				condition = ECondition::ONLY_EQUAL_TYPE;
			}
			else
				cond_values.push_back(stoi(*it));
		}
		else if(*type =="FLOAT") {
			if(*it=="*") {
				cond_values.push_back(0.0F);
				condition = ECondition::ONLY_EQUAL_TYPE;
			}
			else
				cond_values.push_back(stof(*it));
		}
		else if(*type =="STR") {
			if(*it=="*")
				condition = ECondition::ONLY_EQUAL_TYPE; //TODO: possible bug, cannot search for "*"
			cond_values.push_back(*it);
		}
	}

	//TODO check conditions.size() == values.size()
}



