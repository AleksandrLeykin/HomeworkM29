//#include <iostream>
#include "FineGrainedQueue.h"

int main() {
	FineGrainedQueue myList;
	myList.insertIntoMiddle(12, 0);
	myList.insertIntoMiddle(2, 1); 
	myList.insertIntoMiddle(1, 2);
	myList.insertIntoMiddle(7, 3);
	myList.insertIntoMiddle(11, 4);

	myList.printList();
	std::cout << std::endl;

	myList.insertIntoMiddle(77, 2);
	myList.printList();

	myList.remove(2);
	myList.printList();
	
	return 0;
}