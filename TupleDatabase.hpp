#ifndef TUPLEDATABASE_HPP
#define TUPLEDATABASE_HPP

#include <vector>
#include <list>
#include <string>

#include "Tuple.hpp"
#include "TupleQuery.hpp"
#include "ParsedClientRequest.hpp"

namespace linda {

	using std::vector;
	using std::list;
	using std::string;

    /**
	 * \brief database representation, allows to read, input (read with delete), output
	 * operations are not synchronized!
	 */
	class TupleDatabase {
	public:
		Tuple read(const TupleQuery &tq) const;
		Tuple input(const TupleQuery &tq);
		void output(const Tuple& tuple);

        static list<ParsedClientRequest>::iterator searchParsedClientRequest(list<ParsedClientRequest>& pcr_db, const Tuple& t);
	private:
		vector<Tuple> db;
	};
	
	//translation from ParsedClientRequest to TupleQuery
	bool operator==(const ParsedClientRequest& req, const Tuple& t);

    //check if any of ParsedClientRequest matches Tuple t
    //returns NULL if ParsedClientRequest not found
}

#endif
