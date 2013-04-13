/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Team Freetrees",
    /* First member's full name */
    "James E. Jerram",
    /* First member's email address */
    "jej214@lehigh.edu",
    /* Second member's full name (leave blank if none) */
    "Emily Livingston",
    /* Second member's email address (leave blank if none) */
    "ecl214@lehigh.edu"
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define POINTERSIZE (sizeof(int*))

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define HEADSIZE (POINTERSIZE*2+sizeof(size_t)+sizeof(int))

//Header contains 2 pointers, previous and next, size and free
#define DECAPITATE(ptr)  (ptr+HEADSIZE) 

#define FRANKENSTEIN(ptr) (ptr-HEADSIZE) //I AM PLAY GOD

#define FREE(ptr) (*(ptr+(POINTERSIZE*2+sizeof(size_t))))

#define SIZE(ptr) (*(ptr+(POINTERSIZE*2)))
#define NEXT(ptr) (ptr+POINTERSIZE)
#define PREV(ptr) (ptr)


static const int L = 0;
static const int R = 1;
static const int SZ = 1;
static const int LC = 0;
static const int N = 2;

static const int INTSIZE;
static const int INTPTRSIZE;

//true and false
static const int T = 1;
static const int F = 0;


//last allocated block
void *last;

//first block
void *head;

//largest free block
int lgfree;


static void * extend_heap(size_t words){
  char *bp;
	size_t size;
	
	/*Allocate an even number of words to maintain alignment */
	if((long)(bp=mem_sbrk(ALIGN(size)) == -1)
		return NULL;
	
	/*initialize free block header/footer*/
	
	CODE GOES HERE PLZ
	
	/*coalesce if the previous block was free */
	
	return coalesce(bp);
}

//uses header structure to get size of a block
static int mm_getsize(int *p){
	return p[0];
}

//p is the block pointer, t is 0 for size tree, 1 for loc tree, 
//c is 0 for left, 1 for right, (probably should define these as consts)
//and 2 for linear list next
static int * mm_getChild(int **p, int t, int c){
	//uses header structure to get child in either tree
	if(t = 0){
		if(c = 0){
			return *(p + INTSIZE + 2*INTPTRSIZE);
		}
		else if (c = 1){
			return *(p + INTSIZE + 3*);
		}
		else if (c = 2){
			return *(p + INTSIZE + 4*INTPTRSIZE);
		}
		else
			return NULL;
	}
	else if(t = 1){
		if(c = 0){
			return *(p + INTSIZE);
		}
		else if (c = 1){
			return *(p + INTSIZE + INTPTRSIZE);
		}
		else
			return NULL;
	}
	else
		return NULL;

}

static void mm_push(int **p, int **head){
	//pushes a block to the top of a linear list (used to put block in size tree)
	getNext(p)=getNext(head);
	getNext(head)=p;
}

static int * mm_pop(int **head){
	//removes the top block from a linear list (returns NULL if head is only block) (used to allocate blocks in size tree)
	return getNext(head)
}

static int * mm_loc_free_add(int *p){ //Accepts a pointer to free memory to be added to the location-based tree
	int** temp = loc_head; //head of trees
	while(p != temp){ //While p is not equal to temp
		if(p>temp){ //if p is greater than temp
			int *rtchld = mm_getChild(temp,1,1);
			if(rtchld==NULL){ //if it's the end of the tree add temp to it and break out of the loop
				rtchld=p;
				return temp; //returns temp to check if it is adjacent
			}
			else
				temp=rtchld; //otherwise temp = pointer at index of loc_right_child pointer
		}
		else{ //else (p is less than temp
			int *lchld = mm_getChild(temp,1,0)
			if(lchld==NULL){ //if it's the end of the tree add temp to it and break out of the loop
				lchld=p;
				return temp; //returns temp to check if it is adjacent
			}
			else
				temp=lchld; //otherwise temp = pointer at index of loc_left_child pointer
		}
	}
	return NULL; //already in tree error code
}

//Accepts a pointer to a pointer to free memory to be added to the size-based tree NOTE: This is not called if p has been concatenated to another block
static int mm_size_free_add(int **p){
	int** temp = head; //head of trees
	int psize = mm_getsize(p);
	int tsize = mm_getsize(t);
	while(p != temp){ //While p is not equal to temp
		tsize = mm_getsize(t);
		if(psize>tsize){ //if p size is greater than temp size
			int *rtchld = mm_getChild(temp, 0, 1);
			if(rtchld==NULL){ //if it's the end of the tree add temp to it and break out of the loop
				rtchld=p;
				return 1; //added right code
				}
			else
				temp=rtchld; //otherwise temp = pointer at index of loc_right_child pointer
		}
		else{ //else (p is less than temp
			int *lchld = mm_getChild(temp, 0, 0);
			if(lchld==NULL){ //if it's the end of the tree add temp to it and break out of the loop
				lchld=p;
				return 0; //added left code
				}
			else
				temp=lchld; //otherwise temp = pointer at index of loc_left_child pointer
		}
	}
	//if p size == temp size then add p to the top of the linear list at temp
	if(psize == tsize){
		int *pnext = mm_getChild(p, 0, 2);
		int *tnext = mm_getChild(temp, 0, 2);
		pnext = tnext;
		tnext=p;
	}
}

//checks to see if pointers in free list are adjacent, returns 0 if not adjacent, returns 1 if p is greater, returns 2 if temp is greater
//use p[s] temp[s] to find next block for each, use p and temp to see which comes first
static int mm_coalesce_check(int *p, int *temp) {
	//use p[s] temp[s] to find next block for each, use p and temp to see which comes first
	int pfirst = (p+getSize(p)==temp); //checks if next block after p is temp
	int tfirst = (temp+getSize(temp)==p); //checks if next block after temp is p
	if(pfirst==1) 
		return 2;
	else if(tfirst==1) 
		return 1;
	else 
		return 0;
}

//uses coalesce check, coalesce, mm_size_free_add and mm_loc_free_add to place a block in the list
static int mm_free_list() //puts a freed block in the list{
//uses coalesce check, coalesce, mm_size_free_add and mm_loc_free_add to place a block in the list
	int **temp = mm_loc_free_add(p); //node above added block
	int coalesce = coalesce_check(p, temp); //checks if it and the node next to it need to be coalesced, and how
	if(coalesce == 1){ //coelesces p to temp
		mm_size_free_remove(temp); //remove temp from size tree
		mm_loc_free_remove(p); //remove p from loc tree
		coalesce(temp, p); //coalesces p to temp
		mm_size_free_add(temp); //readds temp to size tree
		return 0; //returns coalesced p (therefore not added) code
	}
	else if(coalesce == 2){ //coalesces temp to p
		coalesce(p, temp); //coalesces temp to p
		sizeadd = mm_size_free_add(p); //adds p to size tree
		mm_size_free_remove(temp); //removes temp from size tree
		mm_loc_free_remove(temp); //removes temp from loc tree
		return 2; //returns coalesced to p code
	}
	else if(coalesce == 0){
		sizeadd = mm_size_free_add(p);
		return 1; //returns added normally code
	}
	return -1; //return error code (should never get here)
}


static int mm_foster(int **head, int **orphanL, int **orphanR){
	
}


//removes a block from the size tree due to allocation or size increase from coalescing with another block
//Must take into account the linear list of duplicate sized blocks
static int mm_size_free_remove(int **p){
		//removes a block from the size tree due to allocation or size increase from coalescing with another block
	//Must take into account the linear list of duplicate sized blocks
	int** temp = size_head; //size tree
	int **head; //current head
	int lr; //left or right
	while(mm_getsize(p)!=mm_getsize(temp)){ //while we aren't at temp yet
		if(mm_getsize(p)<mm_getsize(temp)){ //if temp is greater
			head = temp; //make last temp head
			temp = getChild(temp, SZ, R); //get right child
			lr=R; //temp was right child
		}
		if(mm_getsize(p)>mm_getsize(temp)){ //if temp is less
			head = temp; //make last temp head
			temp = getChild(temp, SZ, L); //get left child
			lr=L; //temp was left child
		}
	}
	if(lr==L){
	//deal with orphaned children recursively
	}
	else if(lr==R){
	
	}
}
}

//removes a block from the location tree due to allocation or being coalesced with another block
static int mm_loc_free_remove(int **p){
		//removes a block from the location tree due to allocation or being coalesced with another block
		int** temp = loc_head;	//location tree
	int **head;	//current head
	int lr;	//left or right
	while(mm_getsize(p)!=mm_getsize(temp)){	//while we aren't at temp yet
		if(mm_getsize(p)<mm_getsize(temp)){	//if temp is greater
			head = temp;	//make last temp head
			temp = getChild(temp, SZ, R);	//get right child
			lr=R;	//temp was right child
		}
		if(mm_getsize(p)>mm_getsize(temp)){ //if temp is less
			head = temp;	//make last temp head
			temp = getChild(temp, SZ, L);	//get left child
			lr=L;	//temp was left child
		}
	}
	
	if(lr==L){
		foster(head, getchild(head, SZ, L), 
	}
	else if(lr==R){
	
	}
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void){
	//NEED TO SET HEAD AND LAST
	int newsize = ALIGN(HEADSIZE);
 	int *p = mem_sbrk(newsize);
 	INTSIZE = sizeof(int);
 	INTPTRSIZE = sizeof(int *);
  	if (p == (void *)-1)
  		return NULL;
  	else {
  		SIZE(p) = size; //store size of block (usable)
		PREV(p) = NULL; //store last block
		FREE(p) = 0; //set not free
		NEXT(p) = NULL; //set next to null
		last = p; //set last to current pointer
		lgfree=0;
    	return 0;
	}
}

/* split - splits block bp so that two new blocks are created
 * the block with block size == requested size will be adjacent
 * to the specified "side" of the block
 * the "remainder" block will consist of the unused portion of the split block
 * if side = 1, the "non-remainder" block should begin after PREV(bp)   PPPPP nnnnn rrrrrrr NNNN
 * if side = 2, the "non-remainder" block should end before NEXT(bp)  PPPPP rrrrrrrr nnnnn NNNNN
 * add both blocks to the trees and return with a pointer to the "non-remainder" block */
static void *split(size_t s, int side, int **bp){
}

/*
 *mm_malloc - Allocates a block with header for use by realloc and
 *free
 *Header is the length of one pointer, the returned pointer is
 *actually the length of one pointer beyond the beginning of the
 *allocated range. Free and realloc take this into account. The
 *contents of the header includes a pointer to the next allocated
 *region, ending in a one if the current region is free. (The pointer
 *will never end in one otherwise under normal circumstances)
 */
void *mm_malloc(size_t size){
	
	//the current block being compared to requirements
	int *block = head;
	//the minimum sized block still large enough to fill the request
	int *min = NULL;
	//whether the tree has been fully searched
	int finished = F;
	
	while(!finished){
		int s = mm_getsize(block);
		//if the block we're looking at is equal to the request,
		//exit the loop, we won't find a better fit
		//oh shit there's a list thing
		if(s == size){
			min = block;
			finished = T;
		}
		//if the current block is larger than the requested size,
		//load its smaller child for another loopthrough
		//we are not finished
		else if(s > size){
			min = block;
			block = mm_getchild(block,1,0);
		}
		//if the current block is too small to be used,
		//min remains at its previous value and the loop is finished
		else if(s < size)
			finished = T;
	}
	
	//if no suitable free block was found
	if (min == NULL){
		//extend the heap
		min = extend_heap(size);
		//if extend_heap was unsuccessful...
		//should probably put error code here maybe?
		if (min == -1)
			return NULL;
	}
	
	//if the block chosen is exactly equal to the requested size
	//just change the "allocated? y/n" to "yes" and remove from "freetrees"
	if (mm_getsize(min) == size){
		FREE(min) = T;
		mm_size_free_remove(min);
		mm_loc_free_remove(min);
	}
	
	//if it is not equal, split the block
	//we're putting "big" requests on the end closer to other "big" allocated blocks
	//and "small" requests on the other end
	else{
		//alloc keeps track of the # of adjacent allocated blocks
		int alloc;
		//sum is the sum of their sizes
		int sum;
		//pointers to adjacent allocated blocks
		int *prev = PREV(min);
		int *next = NEXT(min);
		//pointers to distinguish the relative sizes of the adjacent blocks
		int *lg;
		int *sm;
		int isBig = F;
		int usePlace = F;
		
		//pointer to newly allocated block
		int *n = NULL;
		
		//if the previous block has been allocated, add it to sum and increment avg
		if (!FREE(prev)){
			sum = sum + SIZE(prev);
			avg++;
		}
		
		//same for next block
		if (!FREE(next)){
			sum = sum + SIZE(next);
			avg++;
		}
		
		//is the request a "big" request?		
		//only if it is larger than (or equal to) the average of the adjacent blocks
		if(alloc == 2){
			//assign roles to adjacent blocks
			if(SIZE(prev)>=SIZE(next)){
				lg = prev;
				sm = next;
			}
			else{
				lg = next;
				sm = prev;
			}
			if((sum/2) <= size){
				isBig = T;
				usePlace = T;
			}
		}
		
		//if both adjacent blocks aren't allocated,
		//just place the new block on the left side of the block we're splitting
		
		if(usePlace){
			if(isBig){
				if(&prev == &lg)
					n = split(size, 1, min);
				else if(&next == &lg)
					n = split(size, 2, min;
			}
			else{
				if(&prev == &sm)
					n = split(size, 1, min);
				else if(&next == &sm)
					n = split(size, 2, min);
			}

		}
		
		else
			n = split(size, 1, min);

		if(n==NULL)
			error;
		else{
			FREE(n) = T;
			mm_size_free_remove(n);
			mm_loc_free_remove(n);
			return n;
	}
}


/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr){

}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size){
	
	//pointer to returned block
	int *bp = NULL;

	if(ptr==NULL)
		return mm_malloc(size);
	if(size == 0)
		return free(ptr);
	int *next = NEXT(ptr);
	if(FREE(next)){
		if ((SIZE(next)+SIZE(ptr)) >= size)
			next block becomes part of ptr, pad payload section where needed
		else if(&next == &last){
			extend heap
			merge with next
			merge next with ptr
			pad payload section
		}
		return bp;
	}
	else if(&ptr == &last){
		extend heap
		merge resulting space with ptr
		pad payload section
		
		merge resulting space
		
		return bp;
	}
	
	else 
		bp = mm_malloc(size);
		copy payload from ptr to bp
		free(ptr);
		return bp;
		
