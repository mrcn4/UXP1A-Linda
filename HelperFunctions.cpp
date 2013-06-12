#include "HelperFunctions.hpp"


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


    //convert c-style string with ", \", \\ to normal string
    //all unpaired \ are ignored (STR == \ matches empty string, STR == "" also)
    // \\ is changed to \
    // \* is changed to *
    // \" is handled in Tokenize() (changed to ")
    //examples:
    //		\\* 	-> \*
    //		\\\*	-> \*
    //		\* 		-> *
    //		\\\\* 	-> \\*
    string unescaped_string(const string& str)  {

    	string result;
    	if(str.size()==0) return result;

    	for(size_t i=0;i < str.size()-1;++i) {
    		if(str[i] != '\\')
    			result += str[i];
    		else {
    			if(str[i+1] == '\\') {
    				++i;
    				result += '\\';
    			}
    			else if(str[i+1] == '*') {
    				++i;
    				result += '*';
    			}
    		}
    	}

    	//last char
    	result += str.back();

    	return result;
    }

    // "alfa" -> alfa
    void remove_quotations(string& str) {
    	if(str.size() >= 2) {
    		if(str.front() == '"' && str.back() == '"')
    			str = str.substr(1,str.size()-2); //strip first and last character
    	}
    }

    // c-style-inside-string aware function
    // inside "..." few sequences are unescaped: \\ to \ , \* to * , \" to " , unpaired \ are ignored (disappear)
    // to put " within "inside-string with spaces" precede it with \
    // " could be used only in c-style-inside-strings like: "...\"..." which is converted to: ..."...
    // for example: alfa beta "gamma \"citation\""
    // is decomposed to 3 tokens (last one: "gamma "citation"")
    void Tokenize(const string& str, std::vector<string>& tokens) {
    	bool in_string = false;
    	string::size_type pos = 0;
    	string token;

    	while(pos < str.size()) {

    		if(in_string) {
    			//pos >= 1
    			if(str[pos] == '"') {

    				if(str[pos-1] != '\\') {
    					//end of inside-string
    					in_string = false;
    					tokens.push_back(unescaped_string(token));
    					token = "";
    				}
    				else {
    					//escaped "
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

    	if(in_string)
    		throw std::logic_error("Parse query error, missing '\"' !");

    	//last token
    	if(token.size() > 0)
            tokens.push_back(token);
    }

    int is_valid_fd(int fd)
    {
        return fcntl(fd, F_GETFL) != -1 || errno != EBADF;
    }


}		// -----  end of namespace linda  -----
