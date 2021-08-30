#include <linux/uaccess.h>
#include "macros.h"

/* This string must be seen as a "tape" : if the requested read offset goes past the size
 * of the string, it's as if we tried to read beyond the end of the tape.
 *
 * The extra '\0' character is there for convenience, to avoid inserting it by hand.
 */
static const char READ_STRING[] = "You just got kerneled ! >:-)\n\0";
static const ssize_t READ_STRING_SIZE = sizeof(READ_STRING) / sizeof(READ_STRING[0]);

static ssize_t helloworld_read // Return value : bytes read
(
    struct file*    file_pointer,
    char* __user    user_buffer,
    size_t          bytes_to_read,
    loff_t*         position
)
{
    PRINT_CALL();
    printk(KERN_INFO "Read operation : offset = %i, bytes to read = %u\n", *position, count);

    // If we're trying to read starting from beyond the end of the "tape"
    if(*position >= READ_STRING_SIZE)
    {
        return 0;
    }

    // If we're trying to read starting from inside the "tape", but going forward beyond the end
    if( (*position + count) > READ_STRING_SIZE )
    {
        // We'll return only what we can
        count = READ_STRING_SIZE - *position;
    }

    // Transfer to user buffer
    unsigned long return_value = copy_to_user(user_buffer, (READ_STRING + *position), count);

    if(return_value != 0)
        return -EFAULT;

    // Move the "reading head" forward
    *position += count;

    // Return the amount of read bytes
    return count;
}