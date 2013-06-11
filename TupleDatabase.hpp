#ifndef TUPLEDATABASE_HPP
#define TUPLEDATABASE_HPP

#include <vector>
#include <string>
#include <utility> //pair

#include "Tuple.hpp"

namespace linda {

	using std::vector;
	using std::string;
	using std::pair;

	/*
	 * \biref parsed query representation
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
		//checks if tuple match query (TupleQuery)
		bool operator==(const Tuple& t) const;

		//c-tor
		TupleQuery(){};
		TupleQuery(string query);

	private:
		enum class ECondition {NONE, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL, EQUAL, ONLY_EQUAL_TYPE, UNKNOWN} ;

		vector<ECondition> conditions; //due to pair<enum, something> is invalid
		Tuple cond_values;

		static string rewrite_operand(const string& operand);
	};

	/*
	 * \biref database representation, allows to read, input (read with delete), output
	 * operations are not synchronized!
	 */
	class TupleDatabase {
	public:
		//Tuple read(const string& query) const;
		//Tuple input(const string& query);
		Tuple read(const TupleQuery &tq) const;
		Tuple input(const TupleQuery &tq);
		void output(const Tuple& tuple);

	private:
		vector<Tuple> db;
	};

	//translation: Tuple::operator==(TupleQuery) -> TupleQuery::operator==(Tuple)
	bool operator==(const Tuple& t, const TupleQuery& q);
}

#endif
