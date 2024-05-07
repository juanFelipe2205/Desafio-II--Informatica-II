#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Estacion {
private:
    string nombre;
    int tiempoSiguiente;
    int tiempoAnterior;
    bool esTransferencia;

public:
    Estacion(string _nombre, int _tiempoSiguiente, int _tiempoAnterior, bool _esTransferencia) 
        : nombre(_nombre), tiempoSiguiente(_tiempoSiguiente), tiempoAnterior(_tiempoAnterior), esTransferencia(_esTransferencia) {}

    string getNombre() const { return nombre; }
    int getTiempoSiguiente() const { return tiempoSiguiente; }
    int getTiempoAnterior() const { return tiempoAnterior; }
    bool esEstacionTransferencia() const { return esTransferencia; }
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

    int cantidadEstaciones() const {
        return estaciones.size();
    }

    bool tieneEstacion(const string& nombreEstacion) const {
        for (const auto& estacion : estaciones) {
            if (estacion->getNombre() == nombreEstacion)
                return true;
        }
        return false;
    }

    string getNombre() const { return nombre; }

    bool tieneEstacionesTransferencia() const {
        for (const auto& estacion : estaciones) {
            if (estacion->esEstacionTransferencia())
                return true;
        }
        return false;
    }

    const vector<Estacion*>& obtenerEstaciones() const {
        return estaciones;
    }

    vector<Estacion*>& obtenerEstaciones() {
        return estaciones;
    }
};

class RedMetro {
private:
    vector<Linea*> lineas;

public:
    void agregarLinea(Linea* linea) {
        lineas.push_back(linea);
    }

    int cantidadLineas() const {
        return lineas.size();
    }

    int cantidadEstaciones() const {
        int totalEstaciones = 0;
        for (const auto& linea : lineas) {
            totalEstaciones += linea->cantidadEstaciones();
        }
        return totalEstaciones;
    }

    bool eliminarLinea(const string& nombreLinea) {
        for (auto it = lineas.begin(); it != lineas.end(); ++it) {
            if ((*it)->tieneEstacion(nombreLinea)) {
                return false; 
            }
            if ((*it)->getNombre() == nombreLinea) {
                delete *it;
                lineas.erase(it);
                return true; 
            }
        }
        return false; 
    }

    void agregarEstacion(const string& nombreLinea, Estacion* estacion) {
        for (auto& linea : lineas) {
            if (linea->getNombre() == nombreLinea) {
                linea->agregarEstacion(estacion);
                break;
            }
        }
    }

    void eliminarEstacion(const string& nombreLinea, const string& nombreEstacion) {
        for (auto& linea : lineas) {
            if (linea->getNombre() == nombreLinea) {
                auto& estaciones = linea->obtenerEstaciones();
                for (auto it = estaciones.begin(); it != estaciones.end(); ++it) {
                    if ((*it)->getNombre() == nombreEstacion && !(*it)->esEstacionTransferencia()) {
                        delete *it;
                        estaciones.erase(it);
                        break;
                    }
                }
                break;
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
};

void mostrarMenu() {
    cout << "----- Menú -----" << endl;
    cout << "1. Agregar estación a una línea" << endl;
    cout << "2. Eliminar estación de una línea" << endl;
    cout << "3. Agregar línea a la red metro" << endl;
    cout << "4. Eliminar línea de la red metro" << endl;
    cout << "5. Verificar si una estación pertenece a una línea" << endl;
    cout << "6. Salir" << endl;
    cout << "Ingrese el número de la opción que desea realizar: ";
}

int main() {
    RedMetro metro;

    while (true) {
        mostrarMenu();
        int opcion;
        cin >> opcion;

        if (opcion == 1) {
            string nombreLinea, nombreEstacion;
            int tiempoSiguiente, tiempoAnterior;
            bool esTransferencia;

            cout << "Ingrese el nombre de la línea: ";
            cin >> nombreLinea;
            cout << "Ingrese el nombre de la estación: ";
            cin >> nombreEstacion;
            cout << "Ingrese el tiempo hacia la siguiente estación (en segundos): ";
            cin >> tiempoSiguiente;
            cout << "Ingrese el tiempo hacia la estación anterior (en segundos): ";
            cin >> tiempoAnterior;
            cout << "¿Es una estación de transferencia? (1: Sí, 0: No): ";
            cin >> esTransferencia;

            Estacion* estacion = new Estacion(nombreEstacion, tiempoSiguiente, tiempoAnterior, esTransferencia);
            metro.agregarEstacion(nombreLinea, estacion);
        } else if (opcion == 2) {
            string nombreLinea, nombreEstacion;

            cout << "Ingrese el nombre de la línea: ";
            cin >> nombreLinea;
            cout << "Ingrese el nombre de la estación a eliminar: ";
            cin >> nombreEstacion;

            metro.eliminarEstacion(nombreLinea, nombreEstacion);
        } else if (opcion == 3) {
            string nombreLinea;
            cout << "Ingrese el nombre de la línea a agregar: ";
            cin >> nombreLinea;
            Linea* linea = new Linea(nombreLinea);
            metro.agregarLinea(linea);
        } else if (opcion == 4) {
            string nombreLinea;
            cout << "Ingrese el nombre de la línea a eliminar: ";
            cin >> nombreLinea;
            if (!metro.eliminarLinea(nombreLinea)) {
                cout << "No se pudo eliminar la línea. Asegúrese de que no tenga estaciones." << endl;
            }
        } else if (opcion == 5) {
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
        } else if (opcion == 6) {
            break;
        } else {
            cout << "Opción inválida. Por favor, ingrese un número válido." << endl;
        }
    }

    return 0;
}


