#pragma once
#include "VSMatrix3X3W.h"
#include "VSGraphic.h"
#include "VSTransform.h"
/*
	

*/
namespace VSEngine2
{
	class VSGRAPHIC_API VSAnimAtom
	{
	public:
		VSAnimAtom(const VSVector3& fScale,const VSVector3& Pos,const VSQuat& Rotator)
			:m_fScale(fScale),m_Pos(Pos),m_Rotator(Rotator)
		{

		}
		VSAnimAtom()
		{
			Identity();
		}
		~VSAnimAtom()
		{
			
		}
		VSVector3	m_fScale;
		VSVector3	m_Pos;
		VSQuat		m_Rotator;
		void GetMatrix(VSMatrix3X3W & OutMatrix)const;
		void Interpolation(const VSAnimAtom & Atom1 , const VSAnimAtom Atom2,VSREAL t);
		void Identity();
		void FromTransform(const VSTransform &T);
		void FromMatrix(const VSMatrix3X3W & m);
		void ParitialBlend(const VSAnimAtom &Atom, float f);
		void FastParitialBlend(const VSAnimAtom &Atom, float f);
		

		void    operator *= (VSREAL f);
		void    operator += (const VSAnimAtom &Atom);
		void    operator -= (const VSAnimAtom &Atom);
		
		static VSAnimAtom ms_Identity;
		static VSAnimAtom ms_FastBlendZero;
		static VSTransform VSEngineFrom3DMax(const VSAnimAtom & AtomIn3DMax);
		VSTransform GetTransfrom()const;
	};
	VSGRAPHIC_API VSAnimAtom operator +(const VSAnimAtom & Atom1, const VSAnimAtom Atom2);
	VSGRAPHIC_API VSAnimAtom operator -(const VSAnimAtom & Atom1, const VSAnimAtom Atom2);
	VSGRAPHIC_API VSAnimAtom operator *  (const VSAnimAtom & Atom,VSREAL f);
	VSGRAPHIC_API VSAnimAtom FastParitialMul(const VSAnimAtom & Atom, VSREAL f);
}
