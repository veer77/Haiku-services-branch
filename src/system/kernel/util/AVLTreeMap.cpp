/*
 * Copyright 2003-2007, Ingo Weinhold <bonefish@cs.tu-berlin.de>.
 * Distributed under the terms of the MIT License.
 */

#include <util/AVLTreeMap.h>


// #pragma mark - AVLTreeCompare


AVLTreeCompare::~AVLTreeCompare()
{
}


// #pragma mark - AVLTree


AVLTree::AVLTree(AVLTreeCompare* compare)
	: fRoot(NULL),
	  fNodeCount(0),
	  fCompare(compare)
{
}


AVLTree::~AVLTree()
{
}


void
AVLTree::MakeEmpty()
{
	fRoot = NULL;
	fNodeCount = 0;
}


status_t
AVLTree::Insert(AVLTreeNode* nodeToInsert)
{
	int result = _Insert(nodeToInsert);
	switch (result) {
		case OK:
		case HEIGHT_CHANGED:
			return B_OK;
		case NO_MEMORY:
			return B_NO_MEMORY;
		case DUPLICATE:
		default:
			return B_BAD_VALUE;
	}
}


AVLTreeNode*
AVLTree::Remove(const void* key)
{
	// find node
	AVLTreeNode* node = fRoot;
	while (node) {
		int cmp = fCompare->CompareKeyNode(key, node);
		if (cmp == 0)
			break;
		else {
			if (cmp < 0)
				node = node->left;
			else
				node = node->right;
		}
	}

	// remove it
	if (node)
		_Remove(node);

	return node;
}


bool
AVLTree::Remove(AVLTreeNode* node)
{
	switch (_Remove(node)) {
		case OK:
		case HEIGHT_CHANGED:
			return true;
		case NOT_FOUND:
		default:
			return false;
	}
}


AVLTreeNode*
AVLTree::LeftMost(AVLTreeNode* node) const
{
	if (node) {
		while (node->left)
			node = node->left;
	}

	return node;
}


AVLTreeNode*
AVLTree::RightMost(AVLTreeNode* node) const
{
	if (node) {
		while (node->right)
			node = node->right;
	}

	return node;
}


AVLTreeNode*
AVLTree::Previous(AVLTreeNode* node) const
{
	if (node) {
		// The previous node cannot be in the right subtree.
		if (node->left) {
			// We have a left subtree, so go to the right-most node.
			node = node->left;
			while (node->right)
				node = node->right;
		} else {
			// No left subtree: Backtrack our path and stop, where we
			// took the right branch.
			AVLTreeNode* previous;
			do {
				previous = node;
				node = node->parent;
			} while (node && previous == node->left);
		}
	}

	return node;
}


AVLTreeNode*
AVLTree::Next(AVLTreeNode* node) const
{
	if (node) {
		// The next node cannot be in the left subtree.
		if (node->right) {
			// We have a right subtree, so go to the left-most node.
			node = node->right;
			while (node->left)
				node = node->left;
		} else {
			// No right subtree: Backtrack our path and stop, where we
			// took the left branch.
			AVLTreeNode* previous;
			do {
				previous = node;
				node = node->parent;
			} while (node && previous == node->right);
		}
	}

	return node;
}


AVLTreeNode*
AVLTree::Find(const void* key)
{
	AVLTreeNode* node = fRoot;

	while (node) {
		int cmp = fCompare->CompareKeyNode(key, node);
		if (cmp == 0)
			return node;

		if (cmp < 0)
			node = node->left;
		else
			node = node->right;
	}

	return NULL;
}


AVLTreeNode*
AVLTree::FindClose(const void* key, bool less)
{
	AVLTreeNode* node = fRoot;
	AVLTreeNode* parent = NULL;

	while (node) {
		int cmp = fCompare->CompareKeyNode(key, node);
		if (cmp == 0)
			break;

		parent = node;
		if (cmp < 0)
			node = node->left;
		else
			node = node->right;
	}

	// not found: try to get close
	if (!node && parent) {
		node = parent;
		int expectedCmp = (less ? 1 : -1);
		int cmp = fCompare->CompareKeyNode(key, node);
		if (cmp != expectedCmp) {
			// The node's value is less although we were asked for a greater
			// value, or the other way around. We need to iterate to the next
			// node in the respective direction. If there is no node, we fail.
			if (less)
				return Previous(node);
			return Next(node);
		}
	}

	return node;
}


void
AVLTree::_RotateRight(AVLTreeNode** nodeP)
{
	// rotate the nodes
	AVLTreeNode* node = *nodeP;
	AVLTreeNode* left = node->left;

	*nodeP = left;

	left->parent = node->parent;
	node->left = left->right;
	if (left->right)
		left->right->parent = node;
	left->right = node;
	node->parent = left;

	// adjust the balance factors
	// former pivot
	if (left->balance_factor >= 0)
		node->balance_factor++;
	else
		node->balance_factor += 1 - left->balance_factor;

	// former left
	if (node->balance_factor <= 0)
		left->balance_factor++;
	else
		left->balance_factor += node->balance_factor + 1;
}


void
AVLTree::_RotateLeft(AVLTreeNode** nodeP)
{
	// rotate the nodes
	AVLTreeNode* node = *nodeP;
	AVLTreeNode* right = node->right;

	*nodeP = right;

	right->parent = node->parent;
	node->right = right->left;
	if (right->left)
		right->left->parent = node;
	right->left = node;
	node->parent = right;

	// adjust the balance factors
	// former pivot
	if (right->balance_factor <= 0)
		node->balance_factor--;
	else
		node->balance_factor -= right->balance_factor + 1;

	// former right
	if (node->balance_factor >= 0)
		right->balance_factor--;
	else
		right->balance_factor += node->balance_factor - 1;
}


int
AVLTree::_BalanceInsertLeft(AVLTreeNode** node)
{
	if ((*node)->balance_factor < LEFT) {
		// tree is left heavy
		AVLTreeNode** left = &(*node)->left;
		if ((*left)->balance_factor == LEFT) {
			// left left heavy
			_RotateRight(node);
		} else {
			// left right heavy
			_RotateLeft(left);
			_RotateRight(node);
		}
		return OK;
	}

	if ((*node)->balance_factor == BALANCED)
		return OK;

	return HEIGHT_CHANGED;
}


int
AVLTree::_BalanceInsertRight(AVLTreeNode** node)
{
	if ((*node)->balance_factor > RIGHT) {
		// tree is right heavy
		AVLTreeNode** right = &(*node)->right;
		if ((*right)->balance_factor == RIGHT) {
			// right right heavy
			_RotateLeft(node);
		} else {
			// right left heavy
			_RotateRight(right);
			_RotateLeft(node);
		}
		return OK;
	}

	if ((*node)->balance_factor == BALANCED)
		return OK;

	return HEIGHT_CHANGED;
}


int
AVLTree::_Insert(AVLTreeNode* nodeToInsert)
{
	struct node_info {
		AVLTreeNode**	node;
		bool			left;
	};

	node_info stack[kMaxAVLTreeHeight];
	node_info* top = stack;
	const node_info* const bottom = stack;
	AVLTreeNode** node = &fRoot;

	// find insertion point
	while (*node) {
		int cmp = fCompare->CompareNodes(nodeToInsert, *node);
		if (cmp == 0)	// duplicate node
			return DUPLICATE;
		else {
			top->node = node;
			if (cmp < 0) {
				top->left = true;
				node = &(*node)->left;
			} else {
				top->left = false;
				node = &(*node)->right;
			}
			top++;
		}
	}

	// insert node
	*node = nodeToInsert;
	nodeToInsert->left = NULL;
	nodeToInsert->right = NULL;
	nodeToInsert->balance_factor = BALANCED;
	fNodeCount++;

	if (top == bottom)
		nodeToInsert->parent = NULL;
	else
		(*node)->parent = *top[-1].node;

	// init the iterator
//	if (iterator)
//		*iterator = Iterator(this, *node);

	// do the balancing
	int result = HEIGHT_CHANGED;
	while (result == HEIGHT_CHANGED && top != bottom) {
		top--;
		node = top->node;
		if (top->left) {
			// left
			(*node)->balance_factor--;
			result = _BalanceInsertLeft(node);
		} else {
			// right
			(*node)->balance_factor++;
			result = _BalanceInsertRight(node);
		}
	}

	return result;
}


int
AVLTree::_BalanceRemoveLeft(AVLTreeNode** node)
{
	int result = HEIGHT_CHANGED;

	if ((*node)->balance_factor > RIGHT) {
		// tree is right heavy
		AVLTreeNode** right = &(*node)->right;
		if ((*right)->balance_factor == RIGHT) {
			// right right heavy
			_RotateLeft(node);
		} else if ((*right)->balance_factor == BALANCED) {
			// right none heavy
			_RotateLeft(node);
			result = OK;
		} else {
			// right left heavy
			_RotateRight(right);
			_RotateLeft(node);
		}
	} else if ((*node)->balance_factor == RIGHT)
		result = OK;

	return result;
}


int
AVLTree::_BalanceRemoveRight(AVLTreeNode** node)
{
	int result = HEIGHT_CHANGED;

	if ((*node)->balance_factor < LEFT) {
		// tree is left heavy
		AVLTreeNode** left = &(*node)->left;
		if ((*left)->balance_factor == LEFT) {
			// left left heavy
			_RotateRight(node);
		} else if ((*left)->balance_factor == BALANCED) {
			// left none heavy
			_RotateRight(node);
			result = OK;
		} else {
			// left right heavy
			_RotateLeft(left);
			_RotateRight(node);
		}
	} else if ((*node)->balance_factor == LEFT)
		result = OK;

	return result;
}


int
AVLTree::_RemoveRightMostChild(AVLTreeNode** node, AVLTreeNode** foundNode)
{
	AVLTreeNode** stack[kMaxAVLTreeHeight];
	AVLTreeNode*** top = stack;
	const AVLTreeNode* const* const* const bottom = stack;

	// find the child
	while ((*node)->right) {
		*top = node;
		top++;
		node = &(*node)->right;
	}

	// found the rightmost child: remove it
	// the found node might have a left child: replace the node with the
	// child
	*foundNode = *node;
	AVLTreeNode* left = (*node)->left;
	if (left)
		left->parent = (*node)->parent;
	*node = left;
	(*foundNode)->left = NULL;
	(*foundNode)->parent = NULL;

	// balancing
	int result = HEIGHT_CHANGED;
	while (result == HEIGHT_CHANGED && top != bottom) {
		top--;
		node = *top;
		(*node)->balance_factor--;
		result = _BalanceRemoveRight(node);
	}

	return result;
}


int
AVLTree::_Remove(AVLTreeNode* node)
{
	if (!node)
		return NOT_FOUND;

	// remove node
	AVLTreeNode* parent = node->parent;
	bool isLeft = (parent && parent->left == node);
	AVLTreeNode** nodeP
		= (parent ? (isLeft ? &parent->left : &parent->right) : &fRoot);
	int result = HEIGHT_CHANGED;
	AVLTreeNode* replace = NULL;

	if (node->left && node->right) {
		// node has two children
		result = _RemoveRightMostChild(&node->left, &replace);
		replace->parent = parent;
		replace->left = node->left;
		replace->right = node->right;
		if (node->left)	// check necessary, if node->left == replace
			node->left->parent = replace;
		node->right->parent = replace;
		replace->balance_factor = node->balance_factor;
		*nodeP = replace;

		if (result == HEIGHT_CHANGED) {
			replace->balance_factor++;
			result = _BalanceRemoveLeft(nodeP);
		}
	} else if (node->left) {
		// node has only left child
		replace = node->left;
		replace->parent = parent;
		replace->balance_factor = node->balance_factor + 1;
		*nodeP = replace;
	} else if (node->right) {
		// node has only right child
		replace = node->right;
		replace->parent = node->parent;
		replace->balance_factor = node->balance_factor - 1;
		*nodeP = replace;
	} else {
		// node has no child
		*nodeP = NULL;
	}

	fNodeCount--;

	// do the balancing
	while (result == HEIGHT_CHANGED && parent) {
		node = parent;
		parent = node->parent;
		bool oldIsLeft = isLeft;
		isLeft = (parent && parent->left == node);
		nodeP = (parent ? (isLeft ? &parent->left : &parent->right) : &fRoot);
		if (oldIsLeft) {
			// left
			node->balance_factor++;
			result = _BalanceRemoveLeft(nodeP);
		} else {
			// right
			node->balance_factor--;
			result = _BalanceRemoveRight(nodeP);
		}
	}

	return result;
}
