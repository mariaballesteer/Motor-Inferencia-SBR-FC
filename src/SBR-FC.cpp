#include <stdio.h>
#include <iostream>
#include <fstream>      // Librería para el manejo de ficheros
#include <string>       // Librería que incluye los string
#include <vector>       // Librería que incluye los vector
#include <sstream>      // Librería que incluye los stringstream
#include <algorithm>    // Librería que incluye min y max
#include <cmath>        // Librería que incluye el valor absoluto

using namespace std;

ofstream ficheroResultado; // Fichero de resultados

// Estructura para guardar los hechos de las bases de hechos
/*
La estructura en la que entra es:
    nombreHecho, FC=fc
*/
typedef struct {
    string nombreHecho;
    double fc;
}Hecho;

//Estructura para guardar las reglas en las bases de conocimientos
/*
La estructura en la que entra:
nombreRegla: Si hechoAntecedentes Entonces consecuente, FC=fc
*/
typedef struct {
    string nombreRegla;
    vector<string> hechosAntecedentes;
    string enlace; // Mirar si es necesario hacerlo un vector para cuando hay consecuentes con 3 antecedentes que vienen de disrintas reglas (CausanteA ejemplo)
    string consecuente;
    double fc;
}Regla;

/*
En la línea de comando después del ejecutable entran dos ficheros, ejemplo de linea de comandos:
SBR-FC.exe BC.txt BH.txt
SBR-FC.exe --> es el ejecutable
BC.txt --> es la base de conocimento
BH.txt --> es la base de hechos
*/
// Función para leer la bases de conocimiento (string BC --> es el nombre del docuemnto BC.txt)
vector<Regla> leerFicheroBC(string BC){
    vector<Regla> reglas; // Vector de reglas
    int nReglas;    // numero de reglas
    string l;       // lineas

    ifstream bc(BC); //Abrimos y leemos el fichero BC.txt
    bc >> nReglas; //El primer número del fichero es el numero de reglas que tenemos asi que lo guardamos en nReglas
    getline(bc,l); // omision de una linea vacia

    //En cada ejecución del bucle se lee una linea 
    for (int i = 0; i < nReglas; i++) {
        Regla regla; //En cada ejecución lee una regla, así que hay que crear una regla para cada iteración
        string antecedenteRegla;
        getline(bc, l); //En l está la linea que Ri

        istringstream lRegla(l); // Hace más facil trabajar con la linea
        getline(lRegla, regla.nombreRegla, ':'); // Guarda en el nombreRegla el identificador que le haya entrado 
        while((getline(lRegla >> ws, antecedenteRegla, ' ')) && (antecedenteRegla != "Entonces")) {
            if((antecedenteRegla == "y") || antecedenteRegla == "o"){ // Si lo que leemos es un y o un o entra aqui
                regla.enlace = antecedenteRegla;    // Lo guardamos en regla.enlace
            }
            else if(antecedenteRegla != "Si"){ // Si no es la palabra "Si" entra aqui
                regla.hechosAntecedentes.push_back(antecedenteRegla); // Si no es un y o un o lo guardamos en hechosAntecedentes
            }
        }
        getline(lRegla >> ws, regla.consecuente, ','); // Guardamos el consecuente
        lRegla.ignore(4); // Ignoramos los 4 primeros caracteres que quedan, que son " FC="
        lRegla >> regla.fc; // Guardamos el factor de certeza
        reglas.push_back(regla); // Guardamos la regla en el vector de reglas
    }
    bc.close(); // Cerramos el fichero BC.txt
    return reglas; // Devolvemos el vector de reglas

}

// Función para leer la base de hechos (string BH --> es el nombre del docuemnto BH.txt)
vector<Hecho> leerFicheroBH(string BH, string &objetivo){
    vector<Hecho> hechos; // Vector de hechos
    int nHechos;    // numero de hechos
    string l;       // lineas

    ifstream bh(BH); //Abrimos y leemos el fichero BH.txt
    bh >> nHechos; //El primer número del fichero es el numero de hechos que tenemos asi que lo guardamos en nHechos
    getline(bh,l); // omision de una linea vacia

    //En cada ejecución del bucle se lee una linea
    for (int i = 1; i <= nHechos; i++) {
        Hecho hecho; //En cada ejecución lee un hecho, así que hay que crear un hecho para cada iteración

        getline(bh, l); //En l está la linea y en cada linea encontramos: nombreHecho, FC=fc
        istringstream lHecho(l); // Hace más facil trabajar con la linea

        getline(lHecho, hecho.nombreHecho, ','); // Guarda en el nombreHecho el identificador que le haya entrado

        lHecho.ignore(4); // Ignoramos los 4 primeros caracteres que quedan, que son " FC="
        lHecho >> hecho.fc; // Guardamos el factor de certeza

        hechos.push_back(hecho); // Guardamos el hecho en el vector de hechos
    }

    // Ya hemos leido todos los hechos, ahora leemos el objetivo
    getline(bh, l); // Leemos la linea que contiene la palabra "Objetivo"
    getline(bh, objetivo); // Leemos la siguiente lina en la que esta el nombre del objetivo y lo guardamos en la variable objetivo que nos pasan en la función

    bh.close(); // Cerramos el fichero BH.txt
    return hechos; // Devolvemos el vector de hechos
}

// La funcion contenida sirve para ver si un hecho pasado por parametro, el objetivo, ya se encuentra en la base de hechos
double contenida(string objetivo, vector<Hecho> &hechos){
    // Recorre todos los hechos de la base de hechos comparandolo con el objetivo
    for (size_t i = 0; i < hechos.size(); i++){
        if (objetivo == hechos[i].nombreHecho){
            // Si el objetivo ya se encuentra en la base de hechos, devolvemos el factor de certeza
            return hechos[i].fc;
        }
    }
    return 2;
    // Simulamos un valor NULL y para eso ponemos 2 porque los factores de certeza son [-1 >= fc <= 1]
}

vector<Regla> equiparar(vector<Regla> reglas, string objetivo){
    vector<Regla> ConjuntoConflictos; // Creamos un vector de reglas que nos dira que reglas forman el conjunto de conflictos
    // Recorremos todas las reglas de la base de conocimiento
    for (size_t i = 0; i < reglas.size(); i++){
        // Si el objetivo es igual al consecuente de la regla, añadimos la regla al conjunto de conflictos
        if (objetivo == reglas[i].consecuente){
            ConjuntoConflictos.push_back(reglas[i]);
        }
    } 
    // Lo icluimos en el fichero de resultados
    ficheroResultado << "Conjunto de conflictos: ";
    for (size_t i = 0; i < ConjuntoConflictos.size(); i++){
        // Añade el nombre de las reglas que forman el conjunto de conflictos separadas por comas
        ficheroResultado << ConjuntoConflictos[i].nombreRegla;
        if(i != ConjuntoConflictos.size() - 1){ 
            // Si no es el ultimo elemento añade una coma
            ficheroResultado << ", ";
        }
    }
    ficheroResultado << endl; // Añade un salto de linea
    return ConjuntoConflictos;
}

// Devuelve un double con el FC del objetivo dado
double verificar(vector<Regla> reglas, vector<Hecho> &hechos, string objetivo){
    double FCobjetivo = 0; // Variable que va a devolver el resultado
    // Lo inicializamos a 0 porque si no se encuentra el objetivo en la base de hechos, el no se puede afirmar nada ni negar nada
    double FCobjetivoTemp = 0; // Variable que guarda el factor de certeza del objetivo anterior
    if(contenida(objetivo, hechos) != 2){
        // Si el objetivo ya se encuentra en la base de hechos, devolvemos el factor de certeza
        return contenida(objetivo, hechos);
    }
    else{
        // Si no se encuentra el objetivo en la base de hechos, se busca en las reglas los conjuntos conflictos
        vector<Regla> ConjuntoConflictos = equiparar(reglas, objetivo); // Vector de reglas que forman el conjunto
        // Si el conjunto de conflictos no esta vacio entra en el bucle y se mantiene en el hasta que se resuelvan todas las reglas
        unsigned nConflictos = ConjuntoConflictos.size(); // Guardamos el numero de conflictos para saber si hay que realizar el CASO 2
        while(ConjuntoConflictos.size() != 0){
            Regla regla = ConjuntoConflictos.back(); // Guardamos la ultima regla del conjunto de conflictos
            ficheroResultado << "Regla " << regla.nombreRegla << " eliminada" << endl;
            ConjuntoConflictos.pop_back(); // Eliminamos la ultima regla del conjunto de conflictos
            vector<string> NuevosObjetivos = regla.hechosAntecedentes; // Guardamos los antecedentes de la regla
            // Guardamos en el fichero de resultados los antecedentes de la regla
            if(NuevosObjetivos.size() > 1){
                // Si hay mas de un antecedente en la regla entra en este caso
                ficheroResultado << "Antecedentes de la regla " << regla.nombreRegla << ": ";
            }
            else{
                // Si solo hay un antecedente en la regla entra en este caso
                ficheroResultado << "Antecedente de la regla " << regla.nombreRegla << ": ";
            }
            // Guarda los antecedentes uno a uno en el fichero
            for (size_t i = 0; i < NuevosObjetivos.size(); i++){
                ficheroResultado << NuevosObjetivos[i];
                if(i != NuevosObjetivos.size() - 1){ // Si no es el ultimo antecedente añade una coma
                    ficheroResultado << ", ";
                }else{
                    ficheroResultado << endl; // Si es el ultimo antecedente añade un salto de linea
                }
            }

            /*  
                Para detectar si hay más de un antecedente en la regla necesitamos saberlo, para ello
                necesitamos saber si es el primero o el segundo anteceddente para poder transcribir el CASO 1 al
                fichero de resultados con su enlace correspondiente
            */
            double primerAntecedenteFC = 2.0; // Guarda el primer antecedente o unico si hay solo uno
            /*  
                Si primerAntecedente esta a dos es que es el primer antecedente de la regla en entrar, 
                pero si es un numero entre -1 y 1 ya es el segundo antecedente de la regla y hay que ver que
                enlace hay entre los dos antecedentes.
            */

            string primerAntecedenteNombre; // Guarda el nombre del primer antecedente

            while(NuevosObjetivos.size() != 0){
                string nuevoObjetivo = NuevosObjetivos.back(); // Guardamos el ultimo objetivo
                NuevosObjetivos.pop_back(); // Eliminamos el ultimo objetivo que hemos guardado en la variable nuevoObjetivo
                FCobjetivoTemp = verificar(reglas, hechos, nuevoObjetivo); // Guardamos el factor de certeza del nuevo objetivo

                if(primerAntecedenteFC == 2){
                    primerAntecedenteFC = FCobjetivoTemp; // Guardamos el factor de certeza del primer antecedente
                    primerAntecedenteNombre = nuevoObjetivo; // Guardamos el nombre del primer antecedente
                }
                else{ //Si entra aqui es porque hay mas de un antecedente en la regla y tenemos que ver cual es el enlace
                    if(regla.enlace == "y"){ // Si el enlace es "y" entra aqui
                        ficheroResultado << "CASO 1 entre " << nuevoObjetivo << " y " << primerAntecedenteNombre << ": min{" << primerAntecedenteFC << ", " << FCobjetivoTemp << "} = ";
                        // Guardamos el minimo entre el primer antecedente y el segundo antecedente
                        // Esto es por los CASOS que nos proporcionan en el powerpoint de los fundamentos teoricos
                        primerAntecedenteFC = min(primerAntecedenteFC, FCobjetivoTemp);
                        ficheroResultado << primerAntecedenteFC << endl;
                    }
                    else{
                        ficheroResultado << "CASO 1 entre " << nuevoObjetivo << " o " << primerAntecedenteNombre << ": max{" << primerAntecedenteFC << ", " << FCobjetivoTemp << "} = ";
                        // Guardamos el maximo entre el primer antecedente y el segundo antecedente
                        // Esto es por los CASOS que nos proporcionan en el powerpoint de los fundamentos teoricos
                        primerAntecedenteFC = max(primerAntecedenteFC, FCobjetivoTemp);
                        ficheroResultado << primerAntecedenteFC << endl;
                    }                
                }
            }

            // Cuando se juntan dos antecedentes despues hay que hacer el CASO 3 con el FC de la regla y el resultado obtenidola " << regla.nombreRegla << ": " << regla.fc << " * max{0, " << primerAntecedenteFC << "} = " << regla.fc * max(0.0, primerAntecedenteFC) << endl;
            // Guardamos el resultado del CASO 3 en la regla
            ficheroResultado << "CASO 3 con la regla " << regla.nombreRegla << ": " << regla.fc << " * max{0, " << primerAntecedenteFC << "} = ";
            primerAntecedenteFC = regla.fc * max(0.0, primerAntecedenteFC);
            ficheroResultado << primerAntecedenteFC << endl;

            if (nConflictos > 1) { // Más de una regla afecta al mismo consecuente
                if (FCobjetivo >= 0 && primerAntecedenteFC >= 0) { // Ambos factores de certeza son positivos
                    ficheroResultado << "CASO 2 añadiendo al hecho " << objetivo << " el resultado de la regla " << regla.nombreRegla << ": " << FCobjetivo << " + " << primerAntecedenteFC << " * (1 - " << FCobjetivo << ") = " << (FCobjetivo + primerAntecedenteFC * (1 - FCobjetivo)) << endl;
                    FCobjetivo = (FCobjetivo + primerAntecedenteFC * (1 - FCobjetivo));
                } else if (FCobjetivo <= 0 && primerAntecedenteFC <= 0) {
                    // Ambos factores de certeza son negativos
                    ficheroResultado << "CASO 2 añadiendo al hecho " << objetivo << " el resultado de la regla " << regla.nombreRegla << ": " << FCobjetivo << " + " << primerAntecedenteFC << " * (1 + " << FCobjetivo << ") = " << (FCobjetivo + primerAntecedenteFC * (1 + FCobjetivo)) << endl;
                    FCobjetivo = (FCobjetivo + primerAntecedenteFC * (1 + FCobjetivo));
                } else {
                    // Factores de certeza con signos diferentes
                    double divisor = 1 - min(fabs(FCobjetivo), fabs(primerAntecedenteFC));
                    ficheroResultado << "CASO 2 añadiendo al hecho " << objetivo << " el resultado de la regla " << regla.nombreRegla << ": " << FCobjetivo << " + " << primerAntecedenteFC << " / (1 - min{" << fabs(FCobjetivo) << ", " << fabs(primerAntecedenteFC) << "}) = " << (FCobjetivo + primerAntecedenteFC) / divisor << endl;
                    FCobjetivo = (FCobjetivo + primerAntecedenteFC) / divisor;
                }
            } else { // Solo una regla afecta al consecuente
                FCobjetivo = primerAntecedenteFC;
            }

        }
    }

    // Guardamos el resultado en la base de hechos
    Hecho Nuevohecho; // Creamos un hecho
    
    Nuevohecho.nombreHecho = objetivo; // Guardamos el nombre del objetivo en el hecho
    Nuevohecho.fc = FCobjetivo; // Guardamos el factor de certeza que hemos calculado en el hecho 
    hechos.push_back(Nuevohecho); // Añadimos el hecho a la base de hechos
    
    return FCobjetivo; // Devolvemos el factor de certeza del objetivo
}


// Función para realizar el encaminamiento hacia atrás y obtener una conclusión del resultado obtenido
void ENCAMINAMIENTO_HACIA_ATRAS(vector<Regla> reglas, vector<Hecho> &hechos, string objetivo){
    // Con el resultado de verificar hacemos un análisis de casos para obtener una conclusión
    double resultado = verificar(reglas, hechos, objetivo);

    ficheroResultado << endl << endl << "El FC de " << objetivo << " es: " << resultado << endl;

    // Analisis del resultado obtenido
    if (resultado <= 1 && resultado >= 0.75){ // Si el resultado es mayor o igual a 0.75 y menor o igual a 1 entra aqui
        ficheroResultado << "Podemos afirmar que " << objetivo << " va a darse con una probabilidad casi absoluta" << endl;
    }
    else if(resultado < 0.75 && resultado >= 0.5){ // Si el resultado es menor a 0.75 y mayor o igual a 0.5 entra aqui
        ficheroResultado << "Tras analizar los hechos podemos intuir que " << objetivo << " va a darse con una probabilidad alta" << endl;
    }
    else if(resultado < 0.5 && resultado >= 0.25){ // Si el resultado es menor a 0.5 y mayor o igual a 0.25 entra aqui
        ficheroResultado << "Hay indicios de que va a darse " << objetivo << " pero no se puede asegurar" << endl;
    }
    else if(resultado < 0.25 && resultado > 0){ // Si el resultado es menor a 0.25 y mayor a 0 entra aqui
        ficheroResultado << "No hay un resultado claro pero " << objetivo << " tiene mas probabilidad de darse que de no darse" << endl;
    }
    else if (resultado == 0){ // Si el resultado es 0 entra aqui
        ficheroResultado << "No se puede afirmar nada sobre " << objetivo << endl;
    }
    else if (resultado < 0 && resultado >= -0.25){ // Si el resultado es menor a 0 y mayor o igual a -0.25 entra aqui
        ficheroResultado << "No hay un resultado claro pero " << objetivo << " tiene mas probabilidad de no darse que de darse" << endl;
    }
    else if (resultado < -0.25 && resultado >= -0.5){ // Si el resultado es menor a -0.25 y mayor o igual a -0.5 entra aqui
        ficheroResultado << "Hay indicios de que no va a darse " << objetivo << " pero no se puede asegurar" << endl;
    }
    else if(resultado < -0.5 && resultado >= -0.75){ // Si el resultado es menor a -0.5 y mayor o igual a -0.75 entra aqui
        ficheroResultado << "Tras analizar los hechos podemos intuir que " << objetivo << " no va a darse con una probabilidad alta" << endl;
    }
    else if(resultado < -0.75 && resultado >= -1){ // Si el resultado es menor a -0.75 y mayor o igual a -1 entra aqui
        ficheroResultado << "Podemos afirmar que " << objetivo << " no va a darse con una probabilidad casi absoluta" << endl;
    }
}

int main(int argc, char *argv[]){ //Necesitamos poner char *argv[] en los parentesis del main para poder usar el resto de argumentos en la linea de comandos
    
    // Solo leemos el fichero BC.txt porque puede ser que no solo nos incluyan un BH.txt, sino que incluyan más de uno
    vector<Regla> reglas = leerFicheroBC((string)argv[1]); // Guarda las reglas que se encuentran en el fichero BC.txt
    //Con este bucle ejecutamos el programa tantas veces como BH.txt nos hayan introducido
    for (int i = 2; i < argc; i++){
        string nombreFicheroResultado = (string)argv[1] + '_' + (string)argv[i]; 
        // (string)argv[0] --> nombre ejecutable; (string)argv[1] --> nombre de la BC.txt; (string)argv[i] --> nombre de la BH.txt
        ficheroResultado.open(nombreFicheroResultado); // Crea el archivo
        /*
            Al principio del docuemnto pondrá:
            Base de conocimientos: BC.txt
            Base de hechos: BH.txt
        */
        ficheroResultado << "Base de conocimientos: " << (string)argv[1] << "\nBase de hechos: " << (string)argv[i] << endl << endl;


        //Aqui leemos el fichero BH.txt que nos introducen por la linea de comandos en la posición i
        string objetivo; // Guarda el objetivo que se encuentra en el fichero BH.txt
        vector<Hecho> hechos = leerFicheroBH((string)argv[i], objetivo); // Guarda los hechos que se encuentran en el fichero BH.txt

        ENCAMINAMIENTO_HACIA_ATRAS(reglas, hechos, objetivo); // Realiza el encaminamiento hacia atrás

        ficheroResultado.close(); // Cierra el archivo que hemos creado
    }
    return 0;
}