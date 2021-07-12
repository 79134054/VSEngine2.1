#pragma once
#include "VSArray.h"
namespace VSEngine2
{
	template <typename TSignature> class VSDelegate;
	template <typename TSignature> class VSDelegateEvent;
	template<typename RETUREN_TYPE, typename... DELEGATE_TEMPLATE_TYPE>
	class VSDelegate<RETUREN_TYPE(DELEGATE_TEMPLATE_TYPE...)>
	{
	public:
		~VSDelegate()
		{
		}
		VSDelegate(const VSDelegate& Delegate)
		{
			*this = Delegate;
		}
		void operator= (const VSDelegate& Delegate)
		{
			m_F = Delegate.m_F;
			if (!Delegate.LambdaBuffer.GetNum())
			{
				m_p = Delegate.m_p;
				LambdaBuffer.Destroy();
			}
			else
			{
				LambdaBuffer = Delegate.LambdaBuffer;
				m_p = (void *)LambdaBuffer.GetBuffer();
			}
		}
	protected:	
		typedef RETUREN_TYPE(*STATIC_FUNCTION_TYPE)(void * p, DELEGATE_TEMPLATE_TYPE...);

		VSDelegate(void * p, STATIC_FUNCTION_TYPE Funtion,unsigned int BufferSize)
		{			
			m_F = Funtion;
			if (BufferSize)
			{
				LambdaBuffer.SetBufferNum(BufferSize);
				VSMemcpy(LambdaBuffer.GetBuffer(), p, BufferSize);
				m_p = (void *)LambdaBuffer.GetBuffer();
			}
			else
			{
				m_p = p;
			}
		}

		void * m_p;
		STATIC_FUNCTION_TYPE m_F;
		VSArray<unsigned char> LambdaBuffer;


		template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TEMPLATE_TYPE...)>
		static RETUREN_TYPE MethodStub(void * p, DELEGATE_TEMPLATE_TYPE... DELEGATE_VALUE)
		{
			T * Ap = (T *)p;
			return (Ap->*FunName)(DELEGATE_VALUE...);
		}

		template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TEMPLATE_TYPE...)const>
		static RETUREN_TYPE ConstMethodStub(void * p, DELEGATE_TEMPLATE_TYPE... DELEGATE_VALUE)
		{
			T * Ap = (T *)p;
			return (Ap->*FunName)(DELEGATE_VALUE...);
		}

		template<RETUREN_TYPE(*FunName)(DELEGATE_TEMPLATE_TYPE...)>
		static RETUREN_TYPE FunStub(void * p, DELEGATE_TEMPLATE_TYPE... DELEGATE_VALUE)
		{
			return (FunName)(DELEGATE_VALUE...);
		}

		template<typename LAMBDA>
		static RETUREN_TYPE LambdaStub(void * p, DELEGATE_TEMPLATE_TYPE... DELEGATE_VALUE)
		{
			return (*((LAMBDA *)p))(DELEGATE_VALUE...);
		}

		static VSDelegate Create(void * p, STATIC_FUNCTION_TYPE Funtion)
		{
			return VSDelegate(p, Funtion,0);
		}
		template<typename LAMBDA>
		static VSDelegate Create(LAMBDA &L, STATIC_FUNCTION_TYPE Funtion)
		{
			return VSDelegate((void *)&L, Funtion,sizeof(LAMBDA));
		}
	public:
		template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TEMPLATE_TYPE...)const>
		static VSDelegate FromMethod(T * p)
		{
			return Create((void*)p, &ConstMethodStub<T, FunName>);
		}

		template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TEMPLATE_TYPE...)>
		static VSDelegate FromMethod(T * p)
		{
			return Create((void*)p, &MethodStub<T, FunName>);
		}

		template<RETUREN_TYPE(*FunName)(DELEGATE_TEMPLATE_TYPE...)>
		static VSDelegate FromFun()
		{
			return Create(NULL, &FunStub<FunName>);
		}
		template<typename LAMBDA>
		static VSDelegate FromLambda(LAMBDA && L)
		{
			return Create<LAMBDA>(L, &LambdaStub<LAMBDA>);
		}

		RETUREN_TYPE Execute(DELEGATE_TEMPLATE_TYPE... DELEGATE_VALUE)
		{
			return (*m_F)(m_p, DELEGATE_VALUE...);
		}
		RETUREN_TYPE operator()(DELEGATE_TEMPLATE_TYPE... DELEGATE_VALUE) const
		{
			return (*m_F)(m_p, DELEGATE_VALUE...);
		}

		operator bool() const
		{
			return m_F != NULL;
		}

		bool operator!() const
		{
			return !(operator bool());
		}

		bool operator==(const VSDelegate& rhs) const
		{
			return (m_p == rhs.m_p && m_F == rhs.m_F);
		}
		bool operator!=(const VSDelegate& rhs) const
		{
			return (m_p =! rhs.m_p || m_F != rhs.m_F);
		}
	};
	template <typename RETUREN_TYPE, typename... DELEGATE_TEMPLATE_TYPE>
	class VSDelegateEvent<RETUREN_TYPE(DELEGATE_TEMPLATE_TYPE...)>
	{
	public:
		typedef VSDelegate<RETUREN_TYPE(DELEGATE_TEMPLATE_TYPE...)> Handler;
	public:
		VSDelegateEvent() {}

		void operator+=(const Handler& handler)
		{
			this->Add(handler);
		}
		void Add(const Handler& handler)
		{
			assert(!this->Has(handler));

			m_handlers.AddElement(handler);
		}

		template<RETUREN_TYPE(*FunName)(DELEGATE_TEMPLATE_TYPE...)>
		void AddFun()
		{
			Add(Handler::FromFun<FunName>());
		}

		template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TEMPLATE_TYPE...)const>
		void AddMethod(T * p)
		{
			Add(Handler::FromMethod<T, FunName>(p));
		}

		template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TEMPLATE_TYPE...)>
		void AddMethod(T * p)
		{
			Add(Handler::FromMethod<T, FunName>(p));
		}
		void operator-=(const Handler& handler)
		{
			this->Remove(handler);
		}
		void Remove(const Handler& handler)
		{
			unsigned int i = m_handlers.FindElement(handler);

			if (i < m_handlers.GetNum())
			{
				m_handlers.Erase(i);
			}
		}
		template<RETUREN_TYPE(*FunName)(DELEGATE_TEMPLATE_TYPE...)>
		void RemoveFun()
		{
			Remove(Handler::FromFun<FunName>());
		}

		template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TEMPLATE_TYPE...)const>
		void RemoveMethod(T * p)
		{
			Remove(Handler::FromMethod<T, FunName>(p));
		}

		template<class T, RETUREN_TYPE(T::*FunName)(DELEGATE_TEMPLATE_TYPE...)>
		void RemoveMethod(T * p)
		{
			Remove(Handler::FromMethod<T, FunName>(p));
		}
		bool Has(const Handler& handler)
		{
			unsigned int i = m_handlers.FindElement(handler);
			return i != m_handlers.GetNum();
		}
		/// ÊÇ·ñÓÐÐ§
		bool IsValid() const
		{
			return m_handlers.GetNum() > 0;
		}

		void Reset()
		{
			m_handlers.Clear();
		}


		void operator()(DELEGATE_TEMPLATE_TYPE... DELEGATE_VALUE) const
		{
			this->Invoke(DELEGATE_VALUE...);
		}
		void Invoke(DELEGATE_TEMPLATE_TYPE... DELEGATE_VALUE) const
		{
			for (unsigned int i = 0; i < m_handlers.GetNum(); ++i)
			{
				m_handlers[i](DELEGATE_VALUE...);
			}
		}
		void InvokeWithEmitter(DELEGATE_TEMPLATE_TYPE... DELEGATE_VALUE, const Handler& emitter) const
		{
			for (unsigned i = 0; i < m_handlers.GetNum(); ++i)
			{
				if (m_handlers[i] != emitter)
				{
					m_handlers[i](DELEGATE_VALUE...);
				}
			}
		}

	private:
		typedef VSArray<Handler> DelegateList;
		DelegateList m_handlers;
	};
}







