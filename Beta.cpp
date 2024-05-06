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
};

int main() {
    Estacion estacion1("Estacion1", 60, 60, false);
    Estacion estacion2("Estacion2", 45, 45, false);
    Estacion estacion3("Estacion3", 30, 30, false);
    Estacion estacion4("Estacion4", 45, 45, true); 
    // Crear línea
    Linea linea1("Linea1");
    linea1.agregarEstacion(&estacion1);
    linea1.agregarEstacion(&estacion2);
    linea1.agregarEstacion(&estacion3);
    linea1.agregarEstacion(&estacion4);

    RedMetro metro;
    metro.agregarLinea(&linea1);

    cout << "Cantidad de líneas en la red metro: " << metro.cantidadLineas() << endl;
    cout << "Cantidad total de estaciones en la red metro: " << metro.cantidadEstaciones() << endl;

    return 0;
}
