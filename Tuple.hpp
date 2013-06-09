#include <vector>
#include <string>
#include <stdexcept> //std::invalid_argument
#include <cstring>
#include <iostream>
#include <algorithm>
#include <functional>
#include "HelperFunctions.hpp"

namespace linda {
	
	using std::string;
	using std::to_string;

	using std::less;
	using std::less_equal;
	using std::equal_to;
	using std::greater;
	using std::greater_equal;
	
	class TupleElement {
		public:
			enum EType {INT, FLOAT, STRING};
			
			void loadTo(int& var) const {
				require(INT);
				var = *(int*)data_ptr;
			}
			
			void loadTo(float& var) const {
				require(FLOAT);
				var = *(float*)data_ptr;
			}
			
			void loadTo(string& var) const {
				require(STRING);
				var = *(string*)data_ptr;
			}


			TupleElement(int var) : data_type(INT) {
				data_ptr = new int(var);
			}
			
			TupleElement(float var) : data_type(FLOAT) {
				data_ptr = new float(var);
			}
			
			TupleElement(const string& var) : data_type(STRING) {
				data_ptr = new string(var);
			}

			TupleElement(const char* var) : data_type(STRING) {
				data_ptr = new string(var);
			}

			EType getType() const {
				return data_type;
			}

		
			~TupleElement() {
				delete_data();
			}
		
			TupleElement(const TupleElement& other) : data_type(other.data_type) {
				new_data(other);
			}
			
			TupleElement& operator=(const TupleElement& other) {
				if(&other == this) return *this;
				delete_data();
				data_type = other.data_type;
				new_data(other);
				return *this;
			}
			


			template<template<class> class comparator>
			bool compare(const TupleElement& other) const {
				if(data_type != other.data_type) return false;

				switch(data_type) {
					case TupleElement::INT:
						comparator<int> compi;
						return compi( *(int*)(data_ptr), *(int*)(other.data_ptr) );

					case TupleElement::FLOAT:
						comparator<float> compf;
						return compf( *(float*)(data_ptr), *(float*)(other.data_ptr) );

					case TupleElement::STRING:
						comparator<string> comps;
						return comps( *(string*)(data_ptr), *(string*)(other.data_ptr) );
				}

				//assert(0);
				return false; //unknown TupleElement EType
			}
			
			//example operator implementation using compare()
			bool operator==(const TupleElement& other) const {
				return compare<equal_to>(other);
			}

		private:
			void* data_ptr;
			EType data_type;
			
			void require(EType t) const {
				if(data_type != t)
					throw std::invalid_argument("Invalid loadTo() argument");
			}
			
			void delete_data() {
				switch(data_type) {
					case INT:
						delete (int*)data_ptr;
						break;
					case FLOAT:
						delete (float*)data_ptr;
						break;
					case STRING:
						delete (string*)data_ptr;
						break;
				}
			}
			
			void new_data(const TupleElement& initializer) {
				switch(data_type) {
					case INT:
						data_ptr = new int( *(int*)initializer.data_ptr );
						break;
					case FLOAT:
						data_ptr = new float( *(float*)initializer.data_ptr );
						break;
					case STRING:
						data_ptr = new string( *(string*)initializer.data_ptr );
						break;
				}
			}
	};

	class Tuple : public std::vector<TupleElement> {
		public:
			typedef TupleElement::EType EType; //import EType
			string serialize() const;
			void deserialize(string);
	};

}
