#include <iostream>
#include <list>	// I feel kinda uncomfortable to use STL container in such a task
#include <algorithm>

using namespace std;

struct SmartPointer
{
	void * object_ptr;
	unsigned object_size; 
	unsigned object_pos;
};

class SmallAllocator
{
private:
        char Memory[1048576]; // given by THEM
        char * currPtr = &Memory[0];
        unsigned int currPos = 0;

        list<SmartPointer> objects_list;
        unsigned int object_counter = 0;

        list<SmartPointer>::iterator FindPreviouslyAllocatedObject(void * Pointer)
        {

        	list<SmartPointer>::iterator foundObjectIt = find_if(objects_list.begin(), objects_list.end(), 
        										[Pointer](const SmartPointer & obj){return (obj.object_ptr == Pointer);});

        	if (foundObjectIt == objects_list.end())
        	{
        		// cout << "Such object has never been allocated" << endl;
        		return foundObjectIt;
        	}
        	else
        	{
        		// cout << "Found previously allocated object: position in Memory block " << foundObjectIt->object_pos
        			// << " size taken by it " << foundObjectIt->object_size << " and its address in Memory Block " << foundObjectIt->object_ptr << endl;
        		return foundObjectIt;
        	}

        }

        void PrintAllocatedObject(const SmartPointer & currentObject)
        {
        	cout << "The given object's insides (byte-by-byte): " << endl;
        	for (int i = currentObject.object_pos; i < currentObject.object_pos + currentObject.object_size; ++i)
        	{
        		cout << Memory[i];
        	}
        	cout << endl;
        }


public:

		void PrintAll()
		{
			cout << "PRINTING ALL WHAT IS THERE IN MEMORY BLOCK" << endl;
			for (list<SmartPointer>::iterator it = objects_list.begin(); it != objects_list.end(); ++it)
			{
				cout << "Another object, starting at " << it->object_pos << " position in memory block" << endl;
				// PrintAllocatedObject(*it);
			}
		}

		SmartPointer ReturnAllocatedObject(void * Pointer)
		{
			list<SmartPointer>::iterator it = FindPreviouslyAllocatedObject(Pointer);
        	if (it == objects_list.end())
        	{
        		// cout << "Cannot return an object which has never been allocated!" << endl;
        		return (*it);
        	}

        	return (*it);

		}

        void * Alloc(unsigned int Size)
        {
        	// cout << "BEFORE ALLOCATION: Position in the memory block: " <<  currPos << " address " << (void *) currPtr << endl;
        	// cout << "Amount of objects stored in allocator's memory block: " <<  object_counter << endl;

        	/* create a SmartPointer for object we are going to allocate in Memory array */
        	SmartPointer new_object;
        	new_object.object_ptr = (void *) currPtr;
        	new_object.object_size = Size;
        	new_object.object_pos = currPos;
        	
        	/* add smart pointer of a new object to the list of alredy allocated objects */
        	objects_list.push_back(new_object);
        	// PrintAllocatedObject(objects_list.back());
        	object_counter++;
        	
        	/* shift pointer in Memory block and current position to the right (Size bytes, byte per position) */
        	currPtr += Size;
        	currPos += Size;

        	// cout << "AFTER ALLOCATION: Position in the memory block: " <<  currPos << " address " << (void *) currPtr << endl;
        	// cout << "Amount of objects stored in allocator's memory block: " <<  object_counter << endl;

        	return objects_list.back().object_ptr;
        }

        /* It should also copy what was previously stored in the object before reallocation */
        void * ReAlloc(void * Pointer, unsigned int Size) /* size is actually a new size for previously allocated object */
        {
        	list<SmartPointer>::iterator currentObjectIt = FindPreviouslyAllocatedObject(Pointer);
        	if (currentObjectIt == objects_list.end())
        	{
        		// cout << "Cannot reallocate an object which has never been allocated!" << endl;
        		return nullptr;
        	}

        	// PrintAllocatedObject(*currentObjectIt);
        	
        	void * newPtr = Alloc(Size);
        	SmartPointer newObject = objects_list.back();

        	// cout << "Copying byte-by-byte from previous block: " << endl;
        	int j = newObject.object_pos;
        	for (int i = currentObjectIt->object_pos; i < currentObjectIt->object_pos + currentObjectIt->object_size; ++i)
        	{
        		Memory[j] = Memory[i];
        		// cout << (int) Memory[j];
        		Memory[i] = 0;
        		j++;
        	}
        	// cout << endl;

        	// cout << "Print from memory-block byte-by-byte:" << endl;
        	// for (int j = newObject.object_pos; j < newObject.object_pos + newObject.object_size; ++j)
        	// {
        	// 	// cout << (int)Memory[j];
        	// }
        	// cout << endl;

        	objects_list.erase(currentObjectIt);

        	// PrintAllocatedObject(newObject);


        	// cout << "Used to be a position in the memory block: " <<  currPos << " address " << objectPtr << endl;
        	return newObject.object_ptr;
        }


        void Free(void * Pointer)
        {
        	list<SmartPointer>::iterator currentObjectIt = FindPreviouslyAllocatedObject(Pointer);
        	if (currentObjectIt == objects_list.end())
        	{
        		// cout << "Cannot free an object which has never been allocated!" << endl;
        		return;
        	}

        	// PrintAllocatedObject(*currentObjectIt);


        	for (int i = currentObjectIt->object_pos; i < currentObjectIt->object_pos + currentObjectIt->object_size; ++i)
        	{
        		Memory[i] = 0;
        	}

        	objects_list.erase(currentObjectIt);
        	object_counter--;

        }
};

// main() function shamelessly 'borrowed' from Mail.Ru course on Stepik.org

/*
int main()
{

	SmallAllocator A1;
	int * A1_P1 = (int *) A1.Alloc(sizeof(int));
	A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
	A1.Free(A1_P1);

	SmallAllocator A2;
	int * A2_P1 = (int *) A2.Alloc(10 * sizeof(int));

	cout << endl;
	A2.PrintAll();
	cout << endl << endl << endl;

	for(unsigned int i = 0; i < 10; i++)
	{

		A2_P1[i] = i;
		cout << A2_P1[i] << " ";
	}
	cout << endl;

	A2.PrintAll();
	cout << endl << endl << endl;

	SmartPointer a = A2.ReturnAllocatedObject(A2_P1);

	cout << "PRINT From Memory Block!!!!:" << endl;
	for (int i = a.object_pos; i < a.object_pos + (a.object_size / sizeof(int)); ++i)
	{
		cout << (int)((int *)a.object_ptr)[i];
	}
	cout << endl << endl;

	for(unsigned int i = 0; i < 10; i++)
		if (A2_P1[i] != i)
			std::cout << "ERROR 1" << std::endl;

	int * A2_P2 = (int *) A2.Alloc(10 * sizeof(int));

	for(unsigned int i = 0; i < 10; i++)
		A2_P2[i] = -1;

	cout << "Fill block A2_P1(size 10):" << endl;
	for(unsigned int i = 0; i < 10; i++) 
	{
		cout << A2_P1[i] << " ";
		if (A2_P1[i] != i)
			std::cerr << "ERROR 2" << std::endl;
	}
	cout << endl;

	for(unsigned int i = 0; i < 10; i++)
	{
		if (A2_P2[i] != -1)
			std::cout << "ERROR 3" << std::endl;
	}

	A2_P1 = (int *) A2.ReAlloc(A2_P1, 20 * sizeof(int));
	cout << "Fill block A2_P1(rest of later attached 10 elements):" << endl;
	for(unsigned int i = 10; i < 20; i++)
	{
		A2_P1[i] = i;
		cout << A2_P1[i] << " ";
	}
	cout << endl;

	cout << "Testing reallocated stuff:\n";
	for(unsigned int i = 0; i < 20; i++)
	{	
		cout << A2_P1[i] << " ";
		if (A2_P1[i] != i)
			std::cerr << "ERROR 4" << std::endl;
	}
	cout << endl;

	// A2.PrintAll();

	cout << "PRINT From Memory Block!!!!:" << endl;
	a = A2.ReturnAllocatedObject(A2_P1);
	for (int i = a.object_pos; i < a.object_pos + (a.object_size / sizeof(int)); ++i)
	{
		cout << (int)((int *)a.object_ptr)[i];
	}
	cout << endl << endl;

	for(unsigned int i = 0; i < 10; i++)
		if (A2_P2[i] != -1)
			std::cerr << "ERROR 5" << std::endl;

	// A2.PrintAll();

	cout << "PRINT From Memory Block!!!!:" << endl;
	a = A2.ReturnAllocatedObject(A2_P1);
	for (int i = a.object_pos; i < a.object_pos + (a.object_size / sizeof(int)); ++i)
	{
		cout << (int)((int *)a.object_ptr)[i];
	}
	cout << endl << endl;

	A2_P1 = (int *) A2.ReAlloc(A2_P1, 5 * sizeof(int));
	for(unsigned int i = 0; i < 5; i++)
		if (A2_P1[i] != i)
			std::cout << "ERROR 6" << std::endl;

	for(unsigned int i = 0; i < 10; i++)
		if(A2_P2[i] != -1)
			std::cout << "ERROR 7" << std::endl;

	A2.Free(A2_P1);
	A2.Free(A2_P2);

	return 0;
}*/

/*
int main()
{
	SmallAllocator A1;
	int * A1_P1 = (int *) A1.Alloc(sizeof(int));
	A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
	A1.Free(A1_P1);
	SmallAllocator A2;
	int * A2_P1 = (int *) A2.Alloc(10 * sizeof(int));
	for(unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
	for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;
	int * A2_P2 = (int *) A2.Alloc(10 * sizeof(int));
	for(unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
	for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
	for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;
	A2_P1 = (int *) A2.ReAlloc(A2_P1, 20 * sizeof(int));
	for(unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
	for(unsigned int i = 0; i < 20; i++) if(A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
	for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;
	A2_P1 = (int *) A2.ReAlloc(A2_P1, 5 * sizeof(int));
	for(unsigned int i = 0; i < 5; i++) if(A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
	for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
	A2.Free(A2_P1);
	A2.Free(A2_P2);

	return 0;

}
*/