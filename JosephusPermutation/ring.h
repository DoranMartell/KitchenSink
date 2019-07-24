#ifndef RINGED_LIST_H
#define RINGED_LIST_H

#include <iostream>

using namespace std;


/* Ring template is a sort of linked list with one special feature:
	its last element's nextPtr points to the head of the list (the first added element) */
template<typename T>
class Ring
{

private:

	struct Node
	{
		T value;
		Node * nextPtr;
		Node * prevPtr;

		Node() = delete;

		/* the only to move non-copyable value inside of Node is in initializers list,
			it is impossible to do from constructor's body */  
		Node(T && element_of_noncopyable_type) : value(move(element_of_noncopyable_type))
		{
		}
	};

	Node * headPtr;
	Node * currPtr; /* remembering where we stopped will come in handy to solve Josephus permutation problem */

	size_t number_of_nodes;

public:

	/* constructs empty ringed list with both headPtr and currPtr pointing to nowhere */
	Ring();

	/* as always, frees all the memory we allocated from heap for all of ring's nodes,
	   actually, calls method clear(), which, in turn, sequentially calls PopCurrPtr() starting from headPtr */ 
	~Ring();

	/* return reference to the element, which is pointed by currPtr */
	T & ValueOfCurrentlyPointedElement() const;

	/* counts number of steps which we need to make from head to reach node, pointed by currPtr */
	size_t PositionOfCurrentlyPointedElement() const;

	/* sets currPtr pointer to the n-th element starting from head */
	Node * MoveFromHeadToPosition(size_t n);

	/* shifts currPtr pointer to the element, which is located in n steps from currPtr */
	Node * ShiftCurrPtr(size_t n);

	/* besides of adding a new node after curr position,
		moves currPtr to point to the created node (and returns currPtr) */
	Node * AddNodeAfterCurrPosition(T && val); // the argument is an r-value reference, meaning, the value will be moved

	/* takes value of node pointed by currPtr and removes the node from the list */
	T PopCurrPtr();

	/* print all elements of the ringed list (starting from head or end, depending on direction of printing) */
	void PrintTheRingForward() const;
	void PrintTheRingBackward() const;

	/* typical 'STL containers'-like functions */
	void clear();
	bool empty() const;
	size_t size() const;

};

template<typename RandomIt>
Ring<typename RandomIt::value_type> MakeARingFromRange(RandomIt beg, RandomIt end)
{
	Ring<typename RandomIt::value_type> ring;

	for (RandomIt it = beg; it != end; it++)
	{
		cout << "MOVE element from the range (some sequential container with random access) into the ring: " << (*it) << endl;
		ring.AddNodeAfterCurrPosition(move(*it));
	}

	cout << "Lets see what we got:" << endl;
	ring.PrintTheRingForward();

	return ring;
}

/* Shamelessly 'borrowed' from Yandex "Red Belt C++" course, Josephus Permutation problem */
struct NoncopyableInt
{
  int value;

  NoncopyableInt(const NoncopyableInt&) = delete;
  NoncopyableInt& operator=(const NoncopyableInt&) = delete;

  NoncopyableInt(NoncopyableInt&&) = default;
  NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator == (const NoncopyableInt & lhs, const NoncopyableInt & rhs);
ostream & operator << (ostream & os, const NoncopyableInt & v);

#endif