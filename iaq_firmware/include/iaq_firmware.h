#pragma once

#include "iaq_utils/iaq_assert_panic.h"
#define configASSERT_PANIC(__e) ((__e) ? (void)0 : my_assert_func_panic(__FILE__, __LINE__, __func__, #__e))
#define PANIC(str) my_assert_func_panic(__FILE__, __LINE__, __func__, str)