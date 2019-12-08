#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

map<pair<string, string>, float> bigram_counts;
map<string, float> unigram_counts;

void calc_bigrams(const string&);
void lookup_bigram(const string&, const string&);
void process(string&);

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Usage: ./calc_bigrams <FILENAME>\n";
		return 0;
	}

	calc_bigrams(argv[1]);

	vector<pair<string, string>> words = {
		{ "with", "me" },
		{ "the", "grass" },
		{ "the", "king" },
		{ "to", "you" }
	};

	for (auto p : words) {
		lookup_bigram(p.first, p.second);
	}
}

// Given filename fn, read in the file word by word
// for each word:
// 1. process(word)
// 2. increment count of that word in unigram_counts
// 3. increment count of (prevword, word) in bigram_counts
void calc_bigrams(const string& fn) {
	string word;
	string previous = "";
	ifstream myfile_in(fn, ios::in);
	map<string, float>::iterator unigram_it;
	map<pair<string, string>, float>::iterator bigram_it;
	if(myfile_in.is_open()){
		while(myfile_in >> word){
			process(word);
			//check if unigram_counts has the word or not
			//If has, increments; if not, insert a new one.
			unigram_it = unigram_counts.find(word);
			if(unigram_it != unigram_counts.end()){
				unigram_it->second = unigram_it->second + 1;
			}
			else{
				unigram_counts.insert(make_pair(word,1));
			}
			//Check if bigram_counts has the pair(previous,word) or not.
			//If has, increment it; if not, insert a new one.
			bigram_it = bigram_counts.find(make_pair(previous,word));
			if(bigram_it != bigram_counts.end()){
				bigram_it->second = bigram_it->second + 1;
			}
			else{
				bigram_counts.insert(make_pair(make_pair(previous,word),1));
			}
			previous = word;
		}
		myfile_in.close();
	}
	else{
		cout<<"file not be opened."<<endl;
	}
}

// Given words w1 and w2,
// 0. process the words 
// 1. if bigram (w1, w2) is not found, only print "Bigram <w1>, <w2> not found"
// 2. print the words
// 3. print how many times w1 appears
// 4. print how many times (w1, w2) appears
// 5. print count(w1, w2)/count(w1)
// (see example output)
void lookup_bigram(const string& w1, const string& w2) {
	//Init each maps iterator.
	map<string, float>::iterator unigram_it;
	map<pair<string, string>, float>::iterator bigram_it;
	//find the words in each map.
	unigram_it = unigram_counts.find(w1);
	bigram_it = bigram_counts.find(make_pair(w1,w2));
	//if pair<w1,w2> not found in bigram_counts, print not found and quit the function.
	if(bigram_it == bigram_counts.end()){
		cout<<"Bigram "<<w1<<", "<<w2<<" not found"<<endl;
		return;
	}
	//Print the informations.
	cout<<w1<<", "<<w2<<":"<<endl;
	cout<<w1<<" appears "<<unigram_it->second<<" times"<<endl;
	cout<<w1<<", "<<w2<<" appears "<<bigram_it->second<<" times"<<endl;
	cout<<(bigram_it->second/unigram_it->second)<<endl;
}

void process(string& str) {
	for (int i = 0; i < str.length(); ++i) {
		if(isalpha(str[i])) {
			str[i] = tolower(str[i]);
		}
		else {
			str[i] = '*';
		}
	}
	str.erase(remove(str.begin(), str.end(), '*'), str.end());
}
