#pragma once
#include <iostream>
#include <stdio.h>
//using namespace std;





class Iterator{
private:
	int*p;
public:
	Iterator(){
		p = 0;
	}
	Iterator(int * a){
		p = a;
	}
	Iterator & operator ++ (){
		p++;
		return *this;
	}
	Iterator & operator -- (){
		p--;
		return *this;
	}
	int operator * (){
		return *p;
	}
	bool operator != (Iterator & reference){
		return p != reference.p;
	}
	bool operator == (Iterator & reference){
		return p == reference.p;
	}
};

class longint {
private:
	int *a;
public:
	longint();
	longint(const longint&);
	longint(int);
	~longint();
	int& operator[] (int) const;
	int length() const;
	void LevelUp();
	bool operator > (const longint&) const;
	bool operator < (const longint&) const;
	bool operator == (const longint&) const;
	bool operator >= (const longint&) const;
	bool operator <= (const longint&) const;
	friend std::istream& operator >> (std::istream&, longint&);
	friend std::ostream& operator << (std::ostream&, longint&);
	longint  operator + (const longint&) const;
	longint  operator * (const longint&) const;
	longint  operator - (const longint&) const;
	longint  operator / (const longint&) const;
	longint  operator % (const longint&) const;
	longint& operator +=(const longint&);
	longint& operator = (const longint&);
	longint& operator -=(const longint&);
	longint& operator *=(const longint&);
	longint& operator /=(const longint&);
	Iterator begin();
	Iterator end();
};