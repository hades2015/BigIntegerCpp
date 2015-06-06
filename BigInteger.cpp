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

        BigInteger operator+(const BigInteger& postNum) {
            BigInteger result;

            int carry = 0;
            for(int i=0; i < max(num.size(), postNum.num.size()); ++i) {
                int calc = get(i) + postNum.get(i) + carry;
                result.num.push_back(calc % 10);
                carry = calc / 10;
            }
            if (carry)
                result.num.push_back(carry);

            return result;
        }

        void print() {
            for (int i = num.size() - 1; i >= 0; --i)
                cout << num[i];
            cout << endl;
        }

        bool operator==(const BigInteger& other) {
            return isPositive == other.isPositive && num == other.num;
        }

    private :
        vector<int> num;
        bool isPositive;
};


int main() {
    BigInteger num1(100);
    BigInteger zero(0);

    BigInteger result1 = num1 + 12;
    BigInteger answer1(112);

    cout << (result1 == answer1) << endl;
}
