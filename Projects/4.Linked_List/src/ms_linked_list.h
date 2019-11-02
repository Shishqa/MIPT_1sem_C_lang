/*
 * ms_linked_list.h
 *
 *  Created on: 28 окт. 2019 г.
 *      Author: shishqa
 */

#ifndef MS_LINKED_LIST_H_
#define MS_LINKED_LIST_H_

#include <stdio.h>

const int EMPTY_MARKER = -1;
const int DEADLOCK = 0;

template <typename elem_t>
struct Node
{
public:
	elem_t val;
	int next;
	int prev;

	bool init(const elem_t val, const int next, const int prev);
	bool clear(const elem_t poison_val, const int empty);
};

template <typename elem_t>
class LinkedList
{
private:
	Node<elem_t> *data;
	size_t max_size;
	int head;
	int tail;
	int empty;
	const elem_t POISON = -666;
	bool aligned;
	size_t size;

	int cmp(const void *a, const void *b);
	int comp_logic_pos()

public:
	bool init(const size_t size);
	bool clear();

	size_t size();

	int insertAfter(const int index, const elem_t val);
	int insertBefore(const int index, const elem_t val);
	int insertFront(const elem_t val);
	int insertBack(const elem_t val);
	int remove(const int index);

	int findPhysicalPositionByLogical(int index);
	int findPhysicalPositionByValue(const elem_t val);

	bool alignIndexes();

	bool valid();
	bool dataOK();
	bool emptyOK();

	void dump(FILE *log);
};

#endif /* MS_LINKED_LIST_H_ */
