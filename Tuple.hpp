#include <vector>
#include <string>
#include <stdexcept> //std::invalid_argument
#include <cstring>
#include <iostream>

namespace linda {
	
	using std::string;

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
				data_ptr = (void*) new int(var);
				*(int*)data_ptr = var;
			}
			
			TupleElement(float var) : data_type(FLOAT) {
				data_ptr = new float(var);
				*(float*)data_ptr = var;
			}
			
			TupleElement(string var) : data_type(STRING) {
				data_ptr = new string(var);
				*(string*)data_ptr = var;
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
			
			

		private:
			void* data_ptr;
			EType data_type;
			
			void require(EType t) const {
				if(data_type != t)
					throw std::invalid_argument("var");
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
			string serialize() const;
			void deserialize(string) const;
	};
}
