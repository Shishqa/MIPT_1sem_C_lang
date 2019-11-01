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

int main()
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

	test_list.dump(stdout);

	test_list.remove(1);
	test_list.remove(2);

	test_list.dump(stdout);

	printf("3rd element is %d\n", test_list[3]);

	printf("test_list is %s\n", ((test_list.valid()) ? "OK" : "NOT OK"));

	printf("second element is on the %lu position\n", test_list.findPhysicalPositionByLogical(2));

	return (0);
}
