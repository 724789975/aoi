#ifndef __AOI_CONST_H__
#define __AOI_CONST_H__

#include <ostream>

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
#define AOI_THRESHOLD_FOR_DIVIDE 200
/**
 * @brief 
 * 归并阈值
 */
#define AOI_THRESHOLD_FOR_MERGE 10

/**
 * @brief 
 * 视野中实体上限(仅需互相看见实体需要)
 */
#define AOI_MAX_NODE_IN_VIEW 50

/**
 * @brief 
 * 是否使用Y轴
 */
#define AOI_USE_Y_AXIS 1

#if AOI_USE_Y_AXIS
#define AOI_BIT_OFFSET 8
#define AOI_DIVIDE_CHILDREN_NUM 8
#define AOI_UNIT_SUB_SCRIPT unsigned long long
#define AOI_FLAG_MASK 0xFFll
#else
#define AOI_BIT_OFFSET 4
#define AOI_DIVIDE_CHILDREN_NUM 4
#define AOI_UNIT_SUB_SCRIPT unsigned int
#define AOI_FLAG_MASK 0xF
#endif

#define AOI_LEFT_DOWN_BOTTOM		AOI_UNIT_SUB_SCRIPT(0x1)
#define AOI_LEFT_UP_BOTTOM			AOI_UNIT_SUB_SCRIPT(0x2)
#define AOI_RIGHT_DOWN_BOTTOM		AOI_UNIT_SUB_SCRIPT(0x4)
#define AOI_RIGHT_UP_BOTTOM			AOI_UNIT_SUB_SCRIPT(0x8)
#define AOI_LEFT_DOWN_TOP			AOI_UNIT_SUB_SCRIPT(0x10)
#define AOI_LEFT_UP_TOP				AOI_UNIT_SUB_SCRIPT(0x20)
#define AOI_RIGHT_DOWN_TOP			AOI_UNIT_SUB_SCRIPT(0x40)
#define AOI_RIGHT_UP_TOP			AOI_UNIT_SUB_SCRIPT(0x80)

namespace FXAOI
{
	struct NodePosition
	{
		double x;
#if AOI_USE_Y_AXIS
		double y;
#endif
		double z;

		void Debug(std::ostream& refOstream)
		{
			refOstream << "NodePosition:{x:" << this->x << ","
#if AOI_USE_Y_AXIS
				<< "y:" << this->y << ","
#endif
				<< "z:" << this->z << "}";
		}
	};
} // namespace FXAOI

#endif  //!__AOI_CONST_H__
