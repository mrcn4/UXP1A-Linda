#ifndef HELPERFUNCTIONS_HPP
#define HELPERFUNCTIONS_HPP

#include	<string>
#include	<vector>
#include 	<stdexcept>

namespace linda {

	using std::string;
	using std::vector;

    string getSemName(int Pid,int SemNo);
    void Tokenize(const string& str, std::vector<string>& tokens);
    int is_valid_fd(int fd);
}		// -----  end of namespace linda  -----

#endif
