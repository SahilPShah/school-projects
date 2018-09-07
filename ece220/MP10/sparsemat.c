#include "sparsemat.h"
#include <stdio.h>
#include <stdlib.h>

/*Sahil Shah
*sahils2
*This program is designed to perform operations on a sparse matrix
*A sparse matrix is a matrix with mostly zeroes. For the purpose of these operations. Add the nonzero values to the linked list
*save the values in a new file using the save_tuples() function
*/



sp_tuples *load_tuples(char* input_file)
{
    sp_tuples *matrix = malloc(sizeof(sp_tuples));
    FILE *inputFile = fopen(input_file,"r");
    if(inputFile == NULL){//if the file doesnt have access or user doesnt have permission
        printf("Error: NULL POINTER");
        return matrix;
    }
    int row,col;//rows and columns
    double val;//value
    fscanf(inputFile,"%d %d", &row, &col);
    while(fscanf(inputFile, "%d %d %lf", &row, &col, &val)==3) //read three numbers from the file
	{
		set_tuples(matrix, row, col, val); //adds cell to matrix
		fscanf(inputFile, "\n"); //reads new line character
	}
	fclose(inputFile);
    return matrix;
}
double gv_tuples(sp_tuples * mat_t,int row,int col)
{
    sp_tuples_node *node = mat_t->tuples_head; //pointer to the current node
	while ((node!=NULL) && node->row!=row && node->col!=col)//iterate thru the list until the value has been found
		node=node->next;
	if (node==NULL)
   		return 0; //not in matrix
   	else
   		return node->value; //return the value of the cell
    return 0;
}

void save_tuples(char * file_name, sp_tuples * mat_t){
    FILE *inputFile= fopen(file_name, "w"); //opens the file for writing
    if(inputFile==NULL){
        printf("Error: NULL POINTER");
        return;
    }
    fprintf(inputFile, "%d %d", mat_t->m, mat_t->n); //write the dimensions
    sp_tuples_node *node = mat_t->tuples_head;
    while (node!=NULL){//keep iterating as long as a node exists
        fprintf(inputFile, "\n%d %d %f", node->row, node->col, node->value); //write new line of matrix
        node=node->next; //get next node
    }
    fclose(inputFile);
    return;
}

void set_tuples(sp_tuples * mat_t, int row, int col, double value){
    sp_tuples_node **matPTR = &(mat_t->tuples_head);//pointer to the pointer to the head node
    while ((*matPTR)!=NULL && ((*matPTR)->row < row || ((*matPTR)->row==row && (*matPTR)->col<col))){
        //keep iterating while the node isn't null and we havent reached the row or are on the row
        //and havent reached the the col
    		matPTR = &((*matPTR)->next);//set the pointer to the next node
    }
    if(*matPTR!= NULL && (*matPTR)->row==row && (*matPTR)->col==col){//if the node isnt null and the row and col are correct
        if(value==0){//delete the node
            sp_tuples_node *toDelete = *matPTR;//temp pointer to store the pointer to the node to delete
            *matPTR=toDelete->next;//set the pointer to the current node to the next node
            free(toDelete);//free the node
        }
        else//if not zero
            (*matPTR)->value = value;//set the value
    }
    else{//either the node is null or the doesnt exist
        if(value == 0)//do nothing
            return;
        else{//create a new node and add it;
            sp_tuples_node *newNode = malloc(sizeof(sp_tuples_node));//new node to exist
            //initialize newNode
            newNode->row = row;
            newNode->col = col;
            newNode->value = value;
            newNode->next = *matPTR;
            *matPTR = newNode;
            //increment number of nodes in the list
            mat_t->nz++;
        }
	return;
}
}
sp_tuples *add_tuples(sp_tuples * matA, sp_tuples * matB){
    if (matA->m != matB->m || matA->n != matB->n) //check if the matrices are the same size
		return NULL;
	//create new matrix
	sp_tuples *matrix = (sp_tuples *) malloc(sizeof(sp_tuples));//result of the sum
    //initialize
	matrix->m = matA->m;
	matrix->n = matA->n;
	matrix->nz = 0;
	matrix->tuples_head = NULL;
	sp_tuples_node *node = matA->tuples_head;
	while (node!=NULL) //copies matrix A into matrix B
	{
		set_tuples(matrix, node->row, node->col, node->value);
		node = node->next;
	}
	node = matB->tuples_head;
	while (node!=NULL) //adds matrix B into all cells in the new matrix
	{
		set_tuples(matrix, node->row, node->col, node->value+gv_tuples(matrix, node->row, node->col));
		node = node->next;
	}
	return matrix;
}

sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){
    if (matA->n != matB->m) //checks that the matrices can be multiplied together
    		return NULL;
    	//creates the matrix to be returned
        sp_tuples *mult = (sp_tuples *) malloc(sizeof(sp_tuples));//result of the multiplication
        //initialize
        mult->m = matA->m;
        mult->n = matB->n;
        mult->nz = 0;
    	mult->tuples_head = NULL;
        sp_tuples_node *nodeA = matA->tuples_head;
        while (nodeA != NULL) //iterate through every cell in matrix A
        {
        	sp_tuples_node *nodeB = matB->tuples_head;
        	while (nodeB != NULL && nodeA->col != nodeB->row && nodeA->col > nodeB->row) //finds elements in matB with iB = jA
        		nodeB = nodeB->next;
        	if (!(nodeB == NULL || nodeA->col != nodeB->row))
        	{
        		while (nodeB != NULL && nodeA->col == nodeB->row) //iterate through elements in matB with iB = jA
        		{
        			set_tuples(mult, nodeA->row, nodeB->col, nodeA->value*nodeB->value+gv_tuples(mult, nodeA->row, nodeB->col)); //C(iA, jB) = C + A(iA, jA) * B(iB, jB)
        			nodeB = nodeB->next;
        		}
        	}
        	nodeA = nodeA->next;
        }
    return mult;
}
//private function to free all the nodes from the
void freeNodes(sp_tuples_node *node){
    //free all the nodes
    if(node->next!=NULL)//as long as there are still nodes left
        freeNodes(node->next);
    free(node);
    return;
}

void destroy_tuples(sp_tuples * mat_t){
    freeNodes(mat_t->tuples_head);//free all the nodes in the list
    free(mat_t);//free the list
    return;
}
