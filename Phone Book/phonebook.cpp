#include <iostream>
#include <stdlib.h>
#include <iomanip> // setw() fonksiyonunu kullanarak daha d�zenli ve iyi bir g�r�nt� olu�turabiliriz. setw() fonksiyonunu kullanabilmemiz i�in gerekli k�t�phanedir.
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include<cstdlib> // EXIT_SUCCESS : Defined in header <cstdlib>

#define MAXRECORD 100
#define NAME_LENGTH 20
#define PHONE_NUMBER_LENGTH 15

using namespace std;

/* 
Telefon Rehberi Uygulamas� :
Kay�tl� ki�ilerin numaralar�n� g�steren, kay�t ekleme , kay�t silme , kay�t g�ncelleme , kay�t arama gibi 
�zellikleri bulunan bir telefon rehberi uygulamas� yap�lacakt�r.
*/

// Telefon rehberinde tutulacak kay�tlara ili�kin veri yap�s� :
struct Phone_Registration {
	
	char name[NAME_LENGTH]; 
    char phone_number[PHONE_NUMBER_LENGTH]; 
	  
}; 

// Public olarak alg�lan�r.
struct File {
	
	char *filename;
	FILE *phonebook; // Dosya i�aret�isi FILE dosyalar� i�aret etmek i�in kullan�l�r.
	int index[MAXRECORD];
	
	void create();
	void add(Phone_Registration *);
	int search(char []);
	void del(int registration_number);
	void update(int registration_number, Phone_Registration *);
	void create_array();
	void close();
	int numberOfRecords;

};

typedef File DataStructure;
DataStructure registry;

// Dosya olu�turmak/a�mak i�in :
void File::create() {
	
	filename = "phonebook.txt";
	phonebook = fopen( filename, "r+" );
	
	if(!phonebook) {
	    phonebook = fopen( filename, "w+" );
		if(!phonebook){
			cerr << "The file could not be opened." << endl;
			exit(1);
		}
	}
	numberOfRecords=0;
	index[0]=0;
    create_array();
}

// Dosyaya kay�t eklemek i�in :
void File::add(Phone_Registration *ykptr) {

	Phone_Registration k;
	int tmp1,tmp2;
	int counter = 0;
	
	for(int i=0;i<numberOfRecords;i++) {
		fseek(phonebook, index[i]*sizeof(Phone_Registration), SEEK_SET);  // Bo� yer arar.			
		fread( &k, sizeof (Phone_Registration), 1, phonebook);			  // Bo� yer bulununca belle�e veri ekleniyor.
		
		if((strcmp(k.name,ykptr->name) < 0)) {  
			counter++;
		}
		else
			break;     
	}
	
    numberOfRecords++;
	tmp1 = index[counter];
	index[counter] = numberOfRecords-1;
	counter++;
	
	while(counter <= numberOfRecords) {
		tmp2 = index[counter];
		index[counter] = tmp1;
		tmp1 = tmp2;
		counter++;
	}
	
	fseek(phonebook, 0, SEEK_END);
	fwrite(ykptr, sizeof(Phone_Registration), 1, phonebook);

}

// Dosya i�eri�inde arama yapmak i�in :
int File::search(char tobeSearched[]) {
	
	Phone_Registration k;
	int counter=0;
	bool all=false;
	int found=0;
	
	if(strcmp(tobeSearched,"*")==0)
		all=true;
		
	fseek(phonebook, 0, SEEK_SET);
	// int fseek(FILE *stream, long offset, int origin);
	// stream: Konumu de�i�tirilecek dosya ak���n� g�sterir.
	// offset: Origin parametresine g�re kayd�r�lacak karakter say�s�n� g�sterir. Negatif bir de�er ald���nda dosya konum g�stergesi geriye do�ru hareket eder.
	// origin: Dosya ak���n�n baz al�nacak konumunu g�sterir. A�a��daki de�erlerden birini i�erir:
	// SEEK_SET: Dosya ba��
	// SEEK_CUR: Dosyan�n aktif konumu
	// SEEK_END: Dosya sonu
	
	for(int i=0;i<numberOfRecords;i++) {
		
			fseek(phonebook, index[i]*sizeof(Phone_Registration), SEEK_SET);			
			fread( &k, sizeof (Phone_Registration), 1, phonebook);				
			
			if(all && (strcmp(k.name,"")!=0)){ 
				counter ++;
				cout << index[i]+1 << "." << k.name << " " << k.phone_number << endl;
				found++;
			}
			if(!all && strncmp(k.name, tobeSearched, strlen(tobeSearched))==0) {
				cout << index[i]+1 << "." << k.name << " " << k.phone_number << endl;
				found++;
			}		
	}
	
	return found;
}

// Dosya i�eri�inden veri silmek i�in :
void File::del(int registration_number) {
	
	Phone_Registration blank_registration={"",""};
		
	if(fseek(phonebook, sizeof(Phone_Registration)*(registration_number-1), SEEK_SET)==0)
		fwrite(&blank_registration, sizeof(Phone_Registration), 1, phonebook);
}

// Dosya i�eri�inde veri g�ncellemek i�in :
void File::update(int registration_number, Phone_Registration *ykptr) {
	
	if(fseek(phonebook, sizeof(Phone_Registration)*(registration_number-1), SEEK_SET)==0) // Aray�p
		fwrite(ykptr, sizeof(Phone_Registration), 1, phonebook);						  // Veri yazar.
}

void File::create_array() {
	
	Phone_Registration k1,k2;
	int tmp1,tmp2;
    int counter;
	fseek(phonebook, 0, SEEK_SET);
	
	while(!feof(phonebook)) {
		
		fseek(phonebook, numberOfRecords*sizeof (Phone_Registration), SEEK_SET);
		fread( &k1, sizeof (Phone_Registration), 1, phonebook);		
		
		if(feof(phonebook)) 
			break;
			
		numberOfRecords++;
		counter = 0;
		
		for(int i=0;i<numberOfRecords-1;i++) {
			
			fseek(phonebook, index[i]*sizeof (Phone_Registration), SEEK_SET);			
			fread( &k2, sizeof (Phone_Registration), 1, phonebook);	
				
			if((strcmp(k1.name,k2.name)>0)) {  
				counter++;
			}
			
			else
				break;
		}
		
		tmp1 = index[counter];
		index[counter] = numberOfRecords-1;
		counter++;
		
		while(counter<=numberOfRecords){
			tmp2 = index[counter];
			index[counter] = tmp1;
			tmp1 = tmp2;
			counter++;
		}
	}	
}

// Dosyay� kapatmak i�in :
void File::close(){
	fclose(phonebook);
}

void print_menu();
bool make_transaction(char);
void search_record();
void add_record();
void delete_record();
void update_record() ;

// ANA FONKS�YON : main() 
int main() {
	
	registry.create();	
	bool finish = false; 
	char choice; 
		
	while (!finish) { 
		print_menu(); 
		cin >> choice;
		finish = make_transaction(choice); 
	} 	
	
	registry.close();
	return EXIT_SUCCESS; // Successful execution of a program.
}

void print_menu() {
	
	system("cls");  // Windows da konsol ekran�n� temizlemek i�in. Linux'ta ekran temizleme i�in system("clear"); kullan�n.
	cout << endl;
	cout << " \t TELEPHONE DIRECTORY APP  " << endl;
	cout << "\t---------------------------" << endl << endl;
	cout << "\tSelect a transaction : " << endl;
	cout << "\tA: Decryption of records " << endl;
	cout << "\tB: Adding a record " << endl;
	cout << "\tC: Registration update " << endl;
	cout << "\tD: Deletion of registration " << endl;	
 	cout << "\tE: Exit " << endl << endl;	
	cout << "\tEnter an option --> {A, B, C, D, E} : ";
}

bool make_transaction(char choice) {
	bool end = false;
	
	switch (choice) { 
	
		case 'A': case 'a': 
			search_record();
			break; 
			
		case 'B': case 'b': 
			add_record();
			break; 
			
		case 'C': case 'c': 
			update_record();
			break;
			
		case 'D': case 'd': 
			delete_record();
			break;
			
		case 'E': case 'e': 
			cout << "\tAre you sure you want to end the program? (T/F): ";
			cin >> choice;
			
			if(choice=='T' || choice=='t') 
				end = true; 	
			break; 
	    	
		default: 
			cout << "\tError: You logged in incorrectly! " << endl; 
			cout << "\tTry again --> {A, B, C, D, E} : " ;
			cin >> choice;
			end = make_transaction(choice);
			break; 
	}
	return end;
}

// Kay�t arama fonksiyon g�vdesi
void search_record() {
	
	char name[NAME_LENGTH];
	cout << "\nPlease enter the name of the person you want to call (click '*' for the entire list) : " << endl;
	cin.ignore(1000, '\n');
	cin.getline(name,NAME_LENGTH);
			
	if(registry.search(name)==0){
		cout << "The registration according to the criteria you are looking for could not be found." << endl;
	}
	
	getchar();

};

// Kay�t ekleme fonksiyon g�vdesi
void add_record() {
	
	Phone_Registration new_registration;	
	cout << "\nPlease enter the information of the person you want to register : " << endl;
	cout << "Name : " ;	
	cin.ignore(1000, '\n');
	cin.getline(new_registration.name,NAME_LENGTH);
	cout << "Phone number : ";
	cin >> setw(PHONE_NUMBER_LENGTH) >> new_registration.phone_number;
	registry.add(&new_registration);
	
	cout << "Your registration has been added." << endl;
	getchar();
	getchar();

};

// kay�t silme fonksiyon g�vdesi
void delete_record() {
	
	char name[NAME_LENGTH];
	int choice;	
	cout << "\nPlease enter the name of the person whose registration you want to delete (click '*' for the entire list): " << endl;
	cin.ignore(1000, '\n');
	cin.getline(name,NAME_LENGTH);		
	int numberOfPeople = registry.search(name); 
	
	if(numberOfPeople==0) {
		cout << "The registration according to the criteria you are looking for could not be found." << endl;		
	}
	else {
		if (numberOfPeople==1) {		
			cout << "The recording was found." << endl;
			cout << "If this is the record you want to delete, please enter its number (Enter -1 to exit without processing): " ;
		}
		else
			cout << "Enter the number of the record you want to delete (Enter -1 to exit without processing): " ;
		
		cin >> choice;
		if(choice==-1) 
			return;
			
		registry.del(choice);	
		cout << "The record was deleted." <<endl;
	}
	
	getchar();
	getchar();
};

// Kay�t g�ncelleme fonksiyon g�vdesi
void update_record() {
	
	char name[NAME_LENGTH];
	int choice;	
	cout << "\nPlease enter the name of the person whose registration you want to update (click '*' for the entire list): " << endl;
	cin.ignore(1000, '\n');
	cin.getline(name,NAME_LENGTH);		
	int numberOfPeople=registry.search(name); 
	
	if(numberOfPeople==0){
		cout << "The registration according to the criteria you are looking for could not be found." << endl;		
	}
	else {
		if (numberOfPeople==1){		
			cout << "The recording was found." << endl;
			cout << "If this is the record you want to update, please enter the number (Enter -1 to exit without processing): " ;
		}
		else
			cout << "Enter the number of the record you want to update (Enter -1 to exit without processing): " ;
			
		cin >> choice;
		if(choice==-1)
			return;
			
		Phone_Registration new_registration;
		cout << "Please enter the current information : " << endl;
		cout << "Name : ";
		cin.ignore(1000,'\n');
		cin.getline(new_registration.name,NAME_LENGTH);
		cout << "Phone number : ";
		cin >> setw(PHONE_NUMBER_LENGTH) >> new_registration.phone_number;
		registry.update(choice,&new_registration);	
		cout << "Your registration has been successfully updated." <<endl;	
	}
	
	getchar();
	getchar();
};
