/*
    www.youtube.com/YugiHackerChannel
    oj.vnoi.info/user/YugiHackerKhongCopCode
*/
#include <ctime>
#include <bits/stdc++.h>

#define el cout<<'\n'
#define f0(i,n) for(int i=0;i<n;i++)
#define f1(i,n) for(int i=1;i<=n;i++)
#define NAME "bai1"
using namespace std;

mt19937 rd(chrono::steady_clock::now().time_since_epoch().count());
long long Rand(long long l, long long h) { // tra ve so random trong khoang tu l den h
    if(l == 0 and h == 511) return rand() % 512;
    long long ans = 0;
    for(int i = 0; i < 7; i ++) ans += (Rand(0, 511) << (i * 7));
    return ans % (h - l + 1) + l;
}

void MakeTest()
{
    ofstream cout (NAME".inp");
    int n = Rand(500,1000);
    cout<<n<<endl;
    for(int i=1;i<=n;i++)
        cout<<Rand(-1e9,1e9)<<" ";



}
const int NTEST = 1000;
int main()
{
    for(int iTest = 1; iTest <= NTEST; iTest++)
    {
        MakeTest();
        system(NAME"_trau.exe");
        system(NAME".exe");
        /* Khi khong dung freopen
        system(NAME".exe <"NAME".inp >"NAME".out");
        system(NAME"_trau.exe <"NAME".inp >"NAME".ans");
        */
        if(system("fc "NAME".out "NAME".ans") != 0)
        {
            cout << "Test " << iTest << ": WRONG!\n";
            return 0;
        }
        cout << "Test " << iTest << ": CORRECT!\n";
    }
}
