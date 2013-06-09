#include	<string>
#include	<vector>
#include 	<stdexcept>

namespace linda {

	using std::string;
	using std::vector;

    string getSemName(int Pid,int SemNo);
    void Tokenize(const string& str, std::vector<string>& tokens);

}		// -----  end of namespace linda  -----
