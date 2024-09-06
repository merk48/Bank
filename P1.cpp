#include <iostream>
#include <string>
#include <vector>
#include<iomanip>
#include <fstream>
//#include <conio.h>  // For _getch() if you're using Windows

using namespace std;
void ShowMainMenueScreen();
short ReadOpNum();
void ShowClientsListScreen();
void ShowAddNewClientScreen();
void ShowDeleteClientScreen();
void ShowUpdateClientInfoScreen();
void ShowFindClientScreen();
void ShowProgramEndScreen();

const string ClientsFileName = "Clients.txt";

struct sClient {
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance = 0;
    bool MarkForDelete = false;
};
vector<string> SplitString(string S1, string Delim)
{
    vector<string> Tokens;

    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "") // to avoid if there more than one " "
            Tokens.push_back(sWord);

        S1.erase(0, pos + Delim.length()); // erase() until position and move to the next word
    }

    if (S1 != "")
        Tokens.push_back(S1);

    return Tokens;
}
sClient ConvertLineToRecord(string stLine, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(stLine, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]); // Cast string to double


    return Client;
}
void PrintClientRecord(sClient Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintClientRecordByAccountNumber(sClient Client)
{
    cout << "\nClient Data: \n";
    cout << "\n________________________________________________";
    cout << "\nAccount Number  : " << Client.AccountNumber;
    cout << "\nPin Code        : " << Client.PinCode;
    cout << "\nName            : " << Client.Name;
    cout << "\nPhone           : " << Client.Phone;
    cout << "\nAccount Balance : " << Client.AccountBalance;
    cout << "\n________________________________________________\n\n";

}
void PrintAllClientsData(vector<sClient> vClients) {
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (sClient Client : vClients) {
        PrintClientRecord(Client);
        cout << endl;
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}
vector<sClient> LoadClientsDataFromFile(string FileName)
{
    vector<sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();
    }
    return vClients;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient>& vClients, sClient& Client)
{

    for (sClient& C : vClients) 
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }

    return false;
}

sClient ReadNewClientByAccountNumber() {
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    string AccountNumber = "";
    sClient Client;

    //cout << "Enter Account Number? ";
    //// Usage of std::ws will extract all the whitespace character
    //getline(cin >> ws, Client.AccountNumber);

    do
    {
        cout << "Enter Account Number? ";
        // Usage of std::ws will extract all the whitespace character
        getline(cin >> ws, AccountNumber);

        if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        {
            cout << "Client is already exist, try again... \n\n";
        }
        else
        {
            Client.AccountNumber = AccountNumber;
            break;
        }

    } while (true);


    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance? ";
    cin >> Client.AccountBalance;

    return Client;
}
string ConvertRecordToLine(sClient Client, string Seperator = "#//#") {
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}
void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;

        MyFile.close();
    }

}
void AddNewClient()
{
    sClient Client;
    Client = ReadNewClientByAccountNumber();

    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}
void AddClients()
{
    char AddMore = 'Y';
    do {
        system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? [y/n] ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "Please Enter Account Number? ";
    cin >> AccountNumber;

    return AccountNumber;
}
vector<sClient>  SaveClientsDataToFile(string FileName, vector<sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out); // Overwrite

    string DataLine;

    if (MyFile.is_open()) {
        for (sClient& C : vClients) {
            if (C.MarkForDelete == false)
            {
                // only writer records that are not marked for delete.
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }
        }
        MyFile.close();
    }

    return vClients;
}
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{

    for (sClient& C : vClients) {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }
    return false;

}
bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientRecordByAccountNumber(Client);

        cout << "\n\nAre you sure you want to delete this client? y/n ? ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveClientsDataToFile(ClientsFileName, vClients);

            //Refress Clients
            vClients = LoadClientsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully!\n\n";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!\n\n";
        return false;
    }

}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    // Usage of std::ws will extract allthe whitespace character
    cout << "\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance? ";
    cin >> Client.AccountBalance;

    return Client;
}
bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientRecordByAccountNumber(Client);

        cout << "\n\nAre you sure you want to Update this client? y/n ? ";
        cin >> Answer;

        if (toupper(Answer) == 'Y')
        {
            for (sClient& C : vClients) {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveClientsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully!\n\n";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!\n\n";
        return false;
    }

}


int main() {

    short OpNum;
    bool Exit = false;

    do
    {
        ShowMainMenueScreen();
        OpNum = ReadOpNum();

        switch (OpNum)
        {
        case 1:
        {
            ShowClientsListScreen();
            break;
        }
        case 2:
        {
            ShowAddNewClientScreen();
            break;
        }
        case 3:
        {
            ShowDeleteClientScreen();
            break;

        }
        case 4:
        {
            ShowUpdateClientInfoScreen();
            break;

        }
        case 5:
        {
            ShowFindClientScreen();
            break;

        }
        case 6:
        {
            ShowProgramEndScreen();
            Exit = true;
            break;

        }
        default:
            ShowMainMenueScreen();
            break;
        }
    } while (!Exit);
    
    return 0;
}

void ShowMainMenueScreen()
{
    system("cls");
    cout << "================================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "================================================\n";
    cout << "\t[1] Show Clients List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Exist.\n";
    cout << "================================================\n";

}

short ReadOpNum()
{
    short OpNum = 0;
    cout << "Choose What do you want to do ? [1 to 6]?";
    cin >> OpNum;

    return OpNum;
}

void ShowClientsListScreen() {
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

    system("cls");
    PrintAllClientsData(vClients);

    system("pause");
}

void ShowAddNewClientScreen()
{
    system("cls");
    cout << "--------------------------------------------\n";
    cout << "\n\t\t\tAdd New Clients Screen\n\n";
    cout << "--------------------------------------------\n";

    AddClients();

    system("pause");
}

void ShowDeleteClientScreen()
{
    system("cls");
    cout << "--------------------------------------------\n";
    cout << "\n\t\t\tDelete Client Screen\n\n";
    cout << "--------------------------------------------\n";

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    DeleteClientByAccountNumber(AccountNumber, vClients);
    system("pause");

}

void ShowUpdateClientInfoScreen()
{
    system("cls");
    cout << "--------------------------------------------\n";
    cout << "\n\t\t\tUpdate Client Info Screen\n\n";
    cout << "--------------------------------------------\n";

    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    UpdateClientByAccountNumber(AccountNumber, vClients);

    system("pause");

}

void ShowFindClientScreen()
{
    system("cls");
    cout << "--------------------------------------------\n";
    cout << "\n\t\t\tFind Client Screen\n\n";
    cout << "--------------------------------------------\n";

    sClient Client;
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        PrintClientRecordByAccountNumber(Client);
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!\n\n";
    }

    system("pause");


}

void ShowProgramEndScreen() 
{
    system("cls");
    cout << "--------------------------------------------\n";
    cout << "\n\t\t\tProgram Ends :-) \n\n";
    cout << "--------------------------------------------\n";

    system("pause");
}


/*
A150#//#1234#//#Mohammed Abu-Hadhoud#//#07333232#//#1000.000000
A151#//#1234#//#Ali Fadi#//#07555552#//#2000.000000
A152#//#1234#//#Maher Ahmed#//#666666#//#5000.000000
A153#//#1234#//#Omar Mohammed#//#77777#//#9500.000000
A154#//#1234#//#Ali Maher#//#5555882#//#544.000000
*/
