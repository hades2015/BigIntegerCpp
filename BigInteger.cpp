#include<iostream>
#define MAX_DIGIT 30

using namespace std;

class BigInteger
{
private :
	unsigned char num[MAX_DIGIT]; // 0번 인덱스가 최하위 자리, n번 인덱스가 최상위 자리
	bool isPositive;

public : // 생성자, 소멸자
	explicit BigInteger() : isPositive(true)
	{
		int i;
		for(i=0; i<MAX_DIGIT; ++i)
			num[i] = 0;
	}
	BigInteger(long long val)
	{
		if(val >= 0) isPositive = true;
		else
		{
			isPositive = false;
			val *= -1;
		}

		unsigned char* tmp = divideDigit(val);
		int i=0;
		for(i=0; i<MAX_DIGIT; ++i)
			this->num[i] = tmp[i];

		delete []tmp;
	}
	BigInteger(const BigInteger& val)
	{
		this->isPositive = val.isPositive;

		int i;
		for(i=0; i<MAX_DIGIT; ++i)
			this->num[i] = val.num[i];
	}
	~BigInteger(){}
	
public : // 덧셈 부분
	BigInteger operator+(const BigInteger& postNum)
	{
		BigInteger temp;
		if(!isSameSign(this->isPositive, postNum.isPositive)){ // 부호가 다른 경우 덧셈
			temp = postNum;
			temp.isPositive = !temp.isPositive; // 뒤 숫자의 부호를 바꾸고
			return *this-temp; // 뺄셈으로 처리
		}

		// 부호가 같은 경우 덧셈
		int carry = 0;
		int i;
		for(i=0; i<MAX_DIGIT; ++i)
		{
			temp.num[i] = this->num[i] + postNum.num[i] + carry;
			carry = 0;
			if(temp.num[i] >= 10)
			{
				carry = 1;
				temp.num[i] %= 10;
			}
		}
		temp.isPositive = this->isPositive;
		return temp;
	}
	BigInteger operator+(const long long val)
	{
		BigInteger temp(val);
		return *this+temp;
	}
	friend BigInteger operator+(const long long val, const BigInteger& postNum)
	{	
		BigInteger left(val);
		BigInteger right(postNum);
		return left+right;
		//return postNum + val; // postNum의 const를 쓰지 않을 때 가능
	}

public : // 뺄셈 부분
	BigInteger operator-(const BigInteger& postNum)
	{
		BigInteger left, right, temp;
		if(!isSameSign(this->isPositive, postNum.isPositive)){ // 부호가 다른 경우의 뺄셈
			temp = postNum;
			temp.isPositive = !temp.isPositive; // 뒤 숫자의 부호를 바꾸고
			return *this+temp; // 덧셈으로 처리
		}
		
		// 부호가 같은 경우 뺄셈 : 크기비교 -> 절대 값 큰 수 - 절대 값 작은 수
		if(abs(*this) >= abs(postNum))
		{
			left = *this; 
			right = postNum;
			if(this->isPositive) temp.isPositive = true; // 왼쪽 절대 값이 크고 양수면 양수 결과(4-2)
			else temp.isPositive = false; // 왼쪽 절대 값이 크고 음수면 음수 결과(-4 - (-2))
		}
		else
		{
			left = postNum;
			right = *this;
			if(this->isPositive) temp.isPositive = false; // 오른쪽 절대 값이 크고 양수면 음수 결과(2-4)
			else temp.isPositive = true; // 오른쪽 절대 값이 크고 음수면 양수 결과(-2 - (-4))
		}
		int carry = 0;
		int i;
		for(i=0; i<MAX_DIGIT; ++i)
		{
			char sub = 0;
			sub = left.num[i] - right.num[i] + carry;
			carry = 0;
			if(sub < 0)
			{
				carry = -1;
				sub += 10;
			}
			temp.num[i] = sub;
		}

		return temp;
	}
	BigInteger operator-(const long long val)
	{
		BigInteger temp(val);
		return *this-temp;
	}
	friend BigInteger operator-(const long long val, const BigInteger& postNum)
	{
		BigInteger left(val);
		BigInteger right(postNum);
		return left-right;
	}

public : // 곱셈 부분
	BigInteger operator*( BigInteger& postNum)
	{
		/*BigInteger i = 0, sum = 0;
		for(i=0; i<postNum; i=i+1)
			sum = sum + *this;
		return sum;*/

		BigInteger temp;
		int carry = 0;
		int i,j;
		for(i=0; i<MAX_DIGIT; ++i)
		{
			carry = 0;
			for(j=0; j<MAX_DIGIT; ++j)
			{
				temp.num[i+j] += this->num[j] * postNum.num[i] + carry;
				carry = 0;
				if(temp.num[i] >= 10)
				{
					carry = temp.num[i+j] / 10;
					temp.num[i+j] %= 10;
				}
			}
		}
		
		if(!isSameSign(this->isPositive, postNum.isPositive)) // 부호가 다른 경우 음수 결과
			temp.isPositive = false;
		return temp;
	}
	BigInteger operator*(const long long val)
	{
		BigInteger temp(val);
		return (*this)*temp;
	}
	friend BigInteger operator*(const long long val, const BigInteger& postNum)
	{
		BigInteger left(val);
		BigInteger right(postNum);
		return left*right;
	}

public : // 나눗셈 부분
	BigInteger operator/(const BigInteger& postNum)
	{
		BigInteger share = 0;
		BigInteger dividend = *this;
		BigInteger divisor = postNum;
		dividend.isPositive = true;
		divisor.isPositive = true;

		while(dividend >= divisor)
		{
			dividend = dividend - divisor;
			share = share + 1;
		}
		if(!isSameSign(this->isPositive, postNum.isPositive)) share.isPositive = false;
		return share;
	}
	BigInteger operator/(const long long val)
	{
		BigInteger temp(val);
		return (*this) / temp;
	}
	friend BigInteger operator/(const long long val, const BigInteger& postNum)
	{
		BigInteger temp(val);
		return temp / postNum;
	}

public : // 크기 비교 부분
	bool operator>(const BigInteger& postNum)
	{
		if(this->isPositive ^ postNum.isPositive)
		{
			if(this->isPositive) return true;
			else return false;
		}

		bool returnValue = false;

		int i=MAX_DIGIT-1;
		while(i >= 0)
		{
			if(this->num[i] > postNum.num[i])
			{
				returnValue = true;
				break;
			}
			else if(this->num[i] < postNum.num[i])
			{
				returnValue = false;
				break;
			}
			--i;
		}
		if(i <= -1) return false; // 두 수가 같은 경우

		if(!this->isPositive) returnValue = (!returnValue);
		return returnValue;
	}
	bool operator>(const long long val)
	{
		BigInteger temp(val);
		return (*this) > temp ;
	}
	friend bool operator>(const long long val, const BigInteger& postNum)
	{
		BigInteger temp(val);
		return temp > postNum;
	}

	bool operator<(const BigInteger& postNum)
	{
		if(this->isPositive ^ postNum.isPositive)
		{
			if(this->isPositive) return false;
			else return true;
		}

		bool returnValue = false;

		int i=MAX_DIGIT-1;
		while(i >= 0)
		{
			if(this->num[i] < postNum.num[i])
			{
				returnValue = true;
				break;
			}
			else if(this->num[i] > postNum.num[i])
			{
				returnValue = false;
				break;
			}
			--i;
		}
		if(i <= -1) return false; // 두 수가 같은 경우

		if(!this->isPositive) returnValue = (!returnValue);
		return returnValue;
	}
	bool operator<(const long long val)
	{
		BigInteger temp(val);
		return (*this) < temp ;
	}
	friend bool operator<(const long long val, const BigInteger& postNum)
	{
		BigInteger temp(val);
		return temp < postNum;
	}

	bool operator>=(const BigInteger& postNum)
	{
		if(*this > postNum) return true;
		else if(*this < postNum) return false;
		else return true;
	}
	bool operator>=(const long long val)
	{
		BigInteger temp(val);
		return (*this) >= temp;
	}
	friend bool operator>=(const long long val, const BigInteger& postNum)
	{
		BigInteger temp(val);
		return temp >= postNum;
	}

	bool operator<=(const BigInteger& postNum)
	{
		if(*this < postNum) return true;
		else if(*this > postNum) return false;
		else return true;
	}
	bool operator<=(const long long val)
	{
		BigInteger temp(val);
		return (*this) <= temp;
	}
	friend bool operator<=(const long long val, const BigInteger& postNum)
	{
		BigInteger temp(val);
		return temp <= postNum;
	}

	bool operator==(const BigInteger& postNum)
	{
		if((*this > postNum) || (*this < postNum))
			return false;
		return true;
	}
	bool operator==(const long long val)
	{
		BigInteger temp(val);
		return (*this) == temp;
	}
	friend bool operator==(const long long val, const BigInteger& postNum)
	{
		BigInteger temp(val);
		return temp == postNum;
	}

public : // 비트 연산, 전위 연산, 후위 연산

public : // 출력 부분
	friend ostream& operator<<(ostream& os, const BigInteger& val)
	{ 
		if(!val.isPositive) os << "-";
		bool trimFlag = true;
		int i;
		for(i=MAX_DIGIT-1; i>=0; --i)
		{
			if(!trimFlag || val.num[i])
			{
				trimFlag = false;
				os << (int)val.num[i];
			}
		}
		if(trimFlag) os << "0";
		
		return os;
	}
	
public : // 기타 부분
	unsigned char* divideDigit(long long val)
	{
		unsigned char* tmp = new unsigned char[MAX_DIGIT]();
		int i; 
		for(i=0; i<MAX_DIGIT ; ++i)
		{
			int digit = val % 10;
			tmp[i] = digit;
			val /= 10;
		}
		return tmp;
	}
	void printNum()
	{
		if(!this->isPositive) cout << "-";
		int i=0;
		for(i=MAX_DIGIT-1; i>=0; i--)
			cout << (int)num[i];
		cout << endl;
	}
	friend BigInteger abs(const BigInteger& val);
	bool isSameSign(bool left, bool right)
	{
		if(left ^ right) return false;
		return true;
	}
};

BigInteger abs(const BigInteger& val)
{
	BigInteger temp(val);
	temp.isPositive = true;

	return temp;
}


int main()
{
	

	return 0;
}

