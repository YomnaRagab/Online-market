#ifndef PROTO_H_INCLUDED
#define PROTO_H_INCLUDED
#include "types.h"
#define orders 30 //size of the array of line "number of line"
#define max 128    //size of line
#define database "data_base.txt" //the data base file name
typedef struct{
    u8 *category;
    u8 *name;
    u8 quantity;
    u8 price;
}strsort;
/*
    struct type to save all data about each type
*/
void read_file(u8*);
/*
    Function: takes the name of data base file and read it then divides it into lines then call segmentation func which divides the lines into words w.r.t the terminator
    arguments: data base file name which is preprocessing defined.
    return:None.
*/
void read_requests(void);
/*
    function:it reads order file , and divids it into words then store it in array of struct.
    arguments:none.
    return:none.
*/
void classification(void);
/*
    function: it classifies the requests into their categories , comparing the requests names with the data base names , and check if any request is repeated so it deletes it and increase the quantity of it.
    arguments:none.
    return:none.
*/
void receipt(void);
/*
    function: it printing the order and its categories plus to the price.
    arguments: none.
    return:none.
*/
//void categorized(u16 );
//void assign (u8*tok1, u8*tok2,u16 j);
void segmentation (u8*,u8**,u8*);
/*
        function: it takes the line , the token which saves the words in it , and the terminator which is uses to determine where it will cut the line.
        arguments: -pointer to character: the line to be divided.
                   -pointer to pointer to character: the array of words to save the segments into it.
                   -pointer to character: the terminator , which tell the func where it's supposed to cut the line.
        return:none.
*/



#endif // PROTOS_H_INCLUDED
