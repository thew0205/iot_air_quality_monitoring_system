#pragma once

#include "assert_panic.h"
#define configASSERT_PANIC(__e) ((__e) ? (void)0 : my_assert_func(__FILE__, __LINE__, __func__, #__e))