#include <iostream>
#include <stack>
#include <string>
using namespace std;

struct Pelaku {
    string nama;
    string jabatan;
    int kerugian;
    Pelaku* left;
    Pelaku* right;
};

Pelaku* root = NULL;

stack<string> riwayatAksi;
stack<Pelaku*> riwayatData;

Pelaku* buatNode(string nama, string jabatan, int kerugian) {
    Pelaku* baru = new Pelaku;
    baru->nama = nama;
    baru->jabatan = jabatan;
    baru->kerugian = kerugian;
    baru->left = baru->right = NULL;
    return baru;
}

Pelaku* sisip(Pelaku* root, Pelaku* baru) {
    if (root == NULL) return baru;
    if (baru->nama < root->nama)
        root->left = sisip(root->left, baru);
    else if (baru->nama > root->nama)
        root->right = sisip(root->right, baru);
    return root;
}

void tampilInorder(Pelaku* root) {
    if (root == NULL) return;
    tampilInorder(root->left);
    cout << "Nama     : " << root->nama << endl;
    cout << "Jabatan  : " << root->jabatan << endl;
    cout << "Kerugian : Rp " << root->kerugian << endl;
    cout << "-----------------------------\n";
    tampilInorder(root->right);
}

Pelaku* cariMin(Pelaku* node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}

Pelaku* hapusNode(Pelaku* root, string nama, Pelaku*& simpan) {
    if (root == NULL) return NULL;

    if (nama < root->nama)
        root->left = hapusNode(root->left, nama, simpan);
    else if (nama > root->nama)
        root->right = hapusNode(root->right, nama, simpan);
    else {
        simpan = new Pelaku(*root); // simpan data untuk undo

        if (root->left == NULL) {
            Pelaku* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == NULL) {
            Pelaku* temp = root->left;
            delete root;
            return temp;
        }

        Pelaku* pengganti = cariMin(root->right);
        root->nama = pengganti->nama;
        root->jabatan = pengganti->jabatan;
        root->kerugian = pengganti->kerugian;
        root->right = hapusNode(root->right, pengganti->nama, simpan);
    }
    return root;
}

void undoAksi() {
    if (riwayatAksi.empty()) {
        cout << "Tidak ada aksi yang bisa di-undo.\n";
        return;
    }

    string aksi = riwayatAksi.top(); riwayatAksi.pop();
    Pelaku* data = riwayatData.top(); riwayatData.pop();

    if (aksi == "tambah") {
        Pelaku* dummy = NULL;
        root = hapusNode(root, data->nama, dummy);
        cout << "Undo: Data pelaku '" << data->nama << "' telah dihapus.\n";
        delete data;
    } else if (aksi == "hapus") {
        root = sisip(root, data);
        cout << "Undo: Data pelaku '" << data->nama << "' dikembalikan.\n";
    }
}

int main() {
    int pilihan;
    do {
        cout << "\n=== APLIKASI DATA PELAKU KORUPSI ===\n";
        cout << "1. Tambah Data Pelaku\n";
        cout << "2. Tampilkan Semua Data (A-Z)\n";
        cout << "3. Hapus Data Pelaku\n";
        cout << "4. Undo Aksi Terakhir\n";
        cout << "5. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1) {
            string nama, jabatan;
            int kerugian;
            cout << "Nama     : "; getline(cin, nama);
            cout << "Jabatan  : "; getline(cin, jabatan);
            cout << "Kerugian : Rp ";
            while (!(cin >> kerugian)) {
                cout << "Masukkan angka! Coba lagi: Rp ";
                cin.clear();
                cin.ignore(1000, '\n');
            }
            cin.ignore();

            Pelaku* baru = buatNode(nama, jabatan, kerugian);
            root = sisip(root, baru);
            riwayatAksi.push("tambah");
            riwayatData.push(baru);
            cout << "Data berhasil ditambahkan.\n";

        } else if (pilihan == 2) {
            cout << "\n--- Data Pelaku Korupsi (Urut A-Z) ---\n";
            tampilInorder(root);

        } else if (pilihan == 3) {
            string nama;
            cout << "Masukkan nama yang ingin dihapus: ";
            getline(cin, nama);
            Pelaku* simpan = NULL;
            root = hapusNode(root, nama, simpan);
            if (simpan != NULL) {
                riwayatAksi.push("hapus");
                riwayatData.push(simpan);
                cout << "Data berhasil dihapus.\n";
            } else {
                cout << "Data tidak ditemukan.\n";
            }

        } else if (pilihan == 4) {
            undoAksi();

        } else if (pilihan == 5) {
            cout << "Program selesai. Terima kasih!\n";

        } else {
            cout << "Pilihan tidak valid!\n";
        }

    } while (pilihan != 5);

    return 0;
}
