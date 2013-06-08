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

//			template<class T>
//			bool compare(const TupleElement& other, bool (*compare_fun)(const T&, const T&) ) const {
//				if(other.data_type != this->data_type) return false;
//
//				switch(data_type) {
//					case INT:
//						if( !compare_fun( *(int*)(data_ptr), *(int*)(other.data_ptr)) )
//							return false;
//						break;
//					case FLOAT:
//						if( !compare_fun( *(float*)(data_ptr), *(float*)(other.data_ptr)) )
//							return false;
//						break;
//					case STRING:
//						if( !compare_fun( *(string*)(data_ptr), *(string*)(other.data_ptr)) )
//							return false;
//						break;
//				}
//
//				return true;
//			}
//
//			bool operator==(const TupleElement* other) const {
//				return compare(other, []())
//			}

		
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
			typedef TupleElement::EType EType; //import EType
			string serialize() const;
			void deserialize(string) const;
	};
}
