#ifndef TUPLEQUERY_HPP
#define TUPLEQUERY_HPP

#include <vector>
#include <list>
#include <string>

#include "Tuple.hpp"
#include "HelperFunctions.hpp"

namespace linda {

	using std::vector;
	using std::list;
	using std::string;
	
	/**
     * Parsed query representation.
     * query  format: (TYPE OPERATOR VALUE)+
	 * example query: INT > 5 STR == "alfa beta" STR == gamma FLOAT <= 5.2
	 *
	 * special case: \ as first character of STR operation operand
	 * ex. STR = \\alfa\\
	 * ALL \\ are changed to \
	 * ALL unpaired \ are igonred
	 * ALL \* are changed to *
	 * use cases:
	 * A) search for empty string: STR == \
	 * B) search for string containing only *: STR == \*
	 */
	class TupleQuery {
	public:
		/** checks if tuple match query (TupleQuery)*/
		bool operator==(const Tuple& t) const;

		//c-tor
		TupleQuery(){}; 
		TupleQuery(string query);

	private:
		enum class ECondition {NONE, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL, EQUAL, ONLY_EQUAL_TYPE, UNKNOWN} ;

		vector<ECondition> conditions; //due to pair<enum, something> is invalid
		Tuple cond_values;

		string rewrite_operand(const string& operand);
	};
	
	
	
	//translation: Tuple::operator==(TupleQuery) -> TupleQuery::operator==(Tuple)
	bool operator==(const Tuple& t, const TupleQuery& q);
}
#endif //TUPLEQUERY_HPP
