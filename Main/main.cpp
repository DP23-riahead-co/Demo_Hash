#include <iostream>
#include <string>
using namespace std;
int hashFunc(string s, int sizeTab) {
    long sum = 0, mul = 1;
    for (int i = 0; i < s.length(); i++) {
        mul = (i % 4 == 0) ? 1 : mul * 256;
        sum += s.at(i) * mul;
    }
    return (int)(abs(sum) % sizeTab);
}
int main(){
    string name;
    cout<<"Nhap ten ban o day : ";
    getline(cin,name);
    cout<<hashFunc(name,101)<<endl;
    return 0;
}