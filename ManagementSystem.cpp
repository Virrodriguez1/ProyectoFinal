#include <iostream>
#include <fstream>
#include <sstream>

#include "ManagementSystem.h"
#include "ClientStatus.h"

//#define FILENAME_CLIENTS "../clientes.txt"
#define FILENAME_CLIENTS "../clientes2.txt"
#define FILENAME_TRANSACTIONS "../transacciones.txt"

void saveClientToFile(const Client &client, string filename);

using namespace std;

// Asume que MAX_TRANSACTIONS y MAX_CLIENTS son constantes que definen el número máximo de transacciones y clientes

/*
 * Transactions
 */

/*Transaction* ManagementSystem::loadTransactionsFromFile(const std::string& filename, int& transactionCount) {
    transactionCount = 0;
    ifstream file(filename);
    if (!file) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        cerr << "Se retornará un arreglo vacío de transacciones." << endl;
        cerr << "Verifica que el archivo exista y que tenga el formato correcto." << endl;
        cerr << "El formato correcto es: " << endl;
        cerr << "Número de transacción, monto, tipo, día, mes, año" << endl;
        cerr << "Ejemplo: 1 1000 D 1 1 2020" << endl;
        return transactions;
    }

    string line;
    while (getline(file, line)) {
        int transactionNumber, amount, day, month, year;
        char type;

        istringstream iss(line);
        if (!(iss >> transactionNumber >> amount >> type >> day >> month >> year)) {
            break; // Error en el formato del archivo
        }

        transactions[transactionCount].setTransactionNumber(transactionNumber);
        transactions[transactionCount].setAmount(amount);
        transactions[transactionCount].setType(type);
        transactions[transactionCount].setDay(day);
        transactions[transactionCount].setMonth(month);
        transactions[transactionCount].setYear(year);

        transactionCount++;
    }
    //file.close();
    return transactions;
}*/

Transaction* ManagementSystem::loadTransactionsFromFile(const string &filename, int &transactionCount) {
    ifstream file(filename);
    if (!file) {
        cerr << "No se pudo abrir el archivo de clientes: " << filename << endl;
        return nullptr;
    }
    string line;
    while (getline(file, line)) { //iss(line) crea un objeto istringstream llamado iss que contiene el contenido de la cadena line
        istringstream iss(line); // Cada línea se descompone en varios elementos usando un flujo de entrada de cadena
        int id, amount,day,month,year;
        char type;
        if (!(iss >> id >> amount >> type >> day >> month >> year)) {
           // cerr << "Error al leer la línea de clientes: " << line << endl;
            continue;
        }
        if (transactionCount >= MAX_TRANSACTIONS) {
            cerr << "Se ha alcanzado el número máximo de clientes. Ignorando los clientes adicionales." << endl;
            break;
        }

        transactions[transactionCount] = Transaction(id,amount,type,day,month,year);
        transactionCount++;
    }
    return transactions;
}

void ManagementSystem::saveTransactionsToFile(const Transaction *transactions, int transactionCount, const string &filename) {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        return;
    }
    for (int i = 0; i < transactionCount; ++i) {
        file << transactions[i].getTransactionNumber() << "\t"
             << transactions[i].getAmount() << "\t"
             << transactions[i].getType() << "\t"
             << transactions[i].getDay() << "\t"
             << transactions[i].getMonth() << "\t"
             << transactions[i].getYear() << "\n";
    }
    file.close();
}

void ManagementSystem::saveTransactionToFile(const Transaction &transaction) {
    ofstream file(FILENAME_TRANSACTIONS, ios::app);
    if (!file) {
        cerr << "No se pudo abrir el archivo: " << FILENAME_TRANSACTIONS << endl;
        return;
    }
    file << transaction.getTransactionNumber() << "\t"
         << transaction.getAmount() << "\t"
         << transaction.getType() << "\t"
         << transaction.getDay() << "\t"
         << transaction.getMonth() << "\t"
         << transaction.getYear() << "\n";
    file.close();

}
void ManagementSystem::addTransaction(const Transaction &transaction){
    saveTransactionToFile(transaction);
}
/*
 * CLIENTS
 */

void ManagementSystem::addClient(const Client &client){
    saveClientToFile(client);
}

Client ManagementSystem::removeClient(int clientNumber) {
    //1. OBTENER TODOS LOS CLIENTES DEL ARCHIVO
    //2. OBTENER EL CLIENTE QUE QUIERO MODIFICAR DENTRO DE ESA LISTA DE CLIENTES
    //3.MODIFICAR EL STATUS DE CLIENTE A BAJA
    // 3.1 ASEGURAR QUE LA MODIFICACION QUEDO ADENTRO DEL ARRAY CREADO EN EL PUNTO 1
    // 4. EJECUTAR OVERWRITECLIENTSTOFILE
    // 4.1 ENVIAR POR PARAMETRO LA LISTA DE CLIENTE YA MODIFICADA
    Client client = getClientByNumber(clientNumber);
    client.setStatus("baja");
    return client;
}

Client * ManagementSystem::getAllClients(int& clientCount) {
    return loadClientsFromFile(FILENAME_CLIENTS, clientCount);
}


void ManagementSystem::saveClientToFile(const Client &client) {
    ofstream file(FILENAME_CLIENTS, ios::app);
    if (!file) {
        cerr << "No se pudo abrir el archivo: " << FILENAME_CLIENTS << endl;
        return;
    }
    file << client.getClientNumber()  << "\t"
         << client.getName()  << "\t"
         << client.getLastName()  << "\t"
         << client.getAccountType()  << "\t"
         << client.getYear()  << "\t"
         << client.getStatus() <<  "\n";
    file.close();
}

Client* ManagementSystem::loadClientsFromFile(const string& filename, int& clientCount) {
    ifstream file(filename);
    if (!file) {
        cerr << "No se pudo abrir el archivo de clientes: " << filename << endl;
        return nullptr;
    }
    string line;
    while (getline(file, line)) { //iss(line) crea un objeto istringstream llamado iss que contiene el contenido de la cadena line
        istringstream iss(line); // Cada línea se descompone en varios elementos usando un flujo de entrada de cadena
        int id, year;
        string name, surname, type, status;
        if (!(iss >> id >> name >> surname >> type >> year >> status)) {
       //     cerr << "Error al leer la línea de clientes: " << line << endl;
            continue;
        }
        if (clientCount >= MAX_CLIENTS) {
            cerr << "Se ha alcanzado el número máximo de clientes. Ignorando los clientes adicionales." << endl;
            break;
        }

        clients[clientCount] = Client(id, name, surname, type, year, status); // regcl
        clientCount++;
    }
    return clients;
}

Client ManagementSystem::getClientByNumber(int clientNumber) {
    Client* loadedClients = loadClientsFromFile(FILENAME_CLIENTS,clientCount);
    for (int i = 0; i < clientCount; ++i) {
        if (loadedClients[i].getClientNumber() == clientNumber) {
            return loadedClients[i];
        }
    }
    cerr << "No se encontró el cliente con número: " << clientNumber << endl;

    return Client();
}

//Client ManagementSystem::overwriteClientsToFile(Client *clients, int clientCount){}

/*
 * SHOWING METHODS
 */

void ManagementSystem::showTransaction(Transaction transaction) {
    cout << "# Transaction number:  " << transaction.getTransactionNumber() << "\t#"<<endl;
    cout << "# Amount: " << transaction.getAmount() << "\t\t\t#"<<endl;
    cout << "# Type: " << transaction.getType() << "\t\t\t#"<<endl;
    cout << "# Day: " << transaction.getDay() << "\t\t\t#"<<endl;
    cout << "# Month: " << transaction.getMonth() << "\t\t\t#"<<endl;
    cout << "# Year: " << transaction.getYear() << "\t\t\t#"<<endl;
}

void ManagementSystem::showAllTransactions(Transaction *transactions, int transactionCount) {
    for (int i = 0; i < transactionCount; i++) {
        cout << "#################################" << endl;
        showTransaction(transactions[i]);
        cout << "#################################" << endl << endl;
    }
}

void ManagementSystem::showClient(Client client) {
    cout << "#################################" << endl;
    cout << "# Client Number:  " << client.getClientNumber() << "\t\t#"<<endl;
    cout << "# Nombre: " << client.getName() << "\t\t\t#"<<endl;
    cout << "# Apellido: " << client.getLastName() << "\t\t#"<<endl;
    cout << "# Tipo: " << client.getAccountType() << "\t\t\t#"<<endl;
    cout << "# Ano: " << client.getYear() << "\t\t\t#"<<endl;
    cout << "# Estado:\t" << client.getStatus() << "\t\t#"<<endl;
    cout << "#################################" << endl << endl;
}

void ManagementSystem::showAllClients(Client *clients, int clientCount, ClientStatus option) {
    for (int i = 0; i < clientCount ; i++) {
        switch (option) {
            case ACTIVE:
                if (clients[i].getStatus() == "activo")
                    showClient(clients[i]);
                break;
            case INACTIVE:
                if (clients[i].getStatus() == "baja")
                    showClient(clients[i]);
                break;
            default:
                showClient(clients[i]);
        }
    }
}

const Transaction *ManagementSystem::getTransactions() const {
    return transactions;
}

const Client *ManagementSystem::getClients() const {
    return clients;
}

int ManagementSystem::getClientCount() const {
    return clientCount;
}

void ManagementSystem::setClientCount(int clientCount) {
    ManagementSystem::clientCount = clientCount;
}

int ManagementSystem::getTransactionCount() const {
    return transactionCount;
}

void ManagementSystem::setTransactionCount(int transactionCount) {
    ManagementSystem::transactionCount = transactionCount;
}
