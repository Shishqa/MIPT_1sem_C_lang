/*
 * run_list_tests.cpp
 *
 *  Created on: 29 окт. 2019 г.
 *      Author: shishqa
 */

#include <stdio.h>
#include <stdlib.h>

#include "ms_linked_list.h"
#include "ms_linked_node_methods.h"
#include "ms_linked_list_methods.h"
#include "tests.h"

int main()
{
	test_InsertFront();
	test_InsertBack();
	test_InsertAfter();
	test_InsertBefore();

	test_Remove();

	test_FindPhysicalPositionByLogical();
	test_FindPhysicalPositionByValue();

	test_Align();

	test_1();

	return (0);
}

bool test_1()
{
	LinkedList<int> test_list = {};
	test_list.init(17);

	test_list.insertFront(110);
	test_list.insertAfter(1, 150);
	test_list.insertAfter(1, 140);
	test_list.insertAfter(1, 130);
	test_list.insertAfter(1, 120);
	test_list.insertAfter(1, 115);
	test_list.insertBack(160);
	test_list.insertFront(100);

	//test_list.dump(stdout);

	test_list.remove(1);
	test_list.remove(2);

	test_list.dump(stdout);
	test_list.graphDump();

	test_list.alignIndexes();

	test_list.dump(stdout);

	test_list.remove(3);

	test_list.dump(stdout);
}

bool test_InsertFront()
{
	printf("testing InsertFront:\t");

	LinkedList<int> test_list = {};

	test_list.init(20);

	test_list.insertFront(1);
	test_list.insertFront(2);
	test_list.insertFront(3);
	test_list.insertFront(4);
	test_list.insertFront(5);

	if (!test_list.valid() || test_list.getHead() != 5)
	{
		printf("NOT PASSED\n");
		return (false);
	}

	printf("OK\n");
	return (true);
}

bool test_InsertBack()
{
	printf("testing InsertBack:\t");

	LinkedList<int> test_list = {};

	test_list.init(20);

	test_list.insertFront(1);
	test_list.insertFront(2);
	test_list.insertFront(3);
	test_list.insertBack(4);

	if (!test_list.valid() || test_list.getTail() != 4)
	{
		printf("NOT PASSED\n");
		return (false);
	}

	printf("OK\n");
	return (true);
}

bool test_InsertAfter()
{
	printf("testing InsertAfter:\t");

	LinkedList<int> test_list = {};

	test_list.init(20);

	test_list.insertFront(1);
	test_list.insertFront(2);
	test_list.insertFront(3);
	test_list.insertAfter(3, 123);
	test_list.insertBack(12);
	test_list.insertBack(15);
	test_list.insertAfter(1, 123);

	if (!test_list.valid() || test_list.data[7].val != 123 ||
		test_list.insertAfter(10, 1000) != DEADLOCK)
	{
		printf("NOT PASSED\n");
		return (false);
	}

	printf("OK\n");
	return (true);
}

bool test_InsertBefore()
{
	printf("testing InsertBefore:\t");

	LinkedList<int> test_list = {};

	test_list.init(20);

	test_list.insertFront(1);
	test_list.insertFront(2);
	test_list.insertFront(3);
	test_list.insertBefore(3, 200);
	test_list.insertBack(12);
	test_list.insertBack(15);
	test_list.insertBefore(3, 208);

	if (!test_list.valid() || test_list.data[7].val != 208 ||
		test_list.insertBefore(10, 1000) != DEADLOCK)
	{
		printf("NOT PASSED\n");
		return (false);
	}

	printf("OK\n");
	return (true);
}

bool test_Remove()
{
	printf("testing Remove:\t\t");

	LinkedList<int> test_list = {};

	test_list.init(20);

	test_list.insertFront(1);
	test_list.insertFront(2);
	test_list.insertFront(3);
	test_list.insertBefore(3, 200);
	test_list.insertBack(12);
	test_list.insertBack(15);
	test_list.insertBefore(3, 208);

	test_list.remove(3);

	if (!test_list.valid() || test_list.data[3].val != test_list.POISON)
	{
		printf("NOT PASSED\n");
		return (false);
	}

	printf("OK\n");
	return (true);
}

bool test_FindPhysicalPositionByLogical()
{
	printf("testing indexing:\t");

	LinkedList<int> test_list = {};

	test_list.init(20);

	test_list.insertFront(1);
	test_list.insertFront(2);
	test_list.insertFront(3);
	test_list.insertBefore(3, 200);
	test_list.insertBack(12);
	test_list.insertBack(15);
	test_list.insertBefore(3, 208);

	test_list.remove(3);

	if (!test_list.valid() || test_list.findPhysicalPositionByLogical(3) != 2)
	{
		printf("NOT PASSED\n");
		return (false);
	}

	printf("OK\n");
	return (true);
}

bool test_FindPhysicalPositionByValue()
{
	printf("testing valueing:\t");

	LinkedList<int> test_list = {};

	test_list.init(20);

	test_list.insertFront(1);
	test_list.insertFront(2);
	test_list.insertFront(3);
	test_list.insertBefore(3, 200);
	test_list.insertBack(12);
	test_list.insertBack(15);
	test_list.insertBefore(3, 208);

	test_list.remove(3);

	if (!test_list.valid() || test_list.findPhysicalPositionByValue(200) != 4)
	{
		printf("NOT PASSED\n");
		return (false);
	}

	printf("OK\n");
	return (true);
}

bool test_Align()
{
	printf("testing Align:\t\t");

	LinkedList<int> test_list = {};

	test_list.init(20);

	test_list.insertFront(1);
	test_list.insertFront(2);
	test_list.insertFront(3);
	test_list.insertBefore(3, 200);
	test_list.insertBack(12);
	test_list.insertBack(15);
	test_list.insertBefore(3, 208);

	test_list.remove(3);

	test_list.alignIndexes();

	if (!test_list.valid() || test_list.findPhysicalPositionByLogical(3) != 3)
	{
		printf("NOT PASSED\n");
		return (false);
	}

	printf("OK\n");
	return (true);
}