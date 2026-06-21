#include <bits/stdc++.h>
using namespace std;

// --- 1. STACK: Text Editor Undo ---
class TextEditor {
    stack<string> history;
    string currentText;
public:
    void type(string text) {
        history.push(currentText);
        currentText += text;
        cout << ">> Teks saat ini: " << currentText << endl;
    }
    void undo() {
        if (!history.empty()) {
            currentText = history.top();
            history.pop();
            cout << ">> Undo berhasil. Teks kembali ke: " << currentText << endl;
        } else {
            cout << ">> Tidak ada riwayat untuk di-undo." << endl;
        }
    }
};

// --- 2. QUEUE: Antrian Rumah Sakit ---
class HospitalQueue {
    queue<string> patients;
public:
    void arrive(string name) {
        patients.push(name);
        cout << ">> " << name << " telah masuk antrian." << endl;
    }
    void serve() {
        if (!patients.empty()) {
            cout << ">> Melayani pasien: " << patients.front() << endl;
            patients.pop();
        } else {
            cout << ">> Antrian kosong." << endl;
        }
    }
};

// --- 3. DOUBLY LINKED LIST: Playlist Musik ---
struct SongNode {
    string title;
    SongNode *prev, *next;
    SongNode(string t) : title(t), prev(nullptr), next(nullptr) {}
};

class MusicPlaylist {
    SongNode *head, *tail, *current;
public:
    MusicPlaylist() : head(nullptr), tail(nullptr), current(nullptr) {}
    
    void addSong(string title) {
        SongNode* newNode = new SongNode(title);
        if (!head) {
            head = tail = current = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        cout << ">> Lagu '" << title << "' ditambahkan ke playlist." << endl;
    }

    void nextSong() {
        if (current && current->next) {
            current = current->next;
            cout << ">> Memutar lagu berikutnya: " << current->title << endl;
        } else {
            cout << ">> Sudah di akhir playlist." << endl;
        }
    }

    void prevSong() {
        if (current && current->prev) {
            current = current->prev;
            cout << ">> Memutar lagu sebelumnya: " << current->title << endl;
        } else {
            cout << ">> Sudah di awal playlist." << endl;
        }
    }
};

// --- 4. TREE: Struktur Folder ---
struct FolderNode {
    string name;
    vector<FolderNode*> children;
    FolderNode(string n) : name(n) {}
};

void displayTree(FolderNode* node, int level) {
    for (int i = 0; i < level; ++i) cout << "  ";
    cout << "|-- " << node->name << endl;
    for (auto child : node->children) {
        displayTree(child, level + 1);
    }
}

// --- 5. BST: Data Mahasiswa ---
struct StudentNode {
    int nim; 
    string name;
    StudentNode *left, *right;
    StudentNode(int n, string nm) : nim(n), name(nm), left(nullptr), right(nullptr) {}
};

class StudentDB {
    StudentNode* root;
    StudentNode* insertRec(StudentNode* node, int nim, string name) {
        if (!node) return new StudentNode(nim, name);
        if (nim < node->nim) node->left = insertRec(node->left, nim, name);
        else if (nim > node->nim) node->right = insertRec(node->right, nim, name);
        return node;
    }
    StudentNode* searchRec(StudentNode* node, int nim) {
        if (!node || node->nim == nim) return node;
        if (nim < node->nim) return searchRec(node->left, nim);
        return searchRec(node->right, nim);
    }
public:
    StudentDB() : root(nullptr) {}
    void add(int nim, string name) { 
        root = insertRec(root, nim, name); 
        cout << ">> Data mahasiswa berhasil ditambahkan." << endl; 
    }
    void find(int nim) {
        StudentNode* res = searchRec(root, nim);
        if (res) cout << ">> Ditemukan: " << res->name << " (NIM: " << res->nim << ")" << endl;
        else cout << ">> Data mahasiswa tidak ditemukan." << endl;
    }
};

// --- 6. GRAPH: Navigasi Dijkstra ---
class Navigator {
    map<string, vector<pair<int, string>>> adj;
public:
    void addRoad(string u, string v, int w) {
        adj[u].push_back({w, v});
        adj[v].push_back({w, u});
        cout << ">> Jalan terhubung: " << u << " - " << v << " (" << w << " km)" << endl;
    }
    void shortestPath(string start, string end) {
        map<string, int> dist;
        // Menggunakan priority_queue dengan greater untuk min-heap
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        
        for (auto& node : adj) dist[node.first] = INT_MAX;
        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            int d = pq.top().first; 
            string u = pq.top().second; 
            pq.pop();
            
            if (d > dist[u]) continue;
            
            for (auto& edge : adj[u]) {
                int w = edge.first; 
                string v = edge.second;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        
        if (dist[end] == INT_MAX) cout << ">> Tidak ada rute yang tersedia." << endl;
        else cout << ">> Jarak terpendek ke " << end << ": " << dist[end] << " km" << endl;
    }
};

// --- 7. MAP (Hash Table): Login ---
class LoginSys {
    map<string, string> db;
public:
    void reg(string u, string p) { 
        db[u] = p; 
        cout << ">> Akun berhasil dibuat." << endl; 
    }
    void login(string u, string p) {
        if (db.count(u) && db[u] == p) cout << ">> Login Berhasil! Selamat datang." << endl;
        else cout << ">> Login Gagal. Periksa NIM atau Password." << endl;
    }
};

// --- 8. PRIORITY QUEUE: IGD ---
struct Patient {
    string name; 
    int prio;
    // Operator overloading untuk priority queue (min-heap based on priority)
    bool operator>(const Patient& o) const { 
        return prio > o.prio; 
    }
};

class ER {
    priority_queue<Patient, vector<Patient>, greater<Patient>> pq;
public:
    void admit(string n, int p) { 
        pq.push({n, p}); 
        cout << ">> Pasien " << n << " masuk triase (Prioritas: " << p << ")" << endl; 
    }
    void treat() {
        if (!pq.empty()) {
            cout << ">> Merawat pasien: " << pq.top().name << " (Prioritas: " << pq.top().prio << ")" << endl; 
            pq.pop();
        } else {
            cout << ">> Tidak ada pasien dalam antrian." << endl;
        }
    }
};

int main() {
    int choice;
    // Inisialisasi objek untuk setiap studi kasus
    TextEditor editor; 
    HospitalQueue hosp; 
    MusicPlaylist spotify;
    FolderNode* rootFolder = new FolderNode("Root_Direktori");
    StudentDB dbMahasiswa; 
    Navigator navKampus; 
    LoginSys authSystem; 
    ER unitIGD;

    do {
        cout << "\n========================================\n";
        cout << "   MENU STUDI KASUS STRUKTUR DATA\n";
        cout << "========================================\n";
        cout << "1. Text Editor (Stack)\n";
        cout << "2. Antrian Rumah Sakit (Queue)\n";
        cout << "3. Playlist Musik (Linked List)\n";
        cout << "4. Sistem File (Tree)\n";
        cout << "5. Data Mahasiswa (BST)\n";
        cout << "6. Navigasi Rute (Graph)\n";
        cout << "7. Sistem Login (Map/Hash)\n";
        cout << "8. Unit Gawat Darurat (Priority Queue)\n";
        cout << "9. Keluar\n";
        cout << "----------------------------------------\n";
        cout << "Pilihan Anda: "; 
        cin >> choice;

        if (choice == 1) {
            int sub; 
            cout << "\n[Text Editor]\n1. Ketik Teks\n2. Undo\nPilihan: "; 
            cin >> sub;
            if (sub == 1) { 
                string t; 
                cout << "Masukkan teks: "; 
                cin >> t; 
                editor.type(t); 
            }
            else if (sub == 2) editor.undo();
        } 
        else if (choice == 2) {
            int sub; 
            cout << "\n[Antrian RS]\n1. Pasien Datang\n2. Layani Pasien\nPilihan: "; 
            cin >> sub;
            if (sub == 1) { 
                string n; 
                cout << "Nama Pasien: "; 
                cin >> n; 
                hosp.arrive(n); 
            }
            else if (sub == 2) hosp.serve();
        } 
        else if (choice == 3) {
            int sub; 
            cout << "\n[Playlist]\n1. Tambah Lagu\n2. Next\n3. Previous\nPilihan: "; 
            cin >> sub;
            if (sub == 1) { 
                string t; 
                cout << "Judul Lagu: "; 
                cin >> t; 
                spotify.addSong(t); 
            }
            else if (sub == 2) spotify.nextSong();
            else if (sub == 3) spotify.prevSong();
        } 
        else if (choice == 4) {
            string f; 
            cout << "\n[File System]\nMasukkan nama folder baru: "; 
            cin >> f;
            rootFolder->children.push_back(new FolderNode(f));
            cout << "Struktur Folder Saat Ini:\n";
            displayTree(rootFolder, 0);
        } 
        else if (choice == 5) {
            int sub; 
            cout << "\n[Data Mahasiswa]\n1. Tambah Data\n2. Cari Data\nPilihan: "; 
            cin >> sub;
            if (sub == 1) { 
                int n; 
                string nm; 
                cout << "NIM: "; cin >> n; 
                cout << "Nama: "; cin >> nm; 
                dbMahasiswa.add(n, nm); 
            }
            else if (sub == 2) { 
                int n; 
                cout << "Cari NIM: "; 
                cin >> n; 
                dbMahasiswa.find(n); 
            }
        } 
        else if (choice == 6) {
            int sub; 
            cout << "\n[Navigasi]\n1. Tambah Jalan\n2. Cari Rute Tercepat\nPilihan: "; 
            cin >> sub;
            if (sub == 1) { 
                string a, b; 
                int w; 
                cout << "Dari: "; cin >> a; 
                cout << "Ke: "; cin >> b; 
                cout << "Jarak (km): "; cin >> w; 
                navKampus.addRoad(a, b, w); 
            }
            else if (sub == 2) { 
                string a, b; 
                cout << "Rute Dari: "; cin >> a; 
                cout << "Ke Tujuan: "; cin >> b; 
                navKampus.shortestPath(a, b); 
            }
        } 
        else if (choice == 7) {
            int sub; 
            cout << "\n[Sistem Login]\n1. Registrasi\n2. Login\nPilihan: "; 
            cin >> sub;
            if (sub == 1) { 
                string u, p; 
                cout << "Username: "; cin >> u; 
                cout << "Password: "; cin >> p; 
                authSystem.reg(u, p); 
            }
            else if (sub == 2) { 
                string u, p; 
                cout << "Username: "; cin >> u; 
                cout << "Password: "; cin >> p; 
                authSystem.login(u, p); 
            }
        } 
        else if (choice == 8) {
            int sub; 
            cout << "\n[IGD]\n1. Masuk Pasien\n2. Rawat Pasien\nPilihan: "; 
            cin >> sub;
            if (sub == 1) { 
                string n; 
                int p; 
                cout << "Nama Pasien: "; cin >> n; 
                cout << "Prioritas (1-Kritis, 5-Ringan): "; cin >> p; 
                unitIGD.admit(n, p); 
            }
            else if (sub == 2) unitIGD.treat();
        }
    } while (choice != 9);
    
    cout << "Program selesai. Terima kasih!" << endl;
    return 0;
}   