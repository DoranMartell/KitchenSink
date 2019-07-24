#include "ring.h"

#include <vector>
#include <numeric>

using namespace std;

void Test1();
void Test2();
void TestJosephusPermutation(int ); /* takes permutation's step_size as an argument */

int main(int argc, char const *argv[])
{
	// Test1();
	// Test2();
	TestJosephusPermutation(0);
	TestJosephusPermutation(3);

	return 0;
}

void Test1()
{	
	cout << "STARTED TEST1 function!!!" << endl << endl;

	Ring<int> rng;

	cout << "Ring of <int> size: " << rng.size() << endl; 	

	rng.PrintTheRingForward();
	rng.PrintTheRingBackward();

	cout << endl << "Add value 7 in the head of the ring..." << endl << endl;

	rng.ValueOfCurrentlyPointedElement();
	rng.PositionOfCurrentlyPointedElement();

	// cout << "value of variable k before its value was moved to the ring: " << k << endl;
	rng.AddNodeAfterCurrPosition(7);
	// cout << "value of variable k after its value was moved to the ring: " << k << endl;

	cout << "Ring of <int> size after addition of new element: " << rng.size() << endl; 	
	rng.ValueOfCurrentlyPointedElement();
	rng.PositionOfCurrentlyPointedElement();

	cout << "Print the element at the 0 position:" << endl;
	cout << rng.MoveFromHeadToPosition(0)->value << endl;

	cout << "Print the element at the 2nd position (mind the size of the ring!)" << endl;
	cout << rng.MoveFromHeadToPosition(2)->value << endl;

	cout << "Lets print the whole list again (sounds more impressive than how it will look like):" << endl;
	rng.PrintTheRingForward();
	rng.PrintTheRingBackward();

	cout << "Where is currPtr right now?" << endl;
	cout << "currPtr->value " << rng.ValueOfCurrentlyPointedElement();
	cout << " currPtr position (counting from head) " << rng.PositionOfCurrentlyPointedElement();
	cout << endl << endl;

	cout << "Add one another element after 7 : let it have value 99" << endl;
	rng.AddNodeAfterCurrPosition(99);

	cout << "Ring of <int> size after addition of one another new element: " << rng.size() << endl; 	
	
	cout << "Where is currPtr right now? Should point at the element which we just added:" << endl;
	rng.ValueOfCurrentlyPointedElement();
	rng.PositionOfCurrentlyPointedElement();

	cout <<  endl << "Print the element at the 0 position:" << endl;
	cout << rng.MoveFromHeadToPosition(0)->value << endl;

	cout << "Shift the currPtr one position right to the next element" << endl;
	cout << rng.ShiftCurrPtr(1)->value << endl;

	cout << "Shift the currPtr one position right once again" << endl;
	cout << rng.ShiftCurrPtr(1)->value << endl;

	cout << "Print the whole list again (should contain two elements):" << endl;
	rng.PrintTheRingForward();
	rng.PrintTheRingBackward();

	cout << endl << "Return currPtr at the 0 position:" << endl;
	rng.MoveFromHeadToPosition(0);
	cout << "Add one another element after 7 : let it have value 80" << endl;
	rng.AddNodeAfterCurrPosition(80);

	cout << "Ring of <int> size after addition of one another new element: " << rng.size() << endl;

	cout << "Print the whole list again (should contain THREE elements):" << endl;
	rng.PrintTheRingForward();
	rng.PrintTheRingBackward();


	cout << endl << endl << "Move currPtr at the 2 position (currently the last element):" << endl;
	rng.MoveFromHeadToPosition(2);
	cout << "Removed element from the ringed list which had value: " << rng.PopCurrPtr() << endl;

	cout << "Where is currPtr pointing to now??" << endl;
	rng.ValueOfCurrentlyPointedElement();
	rng.PositionOfCurrentlyPointedElement();

	cout << "Ring of <int> size after removal of the last element: " << rng.size() << endl;

	cout << "Print the whole list again (should contain TWO elements):" << endl;
	rng.PrintTheRingForward();
	rng.PrintTheRingBackward();

	cout << endl << endl << "Now we are going to remove HEAD from the list to check whether the new head is correctly determined" << endl;

	cout << "Value of removed element " << rng.PopCurrPtr() << endl;

	cout << "Ring of <int> size after removal of the last element: " << rng.size() << endl;

	cout << "Print the whole list again (should contain ONE element):" << endl;
	rng.PrintTheRingForward();
	rng.PrintTheRingBackward();

	cout << endl << endl << "Move currPtr at the 0 position (check that the head is correctly shifted):" << endl;
	rng.MoveFromHeadToPosition(0);


	cout << endl << endl << "Now we are going to remove the last survived element and thus clear the whole list" << endl;

	cout << "Value of removed element " << rng.PopCurrPtr() << endl;

	cout << "Ring of <int> size after removal of the last element: " << rng.size() << endl;

	cout << "Print the whole list again (should contain ZERO elements):" << endl;
	rng.PrintTheRingForward();
	rng.PrintTheRingBackward();


	cout << endl << "END OF TEST1 function!!!" << endl << endl << endl;
}

void Test2()
{
	cout << "STARTED TEST2 function!!!" << endl << endl;

	vector<char> stuff{'a', 'z', 'v', '+', 'y', 'o', 'p', 'j', '3', '_', 'y', 'z'};

	Ring<char> rng2 = MakeARingFromRange(stuff.begin(), stuff.end());

	cout << endl << "Lets print it backwards:" << endl;
	rng2.PrintTheRingBackward();

	cout << endl << "END OF TEST2 function!!!" << endl << endl << endl;
}

void TestJosephusPermutation(int step_size)
{
	cout << "Testing Josephus Permutation with ringed list with step_size = " << step_size << endl << endl;

	vector<NoncopyableInt> numbers;
	numbers.push_back({0});
	numbers.push_back({1});
	numbers.push_back({2});
	numbers.push_back({3});
	numbers.push_back({4});
	numbers.push_back({5});
	numbers.push_back({6});
	numbers.push_back({7});
	numbers.push_back({8});
	numbers.push_back({9});
	// iota(numbers.begin(), numbers.end(), 0);

	Ring<NoncopyableInt> rng = MakeARingFromRange(numbers.begin(), numbers.end());
	rng.PrintTheRingForward();

	/* after each PopCurrPtr() currPtr already shifts one position to the right */
	int shifting = (step_size == 0) ? 0 : (step_size - 1);

	vector<NoncopyableInt> result;

	rng.MoveFromHeadToPosition(0);
	while (!rng.empty())
	{
		result.push_back(rng.PopCurrPtr());
		rng.ShiftCurrPtr(shifting);
	}


	for (int i = 0; i < result.size(); ++i)
	{
		cout << result[i] << " ";
	}
	cout << endl;

}
