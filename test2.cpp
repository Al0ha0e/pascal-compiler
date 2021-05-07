#include <memory>
#include <vector>
#include <iostream>
using namespace std;

int main()
{
    int *ap = new int(10);
    int *bp = new int(101);
    std::unique_ptr<int> a(ap);
    std::unique_ptr<int> b(std::move(a));
    a = std::unique_ptr<int>(bp);
    std::cout << *a << " " << *ap << " " << *b;
    return 0;
}