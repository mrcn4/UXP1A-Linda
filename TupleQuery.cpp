#include "TupleQuery.hpp"

using namespace linda;
using namespace std;


bool linda::operator==(const Tuple& t, const TupleQuery& q) {
	return q == t;
}

bool linda::TupleQuery::operator ==(const Tuple& t) const {
	//assert(conditions.size() == cond_values.size())
	if(this->conditions.size() == 0) return false; 		  //empty query
	if(t.size() != this->conditions.size()) return false; //different size required by query

	auto cond_it = conditions.begin();
	auto cond_values_element_it = cond_values.begin();    //values.size() always == conditions.size()

	int iii=0;

	//compare each pair of elements
	for(auto tuple_element : t) {

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
				if( tuple_element.getType() != cond_values_element_it->getType())
					return false;
				break;
			case ECondition::NONE:
				break; //always accept TupleElement
			default:
				throw std::logic_error("Unknown condition type in query!");
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

	if(tokens.size() % 3 != 0)
		throw std::logic_error("Malformed query!");

	if(tokens.size() == 0)
		throw std::logic_error("Empty query!");

	auto it = tokens.begin();
	while(it != tokens.end()) {

		//type prefix
		type = it;

		//condition type
		++it;
		if	   (*it == "<" ) condition = ECondition::LESS;
		else if(*it == ">" ) condition = ECondition::GREATER;
		else if(*it == "==") condition = ECondition::EQUAL;
		else if(*it == "<=") condition = ECondition::LESS_EQUAL;
		else if(*it == ">=") condition = ECondition::GREATER_EQUAL;
		else
			throw std::logic_error("Unknown operator in query!");
			//condition = ECondition::UNKNOWN;

		//condition value
		++it;
		if(*type == "INT") {
			if(*it == "*") {
				cond_values.push_back(0);
				condition = ECondition::ONLY_EQUAL_TYPE;
			}
			else
				cond_values.push_back(stoi(*it));
		}
		else if(*type == "FLOAT") {

			if(condition == ECondition::EQUAL)
				throw logic_error("== with FLOAT not allowed!"); //documentation requirement

			if(*it == "*") {
				cond_values.push_back(0.0F);
				condition = ECondition::ONLY_EQUAL_TYPE;
			}
			else
				cond_values.push_back(stof(*it));
		}
		else if(*type == "STR") {

			if(*it == "*")
				condition = ECondition::ONLY_EQUAL_TYPE; //TODO: possible bug, cannot search for "*"

			//handle: search for string with operand "*" -> query should be STR == \*
			//when operand has \ as first character then all \ must be preceded by extra \ and \* is converted to *
//			if(it->size() > 1 && it->at(0) == '\\') {
//				*it = rewrite_operand(*it);
//			}

			cond_values.push_back(*it);
		} else
			throw logic_error("Unknown data type in query");

		conditions.push_back(condition);

		++it;
	}
}




