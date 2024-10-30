//#include <iostream>
#include "FineGrainedQueue.h"

FineGrainedQueue myList;

void add(int value, int pos) {
	myList.insertIntoMiddle(value, pos);
}

int main() {

	std::thread t1(add, 12, 0);
	std::thread t2(add, 2, 1);
	std::thread t3(add, 1, 2);	

	t1.join();
	/*myList.printList();
	std::cout << std::endl;*/
	t2.join();
	/*myList.printList();
	std::cout << std::endl;*/
	t3.join();

	myList.insertIntoMiddle(7, 3);
	myList.insertIntoMiddle(11, 4);

	myList.printList();
	std::cout << std::endl;

	myList.insertIntoMiddle(77, 2);
	myList.printList();
	std::cout << std::endl;

	std::thread t4([]() { myList.remove(2); });

	myList.printList();
	std::cout << std::endl;

	std::thread t5([]() { myList.remove(9); });
	
	t4.join();
	t5.join();

	myList.remove(11);
	myList.printList();
	
	return 0;
}