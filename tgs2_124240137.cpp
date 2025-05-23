#include <iostream>
using namespace std;

struct Buku {
    string judul;
    string penulis;
    Buku* prev;
    Buku* next;
};

Buku* head = nullptr;
Buku* tail = nullptr;

void tambahDepan(string judul, string penulis) {
    Buku* baru = new Buku{judul, penulis, nullptr, head};
    if (head != nullptr)
        head->prev = baru;
    else
        tail = baru;
    head = baru;
    cout << "Buku \"" << judul << "\" ditulis oleh " << penulis << " ditambahkan di depan.\n";
}

void tambahBelakang(string judul, string penulis) {
    Buku* baru = new Buku{judul, penulis, tail, nullptr};
    if (tail != nullptr)
        tail->next = baru;
    else
        head = baru;
    tail = baru;
    cout << "Buku \"" << judul << "\" ditulis oleh " << penulis << " ditambahkan di belakang.\n";
}

void tampilkan() {
    if (head == nullptr) {
        cout << "Daftar buku kosong.\n";
        return;
    }
    cout << "Daftar Buku:\n";
    Buku* sekarang = head;
    int no = 1;
    while (sekarang != nullptr) {
        cout << no++ << ". \"" << sekarang->judul << "\" oleh " << sekarang->penulis << "\n";
        sekarang = sekarang->next;
    }
}

void hapusBuku(string judul) {
    Buku* sekarang = head;
    while (sekarang != nullptr) {
        if (sekarang->judul == judul) {
            if (sekarang->prev)
                sekarang->prev->next = sekarang->next;
            else
                head = sekarang->next;
            if (sekarang->next)
                sekarang->next->prev = sekarang->prev;
            else
                tail = sekarang->prev;
            delete sekarang;
            cout << "Buku \"" << judul << "\" berhasil dihapus.\n";
            return;
        }
        sekarang = sekarang->next;
    }
    cout << "Buku dengan judul \"" << judul << "\" tidak ditemukan.\n";
}

int main() {
    int pilihan;
    string judul, penulis;

    do {
        cout << "\n======= MENU MANAJEMEN BUKU =======\n";
        cout << "1. Tambah buku di depan\n";
        cout << "2. Tambah buku di belakang\n";
        cout << "3. Hapus buku berdasarkan judul\n";
        cout << "4. Tampilkan daftar buku\n";
        cout << "5. Keluar\n";
        cout << "Pilihan Anda: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1:
                cout << "Judul buku: ";
                getline(cin, judul);
                cout << "Penulis: ";
                getline(cin, penulis);
                tambahDepan(judul, penulis);
                break;
            case 2:
                cout << "Judul buku: ";
                getline(cin, judul);
                cout << "Penulis: ";
                getline(cin, penulis);
                tambahBelakang(judul, penulis);
                break;
            case 3:
                cout << "Masukkan judul buku yang ingin dihapus: ";
                getline(cin, judul);
                hapusBuku(judul);
                break;
            case 4:
                tampilkan();
                break;
            case 5:
                cout << " Keluar dari program.\n";
                break;
            default:
                cout << " Pilihan tidak valid. Coba lagi.\n";
        }
    } while (pilihan != 5);

    return 0;
}