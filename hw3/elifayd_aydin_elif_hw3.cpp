/*Written by Elif Aydin 28061*/
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "HashTable.h"
#include "HashTable.cpp"
using namespace std;

struct show {
	double occurance;
	double support;
	string name;

	bool operator==(const show& rhs) const {
		if (name == rhs.name) {
			return true;
		}
		return false;
	}
};

struct showPair {
	double occurance;
	double support;
	show first;
	show second;

	bool operator==(const showPair& rhs) const {
		if ((rhs.first.name == first.name && rhs.second.name == second.name) || (rhs.first.name == second.name && rhs.second.name == first.name)) {
			return true;
		}
		return false;
	}
	showPair& operator=(const showPair& pair)
	{
		occurance = pair.occurance;
		support = pair.support;
		first = pair.first;
		second = pair.second;
		return *this;
	}

};

struct twoWay {
	bool firstPair;
	bool secondPair;
	show lhsSingle; //if firstPair is false (single), only hold show
	show rhsSingle; //if secondPair is false(single), only hold show
	showPair lhsPair; //if firstPair true (pair), assign shows
	showPair rhsPair; //if secondPair is true (pair), assign shows)
	double support_num;
	double support_denom;
	double confidence; //support numerator / support denominator
};

void openFile(ifstream& inFile) { //this function will force to open a file, if it does not exists, it will request name again
	string fName;
	cout << "Please enter the transaction file name: ";
	cin >> fName;
	inFile.open(fName.c_str());
	while (inFile.fail()) {
		cout << "File failed to open!" << endl;
		cout << "Enter the file name, typo? " << endl;
		cin >> fName;
		inFile.open(fName.c_str());
	}
}

int hashSingle(const show& aShow, int arrSize) {
	int hashVal = 0, asc;
	string aMovie = aShow.name;
	for (int i = 0; i < aMovie.size(); i++) {
		asc = aMovie[i] > 96 ? aMovie[i] - 96 : aMovie[i] - 64;
		hashVal = (hashVal * 32 + asc) % arrSize;
	}
	return hashVal;
}

//hash Function for shows
int hashPair(const showPair& shows, int arrSize) {
	int hashVal = 0, asc;
	string aMovie = "";
	aMovie += shows.first.name;
	aMovie += shows.second.name;
	for (int i = 0; i < aMovie.size(); i++) {
		asc = aMovie[i] > 96 ? aMovie[i] - 96 : aMovie[i] - 64;
		hashVal = (hashVal * 32 + asc) % arrSize;
	}	
	return hashVal; //return the index
}

//true if a given (single)item exists in the unique shows vector, then increase the occurance. Else return false
bool doesExists(vector<show>& shows, const show& item) {
	for (int i = 0; i < shows.size(); i++) {
		if (shows[i].name == item.name) {
			shows[i].occurance++;
			return true;
		}
	}
	return false; //does not exists
}

//reads the input txt, stores it in lines matrix, gets the unique movies and the number of lines
void readFile(ifstream& inFile, vector<show>& movies, vector<vector<string>>& lines, int& numLines) {
	string line;
	string name;
	while (getline(inFile, line)) {
		if (line != "") {
			numLines++;
			vector<string> row;
			istringstream input(line);
			while (input >> name) {
				show newMovie;
				newMovie.name = name;
				if (!doesExists(movies, newMovie)) {
					newMovie.occurance = 1.0; //first time
					movies.push_back(newMovie); //push it to unique vector of movies
				}
				row.push_back(name); //push the string to lines
			}
			lines.push_back(row); //push the row to lines matrix
		}
	}
}

//calculate support value for movies which is occurances/number of Lines, if >=threshold support value add to HashTable
void calculateSupport(vector<show>& movies, const int& numLines, const double& threshold_support, HashTable<show>& showsTable, vector<show>& exceed, HashTable<show>& singleShows) {
	int tableSize = showsTable.maxSize(); //get size, we will use it for calculating hashValue
	for (int i = 0; i < movies.size(); i++) {
		movies[i].support = movies[i].occurance / numLines;
		if (movies[i].support >= threshold_support) {
			int hashed = hashSingle(movies[i], tableSize); //get hashed value
			showsTable.insert(movies[i], hashed); //insert to table
			exceed.push_back(movies[i]); //push to single movies vector						
		}
	}
}

//find all pairs and fill to pairs vector
void fillPairs(vector<show>& movies, vector<showPair>& pairs) {
	for (int i = 0; i < movies.size() - 1; i++) {
		for (int j = i + 1; j < movies.size(); j++) {
			showPair currPair; //create new shows for pair
			currPair.first = movies[i];
			currPair.second = movies[j];
			pairs.push_back(currPair);
		}
	}
}

//return number of occurances for a given pair, or 3,4...
double occuranceMultiple(vector<string> showNames, vector<vector<string>> lines) {
	double occurance = 0.0;
	int count = 0;
	int totalItems = showNames.size();
	for (int i = 0; i < lines.size(); i++) { //row
		for (int j = 0; j < lines[i].size(); j++) {
			if (std::find(showNames.begin(), showNames.end(), lines[i][j]) != showNames.end()) {
				count++; //item found in the vector
			}
		}
		if (count == totalItems) { //seen pair together
			occurance++; //increment occurance, they were seen in the same row together
		}
		count = 0; //set count 0 for the next row
	}
	return occurance;
}
void calculateSupportPairs(vector<showPair>& pairs, HashTable<showPair>& showsTable, vector<vector<string>> lines, const int& numLines, const double& threshold_support, vector<showPair>& exceedPairs) {
	int tableSize = showsTable.maxSize();
	for (int i = 0; i < pairs.size(); i++) {
		vector<string> nameList;
		nameList.push_back(pairs[i].first.name);
		nameList.push_back(pairs[i].second.name);
		pairs[i].occurance = occuranceMultiple(nameList, lines);
		pairs[i].support = pairs[i].occurance / numLines;
		int hashed = hashPair(pairs[i], tableSize); //get hashed value
		showsTable.insert(pairs[i], hashed); //insert to HashTable
		//vicea versa
		showPair vicea;
		vicea.first = pairs[i].second;
		vicea.second = pairs[i].first;
		vicea.occurance = pairs[i].occurance;
		vicea.support = pairs[i].support;
		hashed = hashPair(vicea, tableSize); //get hashed value
		showsTable.insert(vicea, hashed); //insert to HashTable

		if (pairs[i].support >= threshold_support) {
			exceedPairs.push_back(pairs[i]); //insert to vector
		}
	}
}

bool existsPair(vector<string> &contains, string& pairs) {
	if (std::find(contains.begin(), contains.end(), pairs) != contains.end()){
		return true;
	}
	else {
		contains.push_back(pairs);
		return false;
	}
}
 //bu fonksiyonu ayýrman lazým
bool existsTriple(vector<string>& triples, string& str1,string& str2, string& str3, bool twoOne) {
	//(2,1) or (1,2)
	if (twoOne) { //(2,1)
		string searched1 = str1 + str2 +">"+ str3;
		string searched2 = str2 + str1 +">" +str3;
		if (std::find(triples.begin(), triples.end(), searched1) != triples.end()) {
			return true;
		}
		if (std::find(triples.begin(), triples.end(), searched2) != triples.end()) {
			return true;
		}
		triples.push_back(searched1);
		return false;
	}
	else { //(1,2)
		string searched1 = str1 +">"+ str2 + str3;
		string searched2 = str1 +">"+ str3 + str2;
		if (std::find(triples.begin(), triples.end(), searched1) != triples.end()) {
			return true;
		}
		if (std::find(triples.begin(), triples.end(), searched2) != triples.end()) {
			return true;
		}
		triples.push_back(searched1);
		return false;
	}
}

vector<twoWay> twoWayPerm(vector<showPair>& exceedPairs, HashTable<showPair>& pairsTable, HashTable<show>& singleTable, const int& numLines, const vector<vector<string>>& lines, const double& confidence) {
	int counter = 0;
	vector<string> contains;
	vector<string> contains3;
	vector<twoWay> permutations;
	string exists = "";
	for (int i = 0; i < exceedPairs.size(); i++) { //iterate over vector get lhs single
		for (int j = i + 1; j < exceedPairs.size(); j++) { //iterate over vector get rhs single
			exists = "";
			twoWay twoTwo;
			twoTwo.firstPair = true;
			twoTwo.secondPair = true;
			twoTwo.lhsPair = exceedPairs[i];
			twoTwo.rhsPair = exceedPairs[j];
			vector<string> names;
			names.push_back(twoTwo.lhsPair.first.name);
			names.push_back(twoTwo.lhsPair.second.name);
			names.push_back(twoTwo.rhsPair.first.name);
			names.push_back(twoTwo.rhsPair.second.name);
			double occurance = occuranceMultiple(names, lines);
			twoTwo.support_num = occurance / numLines;
			showPair denominator;
			denominator.first = twoTwo.lhsPair.first;
			denominator.second = twoTwo.lhsPair.second;
			int hashed = hashPair(denominator, pairsTable.maxSize());
			denominator = pairsTable.find(denominator, hashed);
			twoTwo.support_denom = denominator.support;
			twoTwo.confidence = twoTwo.support_num / twoTwo.support_denom;
			if (twoTwo.confidence >= confidence) {
				permutations.push_back(twoTwo);
				counter++;

			}

			twoWay twoTwo2;
			twoTwo2.firstPair = true;
			twoTwo2.secondPair = true;
			twoTwo2.lhsPair = twoTwo.rhsPair;
			twoTwo2.rhsPair = twoTwo.lhsPair;
			twoTwo2.support_num = twoTwo.support_num;
			showPair denominator2;
			denominator2.first = twoTwo2.lhsPair.first;
			denominator2.second = twoTwo2.lhsPair.second;
			hashed = hashPair(denominator2, pairsTable.maxSize());
			denominator2 = pairsTable.find(denominator2, hashed);
			twoTwo2.support_denom = denominator2.support;
			twoTwo2.confidence = twoTwo2.support_num / twoTwo2.support_denom;
			if (twoTwo2.confidence >= confidence) {
				permutations.push_back(twoTwo2);
				counter++;

			}

			twoWay perm1; //a->b
			perm1.firstPair = false;
			perm1.secondPair = false;
			perm1.lhsSingle = exceedPairs[i].first;
			perm1.rhsSingle = exceedPairs[i].second;
			exists += perm1.lhsSingle.name + perm1.rhsSingle.name;
			perm1.support_denom = perm1.lhsSingle.support;
			showPair numerator;
			numerator.first = perm1.lhsSingle;
			numerator.second = perm1.rhsSingle;
			hashed = hashPair(numerator, pairsTable.maxSize());
			numerator = pairsTable.find(numerator, hashed);
			perm1.support_num = numerator.support; //support(a,b)
			perm1.support_denom = perm1.lhsSingle.support;
			perm1.confidence = perm1.support_num / perm1.support_denom;
			if (perm1.confidence >= confidence) {
				if (!existsPair(contains, exists)) { //element does not exists, push to twoWay permutations
					permutations.push_back(perm1);
					counter++;
				}
			}

			exists = "";
			twoWay perm2;//b->a
			perm2.firstPair = false;
			perm2.secondPair = false;
			perm2.lhsSingle = exceedPairs[i].second;
			perm2.rhsSingle = exceedPairs[i].first;
			exists += perm2.lhsSingle.name + perm2.rhsSingle.name;
			showPair numerator2;
			numerator2 = numerator;
			perm2.support_num = numerator2.support; //support(a,b)
			perm2.support_denom = perm2.lhsSingle.support;
			perm2.confidence = perm2.support_num / perm2.support_denom;
			if (perm2.confidence >= confidence) {
				if (!existsPair(contains, exists)) {
					permutations.push_back(perm2);
					counter++;
				}
			}
			exists = "";
			twoWay perm3;//a->c
			perm3.firstPair = false;
			perm3.secondPair = false;
			perm3.lhsSingle = exceedPairs[i].first;
			perm3.rhsSingle = exceedPairs[j].first;
			exists += perm3.lhsSingle.name + perm3.rhsSingle.name;
			if (perm3.lhsSingle.name != perm3.rhsSingle.name) {
				showPair numerator3;
				numerator3.first = perm3.lhsSingle;
				numerator3.second = perm3.rhsSingle;
				hashed = hashPair(numerator3, pairsTable.maxSize());
				numerator3 = pairsTable.find(numerator3, hashed);
				perm3.support_num = numerator3.support; //support (a,c)
				perm3.support_denom = perm3.lhsSingle.support;
				perm3.confidence = perm3.support_num / perm3.support_denom;
				if (perm3.confidence >= confidence) {
					if (!existsPair(contains, exists)) {
						permutations.push_back(perm3);
						counter++;
					}
				}
				exists = "";
				twoWay perm4; //c->a
				perm4.firstPair = false;
				perm4.secondPair = false;
				perm4.lhsSingle = perm3.rhsSingle;
				perm4.rhsSingle = perm3.lhsSingle;
				exists += perm4.lhsSingle.name + perm4.rhsSingle.name;
				showPair numerator4;
				numerator4 = numerator3;
				perm4.support_num = numerator4.support; //support (a,c)
				perm4.support_denom = perm4.lhsSingle.support;
				perm4.confidence = perm4.support_num / perm4.support_denom;
				if (perm4.confidence >= confidence) {
					if (!existsPair(contains, exists)) {
						permutations.push_back(perm4);
						counter++;
					}
				}
			}
			exists = "";
			twoWay perm5; //a->d
			perm5.firstPair = false;
			perm5.secondPair = false;
			perm5.lhsSingle = exceedPairs[i].first;
			perm5.rhsSingle = exceedPairs[j].second;
			exists += perm5.lhsSingle.name + perm5.rhsSingle.name;
			if (perm5.lhsSingle.name != perm5.rhsSingle.name) {
				showPair numerator5;
				numerator5.first = perm5.lhsSingle;
				numerator5.second = perm5.rhsSingle;
				hashed = hashPair(numerator5, pairsTable.maxSize());
				numerator5 = pairsTable.find(numerator5, hashed);
				perm5.support_num = numerator5.support; //support(a,d)
				perm5.support_denom = perm5.lhsSingle.support;
				perm5.confidence = perm5.support_num / perm5.support_denom;
				if (perm5.confidence >= confidence) {
					if (!existsPair(contains, exists)) {
						permutations.push_back(perm5);
						counter++;
					}
				}
				exists = "";
				twoWay perm6; //d->a
				perm6.firstPair = false;
				perm6.secondPair = false;
				perm6.lhsSingle = perm5.rhsSingle;
				perm6.rhsSingle = perm5.lhsSingle;
				exists += perm6.lhsSingle.name + perm6.rhsSingle.name;
				showPair numerator6;
				numerator6 = numerator5;
				perm6.support_num = numerator6.support; //support(a,d)
				perm6.support_denom = perm6.lhsSingle.support;
				perm6.confidence = perm6.support_num / perm6.support_denom;
				if (perm6.confidence >= confidence) {
					if (!existsPair(contains, exists)) {
						permutations.push_back(perm6);
						counter++;
					}
				}
			}
			exists = "";
			twoWay perm7;//b->c
			perm7.firstPair = false;
			perm7.secondPair = false;
			perm7.lhsSingle = exceedPairs[i].second;
			perm7.rhsSingle = exceedPairs[j].first;
			exists += perm7.lhsSingle.name + perm7.rhsSingle.name;
			if (perm7.lhsSingle.name != perm7.rhsSingle.name) {
				showPair numerator7;
				numerator7.first = perm7.lhsSingle;
				numerator7.second = perm7.rhsSingle;
				hashed = hashPair(numerator7, pairsTable.maxSize());
				numerator7 = pairsTable.find(numerator7, hashed);
				perm7.support_num = numerator7.support; //support (b,c)
				perm7.support_denom = perm3.lhsSingle.support;
				perm7.confidence = perm7.support_num / perm7.support_denom;
				if (perm7.confidence >= confidence) {
					if (!existsPair(contains, exists)) {
						permutations.push_back(perm7);
						counter++;
					}
				}
				exists = "";
				twoWay perm8; //c->b
				perm8.firstPair = false;
				perm8.secondPair = false;
				perm8.lhsSingle = perm7.rhsSingle;
				perm8.rhsSingle = perm7.lhsSingle;
				exists += perm8.lhsSingle.name + perm8.rhsSingle.name;
				showPair numerator8;
				numerator8 = numerator7;
				perm8.support_num = numerator8.support; //support (b,c)
				perm8.support_denom = perm8.lhsSingle.support;
				perm8.confidence = perm8.support_num / perm8.support_denom;
				if (perm8.confidence >= confidence) {
					if (!existsPair(contains, exists)) {
						permutations.push_back(perm8);
						counter++;
					}
				}
			}
			exists = "";
			twoWay perm9; //b->d
			perm9.firstPair = false;
			perm9.secondPair = false;
			perm9.lhsSingle = exceedPairs[i].second;
			perm9.rhsSingle = exceedPairs[j].second;
			exists += perm9.lhsSingle.name + perm9.rhsSingle.name;
			if (perm9.lhsSingle.name != perm9.rhsSingle.name) {
				showPair numerator9;
				numerator9.first = perm9.lhsSingle;
				numerator9.second = perm9.rhsSingle;
				hashed = hashPair(numerator9, pairsTable.maxSize());
				numerator9 = pairsTable.find(numerator9, hashed);
				perm9.support_num = numerator9.support; //support (b,d)
				perm9.support_denom = perm9.lhsSingle.support;
				perm9.confidence = perm9.support_num / perm9.support_denom;
				if (perm9.confidence >= confidence) {
					if (!existsPair(contains, exists)) {
						permutations.push_back(perm9);
						counter++;
					}
				}

				twoWay perm10; //d->b 
				exists = "";
				perm10.firstPair = false;
				perm10.secondPair = false;
				perm10.lhsSingle = perm9.rhsSingle;
				perm10.rhsSingle = perm9.lhsSingle;
				exists += perm10.lhsSingle.name + perm10.rhsSingle.name;
				showPair numerator10;
				numerator10 = numerator9;
				perm10.support_num = numerator10.support; //support (b,d)
				perm10.support_denom = perm10.lhsSingle.support;
				perm10.confidence = perm10.support_num / perm10.support_denom;
				if (perm10.confidence >= confidence) {
					if (!existsPair(contains, exists)) {
						permutations.push_back(perm10);
						counter++;
					}
				}
			}
			//(2,1) and (1,2) permutations
			double occurances;
			twoWay cab; //c-> (a,b)
			cab.firstPair = false;
			cab.secondPair = true;
			cab.lhsSingle = exceedPairs[j].first;
			cab.rhsPair = exceedPairs[i];
			if ((cab.lhsSingle.name != cab.rhsPair.first.name) && (cab.lhsSingle.name != cab.rhsPair.second.name)) {
				vector<string> nameLists;
				nameLists.push_back(cab.lhsSingle.name);
				nameLists.push_back(cab.rhsPair.first.name);
				nameLists.push_back(cab.rhsPair.second.name);
				occurances = occuranceMultiple(nameLists, lines);
				cab.support_num = occurances / numLines; //cab.support_num = support(a,b,c)
				cab.support_denom = cab.lhsSingle.support; //support c
				cab.confidence = cab.support_num / cab.support_denom;
				bool twoOne = false;
				if (!existsTriple(contains3, cab.lhsSingle.name, cab.rhsPair.first.name, cab.rhsPair.second.name, twoOne)) {
					if (cab.confidence >= confidence) {
						permutations.push_back(cab);
						counter++;
					}
				}

				twoWay abc; //(a,b) ->c
				abc.firstPair = true;
				abc.secondPair = false;
				abc.lhsPair = cab.rhsPair;
				abc.rhsSingle = cab.lhsSingle;
				abc.support_num = cab.support_num; //support abc
				abc.support_denom = perm1.support_num; //support (a,b)
				abc.confidence = abc.support_num / abc.support_denom;
				twoOne = true;
				if (!existsTriple(contains3, abc.lhsPair.first.name, abc.lhsPair.second.name, abc.rhsSingle.name, twoOne)) {
					if (abc.confidence >= confidence) {
						permutations.push_back(abc);
						counter++;
					}
				}
			}

			twoWay dab; //d->(a,b)
			dab.firstPair = false;
			dab.secondPair = true;
			dab.lhsSingle = exceedPairs[j].second;
			dab.rhsPair = cab.rhsPair;
			if ((dab.lhsSingle.name != dab.rhsPair.first.name) && (dab.lhsSingle.name != dab.rhsPair.second.name)) {
				vector<string> nameLists;
				nameLists.push_back(dab.lhsSingle.name);
				nameLists.push_back(dab.rhsPair.first.name);
				nameLists.push_back(dab.rhsPair.second.name);
				occurances = occuranceMultiple(nameLists, lines);
				dab.support_num = occurances / numLines; //dab.support_num= support(a,b,d)
				dab.support_denom = dab.lhsSingle.support; //support d
				dab.confidence = dab.support_num / dab.support_denom;
				bool twoOne = false;
				if (!existsTriple(contains3, dab.lhsSingle.name, dab.rhsPair.first.name, dab.rhsPair.second.name, twoOne)) {
					if (dab.confidence >= confidence) {
						permutations.push_back(dab);
						counter++;
					}
				}
				twoWay abd; //(a,b) ->d
				abd.firstPair = true;
				abd.secondPair = false;
				abd.lhsPair = cab.rhsPair;
				abd.rhsSingle = dab.lhsSingle;
				abd.support_num = dab.support_num; //dab.support_num= support(a,b,d)
				abd.support_denom = perm1.support_num; //support(a,b)
				abd.confidence = abd.support_num / abd.support_denom;
				twoOne = true;
				if (!existsTriple(contains3, abd.lhsPair.first.name, abd.lhsPair.second.name, abd.rhsSingle.name, twoOne)) {
					if (abd.confidence >= confidence) {
						permutations.push_back(abd);
						counter++;
					}
				}
			}
			// a-> (c,d)
			twoWay acd;
			acd.firstPair = false;
			acd.secondPair = true;
			acd.lhsSingle = exceedPairs[i].first;
			acd.rhsPair = exceedPairs[j];
			if ((acd.lhsSingle.name != acd.rhsPair.first.name) && (acd.lhsSingle.name != acd.rhsPair.second.name)) {
				vector<string> nameLists2;
				nameLists2.push_back(acd.lhsSingle.name);
				nameLists2.push_back(acd.rhsPair.first.name);
				nameLists2.push_back(acd.rhsPair.second.name);
				occurances = occuranceMultiple(nameLists2, lines);
				acd.support_num = occurances / numLines; //acd.support_num = support(a,c,d)
				acd.support_denom = acd.lhsSingle.support; //support(a)
				acd.confidence = acd.support_num / acd.support_denom;
				bool twoOne = false;
				if (!existsTriple(contains3, acd.lhsSingle.name, acd.rhsPair.first.name, acd.rhsPair.second.name, twoOne)) {
					if (acd.confidence >= confidence) {
						permutations.push_back(acd);
						counter++;
					}
				}

				//(c,d) -> a
				twoWay cda;
				cda.firstPair = true;
				cda.secondPair = false;
				cda.lhsPair = acd.rhsPair;
				cda.rhsSingle = acd.lhsSingle;
				cda.support_num = acd.support_num; //support_cda = support(a,c,d)
				showPair denominatorCD;
				denominatorCD.first = cda.lhsPair.first;
				denominatorCD.second = cda.lhsPair.second;
				hashed = hashPair(denominatorCD, pairsTable.maxSize());
				denominatorCD = pairsTable.find(denominatorCD, hashed);
				cda.support_denom = denominatorCD.support; //support (c,d)
				cda.confidence = cda.support_num / cda.support_denom;
				twoOne = true;
				if (!existsTriple(contains3, cda.lhsPair.first.name, cda.lhsPair.second.name, cda.rhsSingle.name, twoOne)) {
					if (cda.confidence >= confidence) {
						permutations.push_back(cda);
						counter++;
					}
				}
			}

			//b-> (c,d)
			twoWay bcd;
			bcd.firstPair = false;
			bcd.secondPair = true;
			bcd.lhsSingle = exceedPairs[i].second;
			bcd.rhsPair = exceedPairs[j];
			if ((bcd.lhsSingle.name != bcd.rhsPair.first.name) && (bcd.lhsSingle.name != bcd.rhsPair.second.name)) {
				vector<string> nameLists3;
				nameLists3.push_back(bcd.lhsSingle.name);
				nameLists3.push_back(bcd.rhsPair.first.name);
				nameLists3.push_back(bcd.rhsPair.second.name);
				occurances = occuranceMultiple(nameLists3, lines);
				bcd.support_num = occurances / numLines; //bcd.support_num = support(b,c,d)
				bcd.support_denom = bcd.lhsSingle.support; //support b
				bcd.confidence = bcd.support_num / bcd.support_denom;
				bool twoOne = false;
				if (!existsTriple(contains3, bcd.lhsSingle.name, bcd.rhsPair.first.name, bcd.rhsPair.second.name, twoOne)) {
					if (bcd.confidence >= confidence) {
						permutations.push_back(bcd);
						counter++;
					}
				}

				//(c,d) -> b
				twoWay cdb;
				cdb.firstPair = true;
				cdb.secondPair = false;
				cdb.lhsPair = bcd.rhsPair;
				cdb.rhsSingle = bcd.lhsSingle;
				cdb.support_num = bcd.support_num; //support_cdb = support(b,c,d)
				showPair denominatorCD;
				denominatorCD.first = cdb.lhsPair.first;
				denominatorCD.second = cdb.lhsPair.second;
				hashed = hashPair(denominatorCD, pairsTable.maxSize());
				denominatorCD = pairsTable.find(denominatorCD, hashed);
				cdb.support_denom = denominatorCD.support; //support (c,d)
				cdb.confidence = cdb.support_num / cdb.support_denom;
				twoOne = true;
				if (!existsTriple(contains3, cdb.lhsPair.first.name, cdb.lhsPair.second.name, cdb.rhsSingle.name, twoOne)) {
					if (cdb.confidence >= confidence) {
						permutations.push_back(cdb);
						counter++;
					}
				}
			}
			
		}
	}
	exists = "";
	twoWay perm11; //c->d
	perm11.firstPair = false;
	perm11.secondPair = false;
	perm11.lhsSingle = exceedPairs[exceedPairs.size() - 1].first;
	perm11.rhsSingle = exceedPairs[exceedPairs.size() - 1].second;
	exists += perm11.lhsSingle.name + perm11.rhsSingle.name;
	showPair numerator11;
	numerator11.first = perm11.lhsSingle;
	numerator11.second = perm11.rhsSingle;
	int hashed = hashPair(numerator11, pairsTable.maxSize());
	numerator11 = pairsTable.find(numerator11, hashed);
	perm11.support_num = numerator11.support; //support (c,d)
	perm11.support_denom = perm11.lhsSingle.support;
	perm11.confidence = perm11.support_num / perm11.support_denom;
	if (perm11.confidence >= confidence) {
		if (!existsPair(contains, exists)) {
			permutations.push_back(perm11);
			counter++;
		}
	}
	exists = "";
	twoWay perm12; //d->c
	perm12.firstPair = false;
	perm12.secondPair = false;
	perm12.lhsSingle = perm11.rhsSingle;
	perm12.rhsSingle = perm11.lhsSingle;
	exists += perm12.lhsSingle.name + perm12.rhsSingle.name;
	showPair numerator12;
	numerator12 = numerator11;
	perm12.support_num = numerator12.support; //support (c,d)
	perm12.support_denom = perm12.lhsSingle.support;
	perm12.confidence = perm12.support_num / perm12.support_denom;
	if (perm12.confidence >= confidence) {
		if (!existsPair(contains, exists)) {
			permutations.push_back(perm12);
			counter++;
		}
	}
	cout << counter << " rules are written to results.txt" << endl;
	return permutations;
}
int main() {

	ifstream data;
	openFile(data);
	double support, confidence;
	cout << "Please enter support and confidence values between 0 and 1: ";
	cin >> support >> confidence;

	vector<vector<string>> lines; //this corresponds to input.txt
	int numLines = 0;
	vector<show> movies; //whole movies, we will update occurances
	readFile(data, movies, lines, numLines);

	//we have the file stored in lines matrix, and unique movies stored in movies vector
	//1) Calculating Support Values, 
	HashTable<show> singleTable;
	vector<show> exceed; //this vector is for single items that >=threshold_support
	calculateSupport(movies, numLines, support, singleTable, exceed, singleTable); //insert those movies to hashTable
	if (exceed.size() <= 1) {
		cout << "There is no rule for the given support and confidence values." << endl;
	}

	else {//Now we have movies whose supportval>=support_threshold, now we should find all pairs
		vector<showPair> pairs; //vector of possible moviePairs
		fillPairs(exceed, pairs); //find all moviePairs 

		//Now insert those pairs support value to HashTable for pairs HashTable, keep track of shows>threshold_support
		vector<showPair> exceedPairs;
		HashTable<showPair> pairsTable;
		calculateSupportPairs(pairs, pairsTable, lines, numLines, support, exceedPairs); //calculate support value of pairs, insert to hashtable

		ofstream outfile("results.txt");

		if (exceedPairs.size() <= 1) {
			int counter = 0;
			if (exceedPairs.size() == 1) { //only 1 pair, check a->b b->a
				twoWay ab;
				ab.lhsSingle = exceedPairs[0].first;
				ab.rhsSingle = exceedPairs[0].second;
				showPair numerator;
				numerator.first = ab.lhsSingle;
				numerator.second = ab.rhsSingle;
				int hashed = hashPair(numerator, pairsTable.maxSize());
				numerator = pairsTable.find(numerator, hashed);
				ab.support_num = numerator.support; //support (a,b)
				ab.confidence = ab.support_num / ab.lhsSingle.support; //support(a,b) / support(a)
				if (ab.confidence >= confidence) {
					outfile << ab.lhsSingle.name << ";" << ab.rhsSingle.name << ";" << std::fixed << std::setprecision(2) << ab.confidence << endl;
					counter++;
				}

				twoWay ba;
				ba.lhsSingle = ab.rhsSingle;
				ba.rhsSingle = ab.lhsSingle;
				ba.support_num = ab.support_num; //support (a,b)
				ba.confidence = ba.support_num / ba.lhsSingle.support; //support(a,b) /support(b)
				if (ba.confidence >= confidence) {
					outfile << ba.lhsSingle.name << ";" << ba.rhsSingle.name << ";" << std::fixed << std::setprecision(2) << ba.confidence << endl;
					counter++;				
				}
				if (counter == 0) {
					cout << "There is no rule for the given support and confidence values." << endl;
				}
				else { //0 pair
					cout << counter << " rules are written to results.txt" << endl;
				}

			}
			else {
				cout << "There is no rule for the given support and confidence values." << endl; 
			}
		}
		else {
			//We have all pairs that exceed the threshold in exceedPairs, form twoWay Permutations
			vector<twoWay> allPermutations;
			allPermutations = twoWayPerm(exceedPairs, pairsTable, singleTable, numLines, lines, confidence);
			if (allPermutations.size() == 0) {
				cout << "There is no rule for the given support and confidence values." << endl;
			}
			else { //print the rules
				for (int i = 0; i < allPermutations.size(); i++) {
					if (allPermutations[i].firstPair && allPermutations[i].secondPair) { //(2,2)
						outfile << allPermutations[i].lhsPair.first.name << "," << allPermutations[i].lhsPair.second.name << ";" << allPermutations[i].rhsPair.first.name << "," << allPermutations[i].rhsPair.second.name << ";" << std::fixed << std::setprecision(2) << allPermutations[i].confidence << endl;
					}
					else if (allPermutations[i].firstPair && !allPermutations[i].secondPair) { //(2,1)
						outfile << allPermutations[i].lhsPair.first.name << "," << allPermutations[i].lhsPair.second.name << ";" << allPermutations[i].rhsSingle.name << ";" << std::fixed << std::setprecision(2) << allPermutations[i].confidence << endl;
					}
					else if (!allPermutations[i].firstPair && allPermutations[i].secondPair) { //(1,2)
						outfile << allPermutations[i].lhsSingle.name << ";" << allPermutations[i].rhsPair.first.name << "," << allPermutations[i].rhsPair.second.name << ";" << std::fixed << std::setprecision(2) << allPermutations[i].confidence << endl;
					}
					else { //(1,1)
						outfile << allPermutations[i].lhsSingle.name << ";" << allPermutations[i].rhsSingle.name << ";" << std::fixed << std::setprecision(2) << allPermutations[i].confidence << endl;
					}
				}
			}
		}
	}
	
	data.close();
	return 0;
}