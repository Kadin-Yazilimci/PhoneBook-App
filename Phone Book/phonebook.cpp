#include <iostream>
#include <stdlib.h>
#include <iomanip> // setw() fonksiyonunu kullanarak daha düzenli ve iyi bir görüntü oluþturabiliriz. setw() fonksiyonunu kullanabilmemiz için gerekli kütüphanedir.
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include<cstdlib> // EXIT_SUCCESS : Defined in header <cstdlib>

#define MAXRECORD 100
#define NAME_LENGTH 20
#define PHONE_NUMBER_LENGTH 15

using namespace std;

/* 
Telefon Rehberi Uygulamasý :
Kayýtlý kiþilerin numaralarýný gösteren, kayýt ekleme , kayýt silme , kayýt güncelleme , kayýt arama gibi 
özellikleri bulunan bir telefon rehberi uygulamasý yapýlacaktýr.
*/

// Telefon rehberinde tutulacak kayýtlara iliþkin veri yapýsý :
struct Phone_Registration {
	
	char name[NAME_LENGTH]; 
    char phone_number[PHONE_NUMBER_LENGTH]; 
	  
}; 

// Public olarak algýlanýr.
struct File {
	
	char *filename;
	FILE *phonebook; // Dosya iþaretçisi FILE dosyalarý iþaret etmek için kullanýlýr.
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

// Dosya oluþturmak/açmak için :
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

// Dosyaya kayýt eklemek için :
void File::add(Phone_Registration *ykptr) {

	Phone_Registration k;
	int tmp1,tmp2;
	int counter = 0;
	
	for(int i=0;i<numberOfRecords;i++) {
		fseek(phonebook, index[i]*sizeof(Phone_Registration), SEEK_SET);  // Boþ yer arar.			
		fread( &k, sizeof (Phone_Registration), 1, phonebook);			  // Boþ yer bulununca belleðe veri ekleniyor.
		
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

// Dosya içeriðinde arama yapmak için :
int File::search(char tobeSearched[]) {
	
	Phone_Registration k;
	int counter=0;
	bool all=false;
	int found=0;
	
	if(strcmp(tobeSearched,"*")==0)
		all=true;
		
	fseek(phonebook, 0, SEEK_SET);
	// int fseek(FILE *stream, long offset, int origin);
	// stream: Konumu deðiþtirilecek dosya akýþýný gösterir.
	// offset: Origin parametresine göre kaydýrýlacak karakter sayýsýný gösterir. Negatif bir deðer aldýðýnda dosya konum göstergesi geriye doðru hareket eder.
	// origin: Dosya akýþýnýn baz alýnacak konumunu gösterir. Aþaðýdaki deðerlerden birini içerir:
	// SEEK_SET: Dosya baþý
	// SEEK_CUR: Dosyanýn aktif konumu
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

// Dosya içeriðinden veri silmek için :
void File::del(int registration_number) {
	
	Phone_Registration blank_registration={"",""};
		
	if(fseek(phonebook, sizeof(Phone_Registration)*(registration_number-1), SEEK_SET)==0)
		fwrite(&blank_registration, sizeof(Phone_Registration), 1, phonebook);
}

// Dosya içeriðinde veri güncellemek için :
void File::update(int registration_number, Phone_Registration *ykptr) {
	
	if(fseek(phonebook, sizeof(Phone_Registration)*(registration_number-1), SEEK_SET)==0) // Arayýp
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

// Dosyayý kapatmak için :
void File::close(){
	fclose(phonebook);
}

void print_menu();
bool make_transaction(char);
void search_record();
void add_record();
void delete_record();
void update_record() ;

// ANA FONKSÝYON : main() 
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
	
	system("cls");  // Windows da konsol ekranýný temizlemek için. Linux'ta ekran temizleme için system("clear"); kullanýn.
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

// Kayýt arama fonksiyon gövdesi
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

// Kayýt ekleme fonksiyon gövdesi
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

// kayýt silme fonksiyon gövdesi
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

// Kayýt güncelleme fonksiyon gövdesi
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
