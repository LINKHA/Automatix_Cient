#include <iostream>
#include <vector>

class CrossListNode {
public:
	int data;
	int row, col; // �ڵ����ڵ��к���
	CrossListNode* right, * down; // ָ��ͬ�е���һ���ڵ��ͬ�е���һ���ڵ�

	CrossListNode(int data = 0, int row = -1, int col = -1)
		: data(data), row(row), col(col), right(nullptr), down(nullptr) {}
};

class CrossLinkedList {
private:
	std::vector<CrossListNode*> rowSentinels; // ���ڱ��ڵ�
	std::vector<CrossListNode*> colSentinels; // ���ڱ��ڵ�
	int rows, cols;

public:
	CrossLinkedList(int rows, int cols) : rows(rows), cols(cols) {
		// ��ʼ���к��е��ڱ��ڵ�
		for (int i = 0; i < rows; ++i) {
			rowSentinels.push_back(new CrossListNode(-1, i, -1));
		}
		for (int j = 0; j < cols; ++j) {
			colSentinels.push_back(new CrossListNode(-1, -1, j));
		}
	}

	~CrossLinkedList() {
		// �������нڵ㣬�����ڱ��ڵ�
		for (auto sentinel : rowSentinels) {
			CrossListNode* current = sentinel;
			while (current != nullptr) {
				CrossListNode* toDelete = current;
				current = current->right;
				delete toDelete;
			}
		}
		// ���ڱ��ڵ�û�ж���Ľڵ㣬���Բ���Ҫ��������
	}

	void insert(int row, int col, int data) {
		if (row >= rows || col >= cols) return; // Խ����

		CrossListNode* newNode = new CrossListNode(data, row, col);

		// ���뵽��
		insertIntoRow(rowSentinels[row], newNode);

		// ���뵽��
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
		// ��ӡʮ������ķ�����������֤�ṹ��ȷ��
		for (int i = 0; i < rows; ++i) {
			CrossListNode* current = rowSentinels[i]->right;
			while (current != nullptr) {
				std::cout << "Value at (" << current->row << ", " << current->col << ") = " << current->data << std::endl;
				current = current->right;
			}
		}
	}
};