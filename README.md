------REALIZADO POR------
ERIC VARGAS Y SANTIAGO CARRASQUERO

PROYECTO DE SUCESION REAL
Sistema de gestión de un arbol genealógico con asignación automática de sucesores basado en reglas de primogenitura.

DESCRIPCION
Este proyecto implementa un árbol binario para representar la familia real de un reino, con funcionalidades para:

- Cargar árbol genealógico desde un archivo ejecutable (CSV)
- Visualizar el árbol completo con jerarquía
- Mostrar línea de sucesión actual
- Asignar automáticamente nuevo rey/reina según reglas establecidas
- Modificar datos de cualquier miembro de la familia
- Procesar muerte del monarca con sucesión automatica

REGLAS DE SUCESION
El sistema implementa las siguientes reglas:

1. Primogenitura: El primer hijo varón hereda el trono
2. Edad límite: Los reyes deben ser menores de 70 años
3. Búsqueda en ramas: Si el primogénito no está disponible, se busca en:
   - Rama del primogénito
   - Rama del hermano
   - Rama del tío
   - Ancestros con dos hijos
4. Mujeres como reinas: Si no hay varones disponibles:
   - Debe tener entre 15 y 70 años
   - Se elige la más joven de la rama más cercana a primogénitos
5. Sucesión automática: Al morir o cumplir 70 años, se asigna nuevo monarca

-COMPILACION DEL CODIGO-

Podemos compilar de dos formas el codigo

1. Con el repositorio ya clonado en vs code, lo que debemos de hacer es presionar Ctrl+shift+B y se compilaria el ccodigo
2. desde el cmd o el propio terminal de vs code (que podemos abrir con CTRL+Ñ) utilizando el siguiente comando:
3.  g++ -g src/main.cpp src/ArbolReal.cpp -o bin/royal_family -std=c++11

-EJECUCION DEL CODIGO-

1. Si hicimos bien los pasos, posteriormente podremos ejecutar el codigo que se haria de la siguiente manera
al nosotros compilar lo unico que debemos hacer es. En la terminal colocamos el comando cd bin
y posterior a eso solo colocamos el archivo ejecutable que se crea al nosotros compilar el codigo que en este caso seria
./royal_family.exe y de ahi ejecutaria el codigo y apareceria el menu de opciones en el cual podremos ver algunas opciones como

-PROCESAR MUERTE DEL REY
-MOSTRAR ARBOL GENEALOGICO COMPLETO entre otros requerimientos
