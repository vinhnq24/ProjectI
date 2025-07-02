#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <limits>
#include <iomanip>
#include <climits>
using namespace std;
typedef unsigned long long ull;
class RSA {
private:
    ull p, q, n, phi, e, d;

    ull luyThuaMod(ull coSo, ull mu, ull mod) const 
    {
        ull kq = 1;
        coSo %= mod;
        while (mu) 
        {
            if (mu % 2) 
                kq = kq * coSo % mod;
                coSo = coSo * coSo % mod;
                mu /= 2;
        }
        return kq;
    }
        bool laSoNguyenTo(ull num) 
        {
            if (num < 2 || (num % 2 == 0 && num != 2)) return false;
            for (ull i = 3; i * i <= num; i += 2)
                if (num % i == 0) return false;
            return true;
        }

    ull ucln(ull a, ull b) const 
    {
        while (b) 
        tie(a, b) = make_pair(b, a % b);
        return a;
    }

    ull nghichDaoMod(ull a, ull m) const 
    {
        ull m0 = m, x0 = 0, x1 = 1;
        while (a > 1) 
        {
            ull q = a / m;
            tie(a, m) = make_pair(m, a % m);
            tie(x0, x1) = make_pair(x1 - q * x0, x0);
        }
        return (x1 + m0) % m0;
    }

    ull taoSoNguyenTo(ull min, ull max) 
    {
        mt19937 ngauNhien(random_device{}());
        uniform_int_distribution<ull> dis(min, max);
        ull p;
        do { p = dis(ngauNhien) | 1; } 
        while (!laSoNguyenTo(p));
        return p;
    
    }

public:
    void taoKhoa(int doDai = 5) 
    {
        ull min = pow(10, doDai - 1), max = pow(10, doDai);
        do { p = taoSoNguyenTo(min, max); q = taoSoNguyenTo(min, max); } while (p == q);
        n = p * q; phi = (p - 1) * (q - 1); e = 65537;
        while (ucln(e, phi) != 1) e += 2;
        d = nghichDaoMod(e, phi);
    }

    void setN(ull n_) { n = n_; }
    void setE(ull e_) { e = e_; }
    void setD(ull d_) { d = d_; }
    ull getN() const { return n; }
    ull getE() const { return e; }
    ull getD() const { return d; }
    ull luyThuaModPublic(ull coSo, ull mu, ull mod) const { return luyThuaMod(coSo, mu, mod); }

    ull maHoa(ull thongDiep) const {
        return (thongDiep < n) ? luyThuaMod(thongDiep, e, n) : ULLONG_MAX;
    }

    ull giaiMa(ull banMa) const 
    {
        return luyThuaMod(banMa, d, n);
    }

    vector<ull> maHoaChuoi(const string &s, ull eNguoiNhan, ull nNguoiNhan) const 
    {
        vector<ull> kq;
        const int BLOCK_SIZE = 3;
        for (size_t i = 0; i < s.size(); i += BLOCK_SIZE) 
        {
            ull blockVal = 0;
            int len = min((int)s.size() - (int)i, BLOCK_SIZE);
            for (int j = 0; j < len; ++j) 
            {
                blockVal = blockVal * 256 + (unsigned char)s[i + j];
            }
            kq.push_back((blockVal < nNguoiNhan) ? luyThuaMod(blockVal, eNguoiNhan, nNguoiNhan) : ULLONG_MAX);
        }
        return kq;
    }

    string giaiMaChuoi(const vector<ull> &ds) const 
    {
        string s;
        for (auto ma : ds) 
        {
            if (ma == ULLONG_MAX) continue;
            ull val = giaiMa(ma);
            string part;
            while (val > 0) 
            {
                part = (char)(val % 256) + part;
                val /= 256;
            }
            s += part;
        }
        return s;
    }
};

void menu() 
{
    RSA rsa;
    vector<ull> banMa;
    bool daTaoKhoa = false;
    int chon;
    do {
        cout << "\n============ MENU ============\n";
        cout << "1. Tao khoa\n";
        cout << "2. Ma hoa so\n";
        cout << "3. Giai ma so\n";
        cout << "4. Ma hoa chuoi\n";
        cout << "5. Giai ma chuoi\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";
        cin >> chon;
        cin.ignore();

        if (chon == 1) 
        {
            int luaChon;
            cout << "1. Tu dong sinh khoa\n";
            cout << "2. Nhap khoa thu cong\n";
            cout << "Chon: ";
            cin >> luaChon;
            cin.ignore();
            if (luaChon == 1) 
            {
                rsa.taoKhoa();
                daTaoKhoa = true;
                cout << "Khoa cong khai: (" << rsa.getE() << ", " << rsa.getN() << ")\n";
                cout << "Khoa rieng tu : (" << rsa.getD() << ", " << rsa.getN() << ")\n";
            } else if (luaChon == 2) 
            {
                ull n, e, d;
                cout << "Nhap n: ";
                cin >> n;
                cout << "Nhap e: ";
                cin >> e;
                cout << "Nhap d: ";
                cin >> d;
                cin.ignore();
                rsa.setN(n);
                rsa.setE(e);
                rsa.setD(d);
                daTaoKhoa = true;
                cout << "Da nhap khoa thu cong!\n";
            } else {
                cout << "Lua chon khong hop le!\n";
            }
        } 
        else if (chon >= 2 && chon <= 5 && !daTaoKhoa) 
        {
            cout << "Ban chua tao khoa! Vui long chon 1 de tao khoa truoc.\n";
        } 
        else if (chon == 2) 
        {
            ull m, e, n;
            cout << "Nhap so can ma hoa: ";
            cin >> m;
            cout << "Nhap khoa cong khai cua nguoi nhan (e, n): ";
            cin >> e >> n;
            cin.ignore();
            ull c = (m < n) ? rsa.luyThuaModPublic(m, e, n) : ULLONG_MAX;
            if (c != ULLONG_MAX)
                cout << "So ma hoa: " << c << "\n";
            else
                cout << "So qua lon, khong ma hoa duoc!\n";
        } 
        else if (chon == 3) 
        {
            ull c;
            cout << "Nhap so can giai ma: ";
            cin >> c;
            cin.ignore();
            cout << "So giai ma: " << rsa.giaiMa(c) << "\n";
        } 
        else if (chon == 4) 
        {
            string msg;
            ull e, n;
            cout << "Nhap chuoi can ma hoa: ";
            getline(cin, msg);
            cout << "Nhap khoa cong khai cua nguoi nhan (e, n): ";
            cin >> e >> n;
            cin.ignore();
            banMa = rsa.maHoaChuoi(msg, e, n);
            cout << "Chuoi ma hoa: ";
            for (auto ma : banMa) {
                if (ma == ULLONG_MAX)
                    cout << "ERROR ";
                else
                    cout << ma << " ";
            }
            cout << "\n";
        } else if (chon == 5) 
        {
            cout << "Nhap chuoi cac so da ma hoa (vd: 123 456 789): ";
            string dong;
            getline(cin, dong);
            stringstream ss(dong);
            ull so;
            banMa.clear();
            while (ss >> so) banMa.push_back(so);
            if (banMa.empty()) 
            {
                cout << "Khong co du lieu de giai ma.\n";
            } else {
                cout << "Chuoi giai ma: " << rsa.giaiMaChuoi(banMa) << "\n";
            }
        }
    } 
    while (chon != 0);
    cout << "Tam biet!\n";
}

int main() 
{
    cout << "\n=== CHUONG TRINH MA HOA RSA  ===\n";
    menu();
    return 0;
};