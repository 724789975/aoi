#ifndef __AOI_CONST_H__
#define __AOI_CONST_H__

#define AOI_MAX_DIVIDE_NUM 8

#define USE_Y_AXIS

#ifdef USE_Y_AXIS
#define AOI_BIT_OFFSET 8
#define AOI_UNIT_SUB_SCRIPT unsigned long long
#else
#define AOI_BIT_OFFSET 4
#define AOI_UNIT_SUB_SCRIPT unsigned int
#endif

#endif  //!__AOI_CONST_H__
