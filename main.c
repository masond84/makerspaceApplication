/*
    Programmers : Alexander Williams, Shawn Grant, Abigail Christie, Che Hutchinson Serju, Jade Felicien-Brown, Kadeem Jonas
    Date        : 20/11/2018
    Purpose     : HLC, Inc shipping company customer management system
    File name   : main.c
*/


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>

#define SIZE 500
#define PACKNUM 20 //maximum number of packages
#define GCT 1.12 //GCT rate for each package
#define LEVY 1.1 //Environmental levy rate for any packages that exceeds the designated limit
#define LEVYLIMIT 10000 //Designated limit for the environmental limit
#define DESTS 5 //# of destinations
#define CTGS 4 //#of Shipping Categories

//COLORS according to windows.h's setConsoleTextAttribute function
#define RED 12
#define GREEN 10
#define YELLOW 14
#define BLUE 9
#define WHITE 15
#define CYAN 3
#define GRAY 7

typedef struct
{
    char name[25];
    char address[25];
    char isShipping;
    float weights [PACKNUM];
    int destinations [PACKNUM];
    float totalPrice;
    int packageCount;
}CUSTOMER;

void Intro();
void Authenticate(int *);
void UpdateLogin();
void GoodBye();
char Menu();
void AddCustomer();
void Calculations(float [], float *,int[], int);
void GetDestination(char[], int);
void Display (CUSTOMER[], int);
int search(CUSTOMER [], int);
void searchDisplay (CUSTOMER [], int);

void writeCustomer (CUSTOMER);
void readCustomer(CUSTOMER []);
void ReadDestinationRate(float []);
void ReadRates ();
void DisplayRates();
void UpdateRates();
void WriteRatesToFile (float [DESTS][CTGS]);
int CustomerCount();

int main()
{
    CUSTOMER cus[SIZE];
    int searchIndex;
    char choice;
    int totalCustomers, loggedIn;


    Intro();
    Authenticate(&loggedIn);

    totalCustomers=CustomerCount()-1; //updates total amount of customers to include previously entered customers stored in a file


    if(loggedIn == 1)
    {
        ReadRates ();
        choice = Menu();

        while(choice != 'F' && choice != 'f')
        {
            switch (choice)
            {
            case 'A':
            case 'a':
                AddCustomer();
                totalCustomers++;
                break;

            case 'B':
            case 'b':
                searchIndex = search(cus, totalCustomers);
                searchDisplay(cus,searchIndex);
                break;

            case 'C':
            case 'c':
                Display(cus,totalCustomers);
                break;

            case 'D':
            case 'd':
                DisplayRates();
                UpdateRates();
                break;

            case 'E':
            case 'e':
                UpdateLogin();
                break;

            default:
                printf("\nINVALID OPTION\n");
                printf("press any key to continue");
                getch();
                break;
            }
            choice = Menu();
        }
    }
    else
    {
        printf("\nCANNOT AUTHENTICATE USER");
        getch();
    }

    system("cls");
    GoodBye();

    return 0;
}


void ReadRates ()
{
    float rates[DESTS][CTGS];
    int i=0,j=0;
    FILE *fp;

    ///if a file with the rates does not already exist it will promt the user for all the rates.
    if((fp = fopen("HCLrates.rt", "rb")) == NULL)
    {
        system("cls");
        printf("\t  __________________________________________________________ \n");
        printf("\t |Before starting, you need to enter all the shipping rates | \n ");
        printf("\t |__________________________________________________________| \n");
        printf("\t  __________________________________________________________ \n\n");

        printf("\t Rates for the US:\n");
        printf("\t ---------------------\n");
        ReadDestinationRate(rates[0]);

        printf("\t \n---------------------\n\n");
        printf("\t Rates for the Canada:\n");
        printf("\t ---------------------\n");
        ReadDestinationRate(rates[1]);

        printf("\t \n---------------------\n\n");
        printf("\t Rates for the China:\n");
        printf("\t ---------------------\n");
        ReadDestinationRate(rates[2]);

        printf("\t \n---------------------\n\n");
        printf("\t Rates for the Europe:\n");
        printf("\t ---------------------\n");
        ReadDestinationRate(rates[3]);


        printf("\t \n---------------------\n\n");
        printf("\t Rates for the Africa:\n");
        printf("\t ---------------------\n");
        ReadDestinationRate(rates[4]);

        ///creates and writes all the rates to a file.
        if((fp=fopen("HCLrates.rt", "ab"))==NULL)
        {
            fprintf(stderr,"\n\nError Occured in opening file to store the rates");
        }
        else
        {
            //writes all the cateogories of rates for one destination at a time to file
            for (i=0;i<5;i++)
            {
                for (j=0;j<4;j++)
                {
                    fwrite(&rates[i][j],sizeof(float),1,fp);
                }
            }

            WriteRatesToFile(rates);
            fclose(fp);
        }

    }

}

void WriteRatesToFile (float rates[DESTS][CTGS])
{
    FILE *fp;
    int i,j;

    ///writes all the rates to a file.
        if((fp=fopen("HCLrates.rt", "wb"))==NULL)
        {
            fprintf(stderr,"\n\nError Occured in opening file to store the rates");

        }
        else
        {

            //writes all the cateogories of rates for one destination at a time to file
            for (i=0;i<5;i++)
            {
                for (j=0;j<4;j++)
                {
                    fwrite(&rates[i][j],sizeof(float),1,fp);
                }
            }
            fclose(fp);
        }

}

void ReadDestinationRate (float rates[])
{
    //reads all the categories of rates for a destination
        printf("\t 0-5 kg: ");
        scanf("%f", &rates[0]);
        while (isdigit(rates[0])==0)
        {
            printf("\n\tRate must be an integer");
            printf("\t 0-5 kg: ");
            scanf("%f", &rates[0]);
        }

        printf("\n\t 5<kg<10: ");
        scanf("%f", &rates[1]);
        while (isdigit(rates[1])==0)
        {
            printf("\n\tRate must be an integer");
            printf("\t 5<kg<10: ");
            scanf("%f", &rates[1]);
        }

        printf("\n\t 10<kg<20: ");
        scanf("%f", &rates[2]);
        while (isdigit(rates[2])==0)
        {
            printf("\n\tRate must be an integer");
            printf("\t 10<kg<20: ");
            scanf("%f", &rates[2]);
        }

        printf("\n\t 20<kg: ");
        scanf("%f", &rates[3]);
        while (isdigit(rates[3])==0)
        {
            printf("\n\tRate must be an integer");
            printf("\t 20<kg: ");
            scanf("%f", &rates[3]);
        }

        fflush(stdin);
}

void DisplayRates()
{
    float ratesTemp[DESTS][CTGS];
    FILE *fp;
    int i,j;

    system("cls");

    if((fp=fopen("HCLrates.rt", "rb"))==NULL)
    {
        fprintf(stderr,"\t\n\n_______________________________\n");
        fprintf(stderr,"\tRates file could not be opened\n");
        fprintf(stderr,"\t------------------------------\n");
    }
    else
    {
        for (i=0;i<5;i++)
        {
            for (j=0;j<4;j++)
            {
                fread(&ratesTemp[i][j],sizeof(float),1,fp);
            }
        }

        fclose(fp);

        printf("\t -------------------CURRENT RATES---------------------\n");
        printf("\t _____________________________________________________\n");
        printf("\t|Category|   US   | CANADA |  CHINA | EUROPE | AFRICA |\n");
        printf("\t _____________________________________________________\n");

        printf("\t| 0-5 kg |");
        for(i=0;i<5;i++)
        {
            printf("%-8.2f|",ratesTemp[i][0]);
        }

        printf("\n\t _____________________________________________________\n");
        printf("\t| 5<kg<10|");
        for(i=0;i<5;i++)
        {
            printf("%-8.2f|",ratesTemp[i][1]);
        }

        printf("\n\t _____________________________________________________\n");
        printf("\t|10<kg<20|");
        for(i=0;i<5;i++)
        {
            printf("%-8.2f|",ratesTemp[i][2]);
        }

        printf("\n\t _____________________________________________________\n");
        printf("\t| 20<kg  |");

        for(i=0; i < 5; i++)
        {
            printf("%-8.2f|",ratesTemp[i][3]);
        }

        printf("\n\t _____________________________________________________\n");
    }
}


void UpdateRates()
{
    char choice;
    int dest, ctg, i=0, j=0;
    float change, ratesTemp [5][4];
    FILE *fp;


    printf("\n\n\tWould you like to make any upates to the Rates? [Y or N] ");
    scanf("%c", &choice);
    fflush(stdin);


    /*while (choice!= 'Y' || choice!='N')
    {
        printf("\n\nChoice must be either 'Y' -for yes or 'N' -for no: ");
        scanf("%c", &choice);
        strupr(&choice);
        fflush(stdin);
    }*/

    while (choice== 'Y' || choice=='y')
    {
        system("cls");
        printf("\nWhich Destination would you like to update a rate for?\n");
        printf("| 1 - US\n| 2 - Canada\n| 3 - China\n| 4 - Europe\n| 5 - Africa\n");

        printf("Destination [1-5]: ");
        scanf("%i", &dest);
        while (dest> DESTS ||dest < 1 || isdigit(dest)==0)//ensure destination is between 1 and 5
        {
            printf("\n Destintion choice must be from 1-5");
            printf("Destination [1-5]: ");
            scanf("%i", &dest);
        }

        printf("\nWhich Category would you like to update a rate for?\n");
        printf("| 1 - 0-5Kg\n| 2 - 5<kg<10 \n| 3 - 10<kg<20\n| 4 - >20kg\n");

        printf("Destination [1-4]: ");
        scanf("%i", &ctg);
        while (ctg> CTGS ||ctg < 1 || isdigit(ctg)==0 )//ensure destination is between 1 and 5
        {
            printf("\n Destintion choice must be from 1-4");
            printf("Destination [1-4]: ");
            scanf("%i", &ctg);
        }

        printf("\n\n What would you like to change the rate to? ");
        scanf("%f", &change);
        while (isdigit(change)==0 || change<=0)
        {
            printf("Rate must be a NUMBER > 0");
            scanf("%f", &change);
        }
        fflush(stdin);

        if((fp=fopen("HCLrates.rt", "rb"))==NULL)
        {
            fprintf(stderr,"\n\nError Occured in Updating the rates in file.");

        }
        else
        {
            rewind(fp);

            for (i = 0; i < 5; i++)
            {
                for (j = 0; j < 4; j++)
                {
                    fread(&ratesTemp[i][j], sizeof(float), 1, fp);
                }
            }

            ratesTemp[dest-1][ctg-1]=change;
            WriteRatesToFile(ratesTemp);

            fclose(fp);
        }

        DisplayRates();
        printf("\n\n Would you like to make any other Updates? [Y or N]");
        scanf("%c", &choice);
        strupr(&choice);

        /*while(choice!= 'Y' || choice!='N')
        {
            printf("Choice must be either 'Y' -for yes or 'N' -for no");
            scanf("%c", &choice);
            strupr(&choice);
        }*/

    }

}

void Intro()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);//set text color

    printf("\t          WELCOME\n");
    printf("\t____________________________\n");
    printf("\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("\t@@  @@  @@  @@@@@@@       @@\n");
    printf("\t@@  @@  @@  @@@@@@@  @@@@@@@\n");
    printf("\t@@      @@  @@@@@@@  @@@@@@@\n");
    printf("\t@@  @@  @@  @@@@@@@  @@@@@@@\n");
    printf("\t@@  @@  @@      @@@       @@\n");
    printf("\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("\t     SHIPPING, Inc. \n");
    printf("\t____________________________\n");
    printf("\t____________________________\n");

    Sleep(2000);
    system("cls");
}

void GetPassword(char password[])
{
    char c;
    int i = 0;

    while((c = getch()) != '\r')//return or enter key
    {
        if(c == '\b')
        {
            password[i] = '0';
            i--;
            printf("\b \b");
        }else
        {
            password[i] = c;
            printf("*");
            i++;
        }
    }
    password[i] = '\0';
}

void Authenticate(int *loggedIn)
{
    char nameOnFile[25], passwordOnFile[25], uname[25], pwd[25];
    FILE *fpRead, *fpWrite;
    long size;

    if((fpRead = fopen("Login.hlc", "a+")) != NULL)
    {
        fseek(fpRead, 0, SEEK_END);
        size = ftell(fpRead);
        //check if to sign up or login
        if(size != 0)// something is on file
        {
            fseek(fpRead, 0, SEEK_SET);
            fscanf(fpRead, "%s %s", nameOnFile, passwordOnFile);

            //login

            printf("\tLOGIN \n");
            printf("\n________________________\n");
            printf("ENTER YOUR USERNAME: ");
            scanf("%s", uname);
            printf("ENTER YOUR PASSWORD: \n");
            GetPassword(pwd);
            printf("\n________________________\n");
            system("cls");

            while(strcmp(uname, nameOnFile) != 0 || strcmp(pwd, passwordOnFile) != 0)
            {
                printf("Incorrect Username or Password. Try Again");
                printf("\n________________________\n");
                printf("\tLOGIN \n");
                printf("\n________________________\n");
                printf("ENTER YOUR USERNAME: ");
                scanf("%s", uname);
                printf("ENTER YOUR PASSWORD: ");
                GetPassword(pwd);
                printf("\n________________________\n");
                system("cls");
            }
            while(strcmp(uname, nameOnFile) != 0 || strcmp(pwd, passwordOnFile) != 0);

            *loggedIn = 1;
        }
        else
        {
            //sign up
            do
            {
                printf("SIGN UP\n");

                printf("ENTER A USERNAME: \n");
                scanf("%s", uname);
                printf("ENTER A PASSWORD: \n");
                GetPassword(pwd);
                system("cls");
            }
            while(strcmp(uname, "") == 0 || strcmp(pwd, "") == 0);

            ///write the new user to file
            if((fpWrite = fopen("Login.hlc", "w")) != NULL)
            {
                fprintf(fpWrite, "%s %s", uname, pwd);
                *loggedIn = 1;//user logged in
                fclose(fpWrite);
            }else
            {
                *loggedIn = 0;//user not logged in
            }
        }
        fclose(fpRead);
    }
    else
    {
        *loggedIn = 0;//user not logged in
    }
}

void UpdateLogin()
{
    char uname[25], pwd[25];
    FILE *fp;

    system("cls");

    if((fp = fopen("Login.hlc", "w")) != NULL)
    {
        do
        {
            printf("CHANGE USERNAME AND PASSWORD\n");

            printf("ENTER NEW USERNAME: \n");
            scanf("%s", uname);
            printf("ENTER NEW PASSWORD: \n");
            GetPassword(pwd);
            system("cls");
        }
        while(strcmp(uname, "") == 0 || strcmp(pwd, "") == 0);

        fprintf(fp, "%s %s", uname, pwd);
        fclose(fp);
    }else
    {
        printf("\n CANNOT UPDATE AT THIS TIME...");
        getch();
    }
}

char Menu()
{
    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);//set text color

    printf("\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@    |\\ \n");
    printf("\t@@  @@  @@  @@@@@@@       @@    | \\ \n");
    printf("\t@@  @@  @@  @@@@@@@  @@@@@@@    |  |_____________________ \n");
    printf("\t@@      @@  @@@@@@@  @@@@@@@   _|____o o o o o o o o |___\\____  | \n");
    printf("\t@@  @@  @@  @@@@@@@  @@@@@@@  \\__________________|  HLC       |-O> \n");
    printf("\t@@  @@  @@      @@@       @@     \\____________________________/ | \n");
    printf("\t@@@@@@@@@@@@@@@@@@@@@@@@@@@@          OO OO           OO \n");
    printf("\t     SHIPPING, Inc. \n");
    printf("________________________________________________________________________\n");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);//set text color
    printf("\t\t __________________________________ \n");
    printf("\t\t||         CHOOSE AN OPTION       ||\n");
    printf("\t\t||================================||\n");
    printf("\t\t|| [A] Add a customer             ||\n");
    printf("\t\t|| [B] Search for a customer      ||\n");
    printf("\t\t|| [C] Display shipping info      ||\n");
    printf("\t\t|| [D] Display or Update Rates    ||\n");
    printf("\t\t|| [E] Update your Login Info     ||\n");
    printf("\t\t|| [F] Exit the program           ||\n");
    printf("\t\t||________________________________||\n");
    printf("\t\t   Option: ");

    return getch();
}

void AddCustomer()
{
    int numItems;
    int i;
    CUSTOMER temp;

    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);//set text color
    fflush(stdin);

    printf("                          _ \n");
    printf("                         | | \n");
    printf("          ADD        ____| |____  \n");
    printf("------NEW CUSTOMER--|____   ____|-----\n");
    printf("                         | | \n");
    printf("                         |_| \n");

    printf("NAME: ");
    gets(temp.name);
    strupr(temp.name);
    printf("ADDRESS: ");
    gets(temp.address);
    strupr(temp.address);
    fflush(stdin);

    printf("ARE YOU SHIPPING AN ITEM(s) [Y or N]: ");
    scanf("%c", &temp.isShipping);

    //THE CUSTOMMER IS SHIPPING AN ITEM(S)
    if(temp.isShipping == 'Y' || temp.isShipping == 'y')
    {
        system("cls");
        printf("HOW MANY ITEMS ARE YOU SHIPPING? : ");
        scanf("%i", &numItems);
        fflush(stdin);
        while(numItems<0 || isdigit(numItems)==0)
        {
            printf("\nNumber of Items must be an integr >0");
            printf("\nHOW MANY ITEMS ARE YOU SHIPPING? : ");
            scanf("%i", &numItems);
        }
        temp.packageCount = numItems;

        //read the items
        for(i = 0; i < numItems; i++)
        {
            printf("\n===============================\n");
            printf("       ITEM #%i\n\n", i+1);

            printf("WEIGHT (kg): ");
            scanf("%f", &temp.weights[i]);
            fflush(stdin);
            while(temp.weights[i] <= 0 || isdigit(temp.weights[i])==0)//ensure weight is greater than 0
            {
                printf("\n Weight must be greater than 0 Kg\n");
                printf("\nWEIGHT (kg): ");
                scanf("%f", &temp.weights[i]);
            }

            printf("\n----CHOOSE A DESTINATION----\n");
            printf("| 1 - US\n| 2 - Canada\n| 3 - China\n| 4 - Europe\n| 5 - Africa\n");

            printf("Destination [1-5]: ");
            scanf("%i", &temp.destinations[i]);
            fflush(stdin);
            while (temp.destinations[i] > 5 ||temp.destinations[i] < 1 || isdigit(temp.destinations[i])==0)//ensure destination is between 1 and 5
            {
                printf("\n Destintion choice must be from 1-5");
                printf("\nDestination [1-5]: ");
                scanf("%i", &temp.destinations[i]);
            }

            printf("\nPress any key to go on to next item...");
            getch();
            system("cls");
        }

        Calculations(temp.weights, &temp.totalPrice, temp.destinations, numItems);
    }

    writeCustomer(temp);

    //move to next customer index


    printf("\nPress any key to Return to Menu...");
    getch();


}

void writeCustomer(CUSTOMER temp)
{
    FILE *fp;
    if((fp = fopen("customerdata.cu", "ab"))==NULL) //open the file for writing to binary file
    {
        fprintf(stderr, "File could not be opened to store customer to File\n");
    }
    else
    {
        fwrite(&temp, sizeof(CUSTOMER),1,fp);
        fclose(fp);
    }
}


void readCustomer(CUSTOMER temp[])
{
    FILE *fp;
    int i=0;
    if((fp = fopen("customerdata.cu", "rb"))==NULL) //open the file for reading from binary file
    {
        fprintf(stderr, "File could not be opened\n");
    }
    else
    {
        while(!feof(fp))
        {
            fread(&temp[i], sizeof(CUSTOMER),1,fp);
            i++;
        }
        fclose(fp);

    }
}

int CustomerCount()
{
    CUSTOMER temp;
    FILE *fp;
    int i=0; //Counter for the Number of Customers
    if((fp = fopen("customerdata.cu", "rb"))==NULL) //open the file for reading from binary file
    {
        //fprintf(stderr, "File could not be opened\n");
    }
    else
    {
        while(!feof(fp))
        {
            fread(&temp, sizeof(CUSTOMER),1,fp);
            i++;
        }
        fclose(fp);
    }

    return i;
}
int search(CUSTOMER cus[], int noCustomers)
{
    char name[25];
    int i = 0;

    readCustomer(cus);

    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);//set text color

    printf("                       .-""-.   \n");
    printf("   SEARCH     _______//     \\  \n");
    printf("---  FOR     |_______        ; \n");
    printf("   CUSTOMER          \\    _//  \n");
    printf("                      '-..-'   \n");

    printf("\nENTER THE CUSTOMER NAME: ");
    fflush(stdin);
    gets(name);

    //make search case-insensitive
    while (strcmp (cus[i].name, strupr(name)) !=0)
    {
        i ++;
        // stops the loop if it has gone through all names currently entered in the system and not found a match and returns -1 as the value of the index.
        if (i > noCustomers)
        {
            i = -1;
            return i;
        }
    }

    return i;
}

void searchDisplay(CUSTOMER cus[], int i)
{
    int  j;
    char dest[10];

    system("cls");

    readCustomer(cus);

    ///if the index, i is -1 then that means the name searched for was not found and will not display any customer information
    if(i != -1)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);//GREEN FOR SUCCESS
        printf(" _________________________________________________________________\n");
        printf("|-----------------------------------------------------------------|\n");
        printf("|---                 ---  SEARCH RESULTS ---                   ---|\n");
        printf("|-----------------------------------------------------------------|\n");
        printf("| | NAME\t\t| WEIGHT(kg)\t| DESTINATION\t\t| |");
        printf("\n| |-------------------------------------------------------------| |");
        printf("\n| | %-20s|\t\t|\t\t\t| |\n", cus[i].name);

        ///go through the num of items for customer
        for(j = 0; j < cus[i].packageCount; j++)
        {
            GetDestination(dest, cus[i].destinations[j]);
            printf("| |\t\t\t| %-14.2f| %-22s| | \n", cus[i].weights[j], dest);
        }

        printf("| |-------------------------------------------------------------| |\n");
        printf("| | Total Cost for %-21s| $%-21.2f| |", cus[i].name, cus[i].totalPrice);
        printf("\n| |-------------------------------------------------------------| |\n");
        printf("|_|_____________________________________________________________|_|\n");
    }
    else
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);//RED FOR NOT FOUND
        printf("|-----------------------------------------------------------------|\n");
        printf("|---                 ---  SEARCH RESULTS  ---                  ---|\n");
        printf("|-----------------------------------------------------------------|\n");
        printf("|--                         NOT FOUND                           --|\n");
        printf("|_|_____________________________________________________________|_|\n");
    }

    printf("\n Press any key to Return to menu...");
    getch();
}

void Calculations(float packageWeights[], float *totalPrice, int dest[], int numItems)
{
    int r, ctg;
    float packagePrice = 0, rate;
    FILE *fp;



    //go through package by package
    for(r = 0; r < numItems; r++)
    {
        //determines shipping categories
        if(packageWeights[r] <= 5 && packageWeights[r] >= 0)
        {
            ctg=1;
        }
        else
        {
            if(packageWeights[r] <=10)
            {
                ctg=2;
            }
            else
            {
                if(packageWeights[r] <= 20)
                {
                    ctg=3;
                }
                else
                {
                    ctg=4;
                }
            }

        }

        //Finds the rate for that particular destination and Category
        if((fp=fopen("HCLrates.rt", "rb"))==NULL)
        {
            fprintf(stderr,"\n\nRates file could not be opened in order to carry out price calculation.");
        }
        else
        {

            rewind(fp);
                //sets pointer at the position of the needed rate
            fseek(fp, ((4*(dest[r]-1))+(ctg-1)) * sizeof(float), SEEK_SET);
            fread(&rate, sizeof(float),1, fp);
            fclose(fp);
        }

        packagePrice+= (rate * GCT);


        //calculate levy if any
        if (packagePrice > LEVYLIMIT)
        {
            packagePrice = LEVY * packagePrice;
        }
    }

    //at the end save the customers total charge to the array
    *totalPrice = packagePrice;
    //display the total shipping cost
    printf("=============================================\n");
    printf("===   YOUR TOTAL for %i ITEM(S) : $%.2f \n", numItems, packagePrice);
    printf("=============================================\n\n");
}

void Display(CUSTOMER cus[], int numCustomers)
{
    int i, j;
    char dest[10];

    readCustomer(cus);

    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);//set text color

    printf(" _________________________________________________________________\n");
    printf("|-----------------------------------------------------------------|\n");
    printf("|--              -- CUSTOMERS SHIPPING AN ITEM --               --|");


    for(i = 0; i < numCustomers; i++)
    {
        printf("\n|-----------------------------------------------------------------|");
        printf("\n| | NAME\t\t| WEIGHT(kg)\t| DESTINATION\t\t| |");

        //only display if an item is being shipped
        if (cus[i].isShipping == 'Y' || cus[i].isShipping == 'y')
        {
            printf("\n| |-------------------------------------------------------------| |");
            printf("\n| | %-20s|\t\t|\t\t\t| |\n", cus[i].name);

            //go through the num of items for customer
            for(j = 0; j < cus[i].packageCount; j++)
            {
                GetDestination(dest, cus[i].destinations[j]);
                printf("| |\t\t\t| %-14.2f| %-22s| | \n", cus[i].weights[j], dest);
            }

            printf("| |-------------------------------------------------------------| |\n");
            printf("| | ADDRESS             | %-38s| |\n", cus[i].address);
            printf("| |-------------------------------------------------------------| |\n");
            printf("| | TOTAL COST FOR %-21s| $%-21.2f| |", cus[i].name, cus[i].totalPrice);

            //change to gray for even numbered customers to clearly separate each customer
            if(i%2 == 0)
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GRAY);
            else
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
        }

    }

    printf("\n| |-------------------------------------------------------------| |\n");
    printf("| |-------------------------------------------------------------| |\n");
    printf("| | Total Customers\t\t\t| %i\t\t\t| |\n", numCustomers);
    printf("|_|_____________________________________________________________|_|\n");

    printf("\n Press any key to Return to menu...");
    getch();
}

///Translating the destination integer to its corresponding string, saved in 'dest'
void GetDestination(char dest[], int num)
{
    switch(num)
    {
    case 1:
        strcpy(dest, "US");
        break;
    case 2:
        strcpy(dest, "CANADA");
        break;
    case 3:
        strcpy(dest, "CHINA");
        break;
    case 4:
        strcpy(dest, "EUROPE");
        break;
    case 5:
        strcpy(dest, "AFRICA");
        break;
    }
}

void GoodBye()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);//set text color

    printf("\t   __                            __ \n");
    printf("\t  / /   #####  ##  ## ######     \\ \\ \n");
    printf("\t | |    ##  ## ##  ## ##         | |\n");
    printf("\t | | _  #####   ####  ######   _ | |\n");
    printf("\t | || | ##  ##   ##   ##      | || |\n");
    printf("\t |    | #####    ##   ######  |    |\n");
    printf("\t |    /                        \\   |\n");
    printf("\t |   |                          |  |\n");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);//set text color
    printf("___________________________________________________________\n");
    printf("\t_______                        ________ \n");
    printf("\t\\      |______________________|       / \n");
    printf("\t \\     |        THANKS        |      / \n");
    printf("\t /     |         FOR          |      \\ \n");
    printf("\t/______|   SHIPPING WITH HLC  |_______\\ \n");
    printf("\t       |______________________| \n");

    printf("\n Programmers:");
    printf("\n Alexander Williams, Shawn Grant, Abigail Christie\n Jade Felicien-Brown, Che Hutchinson Serju, Kadeem Jonas\n\n");
}
