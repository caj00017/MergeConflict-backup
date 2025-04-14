#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <mpx/vm.h>
#include <mpx/comexec.h>
#include <conversions.h>
#include <mcb.h>

/**
 * @brief Show the details of a specific MCB.
 * @param mcb The MCB to show.
 * @author Chris Jones
 */
void show_mcb(mcb* mcb);

/**
 * @brief Show the allocated memory blocks.
 * @author Chris Jones
 */
void show_alloc_mem(void);

/**
 * @brief Show the free memory blocks.
 * @author Chris Jones
 */
void show_free_mem(void);
