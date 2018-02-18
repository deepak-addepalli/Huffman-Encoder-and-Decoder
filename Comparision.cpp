#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

#define Frequency_Table_Size 1000000
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

 
};

class heap_element{
public:
	int freq;
	huffman_tree* tree;
};


class four_way_optimised_heap{
	vector< heap_element > a; 
	
	public:
	four_way_optimised_heap(){
		heap_element temp;
		for(int i=0;i<3;i++)
			a.push_back(temp);
	}

	void insert(heap_element newElem);

	heap_element extract_Min();

	int parent(int i){
		return ((i/4)+2);
	}

	int left(int i){
		return (4*(i-2));
	}

	int size(){
		return a.size();
	}

};

	void four_way_optimised_heap::insert(heap_element newElem){
		a.push_back(newElem);
		int index = a.size() - 1;
		while(index > 3){
			if (a[index].freq < a[parent(index)].freq || 
				(a[index].freq == a[parent(index)].freq && a[parent(index)].tree->size() > a[index].tree->size())){
				swap(a[index], a[parent(index)]);
				index = parent(index);
			}
			else{
				break;
			}
		}
	}

	heap_element four_way_optimised_heap::extract_Min(){
		heap_element min = a[3];
		a[3] = a.back();
		a.pop_back();
		int index = 3;
		int l = left(index);
		while(l < a.size()){
			int temp = l;
			for(int i=1;i<4 && l+i<a.size();i++){
				if (a[temp].freq > a[l+i].freq || (a[temp].freq == a[l+i].freq && a[temp].tree->size() > a[l+i].tree->size()))
					temp = l + i;
			}
			l = temp;
			
			if (a[index].freq > a[l].freq || (a[index].freq == a[l].freq && a[index].tree->size() > a[l].tree->size())){
				swap(a[index], a[l]);
				index = l;
				l = 4*(index - 2);
			}
			else
				break;
		}
		return min;	
	}




class binary_heap{
	vector< heap_element > a;
public:
	void insert(heap_element newElem);

	heap_element extract_Min();

	int parent(int i){
		return (i-1)/2;
	}

	int left(int i){
		return ((2*i)+1);
	}

	int size(){
		return a.size();
	}
	
	huffman_tree* getHuffmanTree(){
		if(a.size()>0){
			return a[0].tree;
		}
		else
			return NULL;
	}

};

	void binary_heap::insert(heap_element newElem){
		a.push_back(newElem);
		int index = a.size() - 1;
		while(index != 0){
			if(a[parent(index)].freq > a[index].freq){
				swap(a[index], a[parent(index)]);
				index = parent(index);
			}
			else
				break;
		}
	}

	heap_element binary_heap::extract_Min(){
		heap_element min = a[0];
		a[0] = a.back();
		a.pop_back();
		int index = 0;
		int l = left(index);
		while(l < a.size()){
			if (l+1 < a.size() && (a[l].freq > a[l+1].freq))
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




class pairing_heap_node{
public:
	vector< pairing_heap_node* > a;
	heap_element elem;
};


class pairing_heap{
	pairing_heap_node* root;
	int sz;
public:
	pairing_heap(){
		root = NULL;
		sz = 0;
	}

	void insert(pairing_heap_node* newElem);

	heap_element extract_Min();

	int size(){
		return sz;
	}

};

	void pairing_heap::insert(pairing_heap_node* newElem){
		sz++;
		if (root == NULL){
			root = newElem;
		}
		else if (root->elem.freq < newElem->elem.freq)
			root->a.push_back(newElem);
		else{
			newElem->a.push_back(root);
			root = newElem;
		}
	}

	heap_element pairing_heap::extract_Min(){
		sz--;
		heap_element min = root->elem;
		vector<pairing_heap_node*> temp;
		for(int i=root->a.size()-1; i>0; i=i-2){
			if (root->a[i]->elem.freq > root->a[i-1]->elem.freq){
				temp.push_back(root->a[i-1]);
				root->a[i-1]->a.push_back(root->a[i]);
			}
			else{
				temp.push_back(root->a[i]);
				root->a[i]->a.push_back(root->a[i-1]);
			}
			root->a.pop_back();
			root->a.pop_back();
		}

		if (root->a.size() > 0){
			temp.push_back(root->a.back());
			root->a.pop_back();
		}

		pairing_heap_node* tempRoot = NULL;
		if (temp.size() > 0){
			tempRoot = temp.back();
			for(int i=temp.size()-2; i>=0; i--){
				if (temp[i]->elem.freq > tempRoot->elem.freq)
					tempRoot->a.push_back(temp[i]);
				else{
					temp[i]->a.push_back(tempRoot);
					tempRoot = temp[i];
				}
			}
		}

		delete(root);
		root = tempRoot;
		return min;
	}

void build_tree_using_four_way_optimised_heap(){
	four_way_optimised_heap heap;
	heap_element firstElement, secondElement;

	for (int i=0;i<Frequency_Table_Size; i++){

		if (freq_table[i] == 0)
			continue;

		firstElement.freq = freq_table[i];
		firstElement.tree = new huffman_tree();
		firstElement.tree->insert_root(new huffman_tree_node(i));
		heap.insert(firstElement);
	}

	while (heap.size() > 4){
		firstElement = heap.extract_Min();
		secondElement = heap.extract_Min();
		firstElement.tree->combine(secondElement.tree);
		firstElement.freq += secondElement.freq;
		heap.insert(firstElement);
	}
}

huffman_tree* build_tree_using_binary_heap(){
	binary_heap heap;
	heap_element firstElement, secondElement;

	for (int i=0;i<Frequency_Table_Size; i++){

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
}


void build_tree_using_pairing_heap(){
	pairing_heap heap;
	heap_element firstElement, secondElement;
	pairing_heap_node* temp;

	for (int i=0;i<Frequency_Table_Size; i++){

		if (freq_table[i] == 0)
			continue;

		firstElement.freq = freq_table[i];
		firstElement.tree = new huffman_tree();
		firstElement.tree->insert_root(new huffman_tree_node(i));
		temp = new pairing_heap_node();
		temp->elem = firstElement;
		heap.insert(temp);
	}

	while (heap.size() > 1){
		firstElement = heap.extract_Min();
		secondElement = heap.extract_Min();
		firstElement.tree->combine(secondElement.tree);
		firstElement.freq += secondElement.freq;
		temp = new pairing_heap_node();
		temp->elem = firstElement;
		heap.insert(temp);
	}
}



int main(){
	ifstream f;
	int var;
	for(int i=0;i<Frequency_Table_Size;i++)
		freq_table[i]=0;
	f.open("/home/deepak/Downloads/sample_input_large.txt");
	while(f >> var){
		freq_table[var]++;
	}
	f.close();
	
	clock_t start_time;

	// 4-way heap
	start_time = clock();
	for(int i = 0; i < 10; i++){
		build_tree_using_four_way_optimised_heap();
	}
	cout << "Time using 4-way heap (microsecond): " << (clock() - start_time)/10 << endl;

	// binary heap
	start_time = clock();
	for(int i = 0; i < 10; i++){
		build_tree_using_binary_heap();
	}
	cout << "Time using binary heap (microsecond): " << (clock() - start_time)/10 << endl;
	
	//pairing heap
	start_time = clock();
	for(int i = 0; i < 10; i++){
		//run 10 times on given data set
		build_tree_using_pairing_heap();
	}
	cout << "Time using pairing heap (microsecond): " << (clock() - start_time)/10 << endl;

}
