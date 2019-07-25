#include "ring.h"

#include <stdexcept>

template<typename T>
Ring<T>::Ring()
{
	headPtr = nullptr;
	currPtr = nullptr;
	number_of_nodes = 0;
}

template<typename T>
Ring<T>::~Ring()
{
	cout << "Ring's destructor was called: going to free memory taken by ring's nodes..." << endl;
	clear();
	cout << endl << endl;
}

template<typename T>
bool Ring<T>::empty() const
{
	if (headPtr == nullptr)
		return true;

	return false;
}

template<typename T>
size_t Ring<T>::size() const
{
	return number_of_nodes;
}

template<typename T>
T & Ring<T>::ValueOfCurrentlyPointedElement() const
{
	if (currPtr == nullptr)
	{
		// cout << "No elements in the list, currPtr points nowhere" << endl;
		// return 0;
		throw runtime_error("No elements in the list, currPtr points nowhere");
	}
	
	return currPtr->value;
}

template<typename T>
size_t Ring<T>::PositionOfCurrentlyPointedElement() const
{
	if (currPtr == nullptr)
	{
		// cout << "currPtr points to nowhere, it seems that the ring is empty" << endl;
		// return 0;
		throw runtime_error("currPtr points to nowhere, it seems that the ring is empty");
	}

	typename Ring<T>::Node * iterationPtr = headPtr;
	size_t position = 0;

	while (iterationPtr != currPtr)
	{
		cout << "position " << position << " value " << iterationPtr->value << endl;

		position++;
		iterationPtr = iterationPtr->nextPtr;
	}

	return position;
}

/* sets currPtr pointer to the 'step' element if starting from head */
template<typename T>
typename Ring<T>::Node * Ring<T>::MoveFromHeadToPosition(size_t step)
{
	if (empty())
	{
		cout << "The ring is empty, impossible to move currPtr anywhere!" << endl;
		return nullptr;
	}

	typename Ring<T>::Node * iterationPtr = headPtr;


	for (int i = 0; i <= step; ++i)
	{
		cout << i << " nodes from head of the ring: value: " << iterationPtr->value << endl;
		
		/* we need to print value of the element in 'step' steps from current,
			but no need to shift from it to the next one */
		if (i != step)
			iterationPtr = iterationPtr->nextPtr;
	}

	currPtr = iterationPtr;

	return currPtr;
}

/* shifts currPtr pointer to the element which is located in 'step' elements starting from currPtr */
template<typename T>
typename Ring<T>::Node * Ring<T>::ShiftCurrPtr(size_t step)
{
	if (empty())
	{
		cout << "The ring is empty, impossible to move currPtr anywhere!" << endl;
		return nullptr;
	}

	cout << "Position of currently pointed element: " << PositionOfCurrentlyPointedElement() << endl;

	Node * iterationPtr = currPtr;

	for (int i = 0; i <= step; ++i)
	{
		cout << i << " nodes from the original position of currPtr: value: " << iterationPtr->value << endl;
		
		/* we need to print value of the element in 'step' steps from current,
			but no need to shift from it to the next one */
		if (i != step)
			iterationPtr = iterationPtr->nextPtr;
	}

	currPtr = iterationPtr;

	return currPtr;
}

/* besides of adding a new node after curr position,
	moves currPtr to point to the created node */
template<typename T>
typename Ring<T>::Node * Ring<T>::AddNodeAfterCurrPosition(T && val) // the argument is an r-value reference, meaning, the value will be moved
{
	/* allocate some memory from heap for the new node of the ring */
	
		//typename Ring<T>::Node * newNodePtr = new Node; // for copyable type
		typename Ring<T>::Node * newNodePtr = new Node(move(val)); // for non-copyable type
		// newNodePtr->value = move(val);
	

	/* the empty ring is a special case */
	if (empty())
	{
		cout << "the list used to be empty, so, the head of ringed list is its end" << endl;
		newNodePtr->nextPtr = newNodePtr;
		newNodePtr->prevPtr = newNodePtr;

		/* as previously there were no nodes at all */
		headPtr = newNodePtr;

		/* make currPtr point to the only one element in the ringed list */
		currPtr = headPtr;
	}
	else
	{

		cout << "Value of current element " << currPtr->value << endl;
		cout << "Value of next element, which came after current before addition of new element " << currPtr->nextPtr->value << endl;

		/* we should save pointer from current element to the next element, as it would be nextPtr of just added node */
		
			typename Ring<T>::Node * nextAfterAdded = currPtr->nextPtr;

			currPtr->nextPtr = newNodePtr;
			newNodePtr->nextPtr = nextAfterAdded;
		

		/* we should save this value before making prevPtr of previously next node pointing to the just added node */
		
			typename Ring<T>::Node * prevAfterAdded = nextAfterAdded->prevPtr;

			nextAfterAdded->prevPtr = newNodePtr;
			newNodePtr->prevPtr = prevAfterAdded;
	

		/* had to put it there as otherwise it might try to print currPtr->value of empty ringed list */
		cout << "Value of current element " << currPtr->value << endl;
		cout << "Value of next element AFTER addition of new element (it should be new element itself) " << currPtr->nextPtr->value << endl;
		cout << "Value of next element which goes AFTER new element (previously next to current one) " << currPtr->nextPtr->nextPtr->value << endl;
		
		/* finally, move currPtr to the just created node */
		currPtr = currPtr->nextPtr;
		cout << "currPtr moved to the just added element" << endl;
	
	}


	/* increase amount of added nodes */
	number_of_nodes++;

	return currPtr;

}

/* takes value of node pointed by currPtr and removes the node from the list */
template<typename T>
T Ring<T>::PopCurrPtr()
{
	// T result; /* for non-copyable type T it would be impossible to declare it here */

	if (empty())
	{
		// cout << "Nothing left to remove, the ringed list is already empty" << endl;
		// return 0;
		throw runtime_error("Nothing left to remove, the ringed list is already empty");
	}

	if (currPtr == nullptr)
	{
		// cout << "currPtr points nowhere but the list is empty - SOMETHING HAS GONE WRONG!" << endl;
		// return 0;
		throw runtime_error("currPtr points nowhere but the list is not empty - SOMETHING HAS GONE WRONG!");
	}

	/* if the ringed list is not empty, we at least can be sure that currPtr points at some value */
	// result = currPtr->value;
	T result = move(currPtr->value);

	/* the case of having only one element is kinda special */
	if (size() == 1)
	{
		delete currPtr;
		headPtr = nullptr;
		currPtr = nullptr;

	}
	else if (currPtr == headPtr) /* this case is also special as headPtr is a field of Ring class and is set manually */
	{

		/* as we removed the node from the head of the ringed list -
			 headPtr should now point to the node next to it */
		headPtr = headPtr->nextPtr;

		/* the ending node (previous of the current, not yet removed head) should consider new headPtr as its next node */
		currPtr->prevPtr->nextPtr = headPtr;

		/* Previous node for new head should be previous node of current head */
		headPtr->prevPtr = currPtr->prevPtr;

		/* freed memory, which stored node, pointed by currPtr (which used to be a head of the ringed list) */
		delete currPtr;

		/* now currPtr will point to the new head (node, which originally came after the head we just removed) */
		currPtr = headPtr;

	}
	else
	{
		typename Ring<T>::Node * afterRemoved = currPtr->nextPtr;

		/* element, which came prior to the one we are going to remove, should now consider the node, coming after
				the node we are going to remove, as its next node */
		currPtr->prevPtr->nextPtr = currPtr->nextPtr;

		/* element, which came after the one we are going to remove, should now consider the node, coming prior to
				the node we are going to remove, as its previous node */
		currPtr->nextPtr->prevPtr = currPtr->prevPtr;

		delete currPtr;

		/* points to the node, which came right after the one we removed */
		currPtr = afterRemoved;
	}

	number_of_nodes--;
	return result;

}

template<typename T>
void Ring<T>::PrintTheRingForward() const
{
	cout << "Printing the ring forward:" << endl;

	if (empty())
	{
		// cout << "The ring is empty! No elements to be printed." << endl;
		// return;
		throw runtime_error("The ring is empty! No elements to be printed");
	}

	typename Ring<T>::Node * iterationPtr = headPtr;

	for (int i = 0; i < number_of_nodes; ++i)
	{
		cout << iterationPtr->value << " ";
		iterationPtr = iterationPtr->nextPtr;
	}

	cout << endl;
}

template<typename T>
void Ring<T>::PrintTheRingBackward() const
{
	cout << "Printing the ring backward:" << endl;

	if (empty())
	{
		// cout << "The ring is empty! No elements to be printed." << endl;
		// return;
		throw runtime_error("The ring is empty! No elements to be printed");
	}

	/* the ring has its ending node pointing to the beginning */
	typename Ring<T>::Node * iterationPtr = headPtr->prevPtr;

	for (int i = 0; i < number_of_nodes; ++i)
	{
		cout << iterationPtr->value << " ";
		iterationPtr = iterationPtr->prevPtr;
	}

	cout << endl;
}

template<typename T>
void Ring<T>::clear()
{
	/* move currPtr pointer to the ring's head */
	MoveFromHeadToPosition(0);

	while (!empty())
	{
		cout << "Removed value " << PopCurrPtr() << "..." << endl; 
	}

	cout << "Make sure size of ringed list is 0: " << size() << endl;

	cout << "The list have been cleared completely" << endl;
}

/* Function MakeARingFromRange() was moved into header file 
   due to similar reason we manually instantiated templates Ring<int> and Ring<char> */

/* FROM STACKOVERFLOW.COM:
   In ring_test.cpp (with main() function), the compiler will implicitly instantiate
   the template classes Ring<int> and Ring<char> because those particular instantiations are used in ring_test.cpp */

/* Tell the compiler, at the end of ring.cpp, which particular template classes will be required,
   forcing it to compile Ring<int> and Ring<char>. */

template class Ring<int>;
template class Ring<char>;
template class Ring<NoncopyableInt>;

/* Shamelessly 'borrowed' from Yandex "Red Belt C++" course, Josephus Permutation problem */
bool operator == (const NoncopyableInt & lhs, const NoncopyableInt & rhs)
{
  return lhs.value == rhs.value;
}

ostream & operator << (ostream & os, const NoncopyableInt & v)
{
  return os << v.value;
}