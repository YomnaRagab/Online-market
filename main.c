#include "types.h"
#include "protos.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
u8 line[orders][max];
u8 line1[orders][max];
u8 total_req;
u8 total_database;
u8 temp_total_req;
volatile strsort arr_datbase[200];
volatile strsort arr_requests[100];
void read_file(u8 *filename)
{
  FILE *ptrf=NULL;
  u16 rows=0;
  ptrf=fopen(filename,"r");
  if(ptrf==NULL)
  {
    printf("There is no file with that name\n");
    exit(-1);
  }
  while(fgets(line[rows],max,ptrf)) //as long as there are lines continue parsing the lines into pointer to pinter of character
  {
    line[rows][strlen(line[rows])-1]='\0'; //at the end of the line put the terminator character
    rows++;
  }
  u16 i=0;
  u8 j=0;//counter to the arr_datbaseay which sorts the elements
  u8 *token[128];
  u8 *catg;
  for(j=0; j<rows-1; j++)
  {
    if(line[j][0]=='%') //if the line starts with % the it is the category so save it in catg variable and increment the loop
    {
      catg=line[j];
    }
    else // so it's a type , not category
    {
      segmentation (line[j],token,",");  // call the segmentation function and the terminator here it's "," it depends on data_base file format
      arr_datbase[i].category=catg; // here save the category which is known from the first iteration
      arr_datbase[i].name=token[0]; // according to data base file the first word in the line is the type "name" so save it in its struct in name member
      arr_datbase[i].quantity=atoi(token[1]); // according to data base file the second word in the line is the available quantity of this type and convert it to int
      arr_datbase[i].price=(i*2)+1; //random price for each type as it's not determined
      i++;
    }
  }
total_database=i; // save the counter i to global variable which is the total number of types
}
void segmentation (u8 *phrase,u8**token,u8* terminator)
{
  u16 i=0;
  token[i]=strtok(phrase,terminator); //divide the first line w.r.t the terminator , and after that it will put the null character
  while(token[i]!=NULL) //if it wasn't the last line then
  {
    i++; // increase the counter by one
    token[i]=strtok(NULL,terminator); // and divide the new line wich start from the null character w.r.t terminator also
  }
}
void read_requests(void)
{
  FILE *fptr=NULL;
  u8 rows1=0;
  u8 filename[128];
  printf("Please enter the requests file name: "); // ask the user to enter the name of order file
  scanf("%s",&filename);
  fptr=fopen(filename,"r"); // open the order file to read
  if(fptr==NULL)
  {
    printf("There is no file with that name\n"); //if the name of the file is not exist then end the program
    exit(-1);
  }
  while(fgets(line1[rows1],max,fptr)) // divide the file into lines and save the line in line1
  {
    line1[rows1][strlen(line1[rows1])-1]='\0'; //at the end of the line put the terminator character
    rows1++;
  }
  u8 *token1[128];
  u8 count;

  for (count=0; count<rows1; count++)
    {
      segmentation (line1[count],token1,"\n"); // divide the line into words w.r.t \n
      arr_requests[count].name=(u8*)token1[0]; // the first word it the type
      arr_requests[count].quantity=1; // assign its quantity to one.
    }


    total_req=rows1; //save the number of orders into global variable
  }
void classification(void)
{
  u8 x =0;
  u8 y=0;
  u8 i=0;
 temp_total_req=total_req;
 //check if any order is repeated then save it into previous one and increase its quantity then delete it
  for(x=0; x<temp_total_req; x++) //start from first order
  {
      for(y=temp_total_req-1; y>x; y--) //start from last order
      {
        if (!strcmp(arr_requests[x].name,arr_requests[y].name)) // if the order of x is equal to the order of y
        {
            for(i=y; i<=temp_total_req; i++) //then starting from the order y "DELETING THE ORDER Y WHICH IS REPEATED"
        {
            arr_requests[i].name = arr_requests[i+1].name; // copy the next order of y into order y
            arr_requests[i].category = arr_requests[i+1].category; // its category "THis line is not important as there's no category till now for orders"
            arr_requests[i].quantity = arr_requests[i+1].quantity; //its quantity "ALSO here"
            arr_requests[i].price = arr_requests[i+1].price; //its price "ÄLSO not important"
        }
            temp_total_req--; //THEN decremeant the NUMBER of order after deleting the repeated one
            arr_requests[x].quantity++; //INCREASE the quantity of the order which has another repeated
        }
      }
    }
// start classification w.r.t names to assign the category and price
  for (x=0; x<temp_total_req; x++)
  {
      for(y=0; y<total_database; y++)
      {
        if(!strcmp(arr_requests[x].name,arr_datbase[y].name))//if the request x name is equal to data base y name then
        {
          arr_requests[x].category=arr_datbase[y].category; //save the category of data base into category to order
          arr_datbase[y].quantity=arr_datbase[y].quantity-arr_requests[x].quantity; //decrease the quantity of data base according to the order quantity
          arr_requests[x].price=arr_requests[x].quantity*arr_datbase[y].price; //assign the price which is equal to (unit price*quantity)
          break; // terminate the loop of y
        }
        else //after checking all database and not found the name in database
        {
          arr_requests[x].category="NOT FOUND"; // then assign the category is not found
        }
      }
  }
}
void receipt(void)
{
  u8 categories[6][20]={"%Fruits","%vegetables","%dairy","%protein","%grains","NOT FOUND"}; // there are only 6 types
  u8 once=0;
  u8 i,k;
  FILE *fptr=NULL;
  fptr=fopen("receipt.txt","a"); //creat new file in append mode to write into it the receipt
  for(i=0; i<6; i++)
  {
    for(k=0; k<temp_total_req; k++)
    {
      if(!strcmp(categories[i],arr_requests[k].category))//if the category of i is equal to order k category
      {
        if(once==0) {printf("%s\n",categories[i]); once=1;fputs(categories[i],fptr);} //then for just a once print the category
        if(i<5) {printf("Type:%s unit price:%d quantity:%d price:%d\n",arr_requests[k].name,arr_requests[k].price/arr_requests[k].quantity,arr_requests[k].quantity,arr_requests[k].price);fprintf(fptr,"Type:%s",arr_requests[k].name,"Unit price %d",arr_requests[k].price/arr_requests[k].quantity,"Quantity:%d",arr_requests[k].quantity,"Price:%d\n",arr_requests[k].price);}
        if(i==5) {printf("Type %s",arr_requests[k].name);fprintf(fptr,"Type:%s",arr_requests[k].name);} //in case there is type not foind , just print the type not all info like before
      }

    }
    once=0;
  }

}

void main (void)
{
  read_file(database);
  read_requests();
  classification();
  receipt();
/*  u8 i;
for ( i=0;i <temp_total_req; i++)
 {
   puts(arr_requests[i].name);
   puts(arr_requests[i].category);
   printf("%d\n",arr_requests[i].quantity);
 }
    for (i=0;i <total_database; i++)
    {

      puts(arr_datbase[i].name);
      puts(arr_datbase[i].category);
    }
*/
}

