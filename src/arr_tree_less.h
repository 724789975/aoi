#ifndef __ArrTreeLess_H__
#define __ArrTreeLess_H__

namespace FXAOI
{
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

	template< typename Type >
	struct IsClass {
	private:

		template< typename AnswerYes >
		static char TYesNoTester(void(AnswerYes::*)(void));

		template< typename AnswerNo >
		static int TYesNoTester(...);

	public:

		enum {
			Result = sizeof(TYesNoTester<Type>(0)) == sizeof(char)
		};
	};

	template<typename T>
	class Less
	{
	public:
		inline bool operator() (const T& refLeft, const T& refRight)const
		{
			return refLeft < refRight;
		}
	};

	template<typename T, class KeyCompare = Less<T> >
	class Equal
	{
	private:

		inline bool eq (const T& refLeft, const T& refRight, TrueType t)const
		{
			return (!m_oCompare(refLeft, refRight)) && (!m_oCompare(refRight, refLeft));
		}

		inline bool eq (const T& refLeft, const T& refRight, FalseType f)const
		{
			return refLeft == refRight;
		}
	public:
		inline bool operator() (const T& refLeft, const T& refRight)const
		{
			return eq(refLeft, refRight, BooleanType<IsClass<T>::Result >());
		}
		KeyCompare m_oCompare;
	};


};
#endif // !__ArrTreeLess_H__
