#pragma once
#include <stdexcept>

using namespace std;


template <class T>
class List
{
public:
	List();					// constructor
	~List();				// destructor
	void push_back(T);		// add object to the tail
	void push_front(T);		// add object to the head
	void pop_back();		// delete the last object
	void pop_front();		// delete the first object
	void insert(size_t, T); // add a object by index
	void remove(size_t);	// delete object by index
	void clear();			// delete all objects from the list
	void set(size_t, T);	// replacing a data in the object by index with the passed data
	void addSymbol(T&);
	void sorting();
	void changeCode(char symbol, size_t index);

	T at(size_t);			// get data from the object by index

	size_t get_size();		// get size of the list

	bool isEmpty();			// check for empty list

	ostream& operator<< (ostream& output_stream)// output operator
	{
		for (object * tmp = head; tmp != nullptr; tmp = tmp->next)
			cout << tmp->data << endl;
		return output_stream;
	}

	struct object
	{
		object(T data, object * next = nullptr, object * prev = nullptr) // constructor
		{
			this->data = data;
			this->next = next;
			this->prev = prev;
		};
		object * next; // next object
		object * prev; // previous object
		T        data; // data in the object
	};

private:
	object * head; // head of list
	object * tail; // tail of list
	size_t   size; // size of the list
	bool isExist(T);
	object * find(T symbol)
	{
		object * tmp = head;
		object * finder = nullptr;
		do
		{
			if (tmp->data == symbol)
				finder = tmp;
			tmp = tmp->next;
		}while(tmp != NULL);
		return finder;
	}
};

template<class T>
void List<T>::changeCode(char symbol, size_t index)
{
	if (index >= size || index < 0) // index out of range
		throw invalid_argument("Index out of range");
	else
	{
		size_t counter = 0;
		object * tmp = head;
		while (counter != index)
		{
			tmp = tmp->next; 
			counter++;
		}
		tmp->data.code.push_back(symbol);
	}
}

template<class T>
void List<T>::sorting()
{
	object* temp, *temp1, *temp2, *temp3 = nullptr;
	for (object* tmp = head; tmp != nullptr;)
	{
		temp = tmp;
		tmp = tmp->next;
		temp1 = temp3;
		for (temp2 = nullptr; (temp1 != nullptr) && (temp1->data.freq > temp->data.freq);) {
			temp2 = temp1;
			temp1 = temp1->next;
		}
		if (temp2 == nullptr) {
			temp->next = temp3;
			temp3 = temp;
		}
		else {
			temp->next = temp1;
			temp2->next = temp;
		}
	}
	if (temp3 != nullptr)
		head = temp3;
}

template<class T>
void List<T>::addSymbol(T& symbol)
{
	if (!isExist(symbol))
		push_back(symbol);
	else 
	{
		object * node = find(symbol);
		++node->data.freq;
	}
}

template<class T>
List<T>::List()
{
	head = nullptr;
	tail = nullptr;
	size = 0;
}

template<class T>
List<T>::~List()
{
	clear();
}

template <class T>
bool List<T>::isExist(T symbol)
{
	if (isEmpty()) return false;
	else
	{
		bool finder = false;
		object * tmp = head;
		do
		{
			if (tmp->data == symbol) finder = true;
			tmp = tmp->next;
		} while (tmp != NULL);
		return finder;
	}
}

template <class T>
void List<T>::push_back(T newData)
{
	if (isEmpty())
	{
		head = new object(newData); // create first object
		tail = head; // set tail
	}
	else
	{
		tail->next = new object(newData); // create object in the tail
		tail->next->prev = tail; // set prev
		tail = tail->next; // set tail
	}
	size++; // increase size
}

template <class T>
void List<T>::push_front(T newData)
{
	if (isEmpty()) // list empty
	{
		head = new object(newData); // create first object
		tail = head; // set tail
	}
	else
	{
		head = new object(newData, head); // add new object to head of the list, set head in old object
		head->next->prev = head;
	}

	size++; // increase size
}

template<class T>
void List<T>::pop_back()
{
	if (isEmpty()) // list empty
		throw logic_error("The list is empty");

	if (size == 1) // list has one object
	{
		delete head;
		head = nullptr;
		tail = nullptr;
	}
	else // list has more then one object
	{
		object * newtail = tail->prev;
		newtail->next = nullptr;
		delete tail;
		tail = newtail;
	}
	size--; // decrease size
}

template<class T>
void List<T>::pop_front()
{
	if (isEmpty()) // list empty
		throw logic_error("The list is empty");
	if (size == 1) // list has one object
	{
		delete head;
		head = nullptr;
		tail = nullptr;
	}
	else
	{
		object* tmp = head;
		head = tmp->next; // set the pointer of the head on the second object
		head->prev = nullptr; // set null of the first object
		delete tmp; // delete old header
	}
	size--; // decrease size
}

template<class T>
void List<T>::insert(size_t index, T newData)
{
	if (index > size || index < 0) // index out of range
		throw invalid_argument("Index out of range");
	else
	{
		size_t counter = 0; // counter
		object * previous;  // pointer to the previous object to insert

		if (index == 0) // index is zero
		{
			previous = new object(newData, head); // add new object to head of the list, set head in old object
			head->prev = previous;
			head = previous;
		}
		else
		{
			previous = head;
			while (counter != index - 1) // go to the object before required object
			{
				previous = previous->next; // moving
				counter++;
			}
			previous->next = new object(newData, previous->next, previous); // add new object and link to it
			if (index == size) // index is equal size
				tail = previous->next; // set tail to this object
		}
		size++; // increase size
	}
}

template<class T>
void List<T>::remove(size_t index)
{
	if (index >= size || index < 0) // index out of range
		throw invalid_argument("Index out of range");
	else
	{
		size_t counter = 0; // counter
		object * tmp = head;
		if (index == 0) // index is zero
		{
			head = head->next; // move the pointer of the head
			delete tmp; // delete head object
			head->prev = nullptr;
		}
		else
		{
			while (counter != index) // go to the required object 
			{
				tmp = tmp->next; // moving
				counter++;
			}
			tmp->prev->next = tmp->next; // set prev object to next
			if (size == index + 1) // index less of 1 then size
				tail = tmp->prev; // set tail to prev object
			else
				tmp->next->prev = tmp->prev;

			delete tmp; // delete object

		}
		size--; // decrease size
	}
}


template<class T>
size_t List<T>::get_size()
{
	return size;
}
template <class T>
void List<T>::clear()
{
	object * tmp; // pointer to the next object

	while (head != nullptr) // while list not empty
	{
		tmp = head->next; // movement
		delete head; // delete object
		head = tmp; // set new head of the list
	}
	tail = nullptr; // set null pointer tail of the list
	size = 0; // set zero size of the list
}

template<class T>
bool List<T>::isEmpty()
{
	return  (head == nullptr && tail == nullptr && size == 0);
}

template<class T>
T List<T>::at(size_t index)
{
	if (index >= size || index < 0) // index out of range
		throw invalid_argument("Index out of range");
	else
	{
		size_t counter = 0;
		object * tmp = head;
		while (counter != index) // go to the required object 
		{
			tmp = tmp->next; // moving
			counter++;
		}
		return tmp->data; // output	
	}
}

template<class T>
void List<T>::set(size_t index, T newData) {
	if (index >= size || index < 0) // index out of range
		throw invalid_argument("Index out of range");
	else
	{
		size_t counter = 0;
		object * tmp = head;
		while (counter != index) // go to the required object 
		{
			tmp = tmp->next; // moving
			counter++;
		}
		tmp->data = newData; // put data to object
	}
}