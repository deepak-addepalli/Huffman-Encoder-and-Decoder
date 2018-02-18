#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

#define FREQ_TABLE_SIZE 1000000
int freq_table[1000000];
string code_table[1000000];


class huffman_tree_node{
public:	
	huffman_tree_node* left;
	huffman_tree_node* right;
	int val;
	huffman_tree_node(){
		left = NULL;
		right = NULL;
		val = -1;
	}

	huffman_tree_node(int v){
		left = NULL;
		right = NULL;
		val = v;
	}
};


class huffman_tree{
	huffman_tree_node* root;
	int sz;
public:
	huffman_tree(){
		root = NULL;
		sz = 0;
	}

	huffman_tree_node* getRoot(){
		return root;
	}

	void insert_root(huffman_tree_node* node){
		root = node;
		sz++;
	}

	void delete_subTree(huffman_tree_node* root){
		if (root == NULL)
			return;
		delete_subTree(root->left);
		delete_subTree(root->right);
		delete(root->left);
		delete(root->right);
	}

	void delete_tree(){
		delete_subTree(root);
		delete(root);
	}

	void combine(huffman_tree* second){
		huffman_tree_node* newRoot = new huffman_tree_node();
		newRoot->left = root;
		newRoot->right = second->root;
		sz += second->size();
		root = newRoot;
		delete(second);
	}

	int size(){
		return sz;
	}

	void traverse(huffman_tree_node* node, char* ch, int len){
		if (node == NULL)
			return;
		if (node->left == NULL && node->right == NULL){
			code_table[node->val].assign(ch, len); 
		}
		else{
			ch[len] = '0';
			traverse(node->left, ch, len+1);
			ch[len] = '1';
			traverse(node->right, ch, len+1);
		}
	}

};



class heap_element{
public:
	int freq;
	huffman_tree* tree;
};

class binary_heap{
	vector< heap_element > a;
public:
	void insert(heap_element newElem);

	heap_element extract_Min();

	int size(){
		return a.size();
	}

	int parent(int i){
		return (i-1)/2;
	}

	int left(int i){
		return (2*i) + 1;
	}

	huffman_tree* getHuffmanTree(){
		if (a.size() > 0)
			return a[0].tree;
		else
			return NULL;
	}

};

void binary_heap::insert(heap_element newElem){
		a.push_back(newElem);
		int index = a.size() - 1;
		while(index != 0){
			if (a[index].freq < a[parent(index)].freq){
				swap(a[index], a[parent(index)]);
				index = parent(index);
			}
			else{
				break;
			}
		}
	}

heap_element binary_heap::extract_Min(){
		heap_element min = a[0];
		a[0] = a.back();
		a.pop_back();
		int index = 0;
		int l = left(index);
		while(l < a.size()){
			if (l+1 < a.size() && a[l].freq > a[l+1].freq)
				l++;
			
			if (a[index].freq > a[l].freq){
				swap(a[index], a[l]);
				index = l;
				l = left(index);
			}
			else
				break;
		}
		return min;
	}


// End of heap structures



huffman_tree* build_tree_using_binary_heap(){
	binary_heap heap;
	heap_element firstElement, secondElement;

	for (int i=0;i<FREQ_TABLE_SIZE; i++){

		if (freq_table[i] == 0)
			continue;

		firstElement.freq = freq_table[i];
		firstElement.tree = new huffman_tree();
		firstElement.tree->insert_root(new huffman_tree_node(i));
		heap.insert(firstElement);
	}

	while (heap.size() > 1){
		firstElement = heap.extract_Min();
		secondElement = heap.extract_Min();
		firstElement.tree->combine(secondElement.tree);
		firstElement.freq += secondElement.freq;
		heap.insert(firstElement);
	}

	return heap.getHuffmanTree();
}



void encoder(char* str){
	ifstream f;
	ofstream o;
	int var;
	int len=0, tmp=0;
	f.open(str);
	o.open("encoded.bin", std::ios::binary);
	while(f >> var){
		for(int i=0; i<code_table[var].size(); i++){
			if (code_table[var][i] == '1')
				tmp += (1 << len);
			len++;
			if (len == 8){
				o << (char)tmp;
				len = 0;
				tmp = 0;
			}
		}
	}
	f.close();
	o.close();

	o.open("code_table.txt");
	for(int i=0;i<FREQ_TABLE_SIZE ;i++){
		if (code_table[i] != ""){
			o<<i<<" "<<code_table[i]<<endl;
		}
	}
	o.close();

}


int main(int argc, char *argv[]){
	if (argc != 2){
		return 0;
	}
	ifstream f;
	int var;
	for(int i=0;i<FREQ_TABLE_SIZE;i++)
		freq_table[i]=0;
	f.open(argv[1]);
	while(f >> var){
		freq_table[var]++;
	}
	f.close();
	
	huffman_tree* tree = build_tree_using_binary_heap();
	if (tree == NULL)
		return 0;
	else if (tree->size() == 1){
		code_table[tree->getRoot()->val] = "0";
	}
	else{
		char ch[1000001];
		tree->traverse(tree->getRoot(), ch, 0);
	}
	encoder(argv[1]);
	return 0;
}
