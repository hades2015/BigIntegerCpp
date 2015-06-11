#include <iostream>
#include <vector>

using namespace std;

class BigInteger {
public :
	BigInteger() : isPositive(true) {
	}

	BigInteger(long long val) {
		isPositive = val >= 0;

		if (!isPositive)
			val *= -1;

		to_vector(val);
	}

	void to_vector(long long val) {
		num = vector<int>();

		if (val == 0)
			num = vector<int>(1, 0);

		while (val) {
			num.push_back(val % 10);
			val /= 10;
		}
	}

	int get(int digit) const {
		return num.size() <= digit ? 0 : num[digit];
	}

	friend const BigInteger operator+(const BigInteger& n1, const BigInteger& n2){
		BigInteger result, first(n1), second(n2);

		if(n1.isPositive != n2.isPositive){
			first.isPositive = !first.isPositive;
			result = first - second;

			if(n1.getAbs() > n2.getAbs()){
				result.isPositive = n1.isPositive;
			}
			else{
				result.isPositive = n2.isPositive;	
			}

			return result;
		}

		int carry = 0;
		for(int i=0; i < max(first.num.size(), second.num.size()); ++i) {
			int calc = first.get(i) + second.get(i) + carry;
			result.num.push_back(calc % 10);
			carry = calc / 10;
		}
		if (carry)
			result.num.push_back(carry);

		result.isPositive = n1.isPositive;
		return result;
	}

	friend const BigInteger operator-(const BigInteger& n1, const BigInteger& n2){
		BigInteger result, first(n1), second(n2);
		result.isPositive = n1.isPositive;

		if(n1.isPositive != n2.isPositive){
			first.isPositive = !first.isPositive;
			result = first + second;
			result.isPositive = n1.isPositive;

			return result;
		}

		if(n1.getAbs() < n2.getAbs()){
			first = n2;
			second = n1;
			result.isPositive = !result.isPositive;
		}

		int carry = 0;
		for(int i=0; i < max(first.num.size(), second.num.size()); ++i) {
			int calc = first.get(i) - second.get(i) + carry;
			carry = 0;
			if(calc < 0){
				carry = -1;
				calc += 10;
			}
			result.num.push_back(calc);
		}
		if (carry)
			result.num.push_back(carry);


		for(int i=result.num.size()-1; !result.num[i] && i>0; i--){
			result.num.pop_back();
		}

		return result;
	}

	void print() {
		if(!isPositive) cout <<"-";
		for (int i = num.size() - 1; i >= 0; --i)
			cout << num[i];
		//cout << endl;
	}

	friend bool operator==(const BigInteger& left, const BigInteger& right) {
		return left.isPositive == right.isPositive && left.num == right.num;
	}

	friend bool operator>(const BigInteger& left, const BigInteger& right){
		if(left.isPositive != right.isPositive){
			return left.isPositive ? true : false;	
		}

		if(left.num.size() > right.num.size()){
			return left.isPositive ? true : false;
		}
		else if(left.num.size() < right.num.size()){
			return left.isPositive ? false : true;
		}

		for(int i=left.num.size()-1; i>=0; --i){
			if(left.num[i] > right.num[i]) return left.isPositive ? true : false;
			else if(left.num[i] < right.num[i]) return left.isPositive ? false : true;
		}

		return false;
	}

	friend bool operator>=(const BigInteger& left, const BigInteger& right){
		return (left > right) || (left == right);
	}

	friend bool operator<(const BigInteger& left, const BigInteger& right){
		return !(left >= right);
	}

	friend bool operator<=(const BigInteger& left, const BigInteger& right){
		return (left < right || left == right);
	}

	friend bool operator!=(const BigInteger& left, const BigInteger& right) {
		return !(left.isPositive == right.isPositive && left.num == right.num);
	}

	const BigInteger getAbs() const{
		BigInteger result(*this);
		result.isPositive = true;

		return result;
	}

private :
	vector<int> num;
	bool isPositive;
};

void testAdd();
void testLeftGreater();
void testLeftGreaterEqual();
void testRightGreater();
void testRightGreaterEqual();
void testSub();
void testNotEqual();

int main() {
	testAdd();
	testLeftGreater();
	testLeftGreaterEqual();
	testRightGreater();
	testRightGreaterEqual();
	testSub();
	testNotEqual();
}

void testAdd(){
	BigInteger num1(100);
	BigInteger num2(97);
	BigInteger num3(999);
	BigInteger zero(0);
	BigInteger num4(-24);
	BigInteger num5(-87);

	BigInteger result1 = num1 + 12;
	BigInteger result2 = 12 + num1;
	BigInteger result3 = num2 + 36;
	BigInteger result4 = num2 + num3;
	BigInteger result5 = num1 + zero;
	BigInteger result6 = num4 + num5;
	BigInteger result7 = num1 + num4;
	BigInteger result8 = num4 + num1;

	BigInteger answer1(112);
	BigInteger answer2(112);
	BigInteger answer3(133);
	BigInteger answer4(1096);
	BigInteger answer5 = num1;
	BigInteger answer6(-111);
	BigInteger answer7(76);
	BigInteger answer8(76);

	cout << (result1 == answer1) << endl;
	cout << (result2 == answer2) << endl;
	cout << (result3 == answer3) << endl;
	cout << (result4 == answer4) << endl;
	cout << (result5 == answer5) << endl;
	cout << (result6 == answer6) << endl;
	cout << (result7 == answer7) << endl;
	cout << (result8 == answer8) << endl;
}
void testLeftGreater(){
	BigInteger num1(100);
	BigInteger zero(0);
	BigInteger num2(-24);
	BigInteger num3(-87);

	bool result1 = (num1 > zero);
	bool result2 = (zero > num1);
	bool result3 = (num1 > num2);
	bool result4 = (num2 > num1);
	bool result5 = (num2 > num3);
	bool result6 = (num3 > num2);

	bool answer1 = true;
	bool answer2 = false;
	bool answer3 = true;
	bool answer4 = false;
	bool answer5 = true;
	bool answer6 = false;

	cout << (result1 == answer1) << endl;
	cout << (result2 == answer2) << endl;
	cout << (result3 == answer3) << endl;
	cout << (result4 == answer4) << endl;
	cout << (result5 == answer5) << endl;
	cout << (result6 == answer6) << endl;
}
void testLeftGreaterEqual(){
	BigInteger num1(10);
	BigInteger num2(9);
	BigInteger num3(10);
	BigInteger num4(-10);
	BigInteger num5(-9);
	BigInteger num6(-9);

	bool result1 = (num1 >= num2);
	bool result2 = (num2 >= num1);
	bool result3 = (num1 >= num3);
	bool result4 = (num1 >= num4);
	bool result5 = (num4 >= num5);
	bool result6 = (num5 >= num4);
	bool result7 = (num5 >= num6);

	bool answer1 = true;
	bool answer2 = false;
	bool answer3 = true;
	bool answer4 = true;
	bool answer5 = false;
	bool answer6 = true;
	bool answer7 = true;

	cout << (result1 == answer1) << endl;
	cout << (result2 == answer2) << endl;
	cout << (result3 == answer3) << endl;
	cout << (result4 == answer4) << endl;
	cout << (result5 == answer5) << endl;
	cout << (result6 == answer6) << endl;
	cout << (result7 == answer7) << endl;
}
void testRightGreater()
{
	BigInteger num1(100);
	BigInteger zero(0);
	BigInteger num2(-24);
	BigInteger num3(-87);

	bool result1 = (num1 < zero);
	bool result2 = (zero < num1);
	bool result3 = (num1 < num2);
	bool result4 = (num2 < num1);
	bool result5 = (num2 < num3);
	bool result6 = (num3 < num2);

	bool answer1 = false;
	bool answer2 = true;
	bool answer3 = false;
	bool answer4 = true;
	bool answer5 = false;
	bool answer6 = true;

	cout << (result1 == answer1) << endl;
	cout << (result2 == answer2) << endl;
	cout << (result3 == answer3) << endl;
	cout << (result4 == answer4) << endl;
	cout << (result5 == answer5) << endl;
	cout << (result6 == answer6) << endl;
}
void testRightGreaterEqual()
{
	BigInteger num1(10);
	BigInteger num2(9);
	BigInteger num3(10);
	BigInteger num4(-10);
	BigInteger num5(-9);
	BigInteger num6(-9);

	bool result1 = (num1 <= num2);
	bool result2 = (num2 <= num1);
	bool result3 = (num1 <= num3);
	bool result4 = (num1 <= num4);
	bool result5 = (num4 <= num5);
	bool result6 = (num5 <= num4);
	bool result7 = (num5 <= num6);

	bool answer1 = false;
	bool answer2 = true;
	bool answer3 = true;
	bool answer4 = false;
	bool answer5 = true;
	bool answer6 = false;
	bool answer7 = true;

	cout << (result1 == answer1) << endl;
	cout << (result2 == answer2) << endl;
	cout << (result3 == answer3) << endl;
	cout << (result4 == answer4) << endl;
	cout << (result5 == answer5) << endl;
	cout << (result6 == answer6) << endl;
	cout << (result7 == answer7) << endl;
}
void testSub()
{
	BigInteger num1(100);
	BigInteger num2(8);
	BigInteger num3(17);
	BigInteger num4(-24);
	BigInteger num5(-31);

	vector<BigInteger> result;
	result.push_back(num1 - num2);
	result.push_back(num1 - num1);
	result.push_back(num2 - num3);
	result.push_back(num4 - num5);
	result.push_back(num5 - num4);
	result.push_back(num4 - num3);
	result.push_back(num3 - num4);

	vector<BigInteger> answer;
	answer.push_back(92);
	answer.push_back(0);
	answer.push_back(-9);
	answer.push_back(7);
	answer.push_back(-7);
	answer.push_back(-41);
	answer.push_back(41);

	for(size_t i=0; i<result.size(); i++){
		if(result[i] == answer[i]){
			cout << true;
		}
		else{
			cout << false << " // ";
			cout << "actual : ";
			result[i].print();
			cout << ", expected : ";
			answer[i].print();
		}
		cout << endl;
	}
}
void testNotEqual(){
	BigInteger num1(2);
	BigInteger num2(2);
	BigInteger num3(3);

	bool result1 = num1 != num2;
	bool result2 = num1 != num3;

	bool answer1 = false;
	bool answer2 = true;

	cout << (result1 == answer1) << endl;
	cout << (result2 == answer2) << endl;
}