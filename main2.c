/********************************************************************************
 *  main2-1.c                                                                   *
 *  Το αρχείο αυτό προσομοιώνει τον ρόλο ενός υψηλότερου-επιπέδου αρθρώματος    *
 *  ενός ΣΔΒΔ το οποίο καλεί συναρτήσεις του επιπέδου που ασχολείται με  τον    *
 *  χειρισμό b+-δένδρων (ΒΔ). Η προσοχή του αρχείου επικεντρώνεται              * 
 *  στην δημιουργία ΒΔ δένδρων και στην εισαγωγή και αναζήτηση εγγραφών         *
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defn.h"
#include "AM.h"

char empName[40];
char empAge[40];
char empSal[40];
char dnameAge[40];

void insertEntries(int eNentry, int eAentry, int eSentry, int dAentry,
		char* ename, int eage, float esal, char* edname, int recid) {
	char errStr[140];

	strcpy(errStr, "Error in AM_InsertEntry");

	if (AM_InsertEntry(eNentry, (void *) ename, &recid) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", empName);
		AM_PrintError(errStr);
	}
	if (AM_InsertEntry(eAentry, (void *) &eage, ename) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", empAge);
		AM_PrintError(errStr);
	}
	if (AM_InsertEntry(eSentry, (void *) &esal, ename) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", empSal);
		AM_PrintError(errStr);
	}
	if (AM_InsertEntry(dAentry, (void *) edname, &eage) != AME_OK) {
		sprintf(errStr, "Error in AM_InsertEntry called on %s \n", dnameAge);
		AM_PrintError(errStr);
	}

}

int main() {
	int scan1;

	int eNentry;
	int eAentry;
	int eSentry;
	int dAentry;

	int eage;
	float esal;
	char edname[40];
	char ename[40];
	char fltname[40];


	int recordid = 1;

	char errStr[200];

	int* ivalue = NULL;
	char* cvalue = NULL;

	/********************************************************************************
	 *  Αρχικοποίηση των εσωτερικών δομών του λογισμικού των ΒΔ                     *
	 ********************************************************************************/
	AM_Init();

	strcpy(empName, "EMP-NAME");
	strcpy(empAge, "EMP-AGE");
	strcpy(empSal, "EMP-SAL");
	strcpy(dnameAge, "DNAME-AGE");
	strcpy(fltname, "EMP-FAULT");

	
	/********************************************************************************
	 *  ¶νοιγμα των τεσσάρων ΑΚ (με σκοπό την εισαγωγή εγγραφών)                    *
	 ********************************************************************************/

	if ((eNentry = AM_OpenIndex(empName)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	if ((eAentry = AM_OpenIndex(empAge)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	if ((eSentry = AM_OpenIndex(empSal)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	if ((dAentry = AM_OpenIndex(dnameAge)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", dnameAge);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Εισαγωγή των δυάδων (τιμή1, τιμή2) στα ΑΚ. Υπάρχουν 100 συνολικά εισαγωγές    *
	 *  σε κάθε αρχείο, οι οποίες διαχωρίζονται με παύλες για δική σας ευκολία         *
	 ********************************************************************************/

	/* -----------------------------------------------------------------------------*/
	recordid = 200;

	
 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Warner Antonopoulos");
 	 eage = 21;
 	 esal = 113.4;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "O'Neal Fink");
 	 eage = 20;
 	 esal = 197.2;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Pavlou Maras");
 	 eage = 60;
 	 esal = 129.1;
 	 strcpy(edname, "TYRIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Alanis Sloan");
 	 eage = 62;
 	 esal = 255.1;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Howe Anthes");
 	 eage = 53;
 	 esal = 254.2;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "May Benton");
 	 eage = 53;
 	 esal = 455.2;
 	 strcpy(edname, "SALAMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Lloyd Whitley");
 	 eage = 59;
 	 esal = 238.4;
 	 strcpy(edname, "KRASIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Jacobson O'Neal");
 	 eage = 21;
 	 esal = 412.8;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Patel Lawson");
 	 eage = 49;
 	 esal = 195.0;
 	 strcpy(edname, "FROUTA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Vokos Wooten");
 	 eage = 25;
 	 esal = 315.4;
 	 strcpy(edname, "SALAMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Howe Marks");
 	 eage = 20;
 	 esal = 496.8;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Deal Higgins");
 	 eage = 52;
 	 esal = 395.0;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Kallas Lynch");
 	 eage = 59;
 	 esal = 160.8;
 	 strcpy(edname, "FROUTA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Mega Poe");
 	 eage = 53;
 	 esal = 283.5;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Pagonis Spiteri");
 	 eage = 61;
 	 esal = 305.2;
 	 strcpy(edname, "FROUTA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Watson Dragoumis");
 	 eage = 42;
 	 esal = 445.0;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Galatas Humphrey");
 	 eage = 42;
 	 esal = 318.1;
 	 strcpy(edname, "KRASIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Hirsch Browning");
 	 eage = 51;
 	 esal = 277.0;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Weeks Jackson");
 	 eage = 27;
 	 esal = 152.7;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Bush Cora");
 	 eage = 49;
 	 esal = 227.7;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Peris Kalfas");
 	 eage = 51;
 	 esal = 155.2;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Mavros Korba");
 	 eage = 59;
 	 esal = 106.5;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Straton Berger");
 	 eage = 20;
 	 esal = 139.6;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Theotokis Comis");
 	 eage = 26;
 	 esal = 386.0;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Lowry Maxwell");
 	 eage = 23;
 	 esal = 165.9;
 	 strcpy(edname, "LAXANIKA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Sisinis Kokes");
 	 eage = 48;
 	 esal = 101.0;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Petrou Petimezas");
 	 eage = 30;
 	 esal = 446.2;
 	 strcpy(edname, "SALAMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Sotiropoulos Wilson");
 	 eage = 30;
 	 esal = 362.5;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Avramidis Hamilton");
 	 eage = 23;
 	 esal = 108.3;
 	 strcpy(edname, "LAXANIKA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Peck Kirby");
 	 eage = 65;
 	 esal = 311.4;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Batchelor Perris");
 	 eage = 40;
 	 esal = 325.7;
 	 strcpy(edname, "SALAMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Papakonstantinou Economides");
 	 eage = 41;
 	 esal = 123.3;
 	 strcpy(edname, "SALAMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Rines Pepi");
 	 eage = 30;
 	 esal = 159.3;
 	 strcpy(edname, "LAXANIKA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Baxter Pulos");
 	 eage = 66;
 	 esal = 389.3;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Sarkis Cates");
 	 eage = 38;
 	 esal = 347.1;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Craven Gardner");
 	 eage = 57;
 	 esal = 169.1;
 	 strcpy(edname, "SALAMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Nakos West");
 	 eage = 52;
 	 esal = 106.2;
 	 strcpy(edname, "FROUTA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Gordon Antonopoulos");
 	 eage = 49;
 	 esal = 305.6;
 	 strcpy(edname, "TYRIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Metro Swanson");
 	 eage = 48;
 	 esal = 460.8;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Manikas Ballas");
 	 eage = 25;
 	 esal = 421.3;
 	 strcpy(edname, "KRASIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Ennis Lindsey");
 	 eage = 65;
 	 esal = 141.6;
 	 strcpy(edname, "FROUTA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Connor Alexandris");
 	 eage = 46;
 	 esal = 181.2;
 	 strcpy(edname, "SAPOUNIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "McLean Stafford");
 	 eage = 20;
 	 esal = 276.3;
 	 strcpy(edname, "LAXANIKA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Manella Ballas");
 	 eage = 56;
 	 esal = 121.5;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Angelogianopoulos Sloan");
 	 eage = 55;
 	 esal = 228.5;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Gillespie Willis");
 	 eage = 36;
 	 esal = 428.7;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Leandros Savage");
 	 eage = 30;
 	 esal = 385.6;
 	 strcpy(edname, "LAXANIKA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "McDowell Lamm");
 	 eage = 43;
 	 esal = 171.5;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Lyon Carter");
 	 eage = 37;
 	 esal = 108.5;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Gill Barbee");
 	 eage = 39;
 	 esal = 212.9;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Faircloth Hicks");
 	 eage = 37;
 	 esal = 370.4;
 	 strcpy(edname, "KRASIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Hines Lanier");
 	 eage = 69;
 	 esal = 376.2;
 	 strcpy(edname, "LAXANIKA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Asker Krestenitis");
 	 eage = 25;
 	 esal = 228.4;
 	 strcpy(edname, "SAPOUNIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Galanos Hurst");
 	 eage = 31;
 	 esal = 367.5;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Dickinson Rallis");
 	 eage = 39;
 	 esal = 185.5;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Gallo Papathanasiou");
 	 eage = 33;
 	 esal = 248.3;
 	 strcpy(edname, "FROUTA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Chandler Hewitt");
 	 eage = 59;
 	 esal = 398.7;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Vitalis Bowers");
 	 eage = 69;
 	 esal = 405.1;
 	 strcpy(edname, "SALAMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Lee Muto");
 	 eage = 31;
 	 esal = 211.7;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Richmond Pavlou");
 	 eage = 47;
 	 esal = 409.4;
 	 strcpy(edname, "SALAMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Finch Vassos");
 	 eage = 60;
 	 esal = 209.7;
 	 strcpy(edname, "FROUTA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Rouphos Samaras");
 	 eage = 42;
 	 esal = 468.5;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Conner Vlasopoulos");
 	 eage = 40;
 	 esal = 225.8;
 	 strcpy(edname, "KRASIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Long Lynch");
 	 eage = 56;
 	 esal = 279.9;
 	 strcpy(edname, "SAPOUNIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Davies Michael");
 	 eage = 49;
 	 esal = 487.7;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Neal Bates");
 	 eage = 23;
 	 esal = 127.8;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Petimezas Banks");
 	 eage = 23;
 	 esal = 131.4;
 	 strcpy(edname, "TYRIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "McKee Ennis");
 	 eage = 48;
 	 esal = 383.3;
 	 strcpy(edname, "LAXANIKA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Pearson Polites");
 	 eage = 37;
 	 esal = 342.7;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Crawford Brown");
 	 eage = 57;
 	 esal = 403.2;
 	 strcpy(edname, "FROUTA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Yu Starr");
 	 eage = 41;
 	 esal = 392.1;
 	 strcpy(edname, "TYRIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Levy McCarthy");
 	 eage = 64;
 	 esal = 276.6;
 	 strcpy(edname, "SALAMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Stone Pittman");
 	 eage = 47;
 	 esal = 357.8;
 	 strcpy(edname, "PSWMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Foster Fotopoulos");
 	 eage = 48;
 	 esal = 230.2;
 	 strcpy(edname, "SALAMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Callis Bush");
 	 eage = 50;
 	 esal = 351.3;
 	 strcpy(edname, "LAXANIKA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Porter Pepi");
 	 eage = 23;
 	 esal = 245.0;
 	 strcpy(edname, "SALAMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Konstantinou Galanis");
 	 eage = 35;
 	 esal = 425.3;
 	 strcpy(edname, "SAPOUNIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Metaxas McGee");
 	 eage = 64;
 	 esal = 313.9;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Lu Weiner");
 	 eage = 67;
 	 esal = 306.7;
 	 strcpy(edname, "LAXANIKA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Olsen Papakonstantinou");
 	 eage = 32;
 	 esal = 194.6;
 	 strcpy(edname, "SALAMIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Pollock Nikas");
 	 eage = 31;
 	 esal = 299.6;
 	 strcpy(edname, "LAXANIKA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Hendrix Benson");
 	 eage = 31;
 	 esal = 208.9;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Jones Waters");
 	 eage = 56;
 	 esal = 413.3;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Womble Austin");
 	 eage = 52;
 	 esal = 290.9;
 	 strcpy(edname, "SAPOUNIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Blair McKee");
 	 eage = 56;
 	 esal = 379.4;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Chaconas Holt");
 	 eage = 53;
 	 esal = 281.9;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Vaughn Welsh");
 	 eage = 63;
 	 esal = 406.1;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Theodorou Jordan");
 	 eage = 35;
 	 esal = 263.7;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Gillespie Goodman");
 	 eage = 28;
 	 esal = 409.2;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Maheras Ross");
 	 eage = 50;
 	 esal = 275.4;
 	 strcpy(edname, "FROUTA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Hatzidakis Christos");
 	 eage = 24;
 	 esal = 280.2;
 	 strcpy(edname, "LAXANIKA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Daly Maragos");
 	 eage = 46;
 	 esal = 116.8;
 	 strcpy(edname, "SAPOUNIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Frazier Sotir");
 	 eage = 67;
 	 esal = 241.6;
 	 strcpy(edname, "TYRIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Bailey Maheras");
 	 eage = 20;
 	 esal = 249.2;
 	 strcpy(edname, "SAPOUNIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "O'Donnell Paraskevopoulos");
 	 eage = 67;
 	 esal = 382.9;
 	 strcpy(edname, "FROUTA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Rallis Britt");
 	 eage = 20;
 	 esal = 490.7;
 	 strcpy(edname, "XYMOI");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Hood Mitro");
 	 eage = 21;
 	 esal = 447.5;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Chambers Harrington");
 	 eage = 38;
 	 esal = 289.1;
 	 strcpy(edname, "LAXANIKA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Bowling Mitsotakis");
 	 eage = 48;
 	 esal = 252.6;
 	 strcpy(edname, "TYRIA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

 	 /* -----------------------------------------------------------------------------*/

 	 strcpy(ename, "Speros Gabris");
 	 eage = 26;
 	 esal = 184.9;
 	 strcpy(edname, "KREATA");
 	 insertEntries(eNentry, eAentry, eSentry, dAentry, ename, eage, esal,
 		edname, ++recordid);

	/********************************************************************************
	 *  Τέλος των εισαγωγών στα τέσσερα αρχεία ΒΔ                                   *
	 ********************************************************************************/

	if (AM_CloseIndex(eNentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eAentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eSentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(dAentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", dnameAge);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *      Επερώτηση #1                                                            *
	 *  Έυρεση της εγγραφής "KOKKINOS ARISTOTELHS", γνωρίζοντας ότι το όνομα είναι  *
	 *	μοναδικό                                                                    *
	 ********************************************************************************/

	strcpy(ename, "KOKKINOS ARISTOTELHS");
	printf("\nRESULT OF QUERY #1\n\n");

	if ((eNentry = AM_OpenIndex(empName)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Άνοιγμα της αναζήτησης                                                      *
	 ********************************************************************************/
	if ((scan1 = AM_OpenIndexScan(eNentry, EQUAL, (void *) ename)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", empName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Εύρεση της πρώτης εγγραφής με όνομα "KOKKINOS ARISTOTELHS"                  *
	 ********************************************************************************/
	if ((ivalue = (int*) AM_FindNextEntry(scan1)) != NULL)
		printf("%d \n\n", *ivalue);
	else {
		sprintf(errStr, "Error in AM_FindNextEntry called on scan %d \n\n",
				scan1);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Κλείσιμο της αναζήτησης και του αρχείου ΑΚ                                  *
	 ********************************************************************************/
	if (AM_CloseIndexScan(scan1) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",
				scan1);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eNentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *      Επερώτηση #2                                                            *
	 *  Εύρεση των ονομάτων των υπαλλήλων με μισθό μικρότερο από 398.100            *
	 ********************************************************************************/

	esal = 398.1;
	printf("\nRESULT OF QUERY #2\n\n");

	if ((eSentry = AM_OpenIndex(empSal)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  ¶νοιγμα της αναζήτησης                                                      *
	 ********************************************************************************/
	if ((scan1 = AM_OpenIndexScan(eSentry, LESS_THAN_OR_EQUAL, (void *) &esal))
			< 0) {
		sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Σάρωση των σχετικών εγγραφών και εκτύπωση των αντίστοιχων ονομάτων          *
	 ********************************************************************************/
	while ((cvalue = (char*) AM_FindNextEntry(scan1)) != NULL) {
		printf("'%s'\n", cvalue);
	}

	if (AM_errno != AME_EOF) {
		sprintf(errStr, "Error in AM_FindNextEntry called on scan %d \n", scan1);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Κλείσιμο της αναζήτησης και του αρχείου ΑΚ                                  *
	 ********************************************************************************/
	if (AM_CloseIndexScan(scan1) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",
				scan1);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eSentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empSal);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *      Επερώτηση #3                                                            *
	 *  Αναζήτηση στο αρχείο για τα ονόματα αυτών που δεν είναι 19 χρονών           *
	 ********************************************************************************/

	eage = 19;
	printf("\nRESULT OF QUERY #3\n\n");

	if ((eAentry = AM_OpenIndex(empAge)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  'Aνοιγμα της αναζήτησης                                                      *
	 ********************************************************************************/
	if ((scan1 = AM_OpenIndexScan(eAentry, NOT_EQUAL, (void *) &eage)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Σάρωση των σχετικών εγγραφών και εκτύπωση των αντίστοιχων ονομάτων          *
	 ********************************************************************************/
	while ((cvalue = (char*) AM_FindNextEntry(scan1)) != NULL) {
		printf("'%s' \n", cvalue);
	}

	if (AM_errno != AME_EOF) {
		sprintf(errStr, "Error in AM_FindNextEntry called on scan %d \n", scan1);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Κλείσιμο της αναζήτησης και του αρχείου ΒΔ                                  *
	 ********************************************************************************/
	if (AM_CloseIndexScan(scan1) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",
				scan1);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eAentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empAge);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *      Eπερώτηση #4                                                            *
	 *  Αναζήτηση στο enames για να βρεθεί το αναγνωριστικό της εγγραφής ANEMONH    *
	 ********************************************************************************/

	strcpy(ename, "ANEMONH THETIS");
	printf("\nRESULT OF QUERY #4\n\n");

	if ((eNentry = AM_OpenIndex(empName)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  'Aνοιγμα της αναζήτησης                                                      *
	 ********************************************************************************/
	if ((scan1 = AM_OpenIndexScan(eNentry, EQUAL, (void *) ename)) < 0) {
		sprintf(errStr, "Error in AM_OpenIndexScan called on %s \n", empName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Eύρεση της εγγραφής ANEMONH.                                                *
	 ********************************************************************************/
	if ((ivalue = (int*) AM_FindNextEntry(scan1)) != NULL)
		printf("%d \n\n", *ivalue);
	else if (AM_errno != AME_EOF) {
		sprintf(errStr, "ANEMONH was not found due to an error");
		AM_PrintError(errStr);
	} else {
		sprintf(errStr,
				"ANEMONH was not found... (Note: This is the correct behaviour!)");
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Κλείσιμο της αναζήτησης και του αρχείου ΑΚ                                  *
	 ********************************************************************************/
	if (AM_CloseIndexScan(scan1) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndexScan called on scan %d \n",
				scan1);
		AM_PrintError(errStr);
	}

	if (AM_CloseIndex(eNentry) != AME_OK) {
		sprintf(errStr, "Error in AM_CloseIndex called on %s \n", empName);
		AM_PrintError(errStr);
	}

	/********************************************************************************
	 *  Τέλος του δοκιμαστικού προγράμματος για εισαγωγές/ αναζητήσεις    *
	 ********************************************************************************/

	return 0;
}
