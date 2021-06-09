/*Elif Aydin 28061 HW4 Sorting and Searching Algorithms
In large txt, search might give error in debug mode. Release mode works fine*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Query {
	string name;
	string surname;
	string phone;
	string city;
	bool searching;
	Query(string n, string s, string p, string c) { //constructor with parameters
		name = n;
		surname = s;
		phone = p;
		city = c;
		searching = false;
	}
	Query() {} //consturctor without parameters

	Query& operator=(const Query& rhs) { //assignment operator		
		name = rhs.name;
		surname = rhs.surname;
		phone = rhs.phone;
		city = rhs.city;
		return *this;
	}

	bool operator<(const Query& rhs) const
	{
		if (!(searching) && !(rhs.searching)) {
			if (name != rhs.name) {
				if (name < rhs.name) {
					return true;
				}
			}
			else { //equal name-> check surname
				if (surname != rhs.surname) {
					if (surname < rhs.surname) {
						return true;
					}
				}
				else { //equal name,surname -> check phone
					if (phone != rhs.phone) {
						if (phone < rhs.phone) {
							return true;
						}
					}
					else { //equal name,surname,phone -> check city
						if (city != rhs.city) {
							if (city <= rhs.city) {
								return true;
							}
						}
					}
				}
			}
			return false; //equal or larger		
		}
		else { //only name surname
			if (name != rhs.name) {
				if (name < rhs.name) {
					return true;
				}
			}
			else { //equal name-> check surname
				if (surname != rhs.surname) {
					if (surname < rhs.surname) {
						return true;
					}
				}
				return false; //equal or larger
			}
		}
	}

	bool operator>(const Query& rhs) const
	{
		return(!(*this < rhs));
	}
	bool operator==(const Query& rhs) const
	{
		if (!(searching) && !(rhs.searching)) {
			if (name == rhs.name && surname == rhs.surname && phone == rhs.phone && city == rhs.city) {
				return true;
			}
			return false;
		}
		else {
			if (name == rhs.name && surname == rhs.surname) {
				return true;
			}
			return false;
		}

	}
	bool operator<=(const Query& rhs) const
	{
		if (*this < rhs || *this == rhs) {
			return true;
		}
		return false;
	}
	bool operator>=(const Query& rhs) const
	{
		if (*this > rhs || *this == rhs) {
			return true;
		}
		return false;
	}
};


/******************************************************************************************************************/
/*Quick Sort Algortihm*/
 /**
  * Quicksort algorithm (driver).
 */
template <class Comparable>
void quicksort(vector<Comparable>& a) {
	quicksort(a, 0, a.size() - 1);
}

template <class Comparable>
const Comparable& median3(vector<Comparable>& a,
	int left, int right)
{
	int center = (left + right) / 2;
	if (a[center] < a[left])
		std::swap(a[left], a[center]);
	if (a[right] < a[left])
		std::swap(a[left], a[right]);
	if (a[right] < a[center])
		std::swap(a[center], a[right]);

	// Place pivot at position right - 1
	std::swap(a[center], a[right - 1]);
	return a[right - 1];
}


template <class Comparable>
void quicksort(vector<Comparable>& a, int left, int right) {
	if (left + 10 <= right) {
		Comparable pivot = median3(a, left, right);
		int i = left, j = right - 1;
		for (; ; )
		{
			while (a[++i] < pivot) {}

			while (pivot < a[--j]) {}

			if (i < j)
				swap(a[i], a[j]);
			else
				break;
		}

		std::swap(a[i], a[right - 1]);   // Restore pivot
		quicksort(a, left, i - 1);       // Sort small elements
		quicksort(a, i + 1, right);    // Sort large elements

	}
	else  // Do an insertion sort on the subarray
		insertionSort(a, left, right);
}


template <class Comparable>
void insertionSort(vector<Comparable>& a, int left, int right) // for quick sort
{
	for (int p = left + 1; p <= right; p++)
	{
		Comparable tmp = move(a[p]);
		int j;

		for (j = p; j > left && tmp < a[j - 1]; j--)
			a[j] = move(a[j - 1]);
		a[j] = move(tmp);
	}
}


/******************************************************************************************************************/

/******************************************************************************************************************/
/*Insertion Sort Algorithm*/
template <typename Comparable>
void insertionSort(vector<Comparable>& a) {
	for (int p = 1; p < a.size(); ++p) {
		Comparable tmp = std::move(a[p]);
		int j;
		for (j = p; j > 0 && tmp < a[j - 1]; --j) {
			a[j] = std::move(a[j - 1]); //continue comparisons while the selected element is smaller
		}
		a[j] = std::move(tmp); //place the selected element to free slot
	}
}
/******************************************************************************************************************/
/*Left Child for Percolate Down*/
int leftChild(int i) {
	return 2 * i + 1;
}

/*Internal method for heapsort that is used in deleteMax and buildHeap.
 i is the position from which to percolate down.
 n is the logical size of the binary heap.
*/
template < typename Comparable>
void percDown(vector<Comparable>& a, int i, int n) { //root=max
	int child;
	Comparable tmp;
	for (tmp = std::move(a[i]); leftChild(i) < n; i = child) {
		child = leftChild(i); //child= 2*i+1
		if (child != n - 1 && a[child] < a[child + 1]) { //get max child
			++child;
		}
		if (tmp < a[child]) { //compare tmp with max child, if tmp<maxChild, percolate down
			a[i] = std::move(a[child]);
		}
		else {
			break;
		}
	}
	a[i] = std::move(tmp);
}
/*Heap Sort Algortihm*/
template <typename Comparable>
void heapsort(vector<Comparable>& a) {
	for (int i = a.size() / 2 - 1; i >= 0; --i) { //buildHeap
		percDown(a, i, a.size()); //perc down from i'th position to end
	}
	for (int j = a.size() - 1; j > 0; --j) {
		std::swap(a[0], a[j]); //deleteMax to end pos
		percDown(a, 0, j); //perc down from 0th position to logical size j 
	}
}
/******************************************************************************************************************/
/*Mergesort algorithm (driver)*/
template <typename Comparable>
void mergeSort(vector<Comparable>& a) {
	vector<Comparable> tmpArray(a.size()); //tmp array 
	mergeSort(a, 0, a.size() - 1);
}

/**
Internal method that makes recursive calls.
a is an array of Comparable items.
left is the left-most index of the subarray.
right is the right-most index of the subarray.
*/
template <typename Comparable>
void mergeSort(vector<Comparable>& a, int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2; // = (l+r) /r but avoids overflow

		// Sort first and second halves
		mergeSort(a, left, mid);
		mergeSort(a, mid + 1, right);

		merge(a, left, mid, right);
	}
}

/*
Internal method that merges two sorted halves of a subarray.
*/
template <typename Comparable>
void merge(vector<Comparable>& arr, int start, int mid, int end) {
	int start2 = mid + 1; //start of right array 

	// If the direct merge is already sorted
	if (arr[mid] <= arr[start2]) {
		return;
	}

	//start and start2 are pointers for the 2 arrays, traverse the arrays
	while (start <= mid && start2 <= end) {

		// If element 1 is in right place
		if (arr[start] <= arr[start2]) {
			start++;
		}
		else { //not in correct place element2 should be in front
			Comparable value = arr[start2];
			int index = start2;

			// Shift all the elements between element 1
			// element 2, right by 1.
			while (index != start) {
				arr[index] = arr[index - 1];
				index--;
			}
			arr[start] = value; //put element2 in the position of element1

			// Update all the pointers
			start++; //we shifted by 1, increment start1 also
			mid++; //increment mid by 1 because we shifted left array
			start2++; //increment pointer
		}
	}
}

/******************************************************************************************************************/
/*Sorting Algorithms done*/
/******************************************************************************************************************/

void quickSort_measure(vector<Query> quickSort_vec) {
	quicksort(quickSort_vec, 0, quickSort_vec.size() - 1);
}

void insertionSort_measure(vector<Query> insertionSort_vec) {
	insertionSort(insertionSort_vec);
}

void mergeSort_measure(vector<Query> mergeSort_vec) {
	mergeSort(mergeSort_vec);
}

void heapSort_measure(vector<Query> heapSort_vec) {
	heapsort(heapSort_vec);
}


//copy the input txt queries to vectors
void openFile(ifstream& inFile, vector<Query>& insertionSort_vec, vector<Query>& heapSort_vec, vector<Query>& mergeSort_vec, vector<Query>& quickSort_vec) {
	string fName;
	cout << "Please enter the contact file name: " << endl;
	cin >> fName;
	inFile.open(fName.c_str());
	while (inFile.fail()) {
		cout << "File failed to open!" << endl;
		cout << "Enter the file name, typo? " << endl;
		cin >> fName;
		inFile.open(fName.c_str());
	}

	string line;
	while (getline(inFile, line)) {
		if (line != "") {
			string n, s, p, c; //name,surname,phone,city
			istringstream input(line); //Create an input string stream out of the current line
			input >> n >> s >> p >> c;
			Query info(n, s, p, c);
			insertionSort_vec.push_back(info);
			heapSort_vec.push_back(info);
			mergeSort_vec.push_back(info);
			quickSort_vec.push_back(info);
		}
	}
}

void BinarySearchPartial(vector<Query> vec, string partial) {
	int size = partial.size();

	int low = 0;
	int high = vec.size() - 1;

	while (low <= high) {
		int mid = low + (high - low) / 2;

		if (vec[mid].name.substr(0, size) == partial) {
			int start = mid;
			int end = mid;
			while (vec[start].name.substr(0, size) == partial) { //find leftmost
				start--;
			}
			while (vec[end].name.substr(0, size) == partial) { //find rightmost
				end++;
			}
			for (int i = start + 1; i < end; i++) {
				string toUpperName = vec[i].name;
				string toUpperSurName = vec[i].surname;
				std::transform(toUpperName.begin(), toUpperName.end(), toUpperName.begin(), ::toupper);
				std::transform(toUpperSurName.begin(), toUpperSurName.end(), toUpperSurName.begin(), ::toupper);
				cout << toUpperName << " " << toUpperSurName << " " << vec[i].phone << " " << vec[i].city << endl;
			}
			return;
		}
		else {
			if (vec[mid].name.substr(0, size) < partial)
				low = mid + 1;

			else
				high = mid - 1;
		}
	}
	std::transform(partial.begin(), partial.end(), partial.begin(), ::toupper);
	cout << partial << " does NOT exist in the dataset" << endl;
}

void linearSearchPartial(vector<Query> vec, string partial) {
	int size = partial.size();
	string divide;
	bool flag = false;
	for (int i = 0; i < vec.size(); i++) {
		divide = vec[i].name.substr(0, size);
		if (divide == partial) {
			string toUpperName = vec[i].name;
			string toUpperSurName = vec[i].surname;
			std::transform(toUpperName.begin(), toUpperName.end(), toUpperName.begin(), ::toupper);
			std::transform(toUpperSurName.begin(), toUpperSurName.end(), toUpperSurName.begin(), ::toupper);
			cout << toUpperName << " " << toUpperSurName << " " << vec[i].phone << " " << vec[i].city << endl;
			flag = true;
			//continue for other matches
		}
		else if (divide != partial && flag) { //stop looking
			break;
		}
	}
	if (!flag) {
		std::transform(partial.begin(), partial.end(), partial.begin(), ::toupper);
		cout << partial << " does NOT exist in the dataset" << endl;
	}
}

void linearSearchFull(vector<Query> vec, string name, string surname) {
	bool finded = false;
	for (int i = 0; i < vec.size(); i++)
		if (vec[i].name == name && vec[i].surname == surname) {
			string toUpperName = vec[i].name;
			string toUpperSurName = vec[i].surname;
			std::transform(toUpperName.begin(), toUpperName.end(), toUpperName.begin(), ::toupper);
			std::transform(toUpperSurName.begin(), toUpperSurName.end(), toUpperSurName.begin(), ::toupper);
			cout << toUpperName << " " << toUpperSurName << " " << vec[i].phone << " " << vec[i].city << endl;
			finded = true;
			break;
		}
	if (!finded) {
		std::transform(name.begin(), name.end(), name.begin(), ::toupper);
		std::transform(surname.begin(), surname.end(), surname.begin(), ::toupper);
		cout << name << " " << surname << " does NOT exist in the dataset" << endl;
	}

}


void binarySearchFull(vector<Query> vec, string name, string surname) {
	bool finded = false;
	int low = 0;
	int high = vec.size() - 1;
	Query searched;
	searched.searching = true;
	searched.name = name;
	searched.surname = surname;

	while (low <= high) {
		int mid = low + (high - low) / 2;

		if (vec[mid] == searched) {
			string toUpperName = vec[mid].name;
			string toUpperSurName = vec[mid].surname;
			std::transform(toUpperName.begin(), toUpperName.end(), toUpperName.begin(), ::toupper);
			std::transform(toUpperSurName.begin(), toUpperSurName.end(), toUpperSurName.begin(), ::toupper);
			cout << toUpperName << " " << toUpperSurName << " " << vec[mid].phone << " " << vec[mid].city << endl;
			finded = true;
			break;
		}

		if (vec[mid] < searched)
			low = mid + 1;

		else
			high = mid - 1;
	}
	if (!finded) {
		std::transform(name.begin(), name.end(), name.begin(), ::toupper);
		std::transform(surname.begin(), surname.end(), surname.begin(), ::toupper);
		cout << name << " " << surname << " does NOT exist in the dataset" << endl;
	}
}

void measureLinearSearchPartial(vector<Query> vec, string partial) {
	linearSearchPartial(vec, partial);
}

void measureLinearSearchNameFull(vector<Query> vec, string name, string surname) {
	linearSearchFull(vec, name, surname);
}

void measureBinarySearchPartial(vector<Query> vec, string partial) {
	BinarySearchPartial(vec, partial);
}

void measureBinarySearchFull(vector<Query> vec, string name, string surname) {
	binarySearchFull(vec, name, surname);
}

int main() {

	ifstream data;
	//hold 4 vector for 4 different sorting algorithms:
	vector<Query> insertionSort_vec;
	vector<Query> heapSort_vec;
	vector<Query> mergeSort_vec;
	vector<Query> quickSort_vec; //known to be fastest

	openFile(data, insertionSort_vec, heapSort_vec, mergeSort_vec, quickSort_vec);

	string word;
	cout << "Please enter the word to be queried : " << endl;
	cin.ignore();
	getline(cin, word); //it can be name surname or just name
	cout << endl;
	cout << "Sorting the vector copies" << endl;
	cout << "======================================" << endl;


	//measure sorting algorithms timing, for better precision It is safe to call it N times
	int n = 10;
	/********************************************************/
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		quickSort_measure(quickSort_vec);
	}
	auto end = chrono::high_resolution_clock::now();
	double timer1 = (end - start).count() / n;
	cout << "Quick Sort Time: " << timer1 << " Nanoseconds" << endl;
	quicksort(quickSort_vec, 0, quickSort_vec.size() - 1);
	/********************************************************/

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		insertionSort_measure(insertionSort_vec);
	}
	end = chrono::high_resolution_clock::now();
	double timer2 = (end - start).count() / n;
	cout << "Insertion Sort Time: " << timer2 << " Nanoseconds" << endl;
	insertionSort(insertionSort_vec);
	/********************************************************/

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		mergeSort_measure(mergeSort_vec);
	}
	end = chrono::high_resolution_clock::now();
	double timer3 = (end - start).count() / n;
	cout << "Merge Sort Time: " << timer3 << " Nanoseconds" << endl;
	mergeSort(mergeSort_vec);
	/********************************************************/

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < n; i++) {
		heapSort_measure(heapSort_vec);
	}
	end = chrono::high_resolution_clock::now();
	double timer4 = (end - start).count() / n;
	cout << "Heap Sort Time: " << timer4 << " Nanoseconds" << endl;
	heapsort(heapSort_vec);
	/********************************************************/

	cout << endl;
	double timer5; //binary search timer
	double timer6; //sequential search timer
	cout << "Searching for " << word << endl;
	cout << "======================================" << endl;
	string blank = " ";
	std::size_t found = word.find(blank);


	if (found != std::string::npos) { //full search given name and surname
		string searched_name = word.substr(0, found);
		string searched_surname = word.substr(found + 1);

		cout << "Search results for Binary Search:" << endl;
		start = chrono::high_resolution_clock::now();
		measureBinarySearchFull(heapSort_vec, searched_name, searched_surname);
		end = chrono::high_resolution_clock::now();
		timer5 = (end - start).count() / n;

		cout << endl;
		cout << "Binary Search Time: " << timer5 << " Nanoseconds" << endl;
		cout << endl;
		/********************************************************/

		cout << "Search results for Sequential Search:" << endl;
		start = chrono::high_resolution_clock::now();
		measureLinearSearchNameFull(heapSort_vec, searched_name, searched_surname);
		end = chrono::high_resolution_clock::now();
		timer6 = (end - start).count() / n;
		cout << endl;
		cout << "Sequential Search Time: " << timer6 << " Nanoseconds" << endl;
		/********************************************************/
	}
	else { //partial search
		string searched = word;

		cout << "Search results for Binary Search:" << endl;
		start = chrono::high_resolution_clock::now();
		measureBinarySearchPartial(heapSort_vec, searched);
		end = chrono::high_resolution_clock::now();
		timer5 = (end - start).count() / n;
		cout << endl;
		cout << "Binary Search Time: " << timer5 << " Nanoseconds" << endl;
		cout << endl;
		/********************************************************/

		cout << "Search results for Sequential Search:" << endl;
		start = chrono::high_resolution_clock::now();
		measureLinearSearchPartial(heapSort_vec, searched);
		end = chrono::high_resolution_clock::now();
		timer6 = (end - start).count() / n;
		cout << endl;
		cout << "Sequential Search Time: " << timer6 << " Nanoseconds" << endl;
		/********************************************************/
	}
	//calculate speed ups for sorting algorithms and search algorithms (slow/fast)

	double proportion;
	cout << endl;
	cout << "SpeedUp between Search Algorithms" << endl;
	cout << "======================================" << endl;
	proportion = timer6 / timer5;
	cout << "(Sequential Search/ Binary Search) SpeedUp = " << proportion << endl; //timer6=sequential, timer5=binary

	//timer1=quick sort
	cout << endl;
	cout << "SpeedUps between Sorting Algorithms" << endl;
	cout << "======================================" << endl;

	proportion = timer2 / timer1;
	cout << "(Insertion Sort / Quick Sort) SpeedUp = " << proportion << endl;
	proportion = timer3 / timer1;
	cout << "(Merge Sort / Quick Sort) SpeedUp = " << proportion << endl;
	proportion = timer4 / timer1;
	cout << "(Heap Sort / Quick Sort) SpeedUp = " << proportion << endl;
	 
	
	data.close();
	return 0;
}