#include "HashTable.h"

/*Private Member Functions*/
template <class HashedObj>
int HashTable<HashedObj>::hashFunc(const HashedObj& s) const //hash function (utilizes horner's method to prevent overflow on large strings)
{
	int hashVal = 0, asc;
	for (int i = 0; i < s.size(); i++)
	{
		asc = s[i] > 96 ? s[i] - 96 : s[i] - 64;
		hashVal = (hashVal * 32 + asc) % arrSize;
	}
	return hashVal; //return the index
}

template <class HashedObj>
int HashTable<HashedObj>::getPrime(int n) const //return the smallest next prime number >= 2*n
{
	int i = 2 * n;
	while (!isPrime(i))
		i++;
	return i;
}

template <class HashedObj>
bool HashTable<HashedObj>::isPrime(int n) const //check whether n is prime, helper function for getPrime()
{
	bool isPrime = true;
	for (int count = 2; count < n && isPrime; count++)
		if (n % count == 0)
			isPrime = false;
	return isPrime;
}

template <class HashedObj>
void HashTable<HashedObj>::deepCopy(const HashTable<HashedObj>& h)
{
	if (h.arr != NULL)
	{
		numOfItems = h.size(); //current num of items
		arrSize = h.maxSize(); //get array size
		arr = new LinkedList<HashedObj>[arrSize];
		for (int i = 0; i < arrSize; i++)
			arr[i] = h.arr[i]; //linked list = operator
	}
}


/*Public Member Functions*/
template <class HashedObj>
HashTable<HashedObj>::HashTable() //default constructor
{
	arrSize = 101; //by default
	arr = new LinkedList<HashedObj>[arrSize];
	numOfItems = 0;
}

template <class HashedObj>
HashTable<HashedObj>::HashTable(int n) //creates a hash table to store n items where the size of the array is the smallest next prime number >= 2*n
{
	arrSize = getPrime(n); //array size n is the smallest next prime that is > 2*n
	arr = new LinkedList<HashedObj>[arrSize];
	numOfItems = 0;
}

template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashTable<HashedObj>& h) //copy constructor
{
	deepCopy(h);
}

template <class HashedObj>
HashTable<HashedObj>::~HashTable() //destructor
{
	delete[] arr;
}

template <class HashedObj>
HashTable<HashedObj>& HashTable<HashedObj>::operator=(const HashTable<HashedObj>& h) //assignment operator
{
	if (this != &h) //if they are not same
	{
		if (h.arr != NULL) //delete if there are items
			delete[] arr;
		deepCopy(h);
	}
	return *this;
}

template <class HashedObj>
bool HashTable<HashedObj>::insert(const HashedObj& s,int &hashedVal) //inserts obj s if it doesn't exist in the hash table and returns 1 if insertion successful, 0 otherwise
{	
	//int hash = hashFunc(s); //get hashed int
	int hash = hashedVal;
	bool successOrFail = arr[hash].insert(s); //true if element does not exists in linked list, else false
	numOfItems++;
	return successOrFail;
}

template <class HashedObj>
bool HashTable<HashedObj>::remove(const HashedObj& s,int &hashedVal) //removes obj  s if s exist in the hash table and returns 1 if removal successful, 0 otherwise
{
	//int hash = hashFunc(s); //get hashed int
	int hash = hashedVal;
	bool successOrFail = arr[hash].remove(s); //true if element exists
	numOfItems--;
	return successOrFail;
}

template <class HashedObj>
bool HashTable<HashedObj>::search(const HashedObj& s,int &hashedVal) const //returns 1 if s exist in the hash table, 0 otherwise
{
	//int hash = hashFunc(s); //get hashed int
	int hash = hashedVal;
	bool found = arr[hash].search(s); //true if element exists in linked list
	return found;
}

template<class HashedObj>
HashedObj HashTable<HashedObj>::find(HashedObj& searched, int& hashedVal)
{
	if (search(searched, hashedVal)) {
		HashedObj retrieve;
		int hash = hashedVal;
		retrieve = arr[hash].find(searched);
		return retrieve;
	}	
}

template <class HashedObj>
int HashTable<HashedObj>::size() const //returns current numOfItems
{
	return numOfItems;
}


template <class HashedObj>
int HashTable<HashedObj>::maxSize() const //returns arrSize
{
	return arrSize;
}

template <class HashedObj>
double HashTable<HashedObj>::loadFactor() const //returns the load factor of the hash table
{
	return (numOfItems * 1.0) / arrSize; //number of items / total size
}





