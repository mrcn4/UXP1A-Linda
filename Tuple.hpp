#include <vector>
#include <string>
#include <stdexcept> //std::invalid_argument

/*
Szkic krotki. Troche zboczylem z dokumentacji. W sumie na poczatku myslalem ze bedzie mniej kodu, ale chyba nie wyszlo. Zalety to standardowy interface Tuple dzieki uzyciu bezposrednio vector oraz w zamysle wygodniejsze uzywanie. Sam nie wiem czy to sie nadaje.
Co myslicie? 
*/

namespace linda {

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
			
			void loadTo(std::string& var) const {
				require(STRING);
				var = *(std::string*)data_ptr;
			}
			

			TupleElement(int var) : data_type(INT) {
				data_ptr = (void*) new int(var);
				*(int*)data_ptr = var;
			}
			
			TupleElement(float var) : data_type(FLOAT) {
				data_ptr = new float(var);
				*(float*)data_ptr = var;
			}
			
			TupleElement(std::string var) : data_type(STRING) {
				data_ptr = new std::string(var);
				*(std::string*)data_ptr = var;
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
						delete (std::string*)data_ptr;
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
						data_ptr = new std::string( *(std::string*)initializer.data_ptr );
						break;
				}
			}
	};
	
	typedef std::vector<TupleElement> Tuple;
}


/*
#include <iostream>
using namespace std;
int main(...) {
	linda::Tuple t;
	
	t.push_back(999);
	string s = "qwerty";
	t.push_back(s);
	t.push_back(1.5f);
	
	//////////////////////
	
	int a;
	string b;
	float c;
	t[0].loadTo(a);
	t[1].loadTo(b);
	t[2].loadTo(c);
	
	cout << a << b << c;
	
	//t[0].loadTo(c); //invalid_argument exception
	
	return 0;
}
//*/
