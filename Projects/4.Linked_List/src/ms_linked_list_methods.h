/*
 * ms_linked_list_methods.h
 *
 *  Created on: 29 окт. 2019 г.
 *      Author: shishqa
 */

#ifndef MS_LINKED_LIST_METHODS_H_
#define MS_LINKED_LIST_METHODS_H_


template <typename elem_t>
bool LinkedList<elem_t>::init (const size_t size)
{
	this->data = (Node <elem_t> *) calloc (size, sizeof(*this->data));

	this->data[0].init(this->POISON, DEADLOCK, DEADLOCK);

	for (size_t i = 1; i < size - 1; i++)
	{
		this->data[i].init(this->POISON, i + 1, EMPTY_MARKER);
	}

	this->data[size - 1].init(this->POISON, DEADLOCK, EMPTY_MARKER);

	this->empty = 1;
	this->head  = DEADLOCK;
	this->tail  = DEADLOCK;

	this->max_size = size;
	this->size = 0;

	this->aligned = false;

	return (true);
}

template <typename elem_t>
bool LinkedList<elem_t>::clear ()
{
	free (this->data);

	this->max_size = 0;

	return (true);
}

template <typename elem_t>
void LinkedList<elem_t>::dump (FILE* log)
{
	fprintf (log, "LinkedList [%p]\n", this);
	fprintf (log, "-----------------------------------------\n");
	fprintf (log, "i\t|\tval\tnext\tprev\n");

	for (size_t i = 0; i < this->max_size; i++)
	{
		fprintf (log, "%lu ",i);
		fprintf (log, "%s", ((i == this->head) ? ("H") : ("")));
		fprintf (log, "%s", ((i == this->tail) ? ("T") : ("")));
		fprintf (log, "%s", ((i == this->empty) ? ("E") : ("")));
		fprintf (log, "\t|\t");
		fprintf (log, "%d%s\t%d\t%d\n", this->data[i].val,
					((this->data[i].val == this->POISON) ? ("(P)") : ("")),
					this->data[i].next, this->data[i].prev);
	}

	fprintf (log, "-----------------------------------------\n");
}


template <typename elem_t>
int LinkedList<elem_t>::insertAfter (const int index, const elem_t val)
{
	if (index <= 0 || index > this->size || this->empty == DEADLOCK || this->data[index].prev == EMPTY_MARKER)
	{
		return (DEADLOCK);
	}

	int nest_index = this->empty;
	this->empty = this->data[this->empty].next;

	this->data[nest_index].init(val, this->data[index].next, index);

	if (this->data[index].next != DEADLOCK)
	{
		this->data[this->data[nest_index].next].prev = nest_index;
	}
	else
	{
		this->tail = nest_index;
	}

	this->data[index].next = nest_index;

	this->size++;

	return (nest_index);
}

template <typename elem_t>
int LinkedList<elem_t>::insertBefore (const int index, const elem_t val)
{
	if (index <= 0 || index > this->size || this->empty == DEADLOCK || this->data[index].prev == EMPTY_MARKER)
	{
		return (DEADLOCK);
	}

	int nest_index = this->empty;
	this->empty = this->data[this->empty].next;

	this->data[nest_index].init(val, index, this->data[index].prev);

	if (this->data[index].prev != DEADLOCK)
	{
		this->data[this->data[nest_index].prev].next = nest_index;
	}
	else
	{
		this->head = nest_index;
	}

	this->data[index].prev = nest_index;

	this->size++;

	return (nest_index);
}

template <typename elem_t>
int LinkedList<elem_t>::insertBack (const elem_t val)
{
	if (this->empty == DEADLOCK)
	{
		return (DEADLOCK);
	}

	int nest_index = this->empty;
	this->empty = this->data[this->empty].next;

	if (this->size == 0)
	{
		this->data[nest_index].init(val, DEADLOCK, DEADLOCK);
		this->head = nest_index;
		this->tail = nest_index;
	}
	else
	{
		this->data[nest_index].init(val, DEADLOCK, this->tail);
		this->data[this->tail].next = nest_index;
		this->tail = nest_index;
	}

	this->size++;

	return (nest_index);
}


template <typename elem_t>
int LinkedList<elem_t>::insertFront (const elem_t val)
{
	if (this->empty == DEADLOCK)
	{
		return (DEADLOCK);
	}

	int nest_index = this->empty;
	this->empty = this->data[this->empty].next;

	if (this->size == 0)
	{
		this->data[nest_index].init(val, DEADLOCK, DEADLOCK);
		this->head = nest_index;
		this->tail = nest_index;
	}
	else
	{
		this->data[nest_index].init(val, this->head, DEADLOCK);
		this->data[this->head].prev = nest_index;
		this->head = nest_index;
	}

	this->size++;

	return (nest_index);
}


template <typename elem_t>
int LinkedList<elem_t>::remove (const int index)
{
	if (index <= 0 || index > this->size)
	{
		return (DEADLOCK);
	}

	if (index == this->head)
	{
		this->head = this->data[index].next;
	}
	else
	{
		this->data[this->data[index].prev].next = this->data[index].next;
	}

	if (index == this->tail)
	{
		this->tail = this->data[index].prev;
	}
	else
	{
		this->data[this->data[index].next].prev = this->data[index].prev;
	}

	this->data[index].clear(this->POISON, this->empty);

	this->empty = index;

	this->size--;

	return (index);
}

template <typename elem_t>
int LinkedList<elem_t>::findPhysicalPositionByValue (const elem_t val)
{
	for (int i = this->head; i != DEADLOCK; i = this->data[i].next)
	{
		if (this->data[i].val == val)
		{
			return (i);
		}
	}

	return (DEADLOCK);
}

template <typename elem_t>
int LinkedList<elem_t>::findPhysicalPositionByLogical (int index)
{
	if (index > this->size)
	{
		return (DEADLOCK);
	}

	if (index <= this->size / 2)
	{
		for (int i = this->head; i != DEADLOCK; i = this->data[i].next)
		{
			if (--index == 0)
			{
				return (i);
			}
		}
	}
	else
	{
		for (int i = this->tail; i != DEADLOCK; i = this->data[i].prev)
		{
			if (++index > this->size)
			{
				return (i);
			}
		}
	}

	return (DEADLOCK);
}

template <typename elem_t>
elem_t LinkedList<elem_t>::operator[](const int index)
{
	if (this->aligned)
	{
		return (this->data[index].val);
	}
	else
	{
		return (this->data[this->findPhysicalPositionByLogical(index)].val);
	}
}

template <typename elem_t>
bool LinkedList<elem_t>::valid ()
{
	//printf ("data is %s\n", ((this->headMetTail()) ? "OK" : "NOT OK"));
	//printf ("empty is %s\n", ((this->emptyOK()) ? "OK" : "NOT OK"));

	if (!this->headMetTail() || !this->emptyOK())
	{
		return (false);
	}

	return (true);
}


template <typename elem_t>
bool LinkedList<elem_t>::headMetTail ()
{
	size_t elem_cnt = 0;

	bool met_tail = false;

	for (int i = this->head; i != DEADLOCK; i = this->data[i].next)
	{
		elem_cnt++;

		if (i == this->tail && this->data[i].next == DEADLOCK)
		{
			met_tail = true;
			break;
		}

		if (i == this->head && this->data[i].prev != DEADLOCK)
		{
			//printf ("head is not head\n");
			return (false);
		}
		else if (i == this->head)
		{
			continue;
		}

		if (this->data[this->data[i].prev].next != i || this->data[this->data[i].next].prev != i)
		{
			//printf ("prev or next is not linked: %d\n", i);
			return (false);
		}

		if (elem_cnt > this->size)
		{
			//printf ("went over size\n");
			return (false);
		}
	}

	if (elem_cnt != this->size || !met_tail)
	{
		//printf ("counted: %lu\nexepted: %lu\n", this->size);
		return (false);
	}

	return (true);
}

template <typename elem_t>
bool LinkedList<elem_t>::emptyOK ()
{
	size_t elem_cnt = 0;

	for (int i = this->empty; i != DEADLOCK; i = this->data[i].next)
	{
		elem_cnt++;

		if (this->data[i].prev != EMPTY_MARKER)
		{
			return (false);
		}

		if (elem_cnt > this->max_size)
		{
			return (false);
		}
	}

	if (elem_cnt != this->max_size - (this->size + 1))
	{
		//printf ("%lu != %lu\n", elem_cnt, this->max_size - this->size);
		return (false);
	}

	return (true);
}

#endif /* MS_LINKED_LIST_METHODS_H_ */
