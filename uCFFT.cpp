#include "uCFFT.h"
#include<math.h>
#include <windows.h>
// ------------------------------------------------------------------------------
CFFT::CFFT(int _size,TIniFile* _ini)
{
	ini=_ini;
	if(!ini->ValueExists("Default","CFFT_256"))
		ini->WriteBool("Default","CFFT_256",false);
	if(!ini->ValueExists("Default","acfBorderLeft"))
		ini->WriteInteger("Default","acfBorderLeft",10);
	if(!ini->ValueExists("Default","acfBorderRight"))
		ini->WriteInteger("Default","acfBorderRight",474);
	acfBorderLeft=ini->ReadInteger("Default","acfBorderLeft",10);
	acfBorderRight=ini->ReadInteger("Default","acfBorderRight",474);
	if(acfBorderLeft>acfBorderRight)
	{
		int buf=acfBorderLeft;
		acfBorderLeft=acfBorderRight;
		acfBorderRight=buf;
		ini->WriteInteger("Default","acfBorderLeft",acfBorderLeft);
		ini->WriteInteger("Default","acfBorderRight",acfBorderRight);
	}

	CFFT_256=_ini->ReadBool("Default","CFFT_256",false);

	if(CFFT_256)
	{
		size=256;
		m=8;
	}
	else
	{
		size=_size;
		m=1;
		while(_size>>=1)
			++m;
	}
	int i,j,k;
	n=1<<m;
	nv=n>>1;
	h=new int[n];
	c=new double[nv];
	double r=(double)(M_PI/n);
	for(i=0;i<nv;i++)
		c[i]=(double) cos(r*i);
	h[0]=0;
	for(i=1,j=0,k=nv;i<n;i++)
	{
		h[i]=h[j++]+k;
		if((j<<1)>i)
		{
			j=0;
			k=k>>1;
		}
	}
	BUF=new double[n];
	BUF_spectro=new double[n];

}
// ------------------------------------------------------------------------------
CFFT::~CFFT()
{
	delete[]h;
	delete[]c;
	delete[]BUF;
	delete[]BUF_spectro;
}
// ------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
// Реализация методов
//////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------------
void CFFT::Direct(double *x)
{
	int i,ip,ir,is,j,k,ki,kd,l,le,le1,le2;
	double t1,t2,t3,t4,u1,u2;
	for(i=0;i<n;i++)
	{
		j=h[i];
		if(i<j)
		{
			t1=x[i];
			x[i]=x[j];
			x[j]=t1;
		}
	}
	for(l=1;l<=m;l++)
	{
		le=1<<l;
		le1=le>>1;
		for(i=0;i<n;i+=le)
		{
			ip=i+le1;
			t1=x[i];
			t3=x[ip];
			x[i]=t1+t3;
			x[ip]=t1-t3;
		}
		if(l>2)
		{
			le2=le>>2;
			k=n/le1;
			ki=0;
			kd=nv;
			for(j=1;j<le2;j++)
			{
				ki+=k;
				kd-=k;
				u1=c[ki];
				u2=c[kd];
				for(i=j,ir=le1-j;i<n;i+=le,ir+=le)
				{
					ip=i+le1;
					is=ir+le1;
					t1=x[i];
					t2=x[ir];
					t3=x[ip]*u1-x[is]*u2;
					t4=x[is]*u1+x[ip]*u2;
					x[i]=t1+t3;
					x[ir]=t1-t3;
					x[ip]=t4-t2;
					x[is]=t2+t4;
				}
			}
		}
	}
	t1=1.0/((double) le1);
	for(i=0;i<n;i++)
		x[i]*=t1;
}
// ------------------------------------------------------------------------------
void CFFT::Inverse(double *x)
{
	int i,ip,ir,is,j,k,ki,kd,l,le,le1,le2;
	double t1,t2,t3,t4,u1,u2;
	x[0]=0.5*x[0];
	x[nv]=0.5*x[nv];
	for(l=m;l>0;l--)
	{
		le=1<<l;
		le1=le>>1;
		for(i=0;i<n;i+=le)
		{
			ip=i+le1;
			t1=x[i];
			t3=x[ip];
			x[i]=t1+t3;
			x[ip]=t1-t3;
		}
		if(l>2)
		{
			le2=le>>2;
			k=n/le1;
			ki=0;
			kd=nv;
			for(j=1;j<le2;j++)
			{
				ki+=k;
				kd-=k;
				u1=c[ki];
				u2=c[kd];
				for(i=j,ir=le1-j;i<n;i+=le,ir+=le)
				{
					ip=i+le1;
					is=ir+le1;
					t1=x[i]+x[ir];
					t2=x[is]-x[ip];
					t3=x[i]-x[ir];
					t4=x[is]+x[ip];
					x[i]=t1;
					x[ir]=t2;
					x[ip]=t3*u1+t4*u2;
					x[is]=t4*u1-t3*u2;
				}
			}
		}
	}
	for(i=0;i<n;i++)
	{
		j=h[i];
		if(i<j)
		{
			t1=x[i];
			x[i]=x[j];
			x[j]=t1;
		}
	}
}
// ------------------------------------------------------------------------------
void CFFT::Spectrum(double *x)
{
	x[0]*=x[0];
	for(int i=1,j=n-1;i<nv;i++,j--)
	{
		x[i]=x[i]*x[i]+x[j]*x[j];
		x[j]=0;
	}
	x[nv]*=x[nv];
}
// ------------------------------------------------------------------------------
void CFFT::Calc(double* _buf)
{
	if(!CFFT_256)
		ZeroMemory(&BUF[size],sizeof(double)*(n-size));
	for(int z=0;z<size;++z)
		BUF[z]=_buf[z];
	Direct(BUF);
	Spectrum(BUF);
	CopyMemory(BUF_spectro,BUF,sizeof(double)*size);
	if(acfBorderLeft!=0)
		ZeroMemory(BUF,sizeof(double) * acfBorderLeft);
	if(acfBorderRight<size)
		ZeroMemory(BUF+acfBorderRight,sizeof(double) * (size-acfBorderRight));
	Direct(BUF);
	Spectrum(BUF);
}
void CFFT::Calc(char* _buf)
{
	if(!CFFT_256)
		ZeroMemory(&BUF[size],sizeof(double)*(n-size));
	for(int z=0;z<size;++z)
		BUF[z]=_buf[z];
	Direct(BUF);
	Spectrum(BUF);
	CopyMemory(BUF_spectro,BUF,sizeof(double)*size);
	if(acfBorderLeft!=0)
		ZeroMemory(BUF,sizeof(double) * acfBorderLeft);
	if(acfBorderRight<size)
		ZeroMemory(BUF+acfBorderRight,sizeof(double) * (size-acfBorderRight));
	Direct(BUF);
	Spectrum(BUF);
}
void CFFT::SetBorders(int _acfBorderLeft,int _acfBorderRight)
{
	acfBorderLeft=_acfBorderLeft;
	acfBorderRight=_acfBorderRight;
	if(acfBorderLeft<0)
		acfBorderLeft=0;
	if(acfBorderLeft>size)
		acfBorderLeft=size;
	if(acfBorderRight<0)
		acfBorderRight=0;
	if(acfBorderRight>size)
		acfBorderRight=size;
	if(acfBorderRight<acfBorderLeft)
		acfBorderRight=acfBorderLeft;
	ini->WriteInteger("Default","acfBorderLeft",acfBorderLeft);
	ini->WriteInteger("Default","acfBorderRight",acfBorderRight);
}
