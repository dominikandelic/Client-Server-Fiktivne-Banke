#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
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
void pretragaPoIbanu(const char* nazivDatoteke, char* trazeniIban);
void upravljajRacunom(KORISNIK*, int, const char*);
#endif