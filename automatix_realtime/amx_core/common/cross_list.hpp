#include <iostream>
#include <vector>

class CrossListNode {
public:
	int data;
	int row, col; // 节点所在的行和列
	CrossListNode* right, * down; // 指向同行的下一个节点和同列的下一个节点

	CrossListNode(int data = 0, int row = -1, int col = -1)
		: data(data), row(row), col(col), right(nullptr), down(nullptr) {}
};

class CrossLinkedList {
private:
	std::vector<CrossListNode*> rowSentinels; // 行哨兵节点
	std::vector<CrossListNode*> colSentinels; // 列哨兵节点
	int rows, cols;

public:
	CrossLinkedList(int rows, int cols) : rows(rows), cols(cols) {
		// 初始化行和列的哨兵节点
		for (int i = 0; i < rows; ++i) {
			rowSentinels.push_back(new CrossListNode(-1, i, -1));
		}
		for (int j = 0; j < cols; ++j) {
			colSentinels.push_back(new CrossListNode(-1, -1, j));
		}
	}

	~CrossLinkedList() {
		// 清理所有节点，包括哨兵节点
		for (auto sentinel : rowSentinels) {
			CrossListNode* current = sentinel;
			while (current != nullptr) {
				CrossListNode* toDelete = current;
				current = current->right;
				delete toDelete;
			}
		}
		// 列哨兵节点没有额外的节点，所以不需要额外清理
	}

	void insert(int row, int col, int data) {
		if (row >= rows || col >= cols) return; // 越界检查

		CrossListNode* newNode = new CrossListNode(data, row, col);

		// 插入到行
		insertIntoRow(rowSentinels[row], newNode);

		// 插入到列
		insertIntoCol(colSentinels[col], newNode);
	}

private:
	void insertIntoRow(CrossListNode* rowSentinel, CrossListNode* newNode) {
		CrossListNode* current = rowSentinel;
		while (current->right != nullptr && current->right->col < newNode->col) {
			current = current->right;
		}
		newNode->right = current->right;
		current->right = newNode;
	}

	void insertIntoCol(CrossListNode* colSentinel, CrossListNode* newNode) {
		CrossListNode* current = colSentinel;
		while (current->down != nullptr && current->down->row < newNode->row) {
			current = current->down;
		}
		newNode->down = current->down;
		current->down = newNode;
	}

public:
	void print() {
		// 打印十字链表的方法，用于验证结构正确性
		for (int i = 0; i < rows; ++i) {
			CrossListNode* current = rowSentinels[i]->right;
			while (current != nullptr) {
				std::cout << "Value at (" << current->row << ", " << current->col << ") = " << current->data << std::endl;
				current = current->right;
			}
		}
	}
};