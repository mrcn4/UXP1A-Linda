#include "Tuple.hpp"
#include <vector>


namespace linda {

	/*
	 * \biref database representation, allows to read, input (read with delete), output
	 */
	class TupleDatabase {
	public:
		Tuple read() const;
		Tuple input();
		void output(const Tuple& tup);

	private:
		std::vector<Tuple> db;
	};

}
