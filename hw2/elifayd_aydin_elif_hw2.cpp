//written by Elif Aydýn 28061

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "BinaryTree.h"
#include "BinaryTree.cpp"
#include "AvlTree.h"
#include "AvlTree.cpp"
#include <chrono>

using namespace std;

struct item { //each item has a title string and an information string
	string itemName;
	string itemInfo;

	bool operator<(const item& rhs) const
	{
		if (itemName < rhs.itemName)
		{
			return true;
		}
		return false;
	}

	bool operator>(const item& rhs) const
	{
		if (itemName > rhs.itemName)
		{
			return true;
		}
		return false;
	}

	bool operator==(const item& rhs) const
	{
		if (itemName == rhs.itemName)
		{
			return true;
		}
		return false;
	}

	bool operator=(const item& rhs)  {
		itemName = rhs.itemName;
		itemInfo = rhs.itemInfo;
		return this;
	}
	
};

struct section { //each section has got a title string and a tree of items
	string title;
	BinaryTree<item> itemTree;

	bool operator<(const section& rhs) const
	{
		if (title < rhs.title)
		{
			return true;
		}
		return false;
	}

	bool operator>(const section& rhs) const
	{
		if (title > rhs.title)
		{
			return true;
		}
		return false;
	}

	bool operator==(const section& rhs) const
	{
		if (title == rhs.title)
		{
			return true;
		}
		return false;
	}
};

struct section2 { //each section has got a title string and a tree of items
	string title;
	AvlTree<item> itemTree;

	bool operator<(const section2& rhs) const
	{
		if (title < rhs.title)
		{
			return true;
		}
		return false;
	}

	bool operator>(const section2& rhs) const
	{
		if (title > rhs.title)
		{
			return true;
		}
		return false;
	}

	bool operator==(const section2& rhs) const
	{
		if (title == rhs.title)
		{
			return true;
		}
		return false;
	}
};


ostream& operator << (ostream& o, const item& rhs)
	{
	o << rhs.itemName;
	return o;
}

ostream& operator << (ostream& o, const section& rhs)
{
	o << rhs.title;
	return o;
}

ostream& operator << (ostream& o, const section2& rhs)
{
	o << rhs.title;
	return o;
}

void openFile(ifstream& inFile) { //this function will force to open a file, if it does not exists, it will request name again
	string fName = "data.txt";
	inFile.open(fName.c_str());
	while (inFile.fail()) {
		cout << "File failed to open!" << endl;
		cout << "Enter the file name, typo? " << endl;
		cin >> fName;
		inFile.open(fName.c_str());

	}
}

void readFile(ifstream& inFile,BinaryTree<section> &notebook,AvlTree<section2> & notebook2) {
	int counter = 0;
	string line;
	string sectionTitle, itemTitle, itemInfo;
	section currentSection;
	section2 currentSection2;
	int index;
	BinaryTree<item> items;
	AvlTree<item> items2;

	auto start1 = chrono::high_resolution_clock::now(); //bst
	auto end1 = chrono::high_resolution_clock::now();

	auto start2 = chrono::high_resolution_clock::now(); //avl
	auto end2 = chrono::high_resolution_clock::now();

	long long timerAvl = 0.0;
	long long timerBst = 0.0;
	
	while (getline(inFile, line)) {
		item newItem;
		section newSection; //for binaryTree
		section2 newSection2; //for avlTree
		
		if (line != "") {
			if (line[0] != '-') { //that line is a section

				if (counter == 0) {//first section, we dont have items yet
					istringstream iss(line);
					iss >> sectionTitle;
					newSection.title = sectionTitle;
					newSection2.title = sectionTitle;

					currentSection2 = newSection2; //avl
					start2 = chrono::high_resolution_clock::now();
					currentSection = newSection; //bst
					start1 = chrono::high_resolution_clock::now();
				
					counter++;
				}
				else { //we have items stored in the BinaryTree<items>, now get them
					
					currentSection.itemTree = items;
					currentSection2.itemTree = items2;
					items.makeEmpty(); //make empty for other sections
					items2.makeEmpty(); //make empty for other sections

					//now insert whole section to notebook
					notebook2.insert(currentSection2);
					
					end2 = chrono::high_resolution_clock::now(); //end avl
					long long timer = (end2 - start2).count() / 1000.0;
					timer += timerAvl;
					timerAvl = 0.0;
					cout << "Section \"" << currentSection2.title << "\" has been inserted into the AVL notebook." << endl;
					cout << "[AVL] Elapsed time: "<< timer<<" microseconds" << endl;

					
					notebook.insert(currentSection); 
					
					end1 = chrono::high_resolution_clock::now(); //end bst
					timer = (end1 - start1).count() / 1000.0;
					timer += timerBst;
					timerBst = 0.0;
					cout << "Section \"" << currentSection.title << "\" has been inserted into the BST notebook." << endl;
					cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;			
					cout << endl;					
				}
				//now get new section
				istringstream iss(line);
				iss >> sectionTitle;
				newSection.title = sectionTitle;
				newSection2.title = sectionTitle;

				currentSection2 = newSection2;
				start2 = chrono::high_resolution_clock::now(); //avl start
				currentSection = newSection;
				start1 = chrono::high_resolution_clock::now(); //bst start
			}
			else { //that line is a item
				index = line.rfind("-");
				itemTitle = line.substr(1, index - 1);
				itemInfo = line.substr(index + 1);
				//insert item to the BST for that section
				item newItem;
				newItem.itemName = itemTitle;
				newItem.itemInfo = itemInfo;

				auto startIt2 = chrono::high_resolution_clock::now(); //avl istem start
				items2.insert(newItem);
				auto endIt2 = chrono::high_resolution_clock::now(); //avl item end
				timerAvl += (endIt2 - startIt2).count() / 1000.0;

				auto startIt = chrono::high_resolution_clock::now(); //bst item start
				items.insert(newItem);	
				auto endIt = chrono::high_resolution_clock::now(); //bst item end
				timerBst+= (endIt - startIt).count() / 1000.0;			
			}
		}		
	} //now handle last section which we did not include in our while loop

	currentSection.itemTree = items;
	currentSection2.itemTree = items2;

	//now insert whole section to notebook
	notebook2.insert(currentSection2);
	end2 = chrono::high_resolution_clock::now(); //end avl
	long long timer = (end2 - start2).count() / 1000.0;
	timer += timerAvl;
	timerAvl = 0.0;
	cout << "Section \""<<currentSection2.title <<"\" has been inserted into the AVL notebook." << endl;
	cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;
	
	notebook.insert(currentSection);
	end1 = chrono::high_resolution_clock::now(); //end bst
	timer = (end1 - start1).count() / 1000.0;
	timer += timerBst;
	timerBst = 0.0;
	cout << "Section \"" << currentSection.title << "\" has been inserted into the BST notebook." << endl;
	cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;	
}

void displayMenu() {
	cout << " " << endl;
	cout << "MENU" << endl;
	cout << "Please enter an input between [1 - 6]:" << endl;
	cout << "1- Display the sections [AVL]" << endl;
	cout << "2- Display the sections [BST]" << endl;
	cout << "3- Select a section" << endl;
	cout << "4- Add new section" << endl;
	cout << "5- Delete a section" << endl;
	cout << "6- Exit" << endl;
}

void displayBST(BinaryTree<section>& notebook) {
	cout << endl;
	cout << "*****" << endl;
	notebook.printTree();
	cout << "*****" << endl;
}
void displayAVL(AvlTree<section2>& notebook2) {
	cout << endl;
	cout << "*****" << endl;
	notebook2.printTree();
	cout << "*****" << endl;
}

void displayforSection3() {
	cout << "Please enter an input between [1 - 7]:" << endl;
	cout << "1- Display the items [AVL]" << endl;
	cout << "2- Display the items[BST]" << endl;
	cout << "3- Display the information of a item" << endl;
	cout << "4- Add new item" << endl;
	cout << "5- Update the information of a item" << endl;
	cout << "6- Delete an item" << endl;
	cout << "7- Return to main menu" << endl;
}

bool validChoice(string &choice) {
	if (choice != "1" && choice != "2" && choice != "3" && choice != "4" && choice != "5" && choice != "6") {
		return false;
	}
	return true;
}

bool validChoiceMenu3(string& choice) {
	if (choice != "1" && choice != "2" && choice != "3" && choice != "4" && choice != "5" && choice != "6" && choice!="7") {
		return false;
	}
	return true;
}


void menu3Operations(BinaryTree<section>& notebook,AvlTree<section2>&notebook2,section &currentsection, section2& currentsection2,string choice){
	while (validChoiceMenu3(choice)) {
		if (choice == "7") { //exit to sections menu
			break;
		}
		else {
			if (choice == "1") { //display avl
				cout << endl;
				cout << "*****" << endl;
				notebook2.find(currentsection2).itemTree.printTree();		
				cout << "*****" << endl;
			}
			else if (choice == "2") { //display bst
				cout << endl;
				cout << "*****" << endl;
				notebook.find(currentsection).itemTree.printTree();
				cout << "*****" << endl;				
			}
			else if (choice == "3") { //display info of a item 
				string title;
				cout << "Enter the title of the item:";
				cin.ignore();
				getline(cin, title);
				item searchedItem;
				searchedItem.itemName = title;

				bool finded;
				auto start = chrono::high_resolution_clock::now();
				finded = notebook2.find(currentsection2).itemTree.doesExists(searchedItem);
				auto end = chrono::high_resolution_clock::now();
				long long timer = (end - start).count() / 1000.0;
				cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;
				
				start = chrono::high_resolution_clock::now();
				finded=notebook.find(currentsection).itemTree.doesExists(searchedItem);
				end = chrono::high_resolution_clock::now();
				timer = (end - start).count() / 1000.0;
				cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;

				if (!finded) { //item does not exists in the section
					cout << "Invalid title." << endl;
				}
				else { //item exists, display info			
					searchedItem = notebook2.find(currentsection2).itemTree.find(searchedItem);
					string info = searchedItem.itemInfo;				
					info = searchedItem.itemInfo;
					cout << info << endl;
				}
			}
			else if (choice == "4") { //add new item
				string addItem;
				cout << "Enter a title for the item: ";
				cin.ignore();
				getline(cin, addItem);
				item searchedItem;
				searchedItem.itemName = addItem;
				
				bool finded = notebook2.find(currentsection2).itemTree.doesExists(searchedItem);

				if (finded){ //item already exists in the section

					auto start = chrono::high_resolution_clock::now();
					notebook2.find(currentsection2).itemTree.doesExists(searchedItem);
					auto end = chrono::high_resolution_clock::now();
					long long timer = (end - start).count() / 1000.0;
					cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;

					start = chrono::high_resolution_clock::now();
					notebook.find(currentsection).itemTree.doesExists(searchedItem);
					end = chrono::high_resolution_clock::now();
					timer = (end - start).count() / 1000.0;
					cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;

					cout << "Item \""<<addItem<<"\" already exists in the \""<<currentsection.title <<"\"."<< endl;
				}
				else { //add item to that section
					string description;
					cout << "Enter a description for the item: ";
					//cin.ignore();
					getline(cin, description);
					searchedItem.itemInfo = description;

					auto start = chrono::high_resolution_clock::now();
					notebook2.find(currentsection2).itemTree.insert(searchedItem);
					auto end = chrono::high_resolution_clock::now();
					long long timer = (end - start).count() / 1000.0;
					cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;

					start = chrono::high_resolution_clock::now();
					notebook.find(currentsection).itemTree.insert(searchedItem);
					end = chrono::high_resolution_clock::now();
					timer = (end - start).count() / 1000.0;
					cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;

					cout << "The new item \""<<addItem<<"\" has been inserted." << endl;
					
				}
			}
			else if (choice == "5") { //update info of an item
				string addItem;
				cout <<"Enter the title of the item: ";
				cin.ignore();
				getline(cin, addItem);
				item searchedItem;
				searchedItem.itemName = addItem;
				bool finded;				
				finded = notebook2.find(currentsection2).itemTree.doesExists(searchedItem);

				if (!finded) { //item does not exists

					auto start = chrono::high_resolution_clock::now();
					notebook2.find(currentsection2).itemTree.doesExists(searchedItem);
					auto end = chrono::high_resolution_clock::now();
					long long timer = (end - start).count() / 1000.0;
					cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;

					start = chrono::high_resolution_clock::now();
					notebook.find(currentsection).itemTree.doesExists(searchedItem);
					end = chrono::high_resolution_clock::now();
					timer = (end - start).count() / 1000.0;
					cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;

					cout << "Item \"" << addItem << "\" does not exist in the \"" << currentsection.title << "." << endl;
				}
				else { //update it
					string info;
					cout<<"Enter the new information: ";
					//cin.ignore();
					getline(cin, info);

					auto start = chrono::high_resolution_clock::now();
					notebook2.find(currentsection2).itemTree.find(searchedItem).itemInfo = info;
					auto end = chrono::high_resolution_clock::now();
					long long timer = (end - start).count() / 1000.0;
					cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;

					start = chrono::high_resolution_clock::now();
					notebook.find(currentsection).itemTree.find(searchedItem).itemInfo = info;
					end = chrono::high_resolution_clock::now();
					timer = (end - start).count() / 1000.0;
					cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;
			

					cout << "The content "<<addItem<<" has updated." << endl;
				}
			}
			else if (choice == "6") { //delete item
				string deleteItem;
				cout << "Enter the title of the item: ";
				cin.ignore();
				getline(cin, deleteItem);
				item searchedItem;
				item searchedItem2;
				searchedItem.itemName = deleteItem;
				searchedItem2.itemName = deleteItem;
				bool finded;
				finded = notebook2.find(currentsection2).itemTree.doesExists(searchedItem);				

				if (!finded) { //item does not exists

					auto start = chrono::high_resolution_clock::now();
					notebook2.find(currentsection2).itemTree.doesExists(searchedItem);
					auto end = chrono::high_resolution_clock::now();
					long long timer = (end - start).count() / 1000.0;
					cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;

					start = chrono::high_resolution_clock::now();
					notebook.find(currentsection).itemTree.doesExists(searchedItem);
					end = chrono::high_resolution_clock::now();
					timer = (end - start).count() / 1000.0;
					cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;

					cout << "Item \"" << deleteItem << "\" does not exist in the \"" << currentsection.title << "." << endl;
				}
				else { //remove
					auto start = chrono::high_resolution_clock::now();
					notebook2.find(currentsection2).itemTree.remove(searchedItem2);
					auto end = chrono::high_resolution_clock::now();
					long long timer = (end - start).count() / 1000.0;
					cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;

					start = chrono::high_resolution_clock::now();
					notebook.find(currentsection).itemTree.remove(searchedItem);
					end = chrono::high_resolution_clock::now();
					timer = (end - start).count() / 1000.0;
					cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;
														
					cout << "The item \""<<deleteItem<<"\" has been deleted." << endl;
				}
			}
		}	
		cout << endl;
		cout << "Input: ";
		cin >> choice;
		while (!validChoiceMenu3(choice)) {
			cout << "Input ";
			cin >> choice;
		}
	}
}


int main() {

	ifstream data;
	openFile(data); //open the file	
	BinaryTree<section> notebook;
	AvlTree<section2> notebook2;
	cout << "Welcome to the the Notes" << endl;
	cout << endl;
	readFile(data,notebook,notebook2);
		
	string choice;
	displayMenu();
	cout << "Input:";
	cin >> choice;
	while (!validChoice(choice)) { //force to get valid input
		cout << "Input:";
		cin >> choice;
	}
	
	while (validChoice(choice)) { //choice between 1-6
		if (choice == "6") {
			break;
		}
		else {
			if (choice == "1") {
				displayAVL(notebook2);
			}
			else if (choice == "2") {
				displayBST(notebook);
			}
			else if (choice == "3") {
				string title;
				cout << "Enter the title of the section: ";
				cin.ignore();
				getline(cin, title);
				section searched;
				section2 searched2;
				searched.title = title;
				searched2.title = title;
				if (!notebook2.doesExists(searched2)) { //section does not exists
					cout << "Invalid title!" << endl;

				}
				else {
					//direct to sections menu
					string sectionsChoice;
					cout << endl;
					cout << "Selected section -> " << searched.title << endl;
					displayforSection3();
					cout << "Input ";
					cin >> sectionsChoice;
					while (!validChoiceMenu3(sectionsChoice)) {
						cout << "Input ";
						cin >> sectionsChoice;
					}
					menu3Operations(notebook,notebook2,searched,searched2, sectionsChoice);
					displayMenu();
					
				}
			}
			else if (choice == "4") { //insert a section
				string title;
				cout << "Enter a title for the section: ";
				cin.ignore();
				getline(cin, title);
				section searched;
				section2 searched2;
				searched.title = title;
				searched2.title = title;
				if (notebook.doesExists(searched)) { //section is already there
					cout << "Section \"" << title << "\" already exists." << endl;
					
						
				}
				else { //insert the section
					notebook.insert(searched);
					notebook2.insert(searched2);
					cout << "The new section \"" << title <<"\" has been inserted." << endl;
				}
			}
			else if (choice == "5") { //delete a section
				string title;
				cout << "Enter the title of the section: ";
				cin.ignore();
				getline(cin, title);
				section searched;
				section2 searched2;
				searched.title = title;
				searched2.title = title;
				if (!notebook.doesExists(searched)) { //section does not exists
					cout << "Section \"" << title << "\" does not exist." << endl;
				}
				else { //delete that section
					notebook.remove(searched);
					notebook2.remove(searched2);
					cout << "The section has been deleted." << endl;
				}
			}
		}
		cout << endl;
		cout << "Input:";
		cin >> choice;
		while (!validChoice(choice)) { //force to get valid input
			cout << "Input:";
			cin >> choice;
		}
	}
	
	data.close(); //close the file
	return 0;
}