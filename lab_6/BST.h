#pragma once

#ifndef BST_H
#define BST_H

#include <iostream>
#include <cassert>
#include <vector>

#include "Time.h"

struct BinTreeNode {
	Time data;
	BinTreeNode* left;
	BinTreeNode* right;
};


BinTreeNode* create_treenode(Time data, BinTreeNode* left = nullptr, BinTreeNode* right = nullptr) {
	BinTreeNode* new_node = new BinTreeNode;
	new_node->data = data;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

BinTreeNode* create_bst() {
	BinTreeNode* tree = nullptr;
	return tree;
}

void add(BinTreeNode*& root, Time data) {
	if (root == nullptr) {
		root = create_treenode(data);
	}
	else {
		if (data < root->data) {
			add(root->left, data);
		}
		else if (data > root->data) {
			add(root->right, data);
		}
	}
}

void print_as_tree_impl(BinTreeNode* root) {
	if (!root) { return; }
//	std::cout << root->data << "(";
	std::cout << root->data.year << "("; // only for testing
	if (root->left || root->right) {
		if (root->left) {
			print_as_tree_impl(root->left);
			std::cout << ", ";
		}
		else {
			std::cout << " , ";
		}
		print_as_tree_impl(root->right);

	}
	
	std::cout << ")";
}

void print_as_tree(BinTreeNode* root) {
	if (!root) { std::cout << "\nTree is empty"; }
	else {
		print_as_tree_impl(root);
	}
	std::cout << std::endl;
}

void print_impl(BinTreeNode* root) {
	if (!root) { return; }
	print_impl(root->left);
	std::cout << root->data << std::endl;
	print_impl(root->right);
}

void print(BinTreeNode* root) {
	if (!root) { std::cout << "\nTree is empty\n"; }
	else {
		print_impl(root);
	}
}


BinTreeNode* search_prev_child(BinTreeNode* node) {
	BinTreeNode* current = node->left;
	if (!current) { return current; } // no previous child
	while (current->right) {
		current = current->right;
	}
	return current;
}



BinTreeNode* remove_impl(BinTreeNode* root, Time data) {
	if (root == nullptr) { return nullptr; }
	if (data == root->data) {
		if (root->left) {
			if (root->right) { // both childs are exist
				BinTreeNode* prev = search_prev_child(root);
				assert(prev != nullptr);
				root->data = prev->data;
				root->left = remove_impl(root->left, prev->data);
			}
			else {
				BinTreeNode* to_delete = root;
				root = root->left;
				delete to_delete;
			}
		}
		else {
			if (root->right) {
				BinTreeNode* to_delete = root;
				root = root->right;
				delete to_delete;
			}
			else { // no childs
				delete root; 
				root = nullptr;
			}
		}
	}
	else if (data < root->data) {
		root->left = remove_impl(root->left, data);
	}
	else if (data > root->data) {
		root->right = remove_impl(root->right, data);
	}
	return root;
}


void remove(BinTreeNode*& root, Time data) {
	root = remove_impl(root, data);
}


BinTreeNode* search(BinTreeNode* root, Time data) {
	if (root == nullptr) { return nullptr; }

	if (data == root->data) {
		return root;
	}
	else if (data < root->data) {
		return search(root->left, data);
	}
	else if (data > root->data) {
		return search(root->right, data);
	}
}

void search_impl(BinTreeNode* root, Time start, Time end, std::vector<BinTreeNode*>& result) {
	if (root == nullptr) { return; }

	if (root->data > start && root->data < end) {
		search_impl(root->left, start, end, result); // such order of recursive calls is to get right order as a result
		result.push_back(root);
		search_impl(root->right, start, end, result);
	}
	else if (root->data >= end) {
		search_impl(root->left, start, end, result); 
		if (root->data == end) {
			result.push_back(root);
		}
		
	}
	else if (root->data <= start) {
		if (root->data == start) {
			result.push_back(root);
		}
		search_impl(root->right, start, end, result);
	}
}

std::vector<BinTreeNode*> search(BinTreeNode* root, Time start, Time end) {
	assert(start <= end && "Wrong search interval");
	std::vector<BinTreeNode*> result;
	search_impl(root, start, end, result);
	return result;
}

template <typename Callable>
void process_list(BinTreeNode* root, Callable process) {
	assert(root && "Try to process empty list");

	if (root->left) {
		process_list(root->left, process);
	}
	process(root->data);
	if (root->right) {
		process_list(root->right, process);
	}
}

void add_random_items(BinTreeNode*& root, std::size_t size) {
	for (std::size_t i = 0; i < size; i++) {
		add(root, random_time());
	}
}

void clear(BinTreeNode*& root) {
	if (root == nullptr) { return; }

	clear(root->left);
	clear(root->right);
	BinTreeNode* to_delete = root;
	delete to_delete;
	root = nullptr;
}

unsigned int get_memory_impl(BinTreeNode* root) {
	if (root == nullptr) { return 0; }
	
	return sizeof(BinTreeNode) + get_memory_impl(root->left) + get_memory_impl(root->right);
}

unsigned int get_memory(BinTreeNode* root) {
	return sizeof(BinTreeNode*) + get_memory_impl(root);
}

bool check_order_impl(BinTreeNode* root, Time prev, Time next) {
	if (root == nullptr) { return true; }
	if (check_order_impl(root->left, Time{ 0,0,0,0,0,0 }, root->data) == false || check_order_impl(root->right, root->data, Time{ SHRT_MAX,13,32,24,61,61 }) == false) {
		return false;
	}
	if (root->data > prev && root->data < next) { return true; }
	else { return false; }
}

bool check_order(BinTreeNode* root) {
	return check_order_impl(root, Time{ 0,0,0,0,0,0 }, Time{ SHRT_MAX,13,32,24,61,61 });
}


#endif // BST_H




/*    Need remaking            */
/*
BinTreeNode*& search_prev_child(BinTreeNode*& node) {
	BinTreeNode*& current = node->left;
	if (!current) { return current; } // no previous child
	while (current->right) {
		current = current->right;
	}
	return current;
}


bool remove(BinTreeNode*& node, Time data) {
	if (node == nullptr){ return false; }
	if (data == node->data) {
		if (node->left) {
			if (node->right) { // both childs are exist
				BinTreeNode*& prev = search_prev_child(node);
				assert(prev != nullptr);
				Time prev_data = prev->data;
				node->data = prev->data;
				prev->data = data;
				remove(node->left, data); // probles was, that first parameter was "prev", but *& on it wasn't from its parent, that's why we hadx nullptr in the left subtree
			}
			else {
				BinTreeNode* to_delete = node;
				node = node->left;
				delete to_delete;
			}
		}
		else {
			if (node->right) {
				BinTreeNode* to_delete = node;
				node = node->right;
				delete to_delete;
			}
			else { // no childs
				BinTreeNode* to_delete = node;
				node = nullptr;
				delete to_delete;
			}
		}
		return true;
	}
	else if (data < node->data) {
		return remove(node->left, data);
	}
	else {
		return remove(node->right, data);
	}

}
*/
