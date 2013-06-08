#include <vector>
#include <string>

#include "Tuple.hpp"

namespace linda {

	using std::vector;
	using std::string;

	/*
	 * \biref database representation, allows to read, input (read with delete), output
	 * TODO: synchronization
	 */
	class TupleDatabase {
	public:
		Tuple read(std::string query) const;
		Tuple input(std::string query);
		void output(const Tuple& tup);

	private:
		vector<Tuple> db;
	};

	class TupleQuery {
		vector<Tuple::EType> types;
	};

}
