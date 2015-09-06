//#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include "integer.h"
//using namespace std;

const int N = 1000; /* longint size */
const int LL = 10;/*int_max*/

Iterator longint::begin(){ return Iterator(a); }

Iterator longint::end(){
	Iterator i(a + N - 1);
	while (*i == 0)	i--;
	return i++;
}

longint::longint() {
	a = new int[N];
	for (int i = 0; i < N; ++i)
		a[i] = 0;
}

longint::longint(const longint& second) {
	a = new int[N];
	for (int i = 0; i < N; ++i)
		a[i] = second[i];
}

longint::longint(int noun) {
	a = new int[N];
	long long int ten = 1;
	for (int i = 0; i < LL; ++i) {
		a[i] = (noun / ten) % 10;
		ten = ten * 10;
	}
	for (int i = LL; i < N; ++i)
		a[i] = 0;
}

longint::~longint() {
	delete[]a;
}

int longint::length() const {
	int i = N - 1;
	while (a[i] == 0 && i > 0) i--;
	return i + 1;
}

int& longint::operator[](int i) const {
	return a[i];
}

bool longint::operator == (const longint& second) const {
	for (int i = 0; i < N; ++i)
	if (a[i] != second[i]) return false;
	return true;
}

bool longint::operator >(const longint& second) const {
	for (int i = N - 1; i >= 0; --i) {
		if (a[i] > second[i]) return true;
		if (a[i] < second[i]) return false;
	}
	return false;
}

bool longint::operator < (const longint& second) const{
	for (int i = N - 1; i >= 0; --i) {
		if (a[i] > second[i]) return false;
		if (a[i] < second[i]) return true;
	}
	return false;
}

bool longint::operator >= (const longint& second) const {
	return *this > second || *this == second;
}

bool longint::operator <= (const longint& second) const{
	return *this < second || *this == second;
}

std::ostream& operator << (std::ostream& out, longint& first) {
	int i = N - 1;
	while (i >= 0 && first[i] == 0)
		i--;
	if (i == -1) std::cout << 0;
	else
	for (int j = i; j >= 0; --j)
		out << first[j];
	return out;
}

std::istream& operator >> (std::istream& sin, longint& first) {
	char c;
	char buff[N];
	int i = 0;
	while (c = getchar()) {
		if (c == ' ' || c == '\0' || c == '\n' || c == '\t')
			break;
		buff[i] = c;
		i++;
	}
	bool b = false;
	for (int j = 0; j < i; ++j) {
		if (buff[i - j - 1] == '-')
			b = true;
		else
			first[j] = (int)buff[i - j - 1] - (int)'0';
	}
	if (b == true)
		first[first.length() - 1] *= -1;
	return sin;
}

longint longint::operator + (const longint& second) const{
	longint c, one = *this, two = second;
	int sum;

	if (*this > 0 && second < 0) {
		two[two.length() - 1] *= -1;
		return one - two;
	}
	if (*this < 0 && second > 0) {
		one[one.length() - 1] *= -1;
		return two - one;
	}

	bool b = false;
	if (*this < 0 && second < 0) {
		one[one.length() - 1] *= -1;
		two[two.length() - 1] *= -1;
		b = true;
	}

	for (int i = 0; i < N - 1; ++i) {
		sum = one[i] + two[i] + c[i];
		c[i] = sum % 10;
		c[i + 1] = sum / 10;
	}
	if (b == true)
		c[c.length() - 1] *= -1;
	return c;
}

longint longint::operator * (const longint& second) const {
	longint c, one = *this, two = second;
	bool check = false;

	if (*this < 0 && second > 0) {
		one[one.length() - 1] *= -1;
		check = true;
	}
	if (second < 0 && *this > 0) {
		two[two.length() - 1] *= -1;
		check = true;
	}
	if (*this < 0 && second < 0) {
		one[one.length() - 1] *= -1;
		two[two.length() - 1] *= -1;
	}

	for (int i = 0; i < N / 2; ++i)
	for (int j = 0; j < N / 2 - 1; ++j)
		c[i + j] = c[i + j] + one[i] * two[j];

	for (int i = 0; i < N - 1; ++i) {
		c[i + 1] = c[i + 1] + c[i] / 10;
		c[i] = c[i] % 10;
	}

	if (check == true)
		c[c.length() - 1] *= -1;
	return c;
}

longint& longint::operator = (const longint& second) {
	if (this == &second) return *this;
	for (int i = 0; i < N; ++i)
		a[i] = second[i];
	return *this;
}

longint longint::operator - (const longint& second) const {
	longint c;
	bool choose = false;

	if (*this < 0 && second < 0) {
		longint tempfirst = *this;
		longint tempsecond = second;
		tempfirst[length() - 1] *= -1;
		tempsecond[second.length() - 1] *= -1;
		longint Return = tempfirst + tempsecond;
		Return[Return.length() - 1] *= -1;
		return Return;
	}

	if (second < 0 && *this > 0) {
		longint temp = second;
		temp[temp.length() - 1] *= -1;
		return temp + *this;
	}
	if (second > 0 && *this < 0) {
		longint temp = *this;
		temp[temp.length() - 1] *= -1;
		return temp + *this;
	}

	longint one = *this, two = second;
	if (*this < second) {
		std::swap(one, two);
		choose = true;
	}

	for (int i = 0; i < N - 1; ++i) {
		c[i] = c[i] + 10 + one[i] - two[i];
		c[i + 1] = (c[i] >= 10) ? 0 : -1;
		c[i] = c[i] % 10;
	}

	int i = N - 1;
	while (c[i] != 0) {
		c[i] = 0;
		i--;
	}

	if (choose == true)
		c[c.length() - 1] *= -1;
	return c;
}

longint& longint::operator -= (const longint& second) {
	longint a = *this - second;
	return a;
}

void longint::LevelUp()
{
	for (int i = length(); i >= 1; i--)
		a[i] = a[i - 1];
}

longint longint::operator / (const longint& second) const
{
	longint res;
	longint curValue;
	longint one = *this, two = second;
	bool check = false;

	if (*this < 0 && second > 0) {
		one[one.length() - 1] *= -1;
		check = true;
	}
	if (*this > 0 && second < 0) {
		two[two.length() - 1] *= -1;
		check = true;
	}

	for (int i = one.length() - 1; i >= 0; i--)
	{
		curValue.LevelUp(); // * osn
		curValue[0] = one.a[i];

		int x = 0;
		int l = 0, r = 10;
		while (l <= r)
		{
			int m = (l + r) >> 1;
			longint cur = two * m;
			if (cur <= curValue)
			{
				x = m;
				l = m + 1;
			}
			else
				r = m - 1;
		}
		res[i] = x;
		curValue = curValue - two * x;
	}

	int pos = length();
	while (pos >= 0 && !res[pos])
		pos--;
	if (check == true)
		res[res.length() - 1] *= -1;
	return res;
}

longint& longint::operator /= (const longint& second) {
	longint a = *this / second;
	return a;
}

longint longint::operator % (const longint& second) const {
	return *this - (*this / second) * second;
}
