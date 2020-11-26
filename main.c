#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>

typedef struct space
    {
        size_t size; 
        int isFree; //0 if memory is free
        struct space *next; //points to next block
    }space_t;

space_t *list;
void firstAllocate()
{
    list=(space_t*)sbrk(8192);
    list->size=8192;
    list->isFree=1;
    list->next=NULL;
}

void breakUp(space_t *whole, size_t amount)
{
    space_t *created; //=not too sure right now
    created->size=(whole->size)-amount -sizeof(space_t); //metadata
    created->next=NULL;
    created->isFree=0; //set created as free
    whole->size=amount;
    whole->isFree=1;
    whole->next=created;
    
}

void together()
{
    space_t *holder, *last;
    holder=list;
    while(holder->next!=NULL)
    {
        if((holder->isFree==0) && (holder->next->isFree==0)) //if its free and the next is free
        {
            holder->size=(sizeof(space_t))+(holder->size)+(holder->next->size); //the size of is now both spaces
            holder->next=holder->next->next;
        }
        last=holder;
        holder=holder->next;
    }
}

void * new_malloc(size_t need)
{
    space_t *holder,*last;  //holds the current block and the previous block
    if(list->size==0)
    {
        firstAllocate();
    }
    holder=list;

    while(((holder->size)<need) || ((holder->isFree)==0) && ((holder->next)!=NULL))
    {
        last=holder;
        holder=holder->next;
    }

    if((holder->size)==need)
    {
        holder->isFree=0;
        return holder;
    }
    else if((holder->size)>need)
    {
        breakUp(holder, need);
        return holder;
    }


}
void new_free(void *ptr)
{
    space_t *holder=ptr;
    holder->isFree=0; //set it to free
}

    int main()
    {
       firstAllocate();
       new_malloc(300);
       printf("wo");

    }