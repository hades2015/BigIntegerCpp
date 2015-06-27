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

			result.isPositive = (n1.getAbs() > n2.getAbs() ? n1.isPositive : n2.isPositive);
			return result;
		}

		int carry = 0;
		for(size_t i=0; i < max(first.num.size(), second.num.size()); i++) {
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
		for(size_t i=0; i < max(first.num.size(), second.num.size()); ++i) {
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


		for(size_t i=result.num.size()-1; !result.num[i] && i>0; i--){
			result.num.pop_back();
		}

		return result;
	}

	friend const BigInteger operator*(const BigInteger& n1, const BigInteger& n2){
		/*BigInteger result, greater, less;
		greater = max(n1.getAbs(), n2.getAbs());
		less = min(n1.getAbs(), n2.getAbs());

		for(int i=0; i<less; i++){
			result = result + greater;
		}
		result.isPositive = (n1.isPositive == n2.isPositive ? true : false);

		if(result.num.empty()) result = 0;

		return result;*/

		BigInteger result;

		int carry = 0;
		for(size_t i=0; i<n2.num.size(); i++){
			carry = 0;
			for(size_t j=0; j<n1.num.size(); j++){
				int calc = n2.get(i) * n1.get(j) + carry;
				carry = calc / 10;
				if(i+j >= result.num.size()){
					result.num.push_back(calc % 10);
				}
				else{
					result.num[i+j] += calc % 10;
					if(result.num[i+j] >= 10){
						carry += result.num[i+j] / 10;
						result.num[i+j] = result.num[i+j] % 10;
					}
				}
			}
			if(carry)
				result.num.push_back(carry);
		}

		for(size_t i=result.num.size()-1; !result.num[i] && i>0; i--){
			result.num.pop_back();
		}

		result.isPositive = (n1.isPositive == n2.isPositive ? true : false);
		if(result.getAbs() == 0) result.isPositive = true;
		return result;
	}

	friend const BigInteger operator/(const BigInteger& n1, const BigInteger& n2){
		try{
			if(n2 == 0){
				throw "ArithmeticException";
			}
			BigInteger result(0);
			BigInteger dividend(n1.getAbs()), divisor(n2.getAbs());

			while(dividend >= divisor){
				dividend = dividend - divisor;
				result = result + 1;
			}

			result.isPositive = (n1.isPositive == n2.isPositive) ? true : false;
			return result;
		}
		catch(const char* e){
			cout << e << endl;
			return -999999;
		}
	}

	friend const BigInteger operator%(const BigInteger& n1, const BigInteger& n2){
		try{
			if(n2 == 0){
				throw "ArithmeticException";
			}

			BigInteger result, share;
		
			share = n1 / n2;
			result = n1 - share * n2;

			return result;
		}
		catch(const char* e){
			cout << e << endl;
			return -99999;
		}
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

	friend const BigInteger operator<<(const BigInteger& srcNum, const BigInteger& shiftCount){
		try{
			if(shiftCount < 0) throw "shiftCount is not positive" ;
			BigInteger result(srcNum);

			for(int i=0; i<shiftCount; i++){
				result = result * 2;
			}

			return result;
		}
		catch(const char* e){
			cout << e << endl;
			return -99999;
		}
	}

	friend const BigInteger operator>>(const BigInteger& srcNum, const BigInteger& shiftCount){
		try{
			if(shiftCount < 0) throw "shiftCount is not positive";
			BigInteger result(srcNum);

			for(int i=0; i<shiftCount; i++){
				if(srcNum < 0 && result.num[0] & 1) result = result - 1;
				result = result / 2;
			}

			return result;
		}
		catch(const char* e){
			cout << e << endl;
			return -99999;
		}
	}

	friend bool operator&&(const BigInteger& n1, const BigInteger& n2){
		if(n1 == 0 || n2 == 0) return false;
		return true;
	}

	friend bool operator||(const BigInteger& n1, const BigInteger& n2){
		if(n1 == 0 && n2 == 0) return false;
		return true;
	}

	friend const BigInteger operator&(const BigInteger& n1, const BigInteger& n2){
		BigInteger result = n1.innerBitwise(n2, '&');
		return result;
	}

	friend const BigInteger operator|(const BigInteger& n1, const BigInteger& n2){
		BigInteger result = n1.innerBitwise(n2, '|');
		return result;
	}

	friend const BigInteger operator^(const BigInteger& n1, const BigInteger& n2){
		BigInteger result = n1.innerBitwise(n2, '^');
		return result;
	}

	const BigInteger innerBitwise(const BigInteger& other, char op) const{
		BigInteger result, n1_binary(*this), n2_binary(other);
		
		n1_binary = n1_binary.convertToBinary();
		n2_binary = n2_binary.convertToBinary();
		
		for(size_t i=0; i<min(n1_binary.num.size(), n2_binary.num.size()); i++){
			if(op == '&')
				result.num.push_back(n1_binary.get(i) & n2_binary.get(i));
			else if(op == '|')
				result.num.push_back(n1_binary.get(i) | n2_binary.get(i));
			else if(op == '^')
				result.num.push_back(n1_binary.get(i) ^ n2_binary.get(i));
			else{
				cout << "Not Define Operator" << endl;
				exit(1);
			}
		}
		
		if(n1_binary.num.size() < n2_binary.num.size()){
			if(op == '&' && !this->isPositive){
				for(size_t i=n1_binary.num.size(); i<n2_binary.num.size(); i++){
					result.num.push_back(n2_binary.get(i));
				}
			}
			else if(op == '|' && this->isPositive){
				for(size_t i=n1_binary.num.size(); i<n2_binary.num.size(); i++){
					result.num.push_back(n2_binary.get(i));
				}
			}
			else if(op == '^'){
				for(size_t i=n1_binary.num.size(); i<n2_binary.num.size(); i++){
					int bit = n2_binary.get(i);
					if(!this->isPositive) bit = !bit;
					result.num.push_back(bit);
				}
			}
		}
		else if(n1_binary.num.size() > n2_binary.num.size()){
			if(op == '&' && !other.isPositive){
				for(size_t i=n2_binary.num.size(); i<n1_binary.num.size(); i++){
					result.num.push_back(n1_binary.get(i));
				}
			}
			else if(op == '|' && other.isPositive){
				for(size_t i=n2_binary.num.size(); i<n1_binary.num.size(); i++){
					result.num.push_back(n1_binary.get(i));
				}
			}
			else if(op == '^'){
				for(size_t i=n2_binary.num.size(); i<n1_binary.num.size(); i++){
					int bit = n1_binary.get(i);
					if(!other.isPositive) bit = !bit;
					result.num.push_back(bit);
				}
			}
		}
		
		if(op == '&'){
			if(!this->isPositive && !other.isPositive)
				result.isPositive = false;
		}
		else if(op == '|'){
			if(!this->isPositive || !other.isPositive)
				result.isPositive = false;
		}
		else if(op == '^'){
			if(this->isPositive != other.isPositive)
				result.isPositive = false;
		}
		
		result = result.convertToDecimal();
		return result;
	}

	const BigInteger convertToBinary(){
		BigInteger result;

		while(1){
			int calc = this->get(0) % 2;
			result.num.push_back(calc);
			*this = *this >> 1;
			if(*this == 0 || *this == -1) break;
		}
		
		return result;
	}

	const BigInteger convertToDecimal(){
		BigInteger result(0), digit = 1;
		
		for(size_t i=0; i<num.size(); i++){
			result = result + get(i) * digit;
			digit = digit * 2;
		}

		if(!isPositive){
			result = result - digit;
		}

		return result;
	}

	const BigInteger getAbs() const{
		BigInteger result(*this);
		result.isPositive = true;

		return result;
	}

	friend ostream& operator<<(ostream& os, const BigInteger& val)
	{ 
		if(!val.isPositive) os << "-";
		for(int i=val.num.size()-1; i>=0; --i){
				os << val.num[i];
		}

		return os;
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
void testMul();
void testDiv();
void testLeftShift();
void testRightShift();
void testMod();
void testAndLogic();
void testOrLogic();
void testAndBitwise();
void testOrBitWise();
void testXorBitwise();

int main() {
	testAdd();
	testLeftGreater();
	testLeftGreaterEqual();
	testRightGreater();
	testRightGreaterEqual();
	testSub();
	testNotEqual();
	testMul();
	testDiv();
	testLeftShift();
	testRightShift();
	testMod();
	testAndLogic();
	testOrLogic();
	testAndBitwise();
	testOrBitWise();
	testXorBitwise();

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
void testMul(){
	BigInteger num1(13);
	BigInteger num2(17);
	BigInteger num3(-9);
	BigInteger num4(-19);
	BigInteger num5(999);
	BigInteger num6(99);
	BigInteger num7(1);
	BigInteger num8(0);

	vector<BigInteger> result;
	result.push_back(num1 * num2);
	result.push_back(num2 * num3);
	result.push_back(num3 * num2);
	result.push_back(num3 * num4);
	result.push_back(num5 * num6);
	result.push_back(num6 * num7); 
	result.push_back(num4 * num7); 
	result.push_back(num6 * num8); 
	result.push_back(num8 * num4); 

	vector<BigInteger> answer;
	answer.push_back(221);
	answer.push_back(-153);
	answer.push_back(-153);
	answer.push_back(171);
	answer.push_back(98901);
	answer.push_back(99);
	answer.push_back(-19);
	answer.push_back(0);
	answer.push_back(0);

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
void testDiv(){
	BigInteger num1(12);
	BigInteger num2(3);
	BigInteger num3(24);
	BigInteger num4(5);
	BigInteger num5(-4);
	BigInteger num6(-34);
	BigInteger num7(0);

	vector<BigInteger> result;
	result.push_back(num1 / num2);
	result.push_back(num1 / num3);
	result.push_back(num1 / num4);
	result.push_back(num1 / num5);
	result.push_back(num6 / num4);
	result.push_back(num6 / num5);
	result.push_back(num5 / num6);
	result.push_back(num1 / num7); // ArithmeticException

	vector<BigInteger> answer;
	answer.push_back(4);
	answer.push_back(0);
	answer.push_back(2);
	answer.push_back(-3);
	answer.push_back(-6);
	answer.push_back(8);
	answer.push_back(0);
	answer.push_back(-999999); // ArithmeticException

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
void testLeftShift(){
	BigInteger num1(14);
	BigInteger num2(-37);
	BigInteger num3(0);
	BigInteger num4(34);
	BigInteger num5(3);

	vector<BigInteger> result;
	result.push_back(num1 << 2);
	result.push_back(num1 << 0);
	result.push_back(num2 << 3);
	result.push_back(num2 << -1);
	result.push_back(num3 << 3);
	result.push_back(num4 << num5);

	vector<BigInteger> answer;
	answer.push_back(56);
	answer.push_back(14);
	answer.push_back(-296);
	answer.push_back(-99999);
	answer.push_back(0);
	answer.push_back(272);

	for(size_t i=0; i<result.size(); i++){
		if(result[i] == answer[i]) cout << true;
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
void testRightShift(){
	BigInteger num1(5);
	BigInteger num2(-74);
	BigInteger num3(0);
	BigInteger num4(-4);
	BigInteger num5(34);
	BigInteger num6(3);

	vector<BigInteger> result;
	result.push_back(num1 >> 2);
	result.push_back(num2 >> 3);
	result.push_back(num2 >> -3);
	result.push_back(num2 >> 0);
	result.push_back(num3 >> 1);
	result.push_back(num4 >> 4);
	result.push_back(num5 >> num6);

	vector<BigInteger> answer;
	answer.push_back(1);
	answer.push_back(-10);
	answer.push_back(-99999);
	answer.push_back(-74);
	answer.push_back(0);
	answer.push_back(-1);
	answer.push_back(4);


	for(size_t i=0; i<result.size(); i++){
		if(result[i] == answer[i]) cout << true;
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
void testMod(){
	BigInteger num1(14);
	BigInteger num2(3);
	BigInteger num3(27);
	BigInteger num4(-4);
	BigInteger num5(0);
	BigInteger num6(-43);

	vector<BigInteger> result;
	result.push_back(num1 % num2);
	result.push_back(num3 % num4);
	result.push_back(num3 % num5);
	result.push_back(num6 % num2);
	result.push_back(num6 % num4);
	result.push_back(num3 % num2);

	vector<BigInteger> answer;
	answer.push_back(2);
	answer.push_back(3);
	answer.push_back(-99999);
	answer.push_back(-1);
	answer.push_back(-3);
	answer.push_back(0);

	for(size_t i=0; i<result.size(); i++){
		if(result[i] == answer[i]) cout << true;
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
void testAndLogic(){
	BigInteger num1(23);
	BigInteger num2(34);
	BigInteger num3(0);
	BigInteger num4(0);
	BigInteger num5(-5);

	vector<bool> result;
	result.push_back(num1 && num2);
	result.push_back(num2 && num3);
	result.push_back(num3 && num1);
	result.push_back(num3 && num4);
	result.push_back(num2 && num5);

	vector<bool> answer;
	answer.push_back(true);
	answer.push_back(false);
	answer.push_back(false);
	answer.push_back(false);
	answer.push_back(true);

	for(size_t i=0; i<result.size(); i++){
		if(result[i] == answer[i]) cout << true;
		else{
			cout << false << " // ";
			cout << "actual : " << result[i];
			cout << ", expected : " << answer[i];
		}
		cout << endl;
	}
}
void testOrLogic(){
	BigInteger num1(23);
	BigInteger num2(34);
	BigInteger num3(0);
	BigInteger num4(0);
	BigInteger num5(-5);

	vector<bool> result;
	result.push_back(num1 || num2);
	result.push_back(num2 || num3);
	result.push_back(num3 || num1);
	result.push_back(num3 || num4);
	result.push_back(num2 || num5);

	vector<bool> answer;
	answer.push_back(true);
	answer.push_back(true);
	answer.push_back(true);
	answer.push_back(false);
	answer.push_back(true);


	for(size_t i=0; i<result.size(); i++){
		if(result[i] == answer[i]) cout << true;
		else{
			cout << false << " // ";
			cout << "actual : " << result[i];
			cout << ", expected : " << answer[i];
		}
		cout << endl;
	}
}
void testAndBitwise(){
	BigInteger num1(5);
	BigInteger num2(6);
	BigInteger num3(25);
	BigInteger num4(39);
	BigInteger num5(-13);
	BigInteger num6(13);
	BigInteger num7(-39);
	BigInteger num8(0);
	BigInteger num9(134);
	BigInteger num10(-1);
	BigInteger num11(1);

	vector<BigInteger> result;
	result.push_back(num1 & num2);
	result.push_back(num3 & num4);
	result.push_back(num5 & num4);
	result.push_back(num6 & num7);
	result.push_back(num5 & num7);
	result.push_back(num8 & num9);
	result.push_back(num9 & num10);
	result.push_back(num10 & num7);
	result.push_back(num9 & num11);

	vector<BigInteger> answer;
	answer.push_back(4);
	answer.push_back(1);
	answer.push_back(35);
	answer.push_back(9);
	answer.push_back(-47);
	answer.push_back(0);
	answer.push_back(134);
	answer.push_back(-39);
	answer.push_back(0);

	for(size_t i=0; i<result.size(); i++){
		if(result[i] == answer[i]) cout << true;
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
void testOrBitWise(){
	BigInteger num1(5);
	BigInteger num2(6);
	BigInteger num3(25);
	BigInteger num4(39);
	BigInteger num5(-13);
	BigInteger num6(13);
	BigInteger num7(-39);
	BigInteger num8(0);
	BigInteger num9(134);
	BigInteger num10(-1);
	BigInteger num11(1);

	vector<BigInteger> result;
	result.push_back(num1 | num2);
	result.push_back(num3 | num4);
	result.push_back(num5 | num4);
	result.push_back(num6 | num7);
	result.push_back(num5 | num7);
	result.push_back(num8 | num9);
	result.push_back(num9 | num10);
	result.push_back(num10 | num7);
	result.push_back(num9 | num11);

	vector<BigInteger> answer;
	answer.push_back(7);
	answer.push_back(63);
	answer.push_back(-9);
	answer.push_back(-35);
	answer.push_back(-5);
	answer.push_back(134);
	answer.push_back(-1);
	answer.push_back(-1);
	answer.push_back(135);

	for(size_t i=0; i<result.size(); i++){
		if(result[i] == answer[i]) cout << true;
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
void testXorBitwise(){
	BigInteger num1(5);
	BigInteger num2(6);
	BigInteger num3(25);
	BigInteger num4(39);
	BigInteger num5(-13);
	BigInteger num6(13);
	BigInteger num7(-39);
	BigInteger num8(0);
	BigInteger num9(134);
	BigInteger num10(-1);
	BigInteger num11(1);

	vector<BigInteger> result;
	result.push_back(num1 ^ num2);
	result.push_back(num3 ^ num4);
	result.push_back(num5 ^ num4); 
	result.push_back(num6 ^ num7);
	result.push_back(num5 ^ num7);
	result.push_back(num8 ^ num9);
	result.push_back(num9 ^ num10);
	result.push_back(num10 ^ num7);
	result.push_back(num9 ^ num11);

	vector<BigInteger> answer;
	answer.push_back(3);
	answer.push_back(62);
	answer.push_back(-44);
	answer.push_back(-44);
	answer.push_back(42);
	answer.push_back(134);
	answer.push_back(-135);
	answer.push_back(38);
	answer.push_back(135);
	

	for(size_t i=0; i<result.size(); i++){
		if(result[i] == answer[i]) cout << true;
		else{
			cout << false << " // ";
			cout << "actual : " << result[i];
			cout << ", expected : " << answer[i];
		}
		cout << endl;
	}
}