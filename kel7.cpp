#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
using namespace std;

const int MAX_MAHASISWA = 100;
const int MAX_PENARIKAN = 100;

struct Mahasiswa {
    string nama;
    double saldo;
};

struct Penarikan {
    string nama;
    double jumlah;
    string keterangan;
    string waktu;
};

Mahasiswa mahasiswaList[MAX_MAHASISWA];
int mahasiswaCount = 0;
Penarikan penarikanList[MAX_PENARIKAN];
int penarikanCount = 0;

string getCurrentTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", ltm);
    return string(buffer);
}

void loadData() {
    ifstream file("data.txt");
    if (file.is_open()) {
        mahasiswaCount = 0;
        while (file >> mahasiswaList[mahasiswaCount].nama >> mahasiswaList[mahasiswaCount].saldo) {
            mahasiswaCount++;
            if (mahasiswaCount >= MAX_MAHASISWA) break;
        }
        file.close();
    }
}

void saveData() {
    ofstream file("data.txt");
    if (file.is_open()) {
        for (int i = 0; i < mahasiswaCount; i++) {
            file << mahasiswaList[i].nama << " " << mahasiswaList[i].saldo << endl;
        }
        file.close();
    }
}

void saveRekap() {
    ofstream file("rekap_penarikan.txt", ios::app);
    if (file.is_open()) {
        for (int i = 0; i < penarikanCount; i++) {
            file << "Nama: " << penarikanList[i].nama << ", Jumlah: " << penarikanList[i].jumlah
                 << ", Keterangan: " << penarikanList[i].keterangan << ", Waktu: " << penarikanList[i].waktu << endl;
        }
        file.close();
    }
}

void addMahasiswa(const string &nama, double saldoAwal) {
    if (mahasiswaCount < MAX_MAHASISWA) {
        mahasiswaList[mahasiswaCount].nama = nama;
        mahasiswaList[mahasiswaCount].saldo = saldoAwal;
        mahasiswaCount++;
        saveData();
    } else {
        cout << "Data mahasiswa penuh!" << endl;
    }
}

bool addSaldo(const string &nama, double amount) {
    for (int i = 0; i < mahasiswaCount; i++) {
        if (mahasiswaList[i].nama == nama) {
            mahasiswaList[i].saldo += amount;
            saveData();
            return true;
        }
    }
    return false;
}

bool withdrawSaldo(const string &nama, double amount, const string &keterangan) {
    for (int i = 0; i < mahasiswaCount; i++) {
        if (mahasiswaList[i].nama == nama && mahasiswaList[i].saldo >= amount) {
            mahasiswaList[i].saldo -= amount;
            saveData();

            if (penarikanCount < MAX_PENARIKAN) {
                penarikanList[penarikanCount] = {nama, amount, keterangan, getCurrentTime()};
                penarikanCount++;
                saveRekap();
            } else {
                cout << "Data rekap penarikan penuh!" << endl;
            }

            return true;
        }
    }
    return false;
}

bool withdrawFromTotal(double amount, const string &keterangan) {
    double totalSaldo = 0;
    for (int i = 0; i < mahasiswaCount; i++) {
        totalSaldo += mahasiswaList[i].saldo;
    }

    if (totalSaldo >= amount) {
        double originalAmount = amount; // Save the original amount for logging
        for (int i = 0; i < mahasiswaCount; i++) {
            if (amount <= 0) break;

            if (mahasiswaList[i].saldo >= amount) {
                mahasiswaList[i].saldo -= amount;
                amount = 0;
            } else {
                amount -= mahasiswaList[i].saldo;
                mahasiswaList[i].saldo = 0;
            }
        }
        saveData();

        if (penarikanCount < MAX_PENARIKAN) {
            penarikanList[penarikanCount] = {"TOTAL", originalAmount, keterangan, getCurrentTime()};
            penarikanCount++;
            saveRekap();
        } else {
            cout << "Data rekap penarikan penuh!" << endl;
        }

        return true;
    }

    return false;
}

void displayAllMahasiswa() {
    cout << "\n============================================" << endl;
    cout << "          Data Mahasiswa dan Saldo          " << endl;
    cout << "============================================" << endl;
    cout << left << setw(5) << "No" << setw(20) << "Nama" << setw(15) << "Saldo" << endl;
    cout << "--------------------------------------------" << endl;
    for (int i = 0; i < mahasiswaCount; i++) {
        cout << left << setw(5) << i + 1 << setw(20) << mahasiswaList[i].nama << setw(15) << mahasiswaList[i].saldo << endl;
    }
    cout << "============================================" << endl;
}

double getTotalSaldo() {
    double total = 0;
    for (int i = 0; i < mahasiswaCount; i++) {
        total += mahasiswaList[i].saldo;
    }
    return total;
}

void displayRekapPenarikan() {
    cout << "\n============================================" << endl;
    cout << "         Rekapitulasi Penarikan Saldo       " << endl;
    cout << "============================================" << endl;
    cout << left << setw(5) << "No" << setw(20) << "Nama" << setw(10) << "Jumlah" << setw(20) << "Keterangan" << setw(20) << "Waktu" << endl;
    cout << "--------------------------------------------" << endl;
    for (int i = 0; i < penarikanCount; i++) {
        cout << left << setw(5) << i + 1
             << setw(20) << penarikanList[i].nama
             << setw(10) << penarikanList[i].jumlah
             << setw(20) << penarikanList[i].keterangan
             << setw(20) << penarikanList[i].waktu << endl;
    }
    cout << "============================================" << endl;
}

void displayMenu() {
    cout << "\n============================================" << endl;
    cout << "       Aplikasi Uang Kas Mahasiswa          " << endl;
    cout << "============================================" << endl;
    cout << "1. Tampilkan Data Mahasiswa                " << endl;
    cout << "2. Tambah Mahasiswa                        " << endl;
    cout << "3. Tambah Saldo Mahasiswa                  " << endl;
    cout << "4. Tarik Saldo Mahasiswa                   " << endl;
    cout << "5. Tampilkan Total Saldo                   " << endl;
    cout << "6. Tampilkan Rekapitulasi Penarikan        " << endl;
    cout << "7. Tarik Saldo dari Total Dana             " << endl;
    cout << "0. Keluar                                  " << endl;
    cout << "============================================" << endl;
    cout << "Pilihan: ";
}

void searchMahasiswa(const string &nama) {
    bool found = false;
    cout << "\n============================================" << endl;
    cout << "          Hasil Pencarian Mahasiswa         " << endl;
    cout << "============================================" << endl;
    for (int i = 0; i < mahasiswaCount; i++) {
        if (mahasiswaList[i].nama == nama) {
            cout << "Nama: " << mahasiswaList[i].nama << ", Saldo: " << mahasiswaList[i].saldo << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Mahasiswa dengan nama \"" << nama << "\" tidak ditemukan." << endl;
    }
    cout << "============================================" << endl;
}

int main() {
    loadData();
    int choice;

    do {
        displayMenu();
        cin >> choice;

        if (choice == 1) {
            displayAllMahasiswa();
        } else if (choice == 2) {
            string nama;
            double saldoAwal;
            cout << "Masukkan nama mahasiswa: ";
            cin >> nama;
            cout << "Masukkan saldo awal: ";
            cin >> saldoAwal;
            addMahasiswa(nama, saldoAwal);
        } else if (choice == 3) {
            string nama;
            double amount;
            cout << "Masukkan nama mahasiswa: ";
            cin >> nama;
            cout << "Masukkan jumlah saldo yang ditambahkan: ";
            cin >> amount;
            if (!addSaldo(nama, amount)) {
                cout << "Mahasiswa tidak ditemukan." << endl;
            }
        } else if (choice == 4) {
            string nama, keterangan;
            double amount;
            cout << "Masukkan nama mahasiswa: ";
            cin >> nama;
            cout << "Masukkan jumlah saldo yang ditarik: ";
            cin >> amount;
            cout << "Masukkan keterangan penarikan: ";
            cin.ignore();
            getline(cin, keterangan);
            if (!withdrawSaldo(nama, amount, keterangan)) {
                cout << "Saldo tidak mencukupi atau mahasiswa tidak ditemukan." << endl;
            }
        } else if (choice == 5) {
            cout << "Total saldo semua mahasiswa: " << getTotalSaldo() << endl;
        } else if (choice == 6) {
            displayRekapPenarikan();
        } else if
            (choice == 7) {
            string keterangan;
            double amount;
            cout << "Masukkan jumlah saldo yang ingin ditarik dari total dana: ";
            cin >> amount;
            cout << "Masukkan keterangan penarikan: ";
            cin.ignore();
            getline(cin, keterangan);
            if (!withdrawFromTotal(amount, keterangan)) {
                cout << "Saldo total tidak mencukupi untuk penarikan ini." << endl;
            } else {
                cout << "Penarikan sebesar " << amount << " berhasil dilakukan dari total dana." << endl;
            }
        } else if (choice == 0) {
            cout << "Terima kasih telah menggunakan aplikasi ini.\n";
        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (choice != 0);

    return 0;
}
