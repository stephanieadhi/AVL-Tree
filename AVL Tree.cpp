#include <stdio.h>
#include <stdlib.h>

struct node{
	int val;
	int height;
	struct node *left, *right;
};

struct node *createNewNode(int val){
	struct node *newNode = (struct node*) malloc(sizeof(struct node));
	newNode->val = val;
	newNode->height = 1;
	
	newNode->left = newNode->right = NULL;
	return newNode;
}

int getHeight(struct node *node)
{
	if(node == NULL){
		return 0;
	}
	else{
		return node->height;
	}
}

int getBalance(struct node *node)
{
	if(node == NULL){
		return 0;
	}
	else {
		return getHeight(node->left) - getHeight(node->right);
	}
}

int max(int a, int b){
	return (a > b) ? a : b;
}

struct node *rotateRight(struct node *node)
{
	struct node *lSub = node->left;
	struct node *rlSub = lSub->right;
	
	lSub->right = node;
	node->left = rlSub;
	
	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	lSub->height = max(getHeight(lSub->left), getHeight(lSub->right)) + 1;
	
	return lSub;
}

struct node *rotateLeft(struct node *node)
{
	struct node *rSub = node->right;
	struct node *lrSub = rSub->left;
	
	rSub->left = node;
	node->right = lrSub;
	
	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	rSub->height = max(getHeight(rSub->left), getHeight(rSub->right)) + 1;
	
	return rSub;
}

struct node* insertAVLNode(struct node* node, int val){
	if(node == NULL)
	{
		return createNewNode(val);
	}
	
	if(val < node->val){
		node->left = insertAVLNode(node->left, val);
	}
	else if(val > node->val){
		node->right = insertAVLNode(node->right, val);
	}
	else{
		return node;
	}
	
	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	
	int balance = getBalance(node);
	
	if(balance > 1 && val < node->left->val){
		return rotateRight(node);
	}
	else if(balance < -1 && val > node->right->val){
		return rotateLeft(node);
	}
	else if(balance > 1 && val > node->left->val) 
	{
		node->left = rotateLeft(node->left);
		return rotateRight(node);
	}
	else if(balance < -1 && val < node->right->val) 
	{
		node->right = rotateRight(node->right);
		return rotateLeft(node);
	}
	return node;
}

struct node *getMaxNode(struct node *root)
{
	while(root->right!=NULL)
		root = root->right;
	return root;
}

struct node *deletion(struct node *root, int val)
{
	if(root == NULL)
		return root;
	
	else if(val < root->val)
		root->left = deletion(root->left, val);
	else if(val > root->val)
		root->right = deletion(root->right, val);
	else
	{
		if(root->left == NULL && root->right == NULL)
		{
			free(root);
			root = NULL;
			return root;
		}
		else if(root->left == NULL)
		{
			struct node *temp = root;
			root = root->right;
			free(temp);
		}
		else if(root->right == NULL)
		{
			struct node *temp = root;
			root = root->left;
			free(temp);
		}
		else
		{
			struct node *temp = getMaxNode(root->left);
			root->val = temp->val;
			root->left = deletion(root->left, temp->val);
		}	
	}
	
	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	int balance = getBalance(root);
	
	if(balance > 1 && getBalance(root->left) >= 0)
		return rotateRight(root);
	else if(balance < -1 && getBalance(root->right) <= 0)
		return rotateLeft(root);
	else if(balance>1 && getBalance(root->left) < 0)
	{
		root->left = rotateLeft(root->left);
		return rotateRight(root);
	}
	else if(balance < -1 && getBalance(root->right) > 0)
	{
		root->right = rotateRight(root->right);
		return rotateLeft(root);
	}
	return root;
}

struct node *search(struct node *root, int val)
{
	if(root == NULL || root->val == val){
		return root;
	}
	else if(val < root->val){
		return search(root->left, val);
	}
	else if(val > root->val){
		return search(root->right, val);
	}
}

void preOrder(struct node *node){
	if(node == NULL) return;
	printf(" %d", node->val);
	preOrder(node->left);
	preOrder(node->right);
}

void inOrder(struct node *node){
	if(node == NULL) return;
	inOrder(node->left);
	printf(" %d", node->val);
	inOrder(node->right);
}

void postOrder(struct node *node){
	if(node == NULL) return;
	postOrder(node->left);
	postOrder(node->right);
	printf(" %d", node->val);
}

int main(){
	int choose, val;
	struct node *root = NULL;
	struct node *find = NULL;
	do{
		printf("1. Insertion\n");
		printf("2. Deletion\n");
		printf("3. Traversal\n");
		printf("4. Exit\n");
		printf("Choose: ");
		scanf("%d", &choose);
		switch(choose){
			case 1:
				printf("Insert: ");
				scanf("%d", &val);
				root = insertAVLNode(root, val);
				break;
			case 2:
				printf("Delete: ");
				scanf("%d", &val);
				find = search(root, val);
				if(find == NULL){
					printf("Data not found\n");
				}
				else{
					printf("Data Found\n");
					root = deletion(root, val);
					printf("Value %d was deleted\n", val);
				}
				break;
			case 3:
				printf("Preorder:");
				preOrder(root);
				printf("\nInorder:");
				inOrder(root);
				printf("\nPostorder:");
				postOrder(root);
				puts("");
				break;
			case 4:
				printf("Thank you");
				break;
		}
	}while(choose!=4);
	
	return 0;
}
