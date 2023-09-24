// El vuelo de Pepita - Guía Paradigmas de Programación 3 - POO - C++ - Facultad de Ingeniería del Ejército
// Autor: Axel Sacca - Fecha: 12/09/2023

// El juego consiste en que Pepita, una golondrina, debe volar desde la Ciudad Autónoma de Buenos Aires hasta la frontera con Bolivia.
// Para ello, Pepita debe ir volando de lugar en lugar, y debe comer para recuperar energía.
// Cada lugar tiene una distancia en kilómetros desde el kilómetro cero de la Ruta 9, y Pepita puede volar hasta 80 km/h.
// Pepita puede almacenar hasta 2000 joules de energía, y cada kilómetro que vuela le cuesta 1 joule.
// Si Pepita se queda sin energía y sin comida, puede descansar para recuperar energía, lo que le da 30 joules y le toma 4 horas.
// Pepita puede encontrar comida mientras vuela, entre 0.1 y 1 gramo por kilómetro volado, con una probabilidad del 50%.
// Cada gramo de comida le da 4 joules de energía.
// Pepita puede almacenar hasta 500 gramos de comida.
// Pepita puede comer para recuperar energía, lo que le toma 1 hora. No puede comer más de lo que necesita para alcanzar los 2000 joules.
// Toda acción que el usuario elija, sumará horas. El objetivo del juego es llegar a la frontera con Bolivia en la menor cantidad de horas posibles.

// Librerías
#include <iostream> // Input/Output
#include <map> // Mapas
#include <cmath> // Funciones matemáticas
#include <random> // Generador de números aleatorios
#include <memory> // Punteros inteligentes
#include <string> // Strings
#include <stdlib.h> // Funciones estándar
#include <stdio.h> // Funciones estándar
#include <cstdlib> // Funciones estándar
#include <iomanip> // Manipulación de input/output

void clearScreen(); // Prototipo de la función para limpiar la pantalla

// Función para limpiar la pantalla
void clearScreen() { 
        #if defined(_WIN32) // Windows
            system("cls"); 
        #elif defined(__linux__) || defined(__APPLE__) // Linux o Mac
            system("clear");
        #else
            std::cout << "Sistema operativo no reconocido. No se pudo limpiar la pantalla." << std::endl;
        #endif
}

// Clase Lugar
class Lugar {
public:
    std::string nombre; // Nombre del lugar
    int km; // Distancia en kilómetros desde el kilómetro cero de la Ruta 9

    Lugar(std::string nombre, int km) : nombre(nombre), km(km) {} // Constructor de la clase Lugar
};

// Clase Pepita
class Pepita {
private:
    int energia, comida;
    float horas;
    std::shared_ptr<Lugar> lugarActual; // Lugar actual de Pepita
    std::mt19937 gen; // Generador de números aleatorios

public:
    Pepita(int energia, const std::shared_ptr<Lugar>& lugar) : energia(energia), lugarActual(lugar), comida(2000), horas(0.00) { // Constructor de la clase Pepita
        std::random_device rd;
        gen = std::mt19937(rd()); // Inicializar el generador de números aleatorios
    }

    // Método de vuelo que afecta la energía final según la distancia recorrida, las horas transcurridas en vuelo, y la comida encontrada
    void volar(int km) { 
        energia -= km + 10; // Restar energía según los kilómetros volados y un valor fijo de 10
        horas += km / 80.00; // Incrementar las horas según los kilómetros volados
    
        std::uniform_real_distribution<> dis(0.1, 1); // Distribución uniforme de números reales en el rango [0.1, 1] - se usa para el cálculo de comida encontrada por km recorrido
    
        if (dis(gen) < 0.5) { // Probabilidad del 50% de encontrar comida durante el vuelo
            int comidaEncontrada = static_cast<int>(km * dis(gen)); // Calcular la cantidad de comida encontrada
            comida = std::min(comida + comidaEncontrada, 500); // Agregar la comida encontrada a la comida almacenada (hasta un máximo de 500 gramos)
            std::cout << "Pepita encontro " << comidaEncontrada << " gramos de comida durante el vuelo." << std::endl;
        }
    }

    // Método para comer
    void comer() { 
        int energiaPotencial = comida * 4; // Calcular la energía potencial de la comida almacenada
        int energiaNecesaria = std::min(2000 - energia, energiaPotencial); // Calcular la cantidad de energía que se puede conseguir de la comida hasta un máximo de 2000 joules
        energia += energiaNecesaria; // Consumir la comida necesaria para alcanzar el máximo de energía.
        int comidaConsumida = energiaNecesaria / 4; // Calcular la cantidad de comida a consumir según la energía necesaria
        comida -= comidaConsumida; // Restar la comida consumida de la comida almacenada
        horas += 1; // Incrementar las horas en 1 cada vez que Pepita come
        std::cout << "Pepita comio " << comidaConsumida << " gramos de comida y gano " << energiaNecesaria << " de energia." << std::endl;
    }

    // Verificar si Pepita puede ir a un lugar
    bool puedeIrA(std::shared_ptr<Lugar> lugar) const { 
        int distancia = std::abs(lugarActual->km - lugar->km); // Calcular la distancia entre el lugar actual y el lugar al que se quiere ir
        return (energia - distancia - 10) >= 0; // Devolver verdadero si Pepita tiene suficiente energía para ir al lugar, falso en caso contrario
    }

    // Método que ejecuta el vuelo y la actualización de la posición
    void irA(std::shared_ptr<Lugar> lugar) { 
        if(puedeIrA(lugar)) { // Verificar si Pepita puede ir al lugar
            int distancia = std::abs(lugarActual->km - lugar->km); // Calcular la distancia entre el lugar actual y el lugar al que se quiere ir
            volar(distancia); // Volar hacia el lugar
            lugarActual = lugar; // Actualizar el lugar actual de Pepita
        } else {
            std::cout << "\nPepita no tiene suficiente energia para ir a " << lugar->nombre << "\n" << std::endl;
        }
    }

    // Mostrar el estado actual de Pepita (stats)
    void mostrarEstado() const { 
        std::cout << "\nPepita esta en " << lugarActual->nombre << ".\n" << std::endl; // Mostrar el lugar actual de Pepita
        std::cout << "Energia: " << energia << " joules.\t Comida almacenada: " << comida << " gramos.\t Horas transcurridas: " << std::fixed << std::setprecision(2) << horas << std::endl;
    }

    // Mostrar los lugares a los que Pepita puede ir
    void mostrarLugares(const std::map<int, std::shared_ptr<Lugar>>& lugares) const { 
        std::cout << "Volar a..." << std::endl;
        for(auto const& lugar: lugares) { // Iterar sobre los lugares disponibles
            int distancia = std::abs(lugar.second->km - lugarActual->km); // Calcular la distancia entre el lugar actual y cada uno de los lugares disponibles
            std::string outOfRangeIndicator = puedeIrA(lugar.second) ? "" : " (fuera de rango)"; // Indicador "(fuera de rango)" si Pepita no puede ir al lugar
            std::cout << lugar.first << ": " << lugar.second->nombre << " (" << distancia << " km)" // Mostrar el nombre del lugar, la distancia y el indicador
                      << outOfRangeIndicator << std::endl;
        }
    }

    // Método para descansar
    void rest() { 
        energia += 30; // Aumentar la energía en 30 puntos
        horas += 4; // Incrementar las horas en 4
        std::cout << "Pepita descanso y gano 30 puntos de energia." << std::endl;
    }

    // Método para calcular el máximo de kilómetros que Pepita puede volar con su energía actual
    int maxKm() const { 
        return energia - 10;
    }
};


// Función principal
int main() { 
    std::map<int, std::shared_ptr<Lugar>> lugares; // Mapa de lugares disponibles

    // Crear los lugares disponibles
    lugares[3] = std::make_shared<Lugar>("Ciudad Autonoma de Buenos Aires", 0);
    lugares[4] = std::make_shared<Lugar>("Tigre", 32);
    lugares[5] = std::make_shared<Lugar>("Campana", 75);
    lugares[6] = std::make_shared<Lugar>("Zarate", 90);
    lugares[7] = std::make_shared<Lugar>("San Pedro", 160);
    lugares[8] = std::make_shared<Lugar>("Rosario", 300);
    lugares[9] = std::make_shared<Lugar>("Cordoba", 700);
    lugares[10] = std::make_shared<Lugar>("Jesus Maria", 755);
    lugares[11] = std::make_shared<Lugar>("Santiago del Estero", 1140);
    lugares[12] = std::make_shared<Lugar>("Termas de Rio Hondo", 1170);
    lugares[13] = std::make_shared<Lugar>("San Miguel de Tucuman", 1310);
    lugares[14] = std::make_shared<Lugar>("Monteros", 1345);
    lugares[15] = std::make_shared<Lugar>("Tafi del Valle", 1430);
    lugares[16] = std::make_shared<Lugar>("San Salvador de Jujuy", 1550);
    lugares[17] = std::make_shared<Lugar>("Purmamarca", 1620);
    lugares[18] = std::make_shared<Lugar>("Tilcara", 1640);
    lugares[19] = std::make_shared<Lugar>("Humahuaca", 1700);
    lugares[20] = std::make_shared<Lugar>("Abra Pampa", 1800);
    lugares[21] = std::make_shared<Lugar>("La Quiaca", 1860);
    lugares[22] = std::make_shared<Lugar>("Frontera con Bolivia", 1920);

    std::shared_ptr<Lugar> caba = lugares[3]; // Lugar inicial de Pepita

    Pepita pepita(100, caba); // Crear a Pepita con 100 joules de energía y en CABA

    while(true) { // Loop principal
        clearScreen();
        pepita.mostrarEstado(); // Mostrar el estado actual de Pepita
        std::cout << "Pepita puede volar " << pepita.maxKm() << " km con su energia actual.\n" << std::endl; 

        std::cout << "Elija una opcion:\n" << std::endl;
        std::cout << "1: Comer" << std::endl;
        std::cout << "2: Descansar\n" << std::endl;

        pepita.mostrarLugares(lugares); // Mostrar los lugares a los que Pepita puede ir

        int eleccion;
        if (!(std::cin >> eleccion)) { // Si el input no es un número, salir del programa
            std::cout << "Error de entrada. Saliendo..." << std::endl;
            break;
        }

        if(eleccion == 0) { // Si la elección es 0, salir del programa
            break;
        } else if (eleccion == 1) { // Si la elección es 1, comer
            pepita.comer();
        } else if (eleccion == 2) { // Si la elección es 2, descansar
            pepita.rest();
        } else if (lugares.find(eleccion) != lugares.end()) { // Si la elección es un número entre 3 y 22, ir al lugar correspondiente
            pepita.irA(lugares[eleccion]);
            if (eleccion == 22) { // Si el lugar al que Pepita fue es la frontera con Bolivia, ganar el juego
                std::cout << "\nFelicidades! Has llegado a la Frontera con Bolivia. Has ganado el juego!" << std::endl;
                break;
            }
        } else { // Si la elección no es válida, mostrar un mensaje de error
            std::cout << "Esa no es una opción valida.\n" << std::endl;
        }

        std::cout << "\nQue deberia hacer Pepita?\n" << std::endl;
    }

    std::cout << "\nPresione enter para salir...";
    std::cin.clear(); // Limpiar el buffer de entrada
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); // Esperar a que el usuario presione enter para salir
    getchar();

    return 0;
}
