#pragma once
#include <iostream>
#include <string>
#include <mutex>

//������� lock_cout ��� ���������� ������� ������ cout,
std::mutex lock_cout;
//���������������� ����� � std::cout.
void locked_cout(const std::string& str) {
    // ������ �������� ��� ����������� ������
    std::lock_guard<std::mutex> raii(lock_cout);
    std::cout << str << std::endl;
}

struct Node {
    //������� ���������� ���������, ����� ������� �������� � mutex
    //thanks to Alexander Maksakov, found a solution to the problem with mutex
    Node(int data) : value(data), next(nullptr), node_mutex(new std::mutex) {}
    ~Node() {        
        if (node_mutex != nullptr) delete node_mutex;
    }

    int value;
    Node* next;
    std::mutex* node_mutex;
};

//���������������� �������
class FineGrainedQueue {
    Node* head;
    std::mutex* queue_mutex;

public:
    //������� ���������� ���������, ����� ������� �������� � mutex
    //thanks to Alexander Maksakov, found a solution to the problem with mutex
    FineGrainedQueue() : head(nullptr), queue_mutex(new std::mutex) {}
    ~FineGrainedQueue() { 
        if(queue_mutex) delete queue_mutex;        
    }

    //������� ������ �������� ���� � ���������� ��������� value � ������� pos.
    void insertIntoMiddle(int value, int pos) {
        // ������� ����� ����
        Node* node = new Node(value);

        queue_mutex->lock();

        if (head == nullptr) {
            locked_cout("The list is empty, insert into position 1\n");
            head = node;
            queue_mutex->unlock();
            return;
        }
        //�������  � ������ ������
        if (pos == 0) {
            node->next = head;
            head = node;
            queue_mutex->unlock();
            return;
        }
        //���������� ��� �������� �������
        int currentPos = 0;
        //������� ������� ������
        Node* current = head;
        
        // � ����� ���� �� ������, ���� ������ �� ��������, ��� ���� �� ������ �� �������
        while (currentPos < (pos - 1) && current->next != nullptr) {
            current = current->next;
            currentPos++;
        }

        current->node_mutex->lock();

        queue_mutex->unlock();

      
        if (currentPos < pos - 1)
            locked_cout("position not found inserted into position: " + std::to_string(currentPos) + "\n");

        // ������ ��������� �� ��������� ���� �� ��������� �� ����� ����
        Node* nextNode = current->next;       

        current->next = node;
        // ��������� ������ �������, ������ ��������� �� ����, ��������� ����� ������ ����, 
        // �� ��������� �� ����, ��������� �� current
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

        // ����� ������ ���� ��������� ������ ������� ������
        if (head == nullptr) {           
            locked_cout("The list is empty!\n");
            queue_mutex->unlock();
            return;
        }

        prev = this->head;
        cur = this->head->next;

        prev->node_mutex->lock();
        // ����� ������ ���� ��������� ������ �������� ������� ��������
        queue_mutex->unlock();

        if (cur) // ��������� � ������ ����� ��������
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
            if (cur) // ��������� � ������ ����� ��������
                cur->node_mutex->lock();
        }
        prev->node_mutex->unlock();
    }

};