#include <iostream>
using namespace std;

struct Produk
{
    string kode, nama, jenis;
    int stok;
    Produk *kiri;
    Produk *kanan;
    Produk(string k, string n, int s, string j)
    {
        kode = k;
        nama = n;
        stok = s;
        jenis = j;
        kiri = kanan = NULL;
    }
};

struct StackUndo
{
    string aksi; // "tambah" atau "hapus"
    Produk data;
    StackUndo *next;
    StackUndo(string a, Produk d) : aksi(a), data(d), next(NULL) {}
};

// Global root dan undo stack
Produk *root = NULL;
StackUndo *undoTop = NULL;

// Push ke stack undo
void pushUndo(string aksi, Produk *p)
{
    StackUndo *baru = new StackUndo(aksi, *p);
    baru->next = undoTop;
    undoTop = baru;
}

// Tambah produk ke BST
Produk *tambahProduk(Produk *node, string kode, string nama, int stok, string jenis, bool logUndo = true)
{
    if (!node)
    {
        Produk *baru = new Produk(kode, nama, stok, jenis);
        if (logUndo)
            pushUndo("tambah", baru);
        return baru;
    }

    if (kode < node->kode)
        node->kiri = tambahProduk(node->kiri, kode, nama, stok, jenis, logUndo);
    else if (kode > node->kode)
        node->kanan = tambahProduk(node->kanan, kode, nama, stok, jenis, logUndo);
    else
        cout << "Kode produk sudah ada.\n";
    return node;
}

// Tampilkan semua (in-order)
void cetakHeader()
{
    cout << "+------------+----------------------+----------+---------------+\n";
    cout << "| Kode       | Nama Produk          | Stok     | Jenis         |\n";
    cout << "+------------+----------------------+----------+---------------+\n";
}

void cetakBaris(Produk *p)
{
    // Format manual, gunakan lebar tetap dengan padding spasi
    cout << "| " << p->kode;
    for (int i = p->kode.length(); i < 10; i++)
        cout << " ";

    cout << " | " << p->nama;
    for (int i = p->nama.length(); i < 20; i++)
        cout << " ";

    cout << " | " << p->stok;
    int lenStok = to_string(p->stok).length();
    for (int i = lenStok; i < 8; i++)
        cout << " ";

    cout << " | " << p->jenis;
    for (int i = p->jenis.length(); i < 13; i++)
        cout << " ";

    cout << " |\n";
}

void tampilInOrder(Produk *node)
{
    if (!node)
        return;
    tampilInOrder(node->kiri);
    cetakBaris(node);
    tampilInOrder(node->kanan);
}
// Filter berdasarkan jenis
void filterJenis(Produk* node, string jenisCari, bool& ditemukan) {
    if (!node) return;

    filterJenis(node->kiri, jenisCari, ditemukan);

    if (node->jenis == jenisCari) {
        if (!ditemukan) {
            cetakHeader();
            ditemukan = true;
        }
        cetakBaris(node);
    }

    filterJenis(node->kanan, jenisCari, ditemukan);
}

// Cari node terkecil (untuk hapus)
Produk *minValueNode(Produk *node)
{
    Produk *current = node;
    while (current && current->kiri)
        current = current->kiri;
    return current;
}

// Hapus produk
Produk *hapusProduk(Produk *node, string kode, bool logUndo = true)
{
    if (!node)
        return NULL;

    if (kode < node->kode)
        node->kiri = hapusProduk(node->kiri, kode, logUndo);
    else if (kode > node->kode)
        node->kanan = hapusProduk(node->kanan, kode, logUndo);
    else
    {
        if (logUndo)
            pushUndo("hapus", node);

        if (!node->kiri)
        {
            Produk *temp = node->kanan;
            delete node;
            return temp;
        }
        else if (!node->kanan)
        {
            Produk *temp = node->kiri;
            delete node;
            return temp;
        }

        Produk *temp = minValueNode(node->kanan);
        node->kode = temp->kode;
        node->nama = temp->nama;
        node->stok = temp->stok;
        node->jenis = temp->jenis;
        node->kanan = hapusProduk(node->kanan, temp->kode, false);
    }

    return node;
}

// Undo aksi terakhir
void undoAksi()
{
    if (!undoTop)
    {
        cout << "Tidak ada aksi untuk di-undo.\n";
        return;
    }

    StackUndo *top = undoTop;
    Produk data = top->data;

    if (top->aksi == "tambah")
    {
        root = hapusProduk(root, data.kode, false);
        cout << "Undo tambah: produk " << data.kode << " dihapus.\n";
    }
    else if (top->aksi == "hapus")
    {
        root = tambahProduk(root, data.kode, data.nama, data.stok, data.jenis, false);
        cout << "Undo hapus: produk " << data.kode << " ditambahkan kembali.\n";
    }

    undoTop = top->next;
    delete top;
}

// Menu interaktif
void menu()
{
    int pilihan;
    do
    {
        cout << "\n==== MENU MANAJEMEN GUDANG ====\n";
        cout << "1. Tambah Produk\n";
        cout << "2. Tampilkan Semua Produk\n";
        cout << "3. Filter Jenis Produk\n";
        cout << "4. Hapus Produk\n";
        cout << "5. Undo Aksi Terakhir\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1)
        {
            string kode, nama, jenis;
            int stok;
            cout << "Kode Produk: ";
            getline(cin, kode);
            cout << "Nama Produk: ";
            getline(cin, nama);
            cout << "Stok: ";
            cin >> stok;
            cin.ignore();
            cout << "Jenis Produk: ";
            getline(cin, jenis);
            root = tambahProduk(root, kode, nama, stok, jenis);
        }

        else if (pilihan == 2)
        {
            cout << "\n--- Daftar Produk (A-Z) ---\n";
            cetakHeader();
            tampilInOrder(root);
            cout << "+------------+----------------------+----------+---------------+\n";
        }

        else if (pilihan == 3) {
            string jenis;
            cout << "Masukkan jenis yang ingin difilter: ";
            getline(cin, jenis);
            bool ditemukan = false;
            filterJenis(root, jenis, ditemukan);
            if (ditemukan)
                cout << "+------------+----------------------+----------+---------------+\n";
            else
                cout << "Tidak ditemukan produk dengan jenis \"" << jenis << "\".\n";
        }
        
        else if (pilihan == 4)
        {
            string kode;
            cout << "Kode produk yang ingin dihapus: ";
            getline(cin, kode);
            root = hapusProduk(root, kode);
        }
        else if (pilihan == 5)
        {
            undoAksi();
        }
    } while (pilihan != 0);
}

int main()
{
    menu();
    return 0;
}
