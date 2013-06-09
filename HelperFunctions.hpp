// =====================================================================================
// 
//       Filename:  HelperFunctions.hpp
// 
//    Description:  Some helpful functions.
// 
//        Version:  1.0
//        Created:  03.06.2013 16:30:31
//       Revision:  none
//       Compiler:  g++
//         Author:  Marcin Swend (ms), M.Swend@mion.elka.pw.edu.pl
// 
// =====================================================================================

#include	<string>
#include	<vector>

namespace linda {

	using std::string;
	using std::vector;

    string getSemName(int Pid,int SemNo);
    void Tokenize(const string& str, std::vector<string>& tokens);

}		// -----  end of namespace linda  -----
