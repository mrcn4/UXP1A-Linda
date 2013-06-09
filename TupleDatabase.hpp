#include <vector>
#include <string>
#include <utility> //pair

#include "Tuple.hpp"

namespace linda {

	using std::vector;
	using std::string;
	using std::pair;

	/*
	 * \biref database representation, allows to read, input (read with delete), output
	 * TODO: synchronization
	 */
	class TupleDatabase {
	public:
		Tuple read(string query) const;
		Tuple input(string query);
		void output(const Tuple& tuple);

	private:
		vector<Tuple> db;
		vector<Tuple>::iterator database_find(string& query) const;

	};

	//example query: INT > 5 STR == "alfa beta" STR == gamma FLOAT <= 5.2
	class TupleQuery {
	public:

		//checks if tuple match query (TupleQuery)
		bool operator==(const Tuple& t);

		//c-tor
		TupleQuery(string query);

	private:
		enum class ECondition {NONE, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL, EQUAL, ONLY_EQUAL_TYPE, UNKNOWN} ;

		vector<ECondition> conditions; //due to pair<enum, something> is invalid
		Tuple cond_values;
	};

	bool operator==(const Tuple& t, const TupleQuery& q);
}
