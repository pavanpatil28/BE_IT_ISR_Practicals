#include<iostream>
#include<fstream>
#include<vector>
#include<unordered_map>
using namespace std;

unordered_map<string, int> mp = {{"the", 1},{"or", 1},{"out", 1},{"its", 1},{"of", 1},{"by", 1},
								 {"them", 1},{"who", 1},{"and", 1},{"one", 1},{"then", 1},{"now", 1},
							     {"had", 1},{"she", 1},{"people", 1},{"to", 1},{"not", 1},{"many", 1},
							     {"my", 1},{"in", 1},{"but", 1},{"some", 1},{"made", 1},{"is", 1},
							     {"what", 1},{"so", 1},{"over", 1},{"you", 1},{"all", 1},{"this", 1},
							     {"did", 1},{"that", 1},{"were", 1},{"would", 1},{"down", 1},{"it", 1},
							     {"when", 1},{"other", 1},{"only", 1},{"he", 1},{"we", 1},{"into", 1},
							     {"way", 1},{"for", 1},{"there", 1},{"has", 1},{"find", 1},{"was", 1},
							     {"can", 1},{"more", 1},{"use", 1},{"on", 1},{"an", 1},{"her", 1},
							     {"may", 1},{"are", 1},{"your", 1},{"two", 1},{"water", 1},{"as", 1},
							     {"which", 1},{"like", 1},{"long", 1},{"with", 1},{"their", 1},{"him", 1},
							     {"little", 1},{"his", 1},{"a", 1},{"see", 1},{"very", 1},{"they", 1},
							     {"if", 1},{"time", 1},{"after", 1},{"at", 1},{"do", 1},{"could", 1},
							     {"words", 1},{"be", 1},{"will", 1},{"no", 1},{"called", 1},{"this", 1},
							     {"each", 1},{"make", 1},{"just", 1},{"from", 1},{"about", 1},{"than", 1},
							     {"where", 1},{"I", 1},{"how", 1},{"first", 1},{"most", 1},{"have", 1},
							     {"up", 1},{"been", 1},{"know", 1},{"every", 1},{"should", 1},{"these", 1}};

string toLowerCase(string &str){
	for(int i = 0; i < str.length(); i++){
		str[i] = tolower(str[i]);
	}
	return str;
}

bool isStopword(string &str) {
	if(mp[str] == 1)
		return true;
	return false;
}

int main() {
	try{
		ifstream inputFile("sample.txt");
		if (!inputFile.is_open()) { 
			cerr << "Error opening the file!" << endl; 
			return 1; 
		} 
		string line; 
		cout << "Document Data: " << endl; 
		vector<string> doc; 
		while (getline(inputFile, line)) { 
			cout<<line<<endl<<endl;	
			cout<<"After removing stopwords:"<<endl;
			string word = "";
			for(int i = 0; i < line.length(); i++){
				if((line[i] >= 65 && line[i] <= 90) || (line[i] >= 97 && line[i] <= 122)) {
					word += line[i];	
				}
				else{
					word = toLowerCase(word);
					if(!isStopword(word)){
						if(word != ""){
							doc.push_back(word);
						}
					}
					word = "";
				}
			}
		} 
		inputFile.close(); 

		unordered_map<string, int> keywords;
		for(string s: doc){
			if(s.length() > 3 && s.substr(s.length() - 3) == "ing"){
				s = s.substr(0, s.length() - 3);
			}
			else if(s.length() > 3 && s.substr(s.length() - 3) == "ful"){
				s = s.substr(0, s.length() - 3);
			}
			else if(s.length() > 2 && s.substr(s.length() - 2) == "al"){
				s = s.substr(0, s.length() - 2);
			}
			else if(s.length() > 2 && s.substr(s.length() - 2) == "ly"){
				s = s.substr(0, s.length() - 2);
			}
			else if(s.length() > 3 && s.substr(s.length() - 3) == "ted"){
				s = s.substr(0, s.length() - 1);
			}
			else if(s.length() > 2 && s.substr(s.length() - 2) == "ed"){
				s = s.substr(0, s.length() - 2);
			}
			else if(s.length() > 5 && s.substr(s.length() - 5) == "iness"){
				s = s.substr(0, s.length() - 5);
				s += "y";
			}
			else if(s.length() > 4 && s.substr(s.length() - 4) == "ness"){
				s = s.substr(0, s.length() - 4);
			}
			else if(s.length() > 5 && s.substr(s.length() - 5) == "ational"){
				s = s.substr(0, s.length() - 5);
				s += "ate";
			}
			else if(s.length() > 5 && s.substr(s.length() - 5) == "tional"){
				s = s.substr(0, s.length() - 5);
				s += "tion";
			}
			else if(s.length() > 1 && s.substr(s.length() - 1) == "s"){
				s = s.substr(0, s.length() - 1);
			}
			cout<<s<<" ";
			keywords[s]++;
		}
		
		cout<<"\n\nKeywords: "<<endl;
		for(auto word: keywords){
			if(word.second > 2)
				cout<<word.first<<" -> "<<word.second<<endl;
		}
	}
	catch(...){
		cout<<"error";
	}
}
