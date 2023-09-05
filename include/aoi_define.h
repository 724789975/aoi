#ifndef __AOI_CONST_H__
#define __AOI_CONST_H__

typedef unsigned long long NODE_ID;

/**
 * @brief 
 * 最大分裂次数
 */
#define AOI_MAX_DIVIDE_NUM 8

/**
 * @brief 
 * 分裂阈值
 */
#define AOI_THRESHOLD_FOR_DIVIDE 128
/**
 * @brief 
 * 归并阈值
 */
#define AOI_THRESHOLD_FOR_MERGE 16

/**
 * @brief 
 * 是否使用Y轴
 */
#define AOI_USE_Y_AXIS 1

#if AOI_USE_Y_AXIS
#define AOI_BIT_OFFSET 8
#define AOI_UNIT_SUB_SCRIPT unsigned long long
#else
#define AOI_BIT_OFFSET 4
#define AOI_UNIT_SUB_SCRIPT unsigned int
#endif

#endif  //!__AOI_CONST_H__
