322943408
mahmoud dawood
 Virtual Memory Management

===================Description=================
simulator of Virtual Memory Management and CPU read/write
 
page table

valid - the page found in  main memory
 dirty - if we change the page this means we use store function to write *** 
 Permission - it it text area 
 frame - number of the page in ram 
 swap_index - number of the page in the swap file 
===================Functions=================

 tail -> pointer to insert page
 head -> pointer to delete page

enqueue()     //insert page
tail + 1 % rampage
dequeue()     //delete page
head + 1 % rampage

swapPage() array of value 1 or 0 with length of  the swap num page 
this keep the available places in the swap
 available -> 0
full -> 1
emptyPlaceINTheSwap->hold the first available place
findEmptyPlace()->update -> emptyPlaceINTheSwap

==========Program Files============
sim_mem.h 
sim_mem.cpp 
main.cpp 

