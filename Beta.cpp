#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Estacion {
private:
    string nombre;
    bool esTransferencia;

public:
    Estacion(string _nombre, bool _esTransferencia) 
        : nombre(_nombre), esTransferencia(_esTransferencia) {}

    string getNombre() const { return nombre; }
    bool esEstacionTransferencia() const { return esTransferencia; }

    string toString() const {
        return nombre;
    }
};

class Linea {
private:
    string nombre;
    vector<Estacion*> estaciones;

public:
    Linea(string _nombre) : nombre(_nombre) {}

    void agregarEstacion(Estacion* estacion) {
        estaciones.push_back(estacion);
    }

    bool tieneEstacion(const string& nombreEstacion) const {
        return find_if(estaciones.begin(), estaciones.end(), [&](Estacion* estacion) { return estacion->getNombre() == nombreEstacion; }) != estaciones.end();
    }

    string getNombre() const { return nombre; }

    const vector<Estacion*>& obtenerEstaciones() const {
        return estaciones;
    }

    vector<Estacion*>& obtenerEstaciones() {
        return estaciones;
    }

    string toString() const {
        stringstream ss;
        ss << nombre << endl;
        for (const auto& estacion : estaciones) {
            ss << estacion->toString() << endl;
        }
        return ss.str();
    }
};

class RedMetro {
private:
    vector<Linea*> lineas;

public:
    ~RedMetro() {
        for (auto& linea : lineas) {
            for (auto& estacion : linea->obtenerEstaciones()) {
                delete estacion;
            }
            delete linea;
        }
    }

    void agregarLinea(Linea* linea) {
        lineas.push_back(linea);
        guardarDatos();
    }

    int cantidadLineas() const {
        return lineas.size();
    }

    int cantidadEstaciones() const {
        int totalEstaciones = 0;
        for (const auto& linea : lineas) {
            totalEstaciones += linea->obtenerEstaciones().size();
        }
        return totalEstaciones;
    }

    bool eliminarLinea(const string& nombreLinea) {
        auto it = find_if(lineas.begin(), lineas.end(), [&](Linea* l) { return l->getNombre() == nombreLinea; });
        if (it != lineas.end()) {
            if (!(*it)->obtenerEstaciones().empty()) {
                return false; 
            }
            delete *it;
            lineas.erase(it);
            guardarDatos();
            return true;
        }
        return false; 
    }

    void agregarEstacion(const string& nombreLinea, Estacion* estacion) {
        auto it = find_if(lineas.begin(), lineas.end(), [&](Linea* l) { return l->getNombre() == nombreLinea; });
        if (it != lineas.end()) {
            if (!(*it)->tieneEstacion(estacion->getNombre())) {
                (*it)->agregarEstacion(estacion);
                guardarDatos();
            }
        }
    }

    void eliminarEstacion(const string& nombreLinea, const string& nombreEstacion) {
        auto it = find_if(lineas.begin(), lineas.end(), [&](Linea* l) { return l->getNombre() == nombreLinea; });
        if (it != lineas.end()) {
            auto& estaciones = (*it)->obtenerEstaciones();
            auto itEstacion = find_if(estaciones.begin(), estaciones.end(), [&](Estacion* est) { return est->getNombre() == nombreEstacion; });
            if (itEstacion != estaciones.end()) {
                delete *itEstacion;
                estaciones.erase(itEstacion);
                guardarDatos();
            }
        }
    }

    bool perteneceEstacion(const string& nombreLinea, const string& nombreEstacion) const {
        for (const auto& linea : lineas) {
            if (linea->getNombre() == nombreLinea) {
                return linea->tieneEstacion(nombreEstacion);
            }
        }
        return false;
    }

    void simularTiempoDeLlegada(const string& estacionSalida, const string& estacionLlegada, const string& nombreLinea) const {
        auto it = find_if(lineas.begin(), lineas.end(), [&](Linea* l) { return l->getNombre() == nombreLinea; });
        if (it != lineas.end()) {
            const auto& estaciones = (*it)->obtenerEstaciones();
            auto itSalida = find_if(estaciones.begin(), estaciones.end(), [&](Estacion* est) { return est->getNombre() == estacionSalida; });
            auto itLlegada = find_if(estaciones.begin(), estaciones.end(), [&](Estacion* est) { return est->getNombre() == estacionLlegada; });
            if (itSalida != estaciones.end() && itLlegada != estaciones.end()) {
                if (itSalida < itLlegada) {
                    int tiempoTotal = 0;
                    for (auto it = itSalida; it != itLlegada; ++it) {
                        tiempoTotal += 5; 
                    }
                    cout << "El tiempo estimado de llegada es de " << tiempoTotal << " minutos." << endl;
                } else {
                    cout << "La estación de llegada debe ser posterior a la estación de salida en la línea." << endl;
                }
            } else {
                cout << "Al menos una de las estaciones ingresadas no existe en la línea." << endl;
            }
        } else {
            cout << "La línea ingresada no existe." << endl;
        }
    }

    void guardarDatos() const {
        ofstream archivo("datosred.txt");
        if (!archivo.is_open()) {
            cerr << "Error al abrir el archivo de datos." << endl;
            return;
        }

        for (const auto& linea : lineas) {
            archivo << linea->toString();
        }

        archivo.close();
    }

    void cargarDatos() {
        ifstream archivo("datosred.txt");
        if (!archivo.is_open()) {
            cout << "No se encontró el archivo de datos. Se creará uno nuevo." << endl;
            guardarDatos();
            return;
        }

        string nombreLinea;
        while (getline(archivo, nombreLinea)) {
            Linea* linea = new Linea(nombreLinea);
            string estacionStr;
            while (getline(archivo, estacionStr) && !estacionStr.empty()) {
                stringstream ss(estacionStr);
                string nombreEstacion;
                bool esTransferencia;
                ss >> nombreEstacion >> esTransferencia;
                Estacion* estacion = new Estacion(nombreEstacion, esTransferencia);
                linea->agregarEstacion(estacion);
            }
            agregarLinea(linea);
        }

        archivo.close();
    }

    void mostrarLineas() const {
        cout << "----- Líneas -----" << endl;
        for (const auto& linea : lineas) {
            cout << "- " << linea->getNombre() << endl;
        }
    }

    void mostrarEstaciones() const {
        cout << "----- Estaciones -----" << endl;
        for (const auto& linea : lineas) {
            cout << linea->getNombre() << ":" << endl;
            for (const auto& estacion : linea->obtenerEstaciones()) {
                cout << "- " << estacion->getNombre() << endl;
            }
        }
    }
};

void mostrarMenu() {
    cout << "----- Menú -----" << endl;
    cout << "1. Administrar líneas" << endl;
    cout << "2. Administrar estaciones" << endl;
    cout << "3. Verificar si una estación pertenece a una línea" << endl;
    cout << "4. Simular tiempo de llegada" << endl;
    cout << "5. Salir" << endl;
    cout << "Ingrese el número de la opción que desea realizar: ";
}

void mostrarMenuLineas() {
    cout << "----- Menú Líneas -----" << endl;
    cout << "1. Agregar línea" << endl;
    cout << "2. Eliminar línea" << endl;
    cout << "3. Mostrar líneas" << endl;
    cout << "Ingrese el número de la opción que desea realizar: ";
}

void mostrarMenuEstaciones() {
    cout << "----- Menú Estaciones -----" << endl;
    cout << "1. Agregar estación" << endl;
    cout << "2. Eliminar estación" << endl;
    cout << "3. Mostrar estaciones" << endl;
    cout << "Ingrese el número de la opción que desea realizar: ";
}

int main() {
    RedMetro metro;
    metro.cargarDatos();

    while (true) {
        mostrarMenu();
        int opcion;
        cin >> opcion;

        if (opcion == 1) {
            while (true) {
                mostrarMenuLineas();
                int opcionLineas;
                cin >> opcionLineas;

                if (opcionLineas == 1) {
                    string nombreLinea;
                    cout << "Ingrese el nombre de la línea a agregar: ";
                    cin >> nombreLinea;
                    Linea* linea = new Linea(nombreLinea);
                    metro.agregarLinea(linea);
                } else if (opcionLineas == 2) {
                    string nombreLinea;
                    cout << "Ingrese el nombre de la línea a eliminar: ";
                    cin >> nombreLinea;
                    if (!metro.eliminarLinea(nombreLinea)) {
                        cout << "No se pudo eliminar la línea. Asegúrese de que no tenga estaciones." << endl;
                    }
                } else if (opcionLineas == 3) {
                    metro.mostrarLineas();
                } else {
                    cout << "Opción inválida. Por favor, ingrese un número válido." << endl;
                }

                cout << "¿Desea realizar otra operación en líneas? (1: Sí, 0: No): ";
                int continuar;
                cin >> continuar;
                if (continuar != 1) break;
            }
        } else if (opcion == 2) {
            while (true) {
                mostrarMenuEstaciones();
                int opcionEstaciones;
                cin >> opcionEstaciones;

                if (opcionEstaciones == 1) {
                    string nombreLinea, nombreEstacion;
                    bool esTransferencia;

                    cout << "Ingrese el nombre de la línea: ";
                    cin >> nombreLinea;
                    cout << "Ingrese el nombre de la estación: ";
                    cin >> nombreEstacion;
                    cout << "¿Es una estación de transferencia? (1: Sí, 0: No): ";
                    cin >> esTransferencia;

                    Estacion* estacion = new Estacion(nombreEstacion, esTransferencia);
                    metro.agregarEstacion(nombreLinea, estacion);
                } else if (opcionEstaciones == 2) {
                    string nombreLinea, nombreEstacion;

                    cout << "Ingrese el nombre de la línea: ";
                    cin >> nombreLinea;
                    cout << "Ingrese el nombre de la estación a eliminar: ";
                    cin >> nombreEstacion;

                    metro.eliminarEstacion(nombreLinea, nombreEstacion);
                } else if (opcionEstaciones == 3) {
                    metro.mostrarEstaciones();
                } else {
                    cout << "Opción inválida. Por favor, ingrese un número válido." << endl;
                }

                cout << "¿Desea realizar otra operación en estaciones? (1: Sí, 0: No): ";
                int continuar;
                cin >> continuar;
                if (continuar != 1) break;
            }
        } else if (opcion == 3) {
            string nombreLinea, nombreEstacion;
            cout << "Ingrese el nombre de la línea: ";
            cin >> nombreLinea;
            cout << "Ingrese el nombre de la estación: ";
            cin >> nombreEstacion;
            if (metro.perteneceEstacion(nombreLinea, nombreEstacion)) {
                cout << "La estación pertenece a la línea." << endl;
            } else {
                cout << "La estación no pertenece a la línea." << endl;
            }
        } else if (opcion == 4) {
            string nombreLinea, estacionSalida, estacionLlegada;
            cout << "Ingrese el nombre de la línea: ";
            cin >> nombreLinea;
            cout << "Ingrese el nombre de la estación de salida: ";
            cin >> estacionSalida;
            cout << "Ingrese el nombre de la estación de llegada: ";
            cin >> estacionLlegada;
            metro.simularTiempoDeLlegada(estacionSalida, estacionLlegada, nombreLinea);
        } else if (opcion == 5) {
            break;
        } else {
            cout << "Opción inválida. Por favor, ingrese un número válido." << endl;
        }
    }

    return 0;
}



