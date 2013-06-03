// =====================================================================================
// 
//       Filename:  HelperFunctions.cpp
// 
//    Description:  Implementation of helper functions
// 
//        Version:  1.0
//        Created:  03.06.2013 16:32:31
//       Revision:  none
//       Compiler:  g++
//         Author:  Marcin Swend (ms), M.Swend@mion.elka.pw.edu.pl
// 
// =====================================================================================


#include <string>

using std::string;

namespace linda {
    string getSemName(int Pid,int SemNo)
    {
        string ChildPidString = std::to_string(Pid);
        const string NamedSemPrefix = "Linda_";
        const string SemSuffix = "_s";
        string SemName;

        SemName.append(NamedSemPrefix).append(ChildPidString).
            append(SemSuffix).append(std::to_string(SemNo));
        return SemName; 
    }
}		// -----  end of namespace linda  -----
