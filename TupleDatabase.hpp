#include <vector>
#include <string>
#include <utility> //pair

#include "Tuple.hpp"

namespace linda {

	using std::vector;
	using std::string;
	using std::pair;
	using std;

	/*
	 * \biref database representation, allows to read, input (read with delete), output
	 * TODO: synchronization
	 */
	class TupleDatabase {
	public:
		Tuple read(string query) const;
		Tuple input(string query);
		void output(const Tuple& tup);

	private:
		vector<Tuple> db;
	};

	//example query: INT > 5 STR == "alfa beta" STR == gamma FLOAT <= 5.2
	class TupleQuery {
	public:
		bool operator==(const Tuple& t);

		TupleQuery(string query);

	private:
		enum {NONE, GREATER, LOWER, GREATER_EQUAL, LOWER_EQUAL, EQUAL, UNKNOWN} ECondition;
		vector<ECondition> conditions; //due to pair<enum, something> is invalid
		Tuple values;
	};

}
