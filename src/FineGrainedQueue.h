#pragma once
#include <iostream>
#include <string>
#include <mutex>

//мьютекс lock_cout для блокировки объекта вывода cout,
std::mutex lock_cout;
//потокобезопасный вывод в std::cout.
void locked_cout(const std::string& str) {
    // захват мьютекса для защищенного вывода
    std::lock_guard<std::mutex> raii(lock_cout);
    std::cout << str << std::endl;
}

struct Node {
    //спасибо Александру Максакову, нашел решения проблемы с mutex
    //thanks to Alexander Maksakov, found a solution to the problem with mutex
    Node(int data) : value(data), next(nullptr), node_mutex(new std::mutex) {}
    ~Node() {        
        if (node_mutex != nullptr) delete node_mutex;
    }

    int value;
    Node* next;
    std::mutex* node_mutex;
};

//Детализированная очередь
class FineGrainedQueue {
    Node* head;
    std::mutex* queue_mutex;

public:
    //спасибо Александру Максакову, нашел решения проблемы с mutex
    //thanks to Alexander Maksakov, found a solution to the problem with mutex
    FineGrainedQueue() : head(nullptr), queue_mutex(new std::mutex) {}
    ~FineGrainedQueue() { 
        if(queue_mutex) delete queue_mutex;        
    }

    //Функция должна вставить узел с переданным значением value в позицию pos.
    void insertIntoMiddle(int value, int pos) {
        // создаем новый узел
        Node* node = new Node(value);

        queue_mutex->lock();

        if (head == nullptr) {
            locked_cout("The list is empty, insert into position 1\n");
            head = node;
            queue_mutex->unlock();
            return;
        }
        //вставка  в начало списка
        if (pos == 0) {
            node->next = head;
            head = node;
            queue_mutex->unlock();
            return;
        }
        //переменная для подсчета позиции
        int currentPos = 0;
        //текущая пощиция первая
        Node* current = head;
        
        // в цикле идем по списку, пока список не кончится, или пока не дойдем до позиции
        while (currentPos < (pos - 1) && current->next != nullptr) {
            current = current->next;
            currentPos++;
        }

        current->node_mutex->lock();

        queue_mutex->unlock();

      
        if (currentPos < pos - 1)
            locked_cout("position not found inserted into position: " + std::to_string(currentPos) + "\n");

        // меняем указатель на следующий узел на указатель на новый узел
        Node* nextNode = current->next;       

        current->next = node;
        // связываем список обратно, меняем указатель на узел, следующий после нового узла, 
        // на указатель на узел, следующий за current
        node->next = nextNode;

        current->node_mutex->unlock();       
    }

    void printList() {

        Node* newN = head;

        while (newN != nullptr) {
            std::cout << newN->value << " ";
            newN = newN->next;
        }
    }

    void remove(int value)
    {
        Node* prev, * cur;
        queue_mutex->lock();

        // здесь должна быть обработка случая пустого списка
        if (head == nullptr) {           
            locked_cout("The list is empty!\n");
            queue_mutex->unlock();
            return;
        }

        prev = this->head;
        cur = this->head->next;

        prev->node_mutex->lock();
        // здесь должна быть обработка случая удаления первого элемента
        queue_mutex->unlock();

        if (cur) // проверили и только потом залочили
            cur->node_mutex->lock();
        while (cur) {
            if (cur->value == value) {                
                prev->next = cur->next;
                prev->node_mutex->unlock();
                cur->node_mutex->unlock();
                delete cur;                
                return;
            }
            Node* old_prev = prev;
            prev = cur;
            cur = cur->next;
           old_prev->node_mutex->unlock();
            if (cur) // проверили и только потом залочили
                cur->node_mutex->lock();
        }
        prev->node_mutex->unlock();
    }

};