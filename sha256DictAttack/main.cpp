#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <unordered_map>
#include "sha256.h"
using namespace std;


// Dawoud Tabbouh - 17497998.
// All code in this source is uniquely mine

void getSaltValue(string password2);
void printout();
bool checkIfHashDictionaryExist();
void generateHashDictionary(string dir);
void comboToFile(list<string> &wordList, list<string> &verbList, list<string> &nameList, list<string> &numberList, list<string> &combinationList);
string getHash(string password2);
void saltToPassword(string password2);
int convertToHash(list<string> &wordList, list<string> &verbList, list<string> &nameList, list<string> &numberList, list<string> &combinationList);
void toUpperCase(string &str, int start, int end);
void toLowerCase(string &str, int start, int end);
void firstUpperCase(string &str);
void loadHashDictionary(unordered_map<string, string> &hashDict);
void unsaltedPasswordAttack(unordered_map<string, string> &hashDict, string password);


int main() {

	
		
	string dir = "";
	
	if (!checkIfHashDictionaryExist()) {
		cout << "Please enter the directory of a dictionary list\n> ";
		getline(cin, dir);
		cout << "\nPlease wait while word combinations and hashes are computed...\n";
		generateHashDictionary(dir);
	}

	
	int input = '\0';
	
	do{
	
		cout << "\n1: offline attack" <<endl;
		cout << "2: online attack"<<endl;
		cout << "3: exit\n" << endl;
		cout << "Input > ";
		cin >> input;
		cout << endl;
		if(input == 1)
		{
			string password = "";
			cout << "Please enter password directory\n> ";
			cin >> password;
			unordered_map<string, string> hashDict;
			loadHashDictionary(hashDict);
			unsaltedPasswordAttack(hashDict, password);
				
		}
		else if(input == 2)
		{
			string password2 = "";
			cout << "Please enter salted password directory\n> ";
			cin >> password2;
			saltToPassword(password2);
			
		}
		else if(input == 3)
			break;
	
	}while(1);
	
	return 0;
}
// returns true if hash dictionary has already been computed, false otherwise.
bool checkIfHashDictionaryExist() {
	cout << "Searching for hash dictionary..." << endl;
	ifstream file("hashDict.txt");
	bool retVal;
	
	if (file.good()) {
		retVal = true;
		cout << "Hash dictionary found..." << endl;
	} else {
		retVal = false;
		cout << "Hash dictionary not found..." << endl;
	}
	
	file.close();
	return retVal;
}


// generates combination from dictionary with provided criteria

void generateHashDictionary(string dir) {
	cout << "Generating hash dictionary..." << endl;
	ifstream dictFile(dir);
	list<string> wordList, nameList, verbList, numberList, combinationList;
	
	cout << "Loading dictionary words..." << endl;
	if (dictFile.good()) {
		string line;
		while(getline(dictFile, line)) {
			int npos = line.find(",");
			string word = line.substr(0, npos);
			string flag = line.substr(npos + 1, line.size());
			bool added = false;
			list<string> *listPtr;
			for (int i = 0; i < flag.size(); ++i) {
				if (flag[i] == 'f') {
					listPtr = &nameList;
					added = true;
					break;
				} else if (flag[i] == 'v') {
					listPtr = &verbList;
					added = true;
					break;
				}
			}
			if (!added) {
				listPtr = &wordList;
			}
			toLowerCase(word, 0, word.size());
			listPtr->push_back(word);
			toUpperCase(word, 0, word.size());
			listPtr->push_back(word);
			firstUpperCase(word);
			listPtr->push_back(word);
			
		}
	} else {
		cout << "ERROR: dict.txt was not found..." << endl;
	}
	dictFile.close();
	cout << nameList.size() << " names found..." << endl;
	cout << verbList.size() << " verbs found..." << endl;
	cout << wordList.size() << " other words found..." << endl;
	
	cout << "Generating number sequences..." << endl;
	string numbers[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 4; ++j) {
			string numberSequence = "";
			for (int k = 0; k <= j; ++k) {
				numberSequence += numbers[i];
			}
			numberList.push_back(numberSequence);
		}
	}
	for (int i = 0; i < 9; ++i) {
		int counter = 1;
		while (counter < 4) {
			string numberSequence = "";
			int j;
			for (j = i; j <= i + counter && j < 10; ++j) {
				numberSequence += numbers[j];
			}
			numberList.push_back(numberSequence);
			if (j >= 10) {
				break;
			}
			++counter;
		}
	}
	for (int i = 9; i > 0; --i) {
		int counter = 1;
		while (counter < 4) {
			string numberSequence = "";
			int j;
			for (j = i; j >= i - counter && j >= 0; --j) {
				numberSequence += numbers[j];
			}
			numberList.push_back(numberSequence);
			if (j < 0) {
				break;
			}
			++counter;
		}
	}
	cout << numberList.size() << " number sequences generated..." << endl;
	
	cout << "Generating combination list..." << endl;
	for (list<string>::iterator it = wordList.begin(); it != wordList.end(); ++it) {
		for (list<string>::iterator nit = numberList.begin(); nit != numberList.end(); ++nit) {
			combinationList.push_back(*it + *nit);
			combinationList.push_back(*nit + *it);
		}
	}
	for (list<string>::iterator it = verbList.begin(); it != verbList.end(); ++it) {
		for (list<string>::iterator nit = numberList.begin(); nit != numberList.end(); ++nit) {
			combinationList.push_back(*it + *nit);
			combinationList.push_back(*nit + *it);
		}
	}
	for (list<string>::iterator it = nameList.begin(); it != nameList.end(); ++it) {
		for (list<string>::iterator nit = numberList.begin(); nit != numberList.end(); ++nit) {
			combinationList.push_back(*it + *nit);
			combinationList.push_back(*nit + *it);
		}
	}
	for (list<string>::iterator it = verbList.begin(); it != verbList.end(); ++it) {
		for (list<string>::iterator nit = nameList.begin(); nit != nameList.end(); ++nit) {
			combinationList.push_back(*it + *nit);
		}
	}
	cout << combinationList.size() << " combinations generated..." << endl;
	
	comboToFile(wordList, verbList, nameList, numberList, combinationList);
	convertToHash(wordList, verbList, nameList, numberList, combinationList);
	
	
}

// writes all the combinations to a text file (no hash, for online use)
		
void comboToFile(list<string> &wordList, list<string> &verbList, list<string> &nameList, list<string> &numberList, list<string> &combinationList){
	ofstream dictCombo("dictCombo.txt");
	cout << "Building word list..." << endl;	
	for (list<string>::iterator it = wordList.begin(); it !=wordList.end(); ++it) {
		dictCombo << *it << endl;
		}
		cout << "Building verb list..." << endl;
	for (list<string>::iterator it = verbList.begin(); it != verbList.end(); ++it) {
		dictCombo << *it << endl;
		}
		cout << "Building name list..." << endl;
	for (list<string>::iterator it = nameList.begin(); it != nameList.end(); ++it) {
		dictCombo << *it << endl;
		}
		cout << "Building number list..." << endl;
	for (list<string>::iterator it = numberList.begin(); it != numberList.end(); ++it) {
		dictCombo << *it << endl;
		}
		cout << "Building combination list..." << endl;
	for (list<string>::iterator it = combinationList.begin(); it != combinationList.end(); ++it) {
		dictCombo << *it << endl;
		}
		dictCombo.close();

}


// salted password attack (online attack)
void saltToPassword(string password2){
	getSaltValue(password2);
	cout << "Cracking salted passwords...\n" << endl;
	ifstream saltFile("saltFile.txt");
	ifstream passwordFile(password2);
	if(saltFile.good() && passwordFile.good()){
		string lineFromSalt;
		string dictWord;
		string lineFromPassword;
		
		while(getline(saltFile, lineFromSalt)){
			getline(passwordFile, lineFromPassword);
			int npos = lineFromPassword.find(":");
			string username = lineFromPassword.substr(0, npos);
			ifstream dictCombo("dictCombo.txt");
			string hash = getHash(password2);
			while(getline(dictCombo, dictWord)){
				string concat = dictWord + lineFromSalt;
				string hashedConcat = sha256(concat);
				if (hash == hashedConcat) {
					cout << "User: "<< username << endl;
					cout << "Password: " << dictWord << endl;
					break;
				} else {
					continue;
				}
				
			}
			
			dictCombo.close();
		}
	
	}
	else 
		cout << "Couldnt open" << endl;
	saltFile.close();
	passwordFile.close();
	
	cout << "Cracking complete..." << endl;
}
	
	
// creates hash of all combination of dictionary words and writes to file.
	
int convertToHash(list<string> &wordList, list<string> &verbList, list<string> &nameList, list<string> &numberList, list<string> &combinationList){
	cout << "Computing hash values..." << endl;
	ofstream hashFile("hashDict.txt");
	int counter = 1;
	for (list<string>::iterator it = wordList.begin(); it != wordList.end(); ++it) {
		if (counter++ % 99999 == 0) cout << counter << " hashes generated..." << endl;
		hashFile << *it << ":" << sha256(*it) << endl;
	}
	for (list<string>::iterator it = verbList.begin(); it != verbList.end(); ++it) {
		if (counter++ % 99999 == 0) cout << counter << " hashes generated..." << endl;
		hashFile << *it << ":" << sha256(*it) << endl;
	}
	for (list<string>::iterator it = nameList.begin(); it != nameList.end(); ++it) {
		if (counter++ % 99999 == 0) cout << counter << " hashes generated..." << endl;
		hashFile << *it << ":" << sha256(*it) << endl;
	}
	for (list<string>::iterator it = numberList.begin(); it != numberList.end(); ++it) {
		if (counter++ % 99999 == 0) cout << counter << " hashes generated..." << endl;
		hashFile << *it << ":" << sha256(*it) << endl;
	}
	for (list<string>::iterator it = combinationList.begin(); it != combinationList.end(); ++it) {
		if (counter++ % 99999 == 0) cout << counter << " hashes generated..." << endl;
		hashFile << *it << ":" << sha256(*it) << endl;
	}
	cout << counter << " hashes generated..." << endl;
	
	hashFile.close();
	cout << "Hash file generation completed..." << endl;
}


void toUpperCase(string &str, int start, int end) {
	for (int i = start; i < end; ++i) {
		if (str[i] >= 'a' && str[i] <= 'z') {
			str[i] -= 32;
		}
	}
}

void toLowerCase(string &str, int start, int end) {
	for (int i = start; i < end; ++i) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] += 32;
		}
	}
}

void firstUpperCase(string &str) {
	toUpperCase(str, 0, 1);
	toLowerCase(str, 1, str.size());
}

// loads hash dictioary to memory in prep for offline attack
void loadHashDictionary(unordered_map<string, string> &hashDict) {
	cout << "Loading hash dictionary to memory..." << endl;
	ifstream hashFile("hashDict.txt");
	string line;
	while (getline(hashFile, line)) {
		int npos = line.find(":", 0);
		string password = line.substr(0, npos);
		string hash = line.substr(npos + 1, line.size());
		hashDict.insert(make_pair(hash, password));
	}
	cout << "Loading complete..." << endl;
}


// performs unsalted offline attack
void unsaltedPasswordAttack(unordered_map<string, string> &hashDict, string password) {
	cout << "Cracking unsalted passwords...\n" << endl;
	ifstream unsaltedFile(password);
	if (unsaltedFile.good()) {
		string line;
		while (getline(unsaltedFile, line)) {
			int npos = line.find(":");
			string username = line.substr(0, npos);
			string hash = line.substr(npos + 1, line.size());
			unordered_map<string, string>::iterator it = hashDict.find(hash);
			if (it == hashDict.end()) {
				cout << username << " Password Not Found!" << endl;
			} else {
				cout << username << " " << it->second << endl;
			}
		}
	} else {
		cout << "ERROR: "<<password<<" not found..." << endl;
	}
	cout << "\nCracking completed..." << endl;
}


// gets salt value from provided password file
void getSaltValue(string password2){
	cout << "retrieving salt value.." << endl;
	ifstream passwordFile(password2);	
	cout << "Loading dictionary words..." << endl;
	if (passwordFile.good()) {
		string line;
		ofstream saltFile("saltFile.txt");
		while(getline(passwordFile, line)) { // read line from dictionary
			int npos = line.find(":");
			string username = line.substr(0, npos);
			int npos_S = line.find(":", username.size()+1);
			string salt = line.substr(npos+1, npos_S-(npos+1));
			int npos_H = line.find(":", npos+1);
			string hash = line.substr(npos_H+1, line.size());
			saltFile << salt << endl;
			
			
		}
	}

}

// gets hash from provided password file
string getHash(string password2){
	static ifstream passwordFile(password2);	
	if (passwordFile.good()) {
		string line;
		while(getline(passwordFile, line)) { // read line from dictionary
			int npos = line.find(":");
			string username = line.substr(0, npos);
			int npos_S = line.find(":", username.size()+1);
			string salt = line.substr(npos+1, npos_S-(npos+1));
			int npos_H = line.find(":", npos+1);
			string hash = line.substr(npos_H+1, line.size());
			return hash;
	}
	}
}
