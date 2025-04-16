#include <mcb.h>
#include <conversions.h>

void show_mcb(mcb* mcb) {
    clear(str);
    print("MCB: 0x");
    print(custom_itoa((int)mcb->start_addr, str, 16));

    clear(str);
    print("\nSize: ");
    print(custom_itoa((mcb->size+sizeof(*mcb)), str, 10));
    println();
    
    clear(str);
    print("Status: ");
    print(custom_itoa(mcb->status, str, 10));
    println();
}

void show_alloc_mem(void) {
    list* allocated_list = return_list();
    mcb* current_mcb = allocated_list->head;

    print_color("\n==========================", color);
    print("\nAllocated MCBs:");
    print_color("\n==========================", color);

    if (current_mcb == NULL) {
        print("\nNo allocated MCBs found.\n");
        return;
    }
    while (current_mcb != NULL ) {
        if(current_mcb->status == 1){
            print("\n[ALLOCATED] ");
            show_mcb(current_mcb);
            current_mcb = current_mcb->next_node;
            println();
        }
        else{
            current_mcb = current_mcb->next_node;
        }
    }
}

void show_free_mem(void) {
    list* free_list = return_list();
    mcb* current_mcb = free_list->head;

    print_color("\n\n==========================", color);
    print("\nFree MCBs:");
    print_color("\n==========================", color);

    if (current_mcb == NULL) {
        print("\nNo free MCBs found.\n");
        return;
    }

    while (current_mcb != NULL) {
        if(current_mcb->status == 0){
            print("\n[FREE] ");
            show_mcb(current_mcb);
            current_mcb = current_mcb->next_node;
            println();
        }
        else{
            current_mcb = current_mcb->next_node;
        }
    }
}
