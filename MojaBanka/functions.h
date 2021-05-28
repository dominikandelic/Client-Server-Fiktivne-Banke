#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
typedef struct racun {
	int tip_racuna;
	double saldo;
	char* iban;
} RACUN;

typedef struct datum {
	unsigned short dan;
	unsigned short mjesec;
	unsigned short godina;
} DATUM;
typedef struct korisnik {
	int id;
	char ime[25];
	char prezime[25];
	char adresa[35];
	char prebivaliste[35];
	int broj_racuna;
	char postanski_broj[6];
	RACUN* trasankc_racuni;
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
void brisanjeKorisnika(KORISNIK**, const char*);
#endif