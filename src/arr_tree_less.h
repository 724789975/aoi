#ifndef __ArrTreeLess_H__
#define __ArrTreeLess_H__

namespace FXAOI
{
	template<typename T>
	class Less
	{
	public:
		bool operator() (const T& refLeft, const T& refRight)const
		{
			return refLeft < refRight;
		}
	};

	template<typename T, class KeyCompare = Less<T> >
	class Equal
	{
	public:
		bool operator() (const T& refLeft, const T& refRight)const
		{
			return (!KeyCompare()(refLeft, refRight)) && (!KeyCompare()(refRight, refLeft));
		}
	};
};
#endif // !__ArrTreeLess_H__
