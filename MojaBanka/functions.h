#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define _CRT_SECURE_NO_WARNINGS
#define LOZINKA_N 9

typedef struct racun {
	int tip_racuna;
	double saldo;
	char iban[22];
} RACUN;

typedef struct datum {
	unsigned short dan;
	unsigned short mjesec;
	unsigned short godina;
} DATUM;

typedef struct tecaj {
	float usd;
	float eur;
	float aud;
	float chf;
} TECAJ;
typedef struct korisnik {
	int id;
	char ime[25];
	char prezime[25];
	char adresa[35];
	char prebivaliste[35];
	char oib[12];
	char lozinka[9];
	int broj_racuna;
	char postanski_broj[6];
	DATUM datum_rodenja;
	RACUN trasankc_racuni[4];
} KORISNIK;

void izbornik(const char*);
void kreiranjeKorisnika(const char*);
void unosImenaPrezime(KORISNIK*);
void unosAdrese(KORISNIK*);
void unosPrebivalista(KORISNIK*);
void stvoriKonekciju(void);
KORISNIK* pretragaKorisnika(const char*);
KORISNIK* ucitajKorisnike(const char*);
void ispisSvihKorisnika(const char*);
void brisanjeKorisnika(KORISNIK*, const char*);
void dodajRacun(KORISNIK*, const char*);
void generiranjeIbana(KORISNIK*, int);
void pretragaPoIbanu(const char*, char*);
int pretragaPoIbanuKlijent(KORISNIK*, char*);
void upravljajRacunom(KORISNIK*, int, const char*);
void upravljanjeTecajnomListom(void);
int brisanjeTecajneListe(void);
char* generiranjeLozinke(void);
void klijentSucelje(KORISNIK*, const char*);
void promjenaLozinkeKlijenta(KORISNIK*, const char*);
void upravljanjePlacanjemKlijenta(KORISNIK*, const char*);
int placanjeKlijenta(int, const char*, KORISNIK*, char*);
int provjeraOdredisnogIbana(double, char*, const char*);
void ispisTransakcijaKlijent(KORISNIK*, const char*);
void ispisSvihTransakcija(const char*);
void zamjena(double* const, double* const);
void sortiranje(double*, const int, const int);
void sortiranjeImenaKorisnika(const char*);
KORISNIK* pretragaKorisnikaRekurzija(const char*);
#endif