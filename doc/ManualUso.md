# Manual de uso

## FORMA DE EJECUCIÓN

Para probar el programa hay que introducir en la consola de comandos de la carpeta en la que esta el ejecutable, la base de conocimiento y las bases de hechos necesarias:

```bash
.\SBR-FC.exe BC.txt BH.txt
```

## FORMATO DE FICHEROS DE ENTRADA
Los ficheros BC.txt y BH.txt tienen que tener un formato concreto:

### 1. Base de conocimiento (BC.txt).

En la primera línea encontramos el número de reglas que tiene la base de conocimiento. Después en las siguientes líneas encontramos las reglas con un formato definido. A continuación tenemos un ejemplo de lo que podría contener un fichero BC.txt:

```bash
X
RX: Si hecho Entonces hecho, FC=X
RX: Si hecho o hecho Entonces hecho, FC=X
RX: Si hecho y hecho Entonces hecho, FC=X
```

Como podemos ver hay un ejemplo de cada tipo de reglas que podemos encontrar. Una regla simple, una con una conjunción y otra con una intersección.

Si en este ejemplo cambiamos las X por números, los que nosotros consideremos; y donde pone "hecho" por el nombre del hecho que se de en ese ejercicio, sería totalmente funcional.

**IMPORTANTE**: en la parte de la regla que es ", FC=" SIEMPRE debe ser así, con un espacio entre la coma y la F. Sino habría un error al leer las reglas.


### 2. Base de hechos (BH.txt)

En la primera línea encontramos el número de hechos que contiene la base de hechos. Después tantas líneas como hechos hayamos puesto. Y por último dos líneas una con la palabra reservada Objetivo, para marcar que la última línea es la meta a la que queremos llegar. A continuación tenemos un ejemplo de lo que podría contener un fichero BH.txt:

```bash
X
hecho, FC=X
hecho, FC=X
hecho, FC=X
Objetivo
hechoObjetivo
```

Como podemos ver hay ejemplos de como sería la declaración de los hechos y como se escribiría el objetivo.

Si en este ejemplo cambiamos las X por números, los que nosotros consideremos; y donde pone "hecho" por el nombre del hecho que se de en ese ejercicio, sería totalmente funcional.

**IMPORTANTE**: en la parte de la regla que es ", FC=" SIEMPRE debe ser así, con un espacio entre la coma y la F. Sino habría un error al leer las reglas.


## FORMATO DE FICHERO DE SALIDA
El fichero de salida tendrá el nombre BC.txt_BH.txt, en este fichero encontraremos un desglose de las reglas usadas o eliminadas, los cálculos realizados con sus formulas y todo. Y al final del documento una conclusión sacada de analizar el resultado.

Cada vez que se realiza un calculo te dice entre que hechos o reglas son las usadas en ese caso y el resultado que obtienes, por lo cual no hay perdida. Aun así junto a la conclusión se escribe de forma explícita "El FC de hechoObjetivo es: X", para dejar claro el resultado.


