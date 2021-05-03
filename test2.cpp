#include <memory>
#include <vector>
#include <iostream>
using namespace std;

vector<unique_ptr<int>> a;

void GET(unique_ptr<int> &sb)
{
    unique_ptr<int> sb2(std::move(sb));
    cout << *sb2 << endl;
}

int main()
{
    int b = 10132;
    a.push_back(unique_ptr<int>(&b));
    unique_ptr<int> c(move(a[0]));
    cout << *c << endl;
    GET(c);
    return 0;
}