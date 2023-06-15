#include <iostream>
#include <string>

#include <windows.h>

#include "ManagementSystem.h"
#include "Transaction.h"
#include "Client.h"
#include "ClientStatus.h"

bool isValidClient(Client client, ManagementSystem &ms);

using namespace std;

//*********************************************
// Tests para probar las funciones de ManagementSystem
void testLoadTransactionFromFile(ManagementSystem ms) {
    int transactionCount = 0;
    static Transaction transactions[MAX_TRANSACTIONS];
    Transaction* loadedTransactions = ms.loadTransactionsFromFile("../transacciones.txt", transactionCount);

    if (loadedTransactions != nullptr) {
        for (int i = 0; i < transactionCount; i++) {
            transactions[i] = loadedTransactions[i];
        }
        ms.showAllTransactions(transactions, transactionCount);
    }
}


void testLoadClientsFromFile(ManagementSystem ms) {
    int clientCount = 0;
    static Client clients[MAX_CLIENTS];
    Client* loadedClients = ms.loadClientsFromFile("../clientes2.txt",clientCount);

    if (loadedClients != nullptr){
        for (int i=0 ; i < clientCount ; i++){
            clients[i] = loadedClients[i];
        }
        ms.showAllClients(clients, clientCount , ALL);
    }
}

//*********************************************


void showAllClients(ManagementSystem& ms,ClientStatus option){
    int clientCount = 0;
    static Client clients[MAX_CLIENTS];
    Client* loadedClients = ms.getAllClients( clientCount);

    if (loadedClients != nullptr){
        for (int i=0 ; i < clientCount ; i++){
            clients[i] = loadedClients[i];
        }
        ms.showAllClients(clients, clientCount, option);
    }
}

void clearScreen() {
    //cout << "\x1B[2J\x1B[H";
}

void addClientMenu(ManagementSystem& ms) {
    // toma una referencia a un objeto ManagementSystem llamado ms.
    // La referencia permite que la función modifique el objeto ManagementSystem original que se pasa como argumento.
    cout << "Ejecutando: Agregar cliente..." << endl;
    // Crear un dialogo en consola para ingresar los datos necesarios para el cliente
    cout << "Ingrese el número de cliente: ";
    int clientNumber;
    cin >> clientNumber;
    cout << "Ingrese el nombre del cliente: ";
    string name;
    cin >> name;
    cout << "Ingrese el apellido del cliente: ";
    string lastName;
    cin >> lastName;
    cout << "Ingrese el tipo de cuenta del cliente: ";
    string accountType;
    cin >> accountType;
    cout << "Ingrese el año de apertura de la cuenta: ";
    int year;
    cin >> year;

    Client client = Client(clientNumber, name, lastName, accountType, year);

    if (isValidClient(client, ms))
        ms.addClient(client);
}

bool isValidClient(Client client, ManagementSystem &ms) {
    // para que sea valido el cliente debe cumplir con esto:
    // - Los clientes con una antigüedad menor a 3 años no pueden ser de tipo “black”
    // - Los clientes de tipo “oro” o más tienen acceso a una tarjeta de crédito, con un límite
    //   mensual de $50000 para los de tipo “oro”, y de $250000 para los de tipo “black”.

    if(client.getAccountType() == "black" && (2023 - client.getYear()) < 3){
        cout << "El cliente no puede ser de tipo black con menos de 3 años de antiguedad" << endl;
        return false;
    }

    if(client.getAccountType() == "oro" || client.getAccountType() == "black"){
        cout << "El cliente tiene acceso a una tarjeta de credito" << endl;
        if(client.getAccountType() == "oro"){
            cout << "El limite mensual de la tarjeta de credito es de $50.000" << endl;
            return true;
        }
        if(client.getAccountType() == "black"){
            cout << "El limite mensual de la tarjeta de credito es de $250.000" << endl;
            return true;
        }
    }
    // si no se cumple ninguna de las condiciones anteriores, el cliente es valido (Cliente plata entra por aca)
    return true;
}

void modifyData(ManagementSystem& system) {
    int clientNumber;
    cout << "Ingrese el numero a eliminar"<< endl ;
    cin >> clientNumber;
    Client client;
    client = system.removeClient(clientNumber);
    system.saveClientToFile(client);

    // Aquí iría el código para modificar los datos de un cliente.
}



void consultClientByNumber(ManagementSystem& system) {
    cout << "Ejecutando: Consultar cliente por número de cliente..." << endl;
    // Aquí iría el código para consultar un cliente por su número.
    cout<< "Ingrese el numero de cliente: ";
    int clientNumber;
    cin >> clientNumber;
    system.showClient(system.getClientByNumber(clientNumber));
}



// Esta función maneja el submenú para las consultas.
void addTransaction (ManagementSystem& ms){
    cout << "Ingrese el número de Transaccion: ";
    int transactionNumber;
    cin >> transactionNumber;
    cout << "Ingrese el monto: ";
    int amount;
    cin >> amount;
    cout << "Ingrese el tipo ";
    char type;
    cin >> type;
    cout << "Ingrese el dia ";
    int day;
    cin >> day;
    cout << "Ingrese el mes";
    int month;
    cin >> month;
    cout << "Ingrese el ano ";
    int year;
    cin >> year;

    Transaction transaction = Transaction( transactionNumber,amount,type,day,month,year);
    ms.addTransaction (transaction);
}
void consultations(ManagementSystem& system) {
    int option;
    // clearScreen();
    do {
        cout << "1. Consultas \t> \"Cliente\" por número de cliente" << endl;
        cout << "2. Consultas \t> Todos los clientes" << endl;
        cout << "3. Consultas  \t > Clientes Activos" << endl;
        cout << "5. Consultas \t > Clientes Inactivos "<< endl;
        cout << "6.\t\t< Volver al menú anterior" << endl;
        cin >> option;

        switch (option) {
            case 1:
                consultClientByNumber(system);
                break;
            case 2:
                showAllClients(system,ALL);
                break;
            case 3:
                showAllClients(system,ACTIVE);
                break;
            case 4 :
                showAllClients(system,INACTIVE);
                break;
            case 5:
                cout << "Volviendo al menú principal..." << endl;
                break;
            default:
                cout << "Opción incorrecta" << endl;
                break;
        }
        clearScreen();
    } while (option != 6);
}

void debugMenu(ManagementSystem& system){
    int option;
    clearScreen();
    do {
        cout << "1. Imprimir todas las Transacciones" << endl;
        cout << "2. Imprimir todos los Clientes" << endl;
        cin >> option;

        switch (option) {
            case 1:
                testLoadTransactionFromFile(system);
                break;
            case 2:
                testLoadClientsFromFile(system);
                break;
            default:
                cout << "Opción incorrecta" << endl;
                break;
        }
        clearScreen();
    } while (option != 5);
}

// Esta es la función principal que maneja el menú principal.
int main() {
    int option;
    ManagementSystem system;

    do{
        cout << "1. Agregar Cliente" << endl;
        cout << "2. Dar de Baja " << endl;
        cout << "3. Consultas" << endl;
        cout << "4. Menu Debug" << endl; // Debug menu -- imprime
        cout << "5. Agregar Transaccion" << endl;
        cout << "6. Salir" << endl;
        cin >> option;

        switch (option) {
            case 1:
                addClientMenu(system);
                break;
            case 2:
                modifyData(system);
                break;
            case 3:
                consultations(system);
                break;
            case 6:
                cout << "Exiting..." << endl;
                break;
            case 4: // Debug menu -- Imprimir
                debugMenu(system);
                break;
            case 5:
                addTransaction(system);
            default:
                cout << "Incorrect option" << endl;
                break;
        }
        clearScreen();
    } while (option != 6);

    return 0;
}