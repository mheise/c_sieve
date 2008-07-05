/*
 * list.h - a generic linked list container
 *
 * @author		Mike Heise
 * @version		0.1.0.00
 * @build		001
 * @begun		16 Jun 2008
 * @update		16 Jun 2008
 *
 */

typedef struct _node {
		/*	a single node in the list
		 *	this structure is intended for list's internal use only
		 */

		void *data;
		_node *next;
} node;

typedef struct _list{
		/*	the list itself.  singly linked atm.
		 *	by design, this structure should be modified through the functions
		 *	included in this header
		 */
		node head;
		size_t size;
} list;


