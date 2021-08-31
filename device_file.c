// SPDX-License-Identifier: GPL-2.0

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include "macros.h"

/* This string must be seen as a "tape" : if the requested read offset goes past the size
 * of the string, it's as if we tried to read beyond the end of the tape.
 *
 * The extra '\0' character is there for convenience, to avoid inserting it by hand.
 */
static const char READ_STRING[] = "You just got kerneled ! >:-)\n\0";
static const ssize_t READ_STRING_SIZE = ARRAY_SIZE(READ_STRING);

static ssize_t helloworld_read // Return value : bytes read
(
	struct file    *file_pointer,
	char __user    *user_buffer,
	size_t          bytes_to_read,
	loff_t         *position
)
{
	PRINT_CALL();
	pr_info("Read operation : offset = %i, bytes to read = %u\n", (int)*position, (unsigned int)bytes_to_read);

	// If we're trying to read starting from beyond the end of the "tape"
	if (*position >= READ_STRING_SIZE)
		return 0;

	// If we're trying to read starting from inside the "tape", but going forward beyond the end
	if ((*position + bytes_to_read) > READ_STRING_SIZE)
		// We'll return only what we can
		bytes_to_read = READ_STRING_SIZE - *position;

	// Transfer to user buffer
	if (copy_to_user(user_buffer, (READ_STRING + *position), bytes_to_read) != 0)
		return -EFAULT;

	// Move the "reading head" forward
	*position += bytes_to_read;

	// Return the amount of read bytes
	return bytes_to_read;
}

static struct file_operations helloworld_file_ops = {
	.owner = THIS_MODULE,
	.read = helloworld_read,
};

static int helloworld_file_major_number;
static const char helloworld_device_name[] = "HelloWorld device 3000";

int register_device(void)
{
	PRINT_CALL();

	int result = 0;

	result = register_chrdev(0, helloworld_device_name, &helloworld_file_ops);

	if (result < 0)	{
		pr_warn("Can't register character driver / Error code = %i\n", result);
		return result;
	}

	helloworld_file_major_number = result;
	pr_info("Registered character driver / Major number = %i\n", helloworld_file_major_number);

	return 0;
}

void unregister_device(void)
{
	PRINT_CALL();

	if (helloworld_file_major_number != 0)
		unregister_chrdev(helloworld_file_major_number, helloworld_device_name);
}
