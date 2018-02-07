// ---------------------------------------------------------------------------
#ifndef uCRListLTH
#define uCRListLTH
#include <classes.hpp>

// ---------------------------------------------------------------------------
template<class T>
class CRListLT
{
private:
	TList* L;

public:
	CRListLT();
	~CRListLT();
	T* operator[](int _i);
	T* Add(T* _p);

	inline int Count(void)
	{
		return (L->Count);
	};
	void Clear(void);
	T* Insert(int _i, T* _p);
	void Reverse(void);
	void Remove(T* _p);
	void Capacity(int _i);
};

// ------------------------------------------------------------------------------
template<class T>
CRListLT<T>::CRListLT()
{
	L = new TList;
}

// ---------------------------------------------------------------------------
template<class T>
CRListLT<T>::~CRListLT()
{
	delete L;
}

// ---------------------------------------------------------------------------
template<class T>
T* CRListLT<T>:: operator[](int _i)
{
	if (_i > L->Count)
		return (NULL);
	return ((T*)L->Items[_i]);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
template<class T>
T* CRListLT<T>::Add(T* _p)
{
	L->Add(_p);
	return (_p);
};

// ---------------------------------------------------------------------------
template<class T>
void CRListLT<T>::Clear(void)
{
	L->Clear();
}

// ---------------------------------------------------------------------------
template<class T>
T* CRListLT<T>::Insert(int _i, T* _p)
{
	if (_i < 0 || _i >= L->Count)
		return (Add(_p));
	L->Insert(_i, _p);
	return (p);
}

// ---------------------------------------------------------------------------
template<class T>
void CRListLT<T>::Reverse(void)
{

	if (L->Count <= 2)
		return;
	int count = L->Count / 2;
	void* buf;
	for (int i = 0; i < count; i++)
	{
		buf = L->Items[i];
		L->Items[i] = L->Items[L->Count - 1 - i];
		L->Items[L->Count - 1 - i] = buf;
	}
}

// ---------------------------------------------------------------------------
template<class T>
void CRListLT<T>::Remove(T* _p)
{
	L->Remove(_p);
}
// ---------------------------------------------------------------------------
template<class T>
void CRListLT<T>::Capacity(int _i)
{
	L->Capacity=_i;
}
#endif
