#ifndef UNUSED_H
#define UNUSED_H

/* Unused module */

/* Unused function arguments should be annotated with this macro to get rid of
   compiler warnings. */
#define G_UNUSED \
        __attribute__ ((unused)) /*@unused@*/

#endif // UNUSED_H
