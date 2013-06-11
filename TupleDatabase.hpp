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

		//check if any of ParsedClientRequest matches Tuple t
		//returns NULL if ParsedClientRequest not found
		ParsedClientRequest* searchParsedClientRequest(const list<ParsedClientRequest>& pcr_db, const Tuple& t);

	private:
		vector<Tuple> db;
	};
	
	//translation from ParsedClientRequest to TupleQuery
	bool operator==(const ParsedClientRequest& req, const Tuple& t);
}

#endif
