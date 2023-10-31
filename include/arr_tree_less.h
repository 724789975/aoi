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
			return (!m_oCompare(refLeft, refRight)) && (!m_oCompare(refRight, refLeft));
		}
		KeyCompare m_oCompare;
	};


	struct TrueType {
		enum { Result = true };
	};

	struct FalseType {
		enum { Result = false };
	};

	template <bool val>
	struct BooleanType : public FalseType
	{};

	template <>
	struct BooleanType<true> : public TrueType
	{};
};
#endif // !__ArrTreeLess_H__
