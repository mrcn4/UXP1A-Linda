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

    // c-style-string-inside aware function
    // for example: alfa beta "gamma \"citation\""
    // is decomposed to 3 tokens (last one: gamma "citation")
    void Tokenize(const string& str, std::vector<string>& tokens) {
    	bool in_string = false;
    	string::size_type pos = 0;
    	string token;

    	while(pos < str.size()) {

    		if(in_string) {
    			//pos > 0
    			if(str[pos] == '"') {

    				if(str[pos-1] != '\\') {
    					//end of string
    					in_string = false;
    					tokens.push_back(token);
    					token = "";
    				}
    				else {
    					//escaped "
    					if(pos >= str.size())
    						; //todo: error throw?
    					token.back() = str[pos]; //replace \ with "
    				}
    			} else
    				token += str[pos];

    		} else {

    			if(str[pos] == '"') {
    				in_string = true;
    			}
    			else if(str[pos] == ' ') {
    				if(token.size() > 0) {
    					tokens.push_back(token);
    					token="";
    				}
    			}
    			else {
    				token += str[pos];
    			}
    		}

    		++pos;
    	}

    	//last token
    	if(token.size() > 0)
    		tokens.push_back(token);
    }

    }

}		// -----  end of namespace linda  -----
