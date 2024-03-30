// Razi Atyani >> Razi  >> section 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Load 0.75
typedef struct{ //the countN  is the status and the number of countN if its zero then its empty else occupied
    char name[40];
    int countN;
    char activeIngredient[40];
    char category[35];
    char indication[40];
    char expiryDate [40];
    char company [60];
}Pharmacy;
// global variables
void updateLoad(); // flag is the load factor >> update its value >>function
Pharmacy *table_1,*table_2; // 1 is for linear and other is double
int lines =1,cap,size=0,c1=0,c2=0;
float flag ; // flag is the load
int prime(int r){ // receives i value and make it prime
    for(int i=2; i < r; i++){
        if(r % i == 0) {
            r= prime(++r);
            break;
        }
    }
    return r;
}
int prime2(int a){// you should decrement it at first >> function that get the nearest prime less than cap
    for(int i=2;i<a;i++){
        if(a%i==0){
            a = prime2(a - 1);
            break;
        }

    }
    return a;
}
unsigned int hash(char *s){ // returns the hash value of a given string
    unsigned hash =0;
    int i=0;
    while(*s != '\0'&& i!=3){
        hash+= *s++;
    i++;}

    return hash ;
}
void double_hash(char *str, Pharmacy t){ // function that do two hashing methods and insert in the second hash table
    int total=0,R = prime2(cap - 1);
    int hash_val = hash(str) % cap;
    int function = abs(R -(hash_val %  R));
    total+=  hash_val;
    while(table_2[total].countN != 0 && table_2[total].countN != -1) {// iterate until it find an empty or deleted spot
        c2++;
        total += function;
        total%=cap;
    }
    table_2[total] = t; // assign values to hash tables
    size++;
    updateLoad(); // update flag and size every time we add
}
void linear(Pharmacy test, int index){// function that insert in the first (linear hash table)
    while(table_1[index].countN != 0 && table_2[index].countN != -1) {
        c1++;
        index++;
        index %=cap;
    }
    table_1[index] = test;
}
void rehashing(){
    size=0;
    int old_cap = cap;// keep the old cap
    int new = prime(2 * cap);
    cap=new;
    table_1 =  malloc(new * sizeof (Pharmacy));
    for (int i = 0; i < new; ++i) { // iterate over the hash table and empty it
        table_1[i].countN =0;
    }
    c1=0;
    for(int i=0;i<old_cap;i++){ // iterate over the second hash table and insert the rehashed values in the first hash table
        if(table_2[i].countN != 0 && table_2[i].countN != -1){
            Pharmacy  test = table_2[i];
            linear(test, hash(test.name) % cap);

        }
    }
    table_2 = (Pharmacy*) malloc(new * sizeof (Pharmacy));// same function above but for the second hash table
    for (int i = 0; i < new; ++i) {
        table_2[i].countN =0;
    }
    c2 =0;
    for(int i=0;i<new;i++){ // samefunction above but for the second hash table
        table_2[i].countN =0;
        if(table_1[i].countN != 0 && table_2[i].countN != -1){
            Pharmacy  test = table_1[i];
            double_hash(test.name, test);
        }
    }
    updateLoad();
}
void updateLoad(){
    flag = (float )size / cap; // flag function divides the actual size on the cap
    if(flag >= Load)
        rehashing();
}
void file_reader(){ // read the file and insert the values insed hash tables
    Pharmacy test;
    char line[300];
    FILE *file = fopen("pharmacy.txt","r");
    if(!file)
        printf("the file isn't found!!\n");
    while(!feof(file)){
        fgets(line,300,file);
        sscanf(line, "%[^:]:%[^:]:%[^:]:%[^:]:%[^:]:%d:%[^\n]\n", test.name, test.activeIngredient, test.category, test.indication, test.expiryDate, &test.countN, test.company);
        linear(test, hash(test.name) % cap);
        double_hash(test.name, test);
        // update flag
    }
    fclose(file);
}
void num_lines(){ // this function get the number of coursers in a file
    char c;
    FILE *f = fopen("pharmacy.txt","r");
    while(!feof(f)){
        c = fgetc(f);
        if(c == '\n')
            lines++;
    }
    fclose(f);
    cap = prime(2 * lines);
    table_1 = (Pharmacy*)calloc(cap, sizeof (Pharmacy));
    table_2 = (Pharmacy*) calloc(cap, sizeof (Pharmacy));
}
int search_1(char*s){ // search for the Pharmacy name and return index
    int hash_val = hash(s) % cap;
    while(strcmp(table_1[hash_val].name, "") != 0){
        if(strcmp(table_1[hash_val].name, s) == 0){
            if(table_1[hash_val].countN != 0 && table_2[hash_val].countN != -1)
                return hash_val;
            else
                return -1;
        }

        hash_val++;
        hash_val%=cap;
    }
    return -1;
}
int search_2(char*s){ // search for the Pharmacy name and return index
    int R = prime2(cap - 1);
    int hash_val = hash(s) % cap;
    int function = R -(hash_val %  R);
    while(strcmp(table_2[hash_val].name, "") != 0){
        if(strcmp(table_2[hash_val].name, s) == 0) {
            if (table_1[hash_val].countN != 0 && table_2[hash_val].countN != -1)
                return hash_val;
            else
                return -1;
        }
        hash_val+=function;
        hash_val%=cap;
    }
    return -1;
}
void mainMenu(){ // where the menu and the backend functions settle
    int c;
    char str[35];
    char word[35];
    printf("Welcome to another Razi Program \n");
    num_lines();
    file_reader();
    while(c != 7){
        printf("1. Print hashed table (including empty spots).\n"
               "2. Print out table size.\n"
               "3. Print out the used hash functions.\n"
               "4. Insert a new record to hash table .\n"
               "5. Search for a specific word .\n"
               "6. Delete a specific record .\n"
               "7. Save hash table back to a file named Pharmacy.txt \n");
        scanf("%d",&c);
        switch (c) {
            case 1:
                for (int i = 0; i < cap; ++i)
                    printf("%i\t%s\n", i, table_2[i].name);
                break;
            case 2:
                updateLoad();
                printf("size of hash tables => %d\n cap of hash tables => %d\n ", size, cap);
                break;
            case 3:
                printf("h(x) = hash(str) + k");
                break;
            case 4:
                printf (" \nNow we will take the Info\n");
                Pharmacy test;
                printf("medicine NAME: ");
                fflush(stdin);
                gets(test.name);
                fflush(stdin);
                printf("countN: ");
                scanf("%d",&test.countN);
                fflush(stdin);
                printf("activeIngredient: ");
                gets(test.activeIngredient);
                fflush(stdin);
                printf("Indication: ");
                gets(test.indication);
                fflush(stdin);
                printf("company: ");
                gets(test.company);
                fflush(stdin);
                printf("expiryDate: ");
                gets(test.expiryDate);
                fflush(stdin);
                printf("Category: ");
                gets(test.category);
                fflush(stdin);
                linear(test, hash(test.name) % cap);
                double_hash(test.name, test);
                printf("\nhash_table  collisions => %d\n",  c2);
                break;
            case 5:
                printf("pls enter the name of Pharmacy to be searched for\nCourse name");
                fflush(stdin);
                gets(word);
                printf("the index is %d\n",search_2(word));
                break;
            case 6:
                printf("pls enter the name of the Pharmacy to be deleted\n medicine NAME:");
                fflush(stdin);
                gets(str);
                fflush(stdin);
                int index_1 = search_1(str);
                int index_2 = search_2(str);
                if(index_1== -1){
                    printf("NOT FOUND!!!!!\n");
                    return;
                }
                table_1[index_1].countN = -1;
                table_2[index_2].countN = -1;
                strcpy(table_1[index_1].name, "");
                strcpy(table_2[index_2].name, "");
                size--;
                updateLoad();
                break;
            case 7:
                printf("goodBye");
                FILE *save  = fopen("pharmacy.txt","w");
                for (int i = 0; i < cap; ++i) {
                    if(table_2[i].countN != 0 && table_2[i].countN != -1)
                        fprintf(save, "%s:%d:%s:%s:%s:%s:%s\n", table_2[i].name, table_2[i].countN, table_2[i].activeIngredient, table_2[i].category, table_2[i].indication, table_2[i].expiryDate, table_2[i].company);
                }
                fclose(save);
                break;
            default:
                printf("pls enter a valid value\n");
        }
    }
}
int main() {
    mainMenu();
}

