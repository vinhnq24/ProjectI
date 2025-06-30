#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <limits>
#include <iomanip>
using namespace std;

class RSA {
private:
    long long p, q, n, phi, e, d;

    long long luyThuaMod(long long coSo, long long mu, long long mod) const {
        long long kq = 1;
        coSo %= mod;
        while (mu) {
            if (mu % 2) kq = kq * coSo % mod;
            coSo = coSo * coSo % mod;
            mu /= 2;
        }
        return kq;
    }

        bool laSoNguyenTo(long long num) {
        if (num < 2 || (num % 2 == 0 && num != 2)) return false;
        for (long long i = 3; i * i <= num; i += 2)
            if (num % i == 0) return false;
        return true;
    }

    long long ucln(long long a, long long b) const {
        while (b) tie(a, b) = make_pair(b, a % b);
        return a;
    }

    long long nghichDaoMod(long long a, long long m) const {
        long long m0 = m, x0 = 0, x1 = 1;
        while (a > 1) {
            long long q = a / m;
            tie(a, m) = make_pair(m, a % m);
            tie(x0, x1) = make_pair(x1 - q * x0, x0);
        }
        return (x1 + m0) % m0;
    }

    long long taoSoNguyenTo(long long min, long long max) {
        mt19937 ngauNhien(random_device{}());
        uniform_int_distribution<long long> dis(min, max);
        long long p;
        do { p = dis(ngauNhien) | 1; } while (!laSoNguyenTo(p));
        return p;
    
    }

public:
    void taoKhoa(int doDai = 5) {
        long long min = pow(10, doDai - 1), max = pow(10, doDai);
        do { p = taoSoNguyenTo(min, max); q = taoSoNguyenTo(min, max); } while (p == q);
        n = p * q; phi = (p - 1) * (q - 1); e = 65537;
        while (ucln(e, phi) != 1) e += 2;
        d = nghichDaoMod(e, phi);
    }

    long long getN() const { return n; }
    long long getE() const { return e; }
    long long getD() const { return d; }

    long long maHoa(long long thongDiep) const {
        return (thongDiep < n) ? luyThuaMod(thongDiep, e, n) : -1;
    }

    long long giaiMa(long long banMa) const {
        return luyThuaMod(banMa, d, n);
    }

    vector<long long> maHoaChuoi(const string &s) const {
        vector<long long> kq;
        const int BLOCK_SIZE = 3;
        for (size_t i = 0; i < s.size(); i += BLOCK_SIZE) {
            long long blockVal = 0;
            int len = min((int)s.size() - (int)i, BLOCK_SIZE);
            for (int j = 0; j < len; ++j) {
                blockVal = blockVal * 256 + (unsigned char)s[i + j];
            }
            kq.push_back(maHoa(blockVal));
        }
        return kq;
    }

    string giaiMaChuoi(const vector<long long> &ds) const {
        string s;
        for (auto ma : ds) {
            if (ma == -1) continue;
            long long val = giaiMa(ma);
            string part;
            while (val > 0) {
                part = (char)(val % 256) + part;
                val /= 256;
            }
            s += part;
        }
        return s;
    }

    string giaiMaChuoiVoiKhoaRieng(const vector<long long>& ds, long long dNhap, long long nNhap) const {
        string s;
        for (auto ma : ds) {
            if (ma == -1) continue;
            long long val = luyThuaMod(ma, dNhap, nNhap);
            string part;
            while (val > 0) {
                part = (char)(val % 256) + part;
                val /= 256;
            }
            s += part;
        }
        return s;
    }
};

void menu() {
    RSA rsa;
    vector<long long> banMa;
    bool daTaoKhoa = false;
    int chon;
    do {
        cout << "\n============ MENU ============\n";
        cout << "1. Tao khoa\n";
        cout << "2. Ma hoa so\n";
        cout << "3. Giai ma so\n";
        cout << "4. Ma hoa chuoi\n";
        cout << "5. Giai ma chuoi\n";
        cout << "6. Giai ma chuoi bang khoa rieng\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";
        cin >> chon;
        cin.ignore();

        if (chon == 1) {
            rsa.taoKhoa();
            daTaoKhoa = true;
            cout << "Khoa cong khai: (" << rsa.getN() << ", " << rsa.getE() << ")\n";
            cout << "Khoa rieng tu : (" << rsa.getN() << ", " << rsa.getD() << ")\n";
        } else if (chon >= 2 && chon <= 5 && !daTaoKhoa) {
            cout << "Ban chua tao khoa! Vui long chon 1 de tao khoa truoc.\n";
        } else if (chon == 2) {
            long long m;
            cout << "Nhap so can ma hoa: ";
            cin >> m;
            long long c = rsa.maHoa(m);
            if (c != -1)
                cout << "So ma hoa: " << c << "\n";
            else
                cout << "So qua lon, khong ma hoa duoc!\n";
        } else if (chon == 3) {
            long long c;
            cout << "Nhap so can giai ma: ";
            cin >> c;
            cout << "So giai ma: " << rsa.giaiMa(c) << "\n";
        } else if (chon == 4) {
            string msg;
            cout << "Nhap chuoi can ma hoa: ";
            getline(cin, msg);
            banMa = rsa.maHoaChuoi(msg);
            cout << "Chuoi ma hoa: ";
            for (auto ma : banMa)
                cout << ma << " ";
            cout << "\n";
        } else if (chon == 5) {
            cout << "Nhap chuoi cac so da ma hoa (vd: 123 456 789): ";
            string dong;
            getline(cin, dong);
            stringstream ss(dong);
            long long so;
            banMa.clear();
            while (ss >> so) banMa.push_back(so);

            if (banMa.empty()) {
                cout << "Khong co du lieu de giai ma.\n";
            } else {
                cout << "Chuoi giai ma: " << rsa.giaiMaChuoi(banMa) << "\n";
            }
        } else if (chon == 6) {
            cout << "Nhap chuoi cac so da ma hoa (vd: 123 456 789): ";
            string dong;
            getline(cin, dong);
            stringstream ss(dong);
            vector<long long> ds;
            long long so;
            while (ss >> so) ds.push_back(so);

            if (ds.empty()) {
                cout << "Khong co du lieu de giai ma.\n";
            } else {
                long long dNhap, nNhap;
                cout << "Nhap khoa rieng d: ";
                cin >> dNhap;
                cout << "Nhap n: ";
                cin >> nNhap;
                cin.ignore();
                cout << "Chuoi giai ma: " << rsa.giaiMaChuoiVoiKhoaRieng(ds, dNhap, nNhap) << "\n";
            }
        }
    } while (chon != 0);
    cout << "Tam biet!\n";
}

int main() {
    cout << "\n=== CHUONG TRINH MA HOA RSA - Unicode Block Support ===\n";
    menu();
    return 0;
}
