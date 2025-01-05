#include <iostream>
#include <Windows.h>
#include <ctime>
#include <cstdlib>
#include <conio.h>
using namespace std;

enum RENK {  //Renklere sayi tanımlıyoruz.
    RENK_SIYAH = 0,
    RENK_KOYUMAVI = 1,
    RENK_KOYUYESIL = 2,
    RENK_AQUA = 3,
    RENK_KAHVERENGI = 4,
    RENK_KOYUMOR = 5,
    RENK_ACIKKAHVE = 6,
    RENK_GRI = 7,
    RENK_KOYUGRI = 8,
    RENK_MAVI = 9,
    RENK_YESIL = 10,
    RENK_ACIKMAVI = 11,
    RENK_KIRMIZI = 12,
    RENK_MOR = 13,
    RENK_SARI = 14,
    RENK_BEYAZ = 15
};

enum YON { // yönleri sayı ile tanımlıyoruz.
    YON_SOL = 1,
    YON_SAG = 2,
    YON_YUKARI = 3,
    YON_ASAGI = 4
};

struct YilanHucre {
    int x;
    int y;
    int yon;
    char karakter;
};

struct Elma {
    int x;
    int y;
    char karakter = '@'; // Elma karakteri
};

const int genislik = 80; //Terminalde açılan oyunumuzun çerçevesinin genişiliği
const int yukseklik = 20; //Terminalde açılan oyunumuzun çerçevesinin yüksekliği
const int maxYilanUzunlugu = 500;
const char yilanKarakteri = 219; // Yılanın görüntüsünü belirliyor.
int kuyrukUzunlugu = 0;
char tuslar[256];

YilanHucre yilanKuyrugu[maxYilanUzunlugu];

class Sahne {
public:
    char sahne[genislik][yukseklik];

    void sahneyiTemizle() {
        for (int y = 0; y < yukseklik; y++) {
            for (int x = 0; x < genislik; x++) {
                sahne[x][y] = ' ';
            }
        }
    }

    void sahneyiCiz() {
        for (int y = 0; y < yukseklik; y++) { //sahnemizin boyutuunu ayarlıyor.
            for (int x = 0; x < genislik; x++) {
                cout << sahne[x][y];
            }
            cout << endl;
        }
    }

    void sinirlariOlustur() { // sahne kenarlığı
        for (int x = 0; x < genislik; x++) {
            sahne[x][0] = 176;
            sahne[x][yukseklik - 1] = 176;
        }
        for (int y = 0; y < yukseklik; y++) {
            sahne[0][y] = 176;
            sahne[genislik - 1][y] = 176;
        }
    }

    void gotoxy(int x, int y) { // imleci belirli bir konumda tutmak için gereklidir.
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void kursoruGizle() { // oyunumuz çalışırken kursörün sürekli yanıp sönmemesi için gereklidir.
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(out, &cursorInfo);
    }

    void renkAta(RENK arkaPlan, RENK yazirengi) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), arkaPlan * 16 + yazirengi);
    }
};

class Yilan : public Sahne { // sınıf tanımlıyoruz
public:
    void yilanOlustur() {
        kuyrukUzunlugu = 1;
        yilanKuyrugu[0].x = 40; // yılanın oyuna başladığı konumu belirtir.
        yilanKuyrugu[0].y = 15;
        yilanKuyrugu[0].yon = YON_SAG; // Yılanın oyun başladığında hangi yöne doğru başlayacağını belirtir.
        yilanKuyrugu[0].karakter = yilanKarakteri;
        yilanKuyrugunaEkle();
        yilanKuyrugunaEkle();
    }

    void yilanKuyrugunaEkle() {
        if (kuyrukUzunlugu == maxYilanUzunlugu)
            return;

        int x = yilanKuyrugu[kuyrukUzunlugu - 1].x;
        int y = yilanKuyrugu[kuyrukUzunlugu - 1].y;
        int yon = yilanKuyrugu[kuyrukUzunlugu - 1].yon;
        char kar = yilanKuyrugu[kuyrukUzunlugu - 1].karakter;

        switch (yilanKuyrugu[kuyrukUzunlugu - 1].yon) {
        case YON_SAG:
            x--;
            break;
        case YON_SOL:
            x++;
            break;
        case YON_ASAGI:
            y--;
            break;
        case YON_YUKARI:
            y++;
            break;
        }
        yilanKuyrugu[kuyrukUzunlugu].x = x;
        yilanKuyrugu[kuyrukUzunlugu].y = y;
        yilanKuyrugu[kuyrukUzunlugu].karakter = kar;
        yilanKuyrugu[kuyrukUzunlugu].yon = yon;
        kuyrukUzunlugu++;
    }

    void yilaniHareketEttir() {
        for (int i = 0; i < kuyrukUzunlugu; i++) {
            switch (yilanKuyrugu[i].yon) {
            case YON_SAG:
                yilanKuyrugu[i].x++;
                break;
            case YON_SOL:
                yilanKuyrugu[i].x--;
                break;
            case YON_ASAGI:
                yilanKuyrugu[i].y++;
                break;
            case YON_YUKARI:
                yilanKuyrugu[i].y--;
                break;
            }
        }
        for (int i = kuyrukUzunlugu - 1; i > 0; i--) {
            yilanKuyrugu[i].yon = yilanKuyrugu[i - 1].yon;
        }
    }

    void yilaniSahneyeYerlestir() {
        for (int i = 0; i < kuyrukUzunlugu; i++) {
            int x = yilanKuyrugu[i].x;
            int y = yilanKuyrugu[i].y;
            sahne[x][y] = yilanKuyrugu[i].karakter;
        }
    }

    void klavyeKontrol() { // kullanıcının kontrol tuşları tanımlanır.
        for (int x = 0; x < 256; x++) {
            tuslar[x] = (char)(GetAsyncKeyState(x) >> 8);
        }

        if (tuslar['A'] != 0) {
            yilanKuyrugu[0].yon = YON_SOL;
        }
        if (tuslar['D'] != 0) {
            yilanKuyrugu[0].yon = YON_SAG;
        }
        if (tuslar['W'] != 0) {
            yilanKuyrugu[0].yon = YON_YUKARI;
        }
        if (tuslar['S'] != 0) {
            yilanKuyrugu[0].yon = YON_ASAGI;
        }
    }

    bool kendineCarpma() {
        for (int i = 1; i < kuyrukUzunlugu; i++) {
            if (yilanKuyrugu[0].x == yilanKuyrugu[i].x && yilanKuyrugu[0].y == yilanKuyrugu[i].y) {
                return true;
            }
        }
        return false;
    }

    bool sinirlariAsmaKontrol() {
        int x = yilanKuyrugu[0].x;
        int y = yilanKuyrugu[0].y;

        return (x <= 0 || x >= genislik - 1 || y <= 0 || y >= yukseklik - 1);
    }

    bool elmaYedi(Elma& elma) {
        return yilanKuyrugu[0].x == elma.x && yilanKuyrugu[0].y == elma.y;
    }

    void yilaniUzat() {
        yilanKuyrugunaEkle();
    }
};

void elmaYeriniGuncelle(Elma& elma) {
    bool cakisiyor = true;
    while (cakisiyor) {
        elma.x = rand() % (genislik - 2) + 1;
        elma.y = rand() % (yukseklik - 2) + 1;

        cakisiyor = false;
        for (int i = 0; i < kuyrukUzunlugu; i++) {
            if (elma.x == yilanKuyrugu[i].x && elma.y == yilanKuyrugu[i].y) {
                cakisiyor = true;
                break;
            }
        }
    }
}

int main() {
    srand(time(0));
    Yilan yilan;
    Elma elma;

    yilan.sahneyiTemizle();
    yilan.sinirlariOlustur();
    yilan.yilanOlustur();
    elmaYeriniGuncelle(elma);

    while (true) {
        yilan.kursoruGizle();
        yilan.sahneyiTemizle();
        yilan.sinirlariOlustur();
        yilan.klavyeKontrol();
        yilan.yilaniHareketEttir();

        if (yilan.elmaYedi(elma)) {
            yilan.yilaniUzat();
            yilan.yilaniUzat();
            elmaYeriniGuncelle(elma);
        }

        yilan.yilaniSahneyeYerlestir();

        if (yilan.kendineCarpma() || yilan.sinirlariAsmaKontrol()) {
            cout << "Game Over!" << endl;
            break;
        }

        yilan.sahne[elma.x][elma.y] = elma.karakter;
        yilan.gotoxy(0, 0);
        yilan.sahneyiCiz();
        yilan.renkAta(RENK_KOYUGRI, RENK_YESIL);
        Sleep(50);
    }

    cout << "Oyun Bitti! Cikmak icin herhangi bir tusa basin." << endl;
    _getch(); // Oyunun bitiş mesajını görmek için bekle
    return 0;
}
