/*
 * 322943408
 * Mahmoud Dawood
 */

#include "sim_mem.h"

#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

#include <fcntl.h>

char main_memory[MEMORY_SIZE];
void startPageDefault(page_descriptor * S) {
    S -> frame = -1;
    S -> V = 0;
    S -> D = -1;
    S -> swap_index = -1;
}
sim_mem::sim_mem(char exe_file_name1[], char exe_file_name2[], char swap_file_name2[], int text_size,
                 int data_size, int bss_size, int heap_stack_size,
                 int num_of_pages, int page_size, int num_of_process) {
    this ->num_of_proc = num_of_process;
    this -> text_size = text_size;
    this -> data_size = data_size;
    this -> bss_size = bss_size;
    this -> heap_stack_size = heap_stack_size;
    this -> num_of_pages = num_of_pages;
    this -> page_size = page_size;
    if ((this -> page_table = (page_descriptor ** ) malloc(num_of_process * sizeof(page_descriptor * ))) ==nullptr) {//start 1 or 2-page table
        perror("malloc filed");
        this -> ~sim_mem();
        exit(EXIT_FAILURE);
    }
    this -> swap_page = (int)this->num_of_proc * (num_of_pages - (text_size / page_size));
    this -> data_page = (int)(data_size / page_size);
    this -> text_page = (int)(text_size / page_size);
    this -> main_memory_page = (int)(MEMORY_SIZE / this -> page_size);
    if(exe_file_name1 == nullptr || exe_file_name2 == nullptr || swap_file_name2 == nullptr){//check null
        fprintf(stderr, "NULL file name \n");
        this -> ~sim_mem();
        exit(EXIT_FAILURE);
    }
    if (num_of_process == 1) {//1 process
        this -> page_table[1] = nullptr;
    } else if (num_of_process == 2) {//2 process
        if ((this -> program_fd[1] = open(exe_file_name2, O_RDONLY)) <= -1) {//open the program_fd[1] file
            perror("open program_fd[1] filed");
            this -> ~sim_mem();
            exit(EXIT_FAILURE);
        }

        if ((this -> page_table[1] = (page_descriptor * ) malloc(num_of_pages * sizeof(page_descriptor))) == nullptr) {//start page table of process 2
            perror("malloc filed");
            this -> ~sim_mem();
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < num_of_pages; ++i) {//the text_size area
            if (i < this -> text_size / this -> page_size)
                this -> page_table[1][i].P = 0;
            else
                this -> page_table[1][i].P = 1;
            startPageDefault( & this -> page_table[1][i]);//start the default value
        }
    }
    if ((this -> page_table[0] = (page_descriptor * ) malloc(num_of_pages * sizeof(page_descriptor))) == nullptr) {//start page table of process 1
        perror("malloc filed");
        this -> ~sim_mem();
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_of_pages; ++i) {//the text_size area
        if (i < this -> text_size / this -> page_size)
            this -> page_table[0][i].P = 0;
        else
            this -> page_table[0][i].P = 1;
        startPageDefault( & page_table[0][i]);//start the default value
    }
    if ((this -> swapfile_fd = open(swap_file_name2, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR)) == -1) {//open the swap file
        perror("open swap filed");
        this -> ~sim_mem();
        exit(EXIT_FAILURE);
    }
    //program_fd[0] file
    if ((this -> program_fd[0] = open(exe_file_name1, O_RDONLY)) == -1) {//open the program_fd[0] file
        perror("open program_fd[0] filed");
        this -> ~sim_mem();
        exit(EXIT_FAILURE);
    }

    for (char & i : main_memory) {//start the main memory with -> 0 value
        i = '0';
    }

    for (int i = 0; i < this->num_of_proc * ((page_size * num_of_pages) - text_size); i++) {//start the swap file with -> 0 value
        if (write(this -> swapfile_fd, "0", 1) <= -1) {
            this -> ~sim_mem();
            perror("write failed");
            exit(EXIT_FAILURE);
        }
    }
    if ((this -> swapPage = (int * ) malloc( this -> swap_page * sizeof(int))) == nullptr) {//start swap page array
        this -> ~sim_mem();
        perror("malloc filed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < this -> swap_page; ++i) {//start the swap page with -> 0 value - this mean all the cells are available
        this -> swapPage[i] = 0;
    }
}

sim_mem::~sim_mem() {//destructor
    if (this -> num_of_proc == 2) {
        free(this -> page_table[1]);
        if (close(swapfile_fd) == -1 || close(program_fd[1]) == -1 || close(program_fd[0]) == -1) {
            perror("close filed");
            exit(EXIT_FAILURE);
        }
    } else
    if (close(program_fd[0]) == -1 || close(swapfile_fd) == -1) {
        perror("close filed");
        exit(EXIT_FAILURE);
    }
    free(this -> page_table[0]);
    free(this -> page_table);
    free(this -> swapPage);
}

char sim_mem::load(int process_id, int address) {
    int page = (int)(address / this -> page_size); //address of page
    int offset = address % this -> page_size; //offset
    int frameIndex = this -> tail;//if the ram not full yet - put the page in the tail place
    char buff[this -> page_size];//buffer
    // invalid address
    if (page >= num_of_pages) {//if the page out of the limit
        fprintf(stderr, "invalid address-");
        return '\0';
    }
    //load data the first time with  page ->stack , heap
    else if (page >= (this -> text_page + this -> data_page + (this ->bss_size/this->page_size)) &&
             this -> page_table[process_id - 1][page].V == 0 && this -> page_table[process_id - 1][page].swap_index == -1) {
        fprintf(stderr, "invalid address");
        return '\0';
    }
    //already in ram
    if (this -> page_table[process_id - 1][page].V == 1) {
        return main_memory[((this -> page_table[process_id - 1][page].frame * this -> page_size) + offset)];
    } else { //the page not in the ram
        if (this -> main_memory_page == this -> count) { //check if the ram is full
            for (int s = 0; s < this->num_of_proc; ++s)//search in the all pro page
                for (int i = 0; i < this -> num_of_pages; i++) //search for the oldest page
                if (this -> page_table[s][i].frame == this -> head) {
                    if (this -> page_table[s][i].D == 1) { //check if the page is dirty
                        for (int j = 0; j < this -> page_size; j++) //take the page from the ram to the buff
                            buff[j] = main_memory[this -> head * this -> page_size + j];
                        if (lseek(swapfile_fd, page_size * this -> emptyPlaceINTheSwap, SEEK_SET) < 0)
                            return '\0';
                        if (write(swapfile_fd, buff, this -> page_size) != this -> page_size) {
                            perror("Read filed");
                            return '\0';
                        }
                        //update the page
                        this -> page_table[s][i].swap_index = emptyPlaceINTheSwap;
                        this -> swapPage[emptyPlaceINTheSwap] = 1;
                        this -> findEmptyPlace();
                    }
                    this -> page_table[s][i].V = 0;
                    this -> page_table[s][i].frame = -1;
                    frameIndex = this -> head;
                    break;
                }
            dequeue();
        }
        if (this -> page_table[process_id - 1][page].swap_index != -1) { //the page in the swap
            //copy the page from the swap
            if (lseek(this -> swapfile_fd, (page_table[process_id - 1][page].swap_index * this -> page_size), SEEK_SET) < 0)
                return '\0';
            if (read(this -> swapfile_fd, buff, this -> page_size) != page_size)
                return '\0';
            for (int i = 0; i < this -> page_size; i++)
                main_memory[frameIndex * this -> page_size + i] = buff[i];
            //fill the place in the swap with -> 0
            for (int i = 0; i < this -> page_size; ++i) {
                buff[i] = '0';
            }
            if (lseek(this -> swapfile_fd, (page_table[process_id - 1][page].swap_index * this -> page_size), SEEK_SET) < 0)
                return '\0';
            if (write(this -> swapfile_fd, buff, this -> page_size) != page_size)
                return '\0';
            //update the page
            this -> page_table[process_id - 1][page].V = 1;
            enqueue();
            this -> swapPage[page_table[process_id - 1][page].swap_index] = 0;
            this -> findEmptyPlace();
            this -> page_table[process_id - 1][page].swap_index = -1;
        } else { //page is found in the executable
            if ((page < (this -> text_page + this -> data_page + (this ->bss_size/this->page_size)))) { //bring the page from the executable
                if (lseek(this -> program_fd[process_id - 1], page * this -> page_size, SEEK_SET) < 0)
                    return '\0';
                if (read(this -> program_fd[process_id - 1], buff, this -> page_size) != this -> page_size)
                    return '\0';
                for (int i = 0; i < this -> page_size; i++)
                    main_memory[(frameIndex * this -> page_size) + i] = buff[i];
                this -> page_table[process_id - 1][page].V = 1;
                enqueue();
            }
        }
        this -> page_table[process_id - 1][page].frame = frameIndex;
        return main_memory[(this -> page_size * this -> page_table[process_id - 1][page].frame) + offset];
    }
}

void sim_mem::store(int process_id, int address, char value) {
    int page = (int)(address / this -> page_size); //address of page
    int offset = address % this -> page_size; //offset
    int frameIndex = this -> tail;//if the ram not full yet - put the page in the tail place
    char buff[this -> page_size];
    // invalid address
    if (page >= num_of_pages) {
        this -> ~sim_mem();
        fprintf(stderr, "invalid address");
        exit(EXIT_FAILURE);
    }
    //if we try to write to text block
    if (this -> page_table[process_id - 1][page].P == 0) { //code page
        this -> ~sim_mem();
        fprintf(stderr, "cannot write to this block");
        exit(EXIT_FAILURE);
    }
    //already in ram
    if (this -> page_table[process_id - 1][page].V == 1) {
        main_memory[((this -> page_table[process_id - 1][page].frame * this -> page_size) + offset)] = value;
        this -> page_table[process_id - 1][page].D = 1;
        return;
    } else { //the page not in the ram
        if (this -> main_memory_page == this -> count) { //check if the ram is full
            for (int s = 0; s < this->num_of_proc; ++s)//search in the all pro page
                for (int i = 0; i < this -> num_of_pages; i++) //search for the oldest page
                if (this -> page_table[s][i].frame == this -> head) {
                    if (this -> page_table[s][i].D == 1) { //check if the page is dirty
                        for (int j = 0; j < this -> page_size; j++) //take the page from the ram to the buff
                            buff[j] = main_memory[this -> head * this -> page_size + j];
                        if (lseek(swapfile_fd, page_size * this -> emptyPlaceINTheSwap, SEEK_SET) < 0)
                            return;
                        if (write(swapfile_fd, buff, this -> page_size) != this -> page_size) {
                            perror("Read filed");
                            return;
                        }
                        //update the page
                        this -> page_table[s][i].swap_index = emptyPlaceINTheSwap;
                        this -> swapPage[emptyPlaceINTheSwap] = 1;
                        this -> findEmptyPlace();
                    }
                    this -> page_table[s][i].V = 0;
                    this -> page_table[s][i].frame = -1;
                    frameIndex = this -> head;
                    break;
                }
            dequeue();
        }
        if (this -> page_table[process_id - 1][page].swap_index != -1) { //the page in the swap
            //copy the page from the swap
            if (lseek(this -> swapfile_fd, (page_table[process_id - 1][page].swap_index * this -> page_size), SEEK_SET) < 0){
                this -> ~sim_mem();
                perror("lseek filed");
                exit(EXIT_FAILURE);
            }
            if (read(this -> swapfile_fd, buff, this -> page_size) != page_size){
                this -> ~sim_mem();
                perror("read filed");
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < this -> page_size; i++)
                main_memory[frameIndex * this -> page_size + i] = buff[i];
            //fill the place in the swap with -> 0
            for (int i = 0; i < this -> page_size; ++i) {
                buff[i] = '0';
            }
            if (lseek(this -> swapfile_fd, (page_table[process_id - 1][page].swap_index * this -> page_size), SEEK_SET) < 0){
                this -> ~sim_mem();
                perror("lseek filed");
                exit(EXIT_FAILURE);
            }
            if (write(this -> swapfile_fd, buff, this -> page_size) != page_size){
                this -> ~sim_mem();
                perror("write filed");
                exit(EXIT_FAILURE);
            }
            //update the page
            this -> page_table[process_id - 1][page].V = 1;
            this -> page_table[process_id - 1][page].D = 1;
            this -> page_table[process_id - 1][page].frame = frameIndex;
            enqueue();
            this -> swapPage[page_table[process_id - 1][page].swap_index] = 0;
            this -> findEmptyPlace();
            this -> page_table[process_id - 1][page].swap_index = -1;
        } else { //page is found in the executable
            if ((page < (this -> text_page + this -> data_page))) { //bring the page from the executable
                if (lseek(this -> program_fd[process_id - 1], page * this -> page_size, SEEK_SET) < 0){
                    this -> ~sim_mem();
                    perror("lseek filed");
                    exit(EXIT_FAILURE);
                }
                if (read(this -> program_fd[process_id - 1], buff, this -> page_size) != this -> page_size){
                    this -> ~sim_mem();
                    perror("read filed");
                    exit(EXIT_FAILURE);
                }
                for (int i = 0; i < this -> page_size; i++)
                    main_memory[(frameIndex * this -> page_size) + i] = buff[i];
                this -> page_table[process_id - 1][page].V = 1;
                this -> page_table[process_id - 1][page].frame = frameIndex;
                this -> page_table[process_id - 1][page].D = 1;
                enqueue();
            } else if (page >= (this -> text_page + this -> data_page)) {
                for (int i = (frameIndex * this -> page_size); i < (frameIndex * this -> page_size) + page_size; i++) { // write to main memory
                    main_memory[i] = '0';
                }
                this -> page_table[process_id - 1][page].V = 1;
                this -> page_table[process_id - 1][page].D = 1;
                this -> page_table[process_id - 1][page].frame = frameIndex;
                main_memory[(this -> page_size * frameIndex) + offset] = value;
                enqueue();
            }
        }
    }
    main_memory[(this -> page_size * this -> page_table[process_id - 1][page].frame) + offset] = value;
}

/**************************************************************************************/
void sim_mem::print_memory() {
    int i;
    printf("\n Physical memory\n");
    for (i = 0; i < MEMORY_SIZE; i++) {
        printf("[%c]\n", main_memory[i]);
    }
}
/************************************************************************************/
void sim_mem::print_swap() {
    char * str = (char * ) malloc(this -> page_size * sizeof(char));
    int i;
    printf("\n Swap memory\n");
    lseek(swapfile_fd, 0, SEEK_SET); // go to the start of the file
    while (read(swapfile_fd, str, this -> page_size) == this -> page_size) {
        for (i = 0; i < page_size; i++) {
            printf("%d - [%c]\t", i, str[i]);
        }
        printf("\n");
    }
}
/***************************************************************************************/
void sim_mem::print_page_table() {
    int i;
    for (int j = 0; j < num_of_proc; j++) {
        printf("\n page table of process: %d \n", j + 1);
        printf("Valid\tDirty\tPermission\tFrame\tSwap index\n");
        for (i = 0; i < num_of_pages; i++) {
            printf("[%d] \t [%d] \t [%d] \t [%d] \t [%d]\n",
                   page_table[j][i].V,
                   page_table[j][i].D,
                   page_table[j][i].P,
                   page_table[j][i].frame,
                   page_table[j][i].swap_index);
        }
    }
}
//page inserted to the ram
void sim_mem::enqueue() {
    this -> count++;
    this -> tail++;
    if (this -> tail == this -> main_memory_page)
        this -> tail = 0;
}
//page delete from the ram
void sim_mem::dequeue() {
    this -> count--;
    this -> head++;
    if (this -> head == this -> main_memory_page)
        this -> head = 0;
}
void sim_mem::findEmptyPlace() {
    for (int i = 0; i < this->swap_page; ++i) {
        if (this -> swapPage[i] == 0) {
            this -> emptyPlaceINTheSwap = i;
            break;
        }
    }
}