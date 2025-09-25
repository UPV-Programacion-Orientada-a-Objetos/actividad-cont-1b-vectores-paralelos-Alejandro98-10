#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

const int MAX_PRODUCTOS = 100;
int totalProductos = 0;

int codigos[MAX_PRODUCTOS];
string nombres[MAX_PRODUCTOS];
int stock[MAX_PRODUCTOS];
float precios[MAX_PRODUCTOS];
string ubicaciones[MAX_PRODUCTOS];

void cargarDesdeArchivo() {
    ifstream archivo("inventario.txt");
    if (!archivo) {
        cout << "No se encontro inventario.txt. Se iniciara con inventario vacio.\n";
        return;
    }

    string linea;
    while (getline(archivo, linea) && totalProductos < MAX_PRODUCTOS) {
        stringstream ss(linea);
        string campo;

        getline(ss, campo, ',');
        codigos[totalProductos] = stoi(campo);

        getline(ss, nombres[totalProductos], ',');

        getline(ss, campo, ',');
        stock[totalProductos] = stoi(campo);

        getline(ss, campo, ',');
        precios[totalProductos] = stof(campo);

        getline(ss, ubicaciones[totalProductos], ',');

        totalProductos++;
    }
    archivo.close();
    cout << "Datos cargados desde inventario.txt (" << totalProductos << " productos)\n";
}

void guardarEnArchivo() {
    ofstream archivo("inventario.txt");
    if (!archivo) {
        cout << "Error al abrir inventario.txt para guardar.\n";
        return;
    }

    for (int i = 0; i < totalProductos; i++) {
        archivo << codigos[i] << ","
                << nombres[i] << ","
                << stock[i] << ","
                << precios[i] << ","
                << ubicaciones[i] << "\n";
    }

    archivo.close();
    cout << "Datos guardados en inventario.txt\n";
}

int buscarPorCodigo(int codigo) {
    for (int i = 0; i < totalProductos; i++) {
        if (codigos[i] == codigo) return i;
    }
    return -1;
}

int buscarPorUbicacion(string ubicacion) {
    for (int i = 0; i < totalProductos; i++) {
        if (ubicaciones[i] == ubicacion) return i;
    }
    return -1;
}

void registrarProducto() {
    if (totalProductos >= MAX_PRODUCTOS) {
        cout << "No se pueden registrar mas productos.\n";
        return;
    }

    int codigo;
    cout << "Ingrese codigo del producto: ";
    cin >> codigo;

    if (buscarPorCodigo(codigo) != -1) {
        cout << "Error: El codigo ya existe.\n";
        return;
    }

    cin.ignore();
    cout << "Ingrese nombre: ";
    getline(cin, nombres[totalProductos]);

    cout << "Ingrese cantidad en stock: ";
    cin >> stock[totalProductos];

    cout << "Ingrese precio unitario: ";
    cin >> precios[totalProductos];

    cin.ignore();
    cout << "Ingrese ubicacion en almacen: ";
    getline(cin, ubicaciones[totalProductos]);

    codigos[totalProductos] = codigo;
    totalProductos++;

    cout << "Producto registrado exitosamente.\n";
}

void actualizarPorUbicacion() {
    string ubicacion;
    cin.ignore();
    cout << "Ingrese ubicacion del producto: ";
    getline(cin, ubicacion);

    int pos = buscarPorUbicacion(ubicacion);
    if (pos == -1) {
        cout << "Error: Ubicacion no encontrada.\n";
    } else {
        int cantidad;
        cout << "Ingrese cantidad a modificar (positiva o negativa): ";
        cin >> cantidad;

        if (stock[pos] + cantidad < 0) {
            cout << "Error: No puede quedar stock negativo.\n";
        } else {
            stock[pos] += cantidad;
            cout << "Stock actualizado. Nuevo stock: " << stock[pos] << endl;
        }
    }
}

void reporteBajoStock(int umbral = 10) {
    cout << "\nReporte de Bajo Stock (menor a " << umbral << "):\n";
    cout << "---------------------------------------------\n";
    cout << "Codigo\tNombre\t\tStock\tPrecio\tUbicacion\n";
    cout << "---------------------------------------------\n";
    for (int i = 0; i < totalProductos; i++) {
        if (stock[i] < umbral) {
            cout << codigos[i] << "\t" 
                 << nombres[i] << "\t\t" 
                 << stock[i] << "\t" 
                 << "$" << precios[i] << "\t" 
                 << ubicaciones[i] << endl;
        }
    }
    cout << "---------------------------------------------\n";
}

void productoMasBarato() {
    if (totalProductos == 0) {
        cout << "No hay productos cargados.\n";
        return;
    }

    int posMin = 0;
    for (int i = 1; i < totalProductos; i++) {
        if (precios[i] < precios[posMin]) {
            posMin = i;
        }
    }
    cout << "\nProducto mas barato:\n";
    cout << "Nombre: " << nombres[posMin] << endl;
    cout << "Precio: $" << precios[posMin] << endl;
}

void generarReporte() {
    cout << "\nReporte de Inventario:\n";
    cout << "-------------------------------------------------\n";
    cout << "Codigo\tNombre\t\tStock\tPrecio\tUbicacion\n";
    cout << "-------------------------------------------------\n";
    for (int i = 0; i < totalProductos; i++) {
        cout << codigos[i] << "\t" 
             << nombres[i] << "\t\t" 
             << stock[i] << "\t" 
             << "$" << precios[i] << "\t" 
             << ubicaciones[i] << endl;
    }
    cout << "-------------------------------------------------\n";
}

void mostrarMenu() {
    int opcion;
    do {
        cout << "\n=== Sistema de Inventario - Ferreteria 'El Martillo' ===\n";
        cout << "1. Registrar nuevo producto\n";
        cout << "2. Actualizar inventario por ubicacion\n";
        cout << "3. Generar reporte general\n";
        cout << "4. Generar reporte de bajo stock\n";
        cout << "5. Buscar producto mas barato\n";
        cout << "0. Salir y guardar\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: registrarProducto(); break;
            case 2: actualizarPorUbicacion(); break;
            case 3: generarReporte(); break;
            case 4: reporteBajoStock(); break;
            case 5: productoMasBarato(); break;
            case 0: guardarEnArchivo(); cout << "👋 Saliendo...\n"; break;
            default: cout << "❌ Opcion invalida.\n";
        }
    } while (opcion != 0);
}

int main() {
    cargarDesdeArchivo();
    mostrarMenu();
    return 0;
}

