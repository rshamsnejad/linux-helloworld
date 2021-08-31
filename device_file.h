#pragma once

#include <linux/compiler.h>

__must_check int register_device(void);
void unregister_device(void);