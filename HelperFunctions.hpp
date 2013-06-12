#ifndef HELPERFUNCTIONS_HPP
#define HELPERFUNCTIONS_HPP

#include	<string>
#include	<vector>
#include 	<stdexcept>

namespace linda {

	using std::string;
	using std::vector;

    string getSemName(int Pid,int SemNo);
    int is_valid_fd(int fd);


    void Tokenize(const string& str, std::vector<string>& tokens);
    void remove_quotations(string& str);
    //Tokenize helper:
    string unescaped_string(const string& str);


}	// -----  end of namespace linda  -----

#endif
