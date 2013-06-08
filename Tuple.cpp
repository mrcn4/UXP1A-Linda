#include "Tuple.hpp"
#include <iostream>

/*
funkcja serializujca pojedynczy element krotki. Niestety bylem zmuszony odrzucic stringi, poniewaz nie przepisywaly zer zwracanych przez memcpy. Nie jestem w stanie przypisac typu elementu do tablicy wyjsciowej w bardziej elegancki sposob niz przez memcpy
*/
linda::SerializedTuple linda::TupleElement::serialize()
{
	SerializedTuple st;
	char *varInt, *varFloat, *varString;
	char intName[4] = "INT";
	char floatName[6] = "FLOAT";
	char stringName[7] = "STRING";
	std::string tmp;
	int l;
	switch(data_type)
	{
		case INT:	varInt = new char[3 + sizeof(int)];
				memcpy(varInt, intName, 3);
				memcpy(varInt+3, data_ptr, sizeof(int));
				st.length = 3 + sizeof(int);
				st.data = varInt;
				break;
		case FLOAT:	varFloat = new char[5 + sizeof(float)];
				memcpy(varFloat, floatName, 5);
				memcpy(varFloat+5, data_ptr, sizeof(float));
				st.length = 5 + sizeof(float);
				st.data = varFloat;
				break;
		case STRING:	tmp = (*(std::string*)data_ptr);
				l = tmp.length();
				st.length = 6 + sizeof(int) + l;
				varString = new char[st.length];
				memcpy(varString, stringName, 6);
				memcpy(varString+6, &l, sizeof(int));
				for(int i = 10; i < st.length; ++i) varString[i] = tmp[i-10];
				st.data = varString;
				break;
		default: 	break;
	}
	return st;
}

/*
funkcja serializujaca cala krotke - pobiera wyniki serializacji kolejnych elementow krotki i zapisuje je w wektorze znakow. Na samym koncu pakuje dane do struktury, ktora mozna praktycznie od razu wyslac przez PIPE.
*/
linda::SerializedTuple linda::Tuple::serialize()
{
	SerializedTuple st, tmp;
	std::vector<char> out;
	unsigned int i;
	for(auto it = values.begin(); it != values.end(); ++it)
	{
		tmp = it->serialize();
		std::cout << tmp.length << "\t" << tmp.data << std::endl;
		for(i = 0; i < tmp.length; ++i) out.push_back(tmp.data[i]);
		delete tmp.data;
	}
	st.length = out.size();
	st.data = new char[st.length];
	for(i = 0; i < st.length; ++i) st.data[i] = out[i];
	return st;
}

/*
funkcja deserializujaca wywolywana na rzecz obiektu krotki do ktorej ma zostac zapisany wynik deserializacji. Rowniez tutaj gesto korzystam z memcpy - gdyby ktos znalazl bardziej elegancki sposob prosze sie nie krepowac i poprawic.
przyjmuje parametry:
int length - dlugosc zserializowanych danych
char* tuple - wskaznik na zserializowane dane
*/
void linda::Tuple::deserialize(int length, char* tuple)
{
	int varInt;
	char intName[4];
	intName[3] = 0;
	float varFloat;
	char floatName[6];
	floatName[5] = 0;
	std::string varString;
	char stringName[7];
	stringName[6] = 0;
	char* tmpString;
	for(int i = 0; i < length; )
	{
		switch(tuple[i])
		{
			case 'I':	memcpy(intName, tuple + i, 3);
					i += 3;
					if(!strcmp(intName, "INT"))
					{
						memcpy(&varInt, tuple + i, sizeof(int));
						values.push_back(varInt);
						i += sizeof(int);
					}
					else throw std::invalid_argument("Invalid type - INT expected");
					break;
			case 'F':	memcpy(floatName, tuple + i, 5);
					i += 5;
					if(!strcmp(floatName, "FLOAT"))
					{
						memcpy(&varFloat, tuple + i, sizeof(float));
						values.push_back(varFloat);
						i += sizeof(float);
					}
					else throw std::invalid_argument("Invalid type - FLOAT expected");
					break;
			case 'S':	memcpy(stringName, tuple + i, 6);
					i += 6;
					if(!strcmp(stringName, "STRING"))
					{
						memcpy(&varInt, tuple + i, sizeof(int));
						tmpString = new char[varInt + 1];
						tmpString[varInt] = 0;
						i += sizeof(int);
						memcpy(tmpString, tuple + i, varInt);
						varString = tmpString;
						values.push_back(varString);
						delete tmpString;
						i += varInt;
					}
					else throw std::invalid_argument("Invalid type - STRING expected");
					break;
			default:	throw std::invalid_argument("Invalid type");
					break;
		}
	}
}
