#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

// Definisi tipe data untuk kemudahan
#define pqi(x) priority_queue<x, vector<x>, greater<x>>
#define pii pair<int, int>

// Struktur Data Tree untuk Organisasi
struct Tree
{
    string root;
    unordered_map<string, vector<string>> nodes;
};

// Struktur Data Mahasiswa
struct Mahasiswa
{
    string nama, nim, password, prodi;
    int semester;
};

// Struktur Data Konsultasi
struct Konsultasi
{
    int id;
    string nim, nama, dosen, waktu;
};

// Struktur Data Gedung (untuk referensi, meski navigasi pakai map langsung)
struct Gedung
{
    int id;
    string nama, lokasi, koordinat;
};

// Global Variables
unordered_map<string, Mahasiswa> daftarMhs; // Hash Table untuk Login
vector<Gedung> daftar_gedung;
unordered_map<string, vector<pair<string, int>>> adj; // Graph untuk Navigasi

stack<string> KRS;            // Stack untuk Undo KRS
queue<Konsultasi> konsultasi; // Queue untuk Antrian Dosen

typedef pair<int, string> pis;
pqi(pis) prio_layanan; // Priority Queue untuk Layanan Prioritas

list<string> history; // Linked List untuk Riwayat Aktivitas
Tree struktur;        // Tree untuk Organisasi

bool loggedIn = false;
Mahasiswa currentMahasiswa;

// Helper Function: Print detail konsultasi
void printKonsultasi(Konsultasi konsul)
{
    cout << endl;
    cout << "ID Antrian : " << konsul.id << endl;
    cout << "Nama       : " << konsul.nama << endl;
    cout << "NIM        : " << konsul.nim << endl;
    cout << "Dosen      : " << konsul.dosen << endl;
    cout << "Waktu      : " << konsul.waktu << endl;
    cout << "-----------------------------" << endl;
}

// --- FUNGSI TREE (STRUKTUR ORGANISASI) ---

bool nodeExists(Tree &tree, const string &nama)
{
    if (tree.root == nama)
        return true;
    if (tree.nodes.find(nama) != tree.nodes.end())
        return true;
    for (auto &pair : tree.nodes)
        for (auto &child : pair.second)
            if (child == nama)
                return true;
    return false;
}

void tambahJabatan(Tree &tree)
{
    string parent, jabatanBaru;
    cout << "Masukkan nama atasan (parent): ";
    cin.ignore();
    getline(cin, parent);

    // Cek apakah parent ada
    if (!nodeExists(tree, parent))
    {
        cout << "Error: Atasan '" << parent << "' tidak ditemukan dalam struktur.\n";
        return;
    }

    cout << "Masukkan nama jabatan baru: ";
    getline(cin, jabatanBaru);

    tree.nodes[parent].push_back(jabatanBaru);
    cout << "Berhasil menambahkan \"" << jabatanBaru << "\" di bawah \"" << parent << "\".\n\n";
    history.push_back("Menambahkan Jabatan " + jabatanBaru);
}

void hapusSubTree(Tree &tree, const string &nama)
{
    if (tree.nodes.find(nama) != tree.nodes.end())
    {
        vector<string> anak = tree.nodes[nama];
        for (auto &c : anak)
            hapusSubTree(tree, c);
        tree.nodes.erase(nama);
    }
}

void hapusJabatan(Tree &tree)
{
    string nama;
    cout << "Masukkan nama jabatan yang akan dihapus: ";
    cin.ignore();
    getline(cin, nama);

    if (nama == tree.root)
    {
        tree.root.clear();
        tree.nodes.clear();
        cout << "Root dan seluruh struktur dihapus.\n\n";
        history.push_back("Menghapus Root Struktur");
        return;
    }

    // Hapus dari parent
    bool found = false;
    for (auto &pair : tree.nodes)
    {
        auto &children = pair.second;
        for (size_t i = 0; i < children.size(); i++)
        {
            if (children[i] == nama)
            {
                children.erase(children.begin() + i);
                found = true;
                break;
            }
        }
        if (found)
            break;
    }

    // Hapus subtree
    hapusSubTree(tree, nama);
    cout << "Berhasil menghapus \"" << nama << "\" beserta bawahannya.\n\n";
    history.push_back("Menghapus Jabatan " + nama);
}

void cetakTree(Tree &tree, const string &nama, const string &prefix, bool isLast, bool isRoot = false)
{
    if (isRoot)
    {
        cout << "[ROOT] " << nama << endl;
    }
    else
    {
        cout << prefix << (isLast ? "|___ " : "|-- ") << nama << endl;
    }

    auto it = tree.nodes.find(nama);
    if (it == tree.nodes.end() || it->second.empty())
        return;

    string newPrefix = prefix;
    if (!isRoot)
    {
        newPrefix += (isLast ? "     " : "|    ");
    }

    auto &children = it->second;
    for (size_t i = 0; i < children.size(); i++)
    {
        bool last = (i == children.size() - 1);
        cetakTree(tree, children[i], newPrefix, last);
    }
}

void inisialisasiRoot(Tree &tree)
{
    string jabatan;
    cout << "Struktur masih kosong. Masukkan jabatan tertinggi (Root): ";
    cin.ignore();
    getline(cin, jabatan);

    tree.root = jabatan;
    cout << "Root berhasil diset ke: " << tree.root << "\n\n";
    history.push_back("Inisialisasi Root: " + jabatan);
}

void tampilkanStruktur(Tree &tree)
{
    if (tree.root.empty())
    {
        cout << ">> Struktur organisasi masih kosong.\n\n";
        return;
    }
    cout << "\n=== STRUKTUR ORGANISASI ===\n";
    cetakTree(tree, tree.root, "", true, true);
    cout << "===========================\n\n";
}

// --- MOCK DATA (BAGIAN YANG DIMODIFIKASI) ---
void mockdata()
{
    // Data Mahasiswa Baru (Anti-Plagiat)
    daftarMhs["rinaltra"] = {
        .nama = "Rinaltra Nabasa S",
        .nim = "5025251024",
        .password = "admin123",
        .prodi = "Teknik Informatika",
        .semester = 4};

    daftarMhs["budi_santoso"] = {
        .nama = "Budi Santoso",
        .nim = "5025251055",
        .password = "mahasiswa1",
        .prodi = "Sistem Informasi",
        .semester = 2};

    daftarMhs["siti_aminah"] = {
        .nama = "Siti Aminah",
        .nim = "5025251089",
        .password = "beasiswa2026",
        .prodi = "Ilmu Komputer",
        .semester = 6};

    // Data Graph Navigasi Kampus
    adj["Gerbang_Utama"] = {{"Gedung_Rektorat", 5}, {"Fakultas_Teknik", 10}};
    adj["Gedung_Rektorat"] = {{"Gerbang_Utama", 5}, {"Perpustakaan_Pusat", 3}};
    adj["Fakultas_Teknik"] = {
        {"Gerbang_Utama", 10}, {"Perpustakaan_Pusat", 4}, {"Kantin_Mahasiswa", 2}};
    adj["Perpustakaan_Pusat"] = {
        {"Gedung_Rektorat", 3}, {"Fakultas_Teknik", 4}, {"Lab_Komputer", 5}};
    adj["Kantin_Mahasiswa"] = {{"Fakultas_Teknik", 2}, {"Lab_Komputer", 3}};
    adj["Lab_Komputer"] = {{"Perpustakaan_Pusat", 5}, {"Kantin_Mahasiswa", 3}};
}

// --- FITUR RIWAYAT (LINKED LIST) ---
void riwayatAktivitas()
{
    int opsi = 0;
    while (opsi != 4)
    {
        cout << "=========================" << endl;
        cout << "   RIWAYAT AKTIVITAS     " << endl;
        cout << "=========================" << endl;
        cout << "1. Hapus riwayat terakhir" << endl;
        cout << "2. Hapus semua riwayat" << endl;
        cout << "3. Tampilkan semua riwayat" << endl;
        cout << "4. Kembali" << endl
             << endl;

        cout << "Pilih Opsi: ";
        cin >> opsi;
        cout << endl;

        if (opsi == 1)
        {
            if (!history.empty())
            {
                history.pop_back();
                cout << ">> Riwayat terakhir berhasil dihapus!" << endl;
            }
            else
            {
                cout << ">> Riwayat sudah kosong!" << endl;
            }
        }
        else if (opsi == 2)
        {
            history.clear();
            cout << ">> Semua riwayat berhasil dihapus!" << endl;
        }
        else if (opsi == 3)
        {
            if (history.empty())
            {
                cout << "(Belum ada riwayat aktivitas)" << endl;
            }
            else
            {
                int no = 1;
                for (const auto &x : history)
                {
                    cout << no << ". " << x << endl;
                    no++;
                }
            }
        }
        cout << endl;
    }
}

// --- FITUR ANTRIAN (QUEUE) ---
void antrianKonsultasi()
{
    cout << "===================================" << endl;
    cout << "     ANTRIAN KONSULTASI DOSEN      " << endl;
    cout << "===================================" << endl
         << endl;

    int opsi;
    while (true)
    {
        cout << "1. Ambil Nomor Antrian" << endl;
        cout << "2. Panggil Antrian Berikutnya" << endl;
        cout << "3. Lihat Antrian Depan" << endl;
        cout << "4. Kembali" << endl
             << endl;
        cout << "Pilih Opsi: ";
        cin >> opsi;
        cout << endl;

        if (opsi == 4)
            return;

        else if (opsi == 3)
        {
            if (!konsultasi.empty())
            {
                cout << "Antrian Saat Ini:" << endl;
                printKonsultasi(konsultasi.front());
            }
            else
            {
                cout << ">> Antrian konsultasi kosong!" << endl;
            }
        }
        else if (opsi == 2)
        {
            if (!konsultasi.empty())
            {
                cout << "Memanggil antrian nomor: " << konsultasi.front().id << endl;
                printKonsultasi(konsultasi.front());

                history.push_back("Melayani Konsultasi: " + konsultasi.front().nama);
                konsultasi.pop();
                cout << ">> Pasien telah dilayani dan dikeluarkan dari antrian.\n";
            }
            else
            {
                cout << ">> Antrian konsultasi kosong!" << endl;
            }
        }
        else if (opsi == 1)
        {
            Konsultasi konsul;
            konsul.id = konsultasi.size() + 1;

            cout << "Masukkan Nama Mahasiswa: ";
            cin >> konsul.nama;
            cout << "Masukkan NIM: ";
            cin >> konsul.nim;
            cout << "Masukkan Nama Dosen: ";
            cin >> konsul.dosen;
            cout << "Masukkan Waktu (contoh: 10:00): ";
            cin >> konsul.waktu;

            konsultasi.push(konsul);
            history.push_back("Ambil Antrian: " + konsul.nama + " (" + konsul.nim + ")");

            cout << ">> Nomor Antrian Anda: " << konsul.id << "\n";
            cout << ">> Konsultasi berhasil ditambahkan!\n";
        }
        else
        {
            cout << ">> Opsi tidak ditemukan!" << endl;
        }
    }
}

// --- FITUR STRUKTUR ORGANISASI (TREE) ---
void strukturOrganisasi()
{
    cout << "===================================" << endl;
    cout << "   STRUKTUR ORGANISASI KAMPUS      " << endl;
    cout << "===================================" << endl
         << endl;

    int opsi;
    while (true)
    {
        cout << "1. Tambahkan Jabatan/Bawahan" << endl;
        cout << "2. Hapus Jabatan" << endl;
        cout << "3. Tampilkan Struktur" << endl;
        cout << "4. Kembali" << endl
             << endl;
        cout << "Pilih Opsi: ";
        cin >> opsi;

        if (opsi == 4)
            return;
        else if (opsi == 3)
            tampilkanStruktur(struktur);
        else if (opsi == 2)
            hapusJabatan(struktur);
        else if (opsi == 1)
        {
            if (struktur.root.empty())
                inisialisasiRoot(struktur);
            else
                tambahJabatan(struktur);
        }
        else
            cout << ">> Opsi tidak ditemukan!" << endl;
    }
}

// --- FITUR NAVIGASI (GRAPH + DIJKSTRA) ---
void navigasiKampus()
{
    cout << "===================================" << endl;
    cout << "      NAVIGASI KAMPUS (GRAPH)      " << endl;
    cout << "===================================" << endl
         << endl;

    int opsi;
    while (true)
    {
        cout << "1. Cari Rute Terpendek (Dijkstra)" << endl;
        cout << "2. Tampilkan Peta & Jalur" << endl;
        cout << "3. Tambah Lokasi Baru" << endl;
        cout << "4. Tambah Jalan Baru" << endl;
        cout << "5. Kembali" << endl
             << endl;
        cout << "Pilih Opsi: ";
        cin >> opsi;
        cout << endl;

        if (opsi == 5)
            return;

        else if (opsi == 2)
        {
            cout << "--- Peta Kampus Saat Ini ---" << endl;
            for (auto const &[asal, list_tujuan] : adj)
            {
                cout << "[" << asal << "] terhubung ke:\n";
                for (auto const &tetangga : list_tujuan)
                {
                    cout << "  -> " << tetangga.first << " (Jarak: " << tetangga.second << "m)\n";
                }
            }
            cout << endl;
        }
        else if (opsi == 3)
        {
            string newVertex;
            cout << "Masukkan nama lokasi baru: ";
            cin >> newVertex;
            if (adj.find(newVertex) == adj.end())
            {
                adj[newVertex] = vector<pair<string, int>>();
                cout << ">> Lokasi \"" << newVertex << "\" berhasil ditambahkan!\n\n";
                history.push_back("Tambah Lokasi: " + newVertex);
            }
            else
            {
                cout << ">> Lokasi sudah terdaftar!\n\n";
            }
        }
        else if (opsi == 4)
        {
            string asal, tujuan;
            int bobot;
            cout << "Lokasi Asal: ";
            cin >> asal;
            cout << "Lokasi Tujuan: ";
            cin >> tujuan;
            cout << "Jarak (meter): ";
            cin >> bobot;

            if (adj.find(asal) == adj.end() || adj.find(tujuan) == adj.end())
            {
                cout << ">> Error: Salah satu lokasi belum terdaftar di peta!\n\n";
                continue;
            }

            adj[asal].push_back({tujuan, bobot});
            adj[tujuan].push_back({asal, bobot});
            cout << ">> Jalan baru berhasil ditambahkan!\n\n";
            history.push_back("Tambah Jalan: " + asal + " - " + tujuan);
        }
        else if (opsi == 1)
        {
            string asal, tujuan;
            cout << "Lokasi Awal (Contoh: Gerbang_Utama): ";
            cin >> asal;
            cout << "Lokasi Tujuan: ";
            cin >> tujuan;

            if (adj.find(asal) == adj.end() || adj.find(tujuan) == adj.end())
            {
                cout << ">> Lokasi tidak valid!\n\n";
                continue;
            }

            // Algoritma Dijkstra
            unordered_map<string, int> dist;
            unordered_map<string, string> parent;
            for (auto const &[key, val] : adj)
                dist[key] = 1e9;

            pqi(pis) pq;
            dist[asal] = 0;
            pq.push({0, asal});

            while (!pq.empty())
            {
                string u = pq.top().second;
                int d = pq.top().first;
                pq.pop();

                if (d > dist[u])
                    continue;

                for (auto const &edge : adj[u])
                {
                    string v = edge.first;
                    int weight = edge.second;

                    if (dist[u] + weight < dist[v])
                    {
                        dist[v] = dist[u] + weight;
                        parent[v] = u;
                        pq.push({dist[v], v});
                    }
                }
            }

            if (dist[tujuan] == 1e9)
            {
                cout << ">> Tidak ada rute yang menghubungkan kedua lokasi tersebut.\n\n";
            }
            else
            {
                cout << ">> Jarak Terpendek: " << dist[tujuan] << " meter\n";

                // Rekonstruksi Jalur
                vector<string> path;
                string curr = tujuan;
                while (curr != asal)
                {
                    path.push_back(curr);
                    curr = parent[curr];
                }
                path.push_back(asal);
                reverse(path.begin(), path.end());

                cout << ">> Rute: ";
                for (size_t i = 0; i < path.size(); i++)
                {
                    cout << path[i];
                    if (i < path.size() - 1)
                        cout << " -> ";
                }
                cout << endl
                     << endl;
                history.push_back("Cari Rute: " + asal + " ke " + tujuan);
            }
        }
        else
        {
            cout << ">> Opsi tidak ditemukan!" << endl;
        }
    }
}

// --- FITUR KRS (STACK) ---
void operasiKRS()
{
    cout << "===================================" << endl;
    cout << "       PENGISIAN KRS (STACK)       " << endl;
    cout << "===================================" << endl
         << endl;

    int opsi;
    while (true)
    {
        cout << "1. Tambah Mata Kuliah" << endl;
        cout << "2. Undo (Hapus Terakhir)" << endl;
        cout << "3. Lihat Mata Kuliah Terakhir" << endl;
        cout << "4. Kembali" << endl
             << endl;
        cout << "Pilih Opsi: ";
        cin >> opsi;
        cout << endl;

        if (opsi == 4)
            return;

        else if (opsi == 3)
        {
            if (!KRS.empty())
            {
                cout << ">> Mata kuliah terakhir dipilih: " << KRS.top() << endl;
            }
            else
            {
                cout << ">> KRS masih kosong!" << endl;
            }
        }
        else if (opsi == 2)
        {
            if (!KRS.empty())
            {
                cout << ">> Menghapus: " << KRS.top() << endl;
                history.push_back("Undo KRS: " + KRS.top());
                KRS.pop();
                cout << ">> Berhasil di-undo!\n";
            }
            else
            {
                cout << ">> KRS kosong!" << endl;
            }
        }
        else if (opsi == 1)
        {
            string pilihan;
            cout << "Masukkan Kode/Nama MK: ";
            cin >> pilihan;

            KRS.push(pilihan);
            history.push_back("Tambah KRS: " + pilihan);
            cout << ">> " << pilihan << " berhasil ditambahkan!\n";
        }
        else
        {
            cout << ">> Opsi tidak ditemukan!" << endl;
        }
    }
}

// --- FITUR LAYANAN PRIORITAS (PRIORITY QUEUE) ---
void prioritasLayanan()
{
    cout << "===================================" << endl;
    cout << "    LAYANAN PRIORITAS (HEAP)       " << endl;
    cout << "===================================" << endl
         << endl;

    int opsi;
    while (true)
    {
        cout << "1. Daftar Layanan Prioritas" << endl;
        cout << "2. Proses Layanan Tertinggi" << endl;
        cout << "3. Cek Antrian Prioritas" << endl;
        cout << "4. Kembali" << endl
             << endl;
        cout << "Pilih Opsi: ";
        cin >> opsi;
        cout << endl;

        if (opsi == 4)
            return;

        else if (opsi == 3)
        {
            if (!prio_layanan.empty())
            {
                cout << ">> Prioritas Tertinggi Saat Ini: " << prio_layanan.top().second << endl;
            }
            else
            {
                cout << ">> Antrian prioritas kosong!" << endl;
            }
        }
        else if (opsi == 2)
        {
            if (!prio_layanan.empty())
            {
                cout << ">> Melayani: " << prio_layanan.top().second << endl;
                history.push_back("Layanan Prioritas: " + prio_layanan.top().second);
                prio_layanan.pop();
                cout << ">> Selesai diproses.\n";
            }
            else
            {
                cout << ">> Antrian prioritas kosong!" << endl;
            }
        }
        else if (opsi == 1)
        {
            string nama, kategori;
            cout << "Nama Mahasiswa: ";
            cin >> nama;
            cout << "Kategori (Beasiswa/Disabilitas/TingkatAkhir/Lainnya): ";
            cin.ignore();
            getline(cin, kategori);

            int pr = 4; // Default rendah
            if (kategori == "Beasiswa")
                pr = 1;
            else if (kategori == "Disabilitas")
                pr = 2;
            else if (kategori == "TingkatAkhir")
                pr = 3;

            prio_layanan.push({pr, nama});
            history.push_back("Daftar Prioritas: " + nama + " (" + kategori + ")");
            cout << ">> Berhasil masuk antrian prioritas!\n";
        }
        else
        {
            cout << ">> Opsi tidak ditemukan!" << endl;
        }
    }
}

// --- FITUR LOGIN & AKUN (HASH TABLE) ---
void cariAkun()
{
    string query;
    cout << "===================================" << endl;
    cout << "       PENCARIAN AKUN MAHASISWA    " << endl;
    cout << "===================================" << endl
         << endl;

    cout << "Masukkan Username (ketik 'exit' untuk keluar): ";
    while (cin >> query && query != "exit")
    {
        auto findUsername = daftarMhs.find(query);
        if (findUsername != daftarMhs.end())
        {
            cout << "\n--- Detail Akun ---" << endl;
            cout << "Username : " << query << endl;
            cout << "Nama     : " << daftarMhs[query].nama << endl;
            cout << "NIM      : " << daftarMhs[query].nim << endl;
            cout << "Prodi    : " << daftarMhs[query].prodi << endl;
            cout << "Semester : " << daftarMhs[query].semester << endl
                 << endl;
        }
        else
        {
            cout << ">> Username tidak ditemukan!\n\n";
        }
        cout << "Cari lagi (atau ketik 'exit'): ";
    }
    history.push_back("Mencari Akun");
}

void login()
{
    string username, pass;

    cout << "Username: ";
    cin >> username;
    auto found = daftarMhs.find(username);

    if (found == daftarMhs.end())
    {
        cout << ">> Error: Akun tidak terdaftar!" << endl
             << endl;
        return;
    }

    cout << "Password: ";
    cin >> pass;
    if (pass != daftarMhs[username].password)
    {
        cout << ">> Error: Password salah!" << endl
             << endl;
        return;
    }

    loggedIn = true;
    currentMahasiswa = daftarMhs[username];
    cout << "\n>> Login Berhasil! Selamat Datang, " << daftarMhs[username].nama << "!" << endl
         << endl;
    history.push_back("Login: " + username);
}

void buatAkun()
{
    Mahasiswa newUser;
    string username;

    cout << "--- Registrasi Akun Baru ---" << endl;
    cout << "Username: ";
    cin >> username;

    if (daftarMhs.find(username) != daftarMhs.end())
    {
        cout << ">> Username sudah digunakan!" << endl;
        return;
    }

    cout << "Nama Lengkap: ";
    cin >> newUser.nama;
    cout << "NIM: ";
    cin >> newUser.nim;
    cout << "Program Studi: ";
    cin >> newUser.prodi;
    cout << "Password: ";
    cin >> newUser.password;
    cout << "Semester: ";
    cin >> newUser.semester;

    daftarMhs[username] = newUser;
    cout << "\n>> Akun berhasil dibuat! Silakan Login." << endl
         << endl;
    history.push_back("Registrasi: " + username);
}

// --- MAIN PROGRAM ---
int main()
{
    cout << endl
         << "==================================================" << endl;
    cout << "   SMART CAMPUS NAVIGATION SYSTEM v1.0" << endl;
    cout << "==================================================" << endl
         << endl;

    mockdata(); // Load data awal

    // Loop Login
    while (!loggedIn)
    {
        cout << "1. Login Mahasiswa" << endl;
        cout << "2. Registrasi Akun Baru" << endl
             << endl;
        cout << "Pilih Opsi: ";

        int logOrReg;
        cin >> logOrReg;

        if (logOrReg == 1)
            login();
        else if (logOrReg == 2)
            buatAkun();
        else
            cout << ">> Opsi tidak valid!" << endl;
    }

    // Loop Menu Utama
    while (true)
    {
        cout << "==============================================" << endl;
        cout << " MENU UTAMA SMART CAMPUS (" << currentMahasiswa.nama << ")" << endl;
        cout << "==============================================" << endl;
        cout << "1. Riwayat Aktivitas (Linked List)" << endl;
        cout << "2. Antrian Konsultasi (Queue)" << endl;
        cout << "3. Struktur Organisasi (Tree)" << endl;
        cout << "4. Navigasi Kampus (Graph)" << endl;
        cout << "5. Pengisian KRS (Stack)" << endl;
        cout << "6. Layanan Prioritas (Priority Queue)" << endl;
        cout << "7. Cari Data Mahasiswa (Hash Table)" << endl;
        cout << "8. Logout" << endl
             << endl;

        int layanan;
        cout << "Pilih Layanan: ";
        cin >> layanan;
        cout << endl;

        switch (layanan)
        {
        case 1:
            riwayatAktivitas();
            break;
        case 2:
            antrianKonsultasi();
            break;
        case 3:
            strukturOrganisasi();
            break;
        case 4:
            navigasiKampus();
            break;
        case 5:
            operasiKRS();
            break;
        case 6:
            prioritasLayanan();
            break;
        case 7:
            cariAkun();
            break;
        case 8:
            cout << "Logout berhasil. Sampai jumpa!" << endl;
            return 0;
        default:
            cout << ">> Layanan tidak ditemukan!" << endl;
        }
        cout << endl;
    }

    return 0;
}