#include "abb.h"
#include "testing.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void prueba_crear_arbol_vacio(){

	fputs("### INICIO DE PRUEBAS ARBOL VACIO ###\n", stdout);
	abb_t* arbol = abb_crear(strcmp,NULL);
	print_test("Se creo un arbol vacio", arbol != NULL);
	print_test("Cantidad de nodos es 0", abb_cantidad(arbol)==0);
	print_test("Obtener devuelve NULL", abb_obtener(arbol,"clave")==false);
	print_test("No existe ninguna clave", abb_pertenece(arbol,"clave")==false);
	abb_destruir(arbol);
	print_test("Arbol destruido",true);
	printf("\n");
}

void prueba_abb_null(){
	fputs("### INICIO DE PRUEBAS CON NULL ###\n", stdout);
	abb_t* arbol2 = abb_crear(strcmp,NULL);
	print_test("Insertar clave vacia y valor NULL", abb_guardar(arbol2, "", NULL));
    print_test("la cantidad de elementos es 1", abb_cantidad(arbol2) == 1);
    print_test("Obtener clave vacia es valor NULL", abb_obtener(arbol2, "") == NULL);
    print_test("Pertenece clave vacia, es true", abb_pertenece(arbol2, ""));
    print_test("Borrar clave vacia, es valor NULL", abb_borrar(arbol2, "") == NULL);
    print_test("La cantidad de elementos es 0", abb_cantidad(arbol2) == 0);
    abb_destruir(arbol2);
    printf("\n");
}

void prueba_algunos_elementos(){
	fputs("### INICIO DE PRUEBAS CON ALGUNOS ELEMENTOS ###\n",stdout);
	abb_t* arbol3 = abb_crear(strcmp,NULL);
	char* clave1 = "hola";
	char* valor1 = "chau";
	char* clave2 = "jaja";
	char* valor2 = "jeje";
	print_test("Insertar clave1 y valor1", abb_guardar(arbol3,clave1,valor1));
	print_test("Insertar clave2 y valor 2", abb_guardar(arbol3,clave2,valor2));
	print_test("Cantidad es 2", abb_cantidad(arbol3)==2);
	print_test("Pertenece clave1 es true", abb_pertenece(arbol3,clave1));
	print_test("Pertenece clave2 es true", abb_pertenece(arbol3,clave2));
	print_test("Obtener clave1 es valor1", abb_obtener(arbol3,clave1)==valor1);
	print_test("Obtener clave2 es valor2", abb_obtener(arbol3,clave2)==valor2);
	print_test("Borrar clave1 es valor1", abb_borrar(arbol3,clave1)==valor1);
	print_test("Borrar clave2 es valor2", abb_borrar(arbol3,clave2)==valor2);
	print_test("Cantidad es 0", abb_cantidad(arbol3)==0);
	abb_destruir(arbol3);
	printf("\n");

}

void prueba_muchos_elementos(size_t largo){
	fputs("### INICIO DE PRUEBAS CON MUCHOS ELEMENTOS ###\n",stdout);
	//Inserto 'cantidad_elementos' veces el mismo par clave,valor
	abb_t* arbol4 = abb_crear(strcmp,free);
	//FUNCION EXTRAIDA DE LAS PRUEBAS DE LA CATEDRA DEL HASH.
	const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);
	unsigned* valores[largo];
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        for(size_t j=0; j<largo_clave-1; j++)
        {
            int x = 'A' + (rand() % 26);
            claves[i][j] = (char) x;
        }
        claves[i][largo_clave-1] = '\0';
        *valores[i] = i;
        ok = abb_guardar(arbol4, claves[i], valores[i]);
        if (!ok) break;
    }
    print_test("Se insertaron muchos elementos", ok);
	print_test("Cantidad es largo", abb_cantidad(arbol4)==largo);
	bool pertenece;
	for(size_t j=0; j<largo;j++){
		pertenece = abb_pertenece(arbol4,claves[j]);
	}
	print_test("Las claves pertenecen",pertenece);
	bool obtener;
	for(size_t x=0;x<largo;x++){
		obtener = abb_obtener(arbol4,claves[x])==valores[x];
	}
	print_test("Obtener devuelve valores correctos", obtener);
	bool borrados = true;
	for(size_t i=0;i<largo;i++){
		borrados = abb_borrar(arbol4,claves[i])==valores[i];
	}
	print_test("Se borraron 'largo' elementos", borrados);
	print_test("Cantidad es 0", abb_cantidad(arbol4)==0);
	abb_destruir(arbol4); //No se destruyen los datos, creo otro arbol que si destruye
	abb_t* abb = abb_crear(strcmp, free);
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }
    free(claves);
    print_test("Destruir", true);
    abb_destruir(abb);
	printf("\n");

}

void pruebas_iter_abb_vacio(){
	fputs("### INICIO DE PRUEBAS CON ITER ABB VACIO ###\n",stdout);
	abb_t* arbol5 = abb_crear(strcmp,NULL);
	abb_iter_t* iter1 = abb_iter_in_crear(arbol5);
	print_test("Se creo un iterador",iter1!=NULL);
	print_test("Iterador esta al final",abb_iter_in_al_final(iter1));
	print_test("Avanzar en abb vacio es false", !abb_iter_in_avanzar(iter1));
	print_test("Ver actual es NULL", abb_iter_in_ver_actual(iter1)==NULL);
	abb_destruir(arbol5);
	abb_iter_in_destruir(iter1);
	printf("\n");
}

void pruebas_iter_algunos_elementos(){
	fputs("### INICIO DE PRUEBAS CON ITER ABB ALGUNOS ELEMENTOS ###\n",stdout);
	abb_t* arbol6 = abb_crear(strcmp,NULL);
	char* claves[5] = {"1","2","3","4","5"};
	char* valores[5] = {"programacion","wachenchauzer","busqueda","juli"};
	//Inserto 5 pares clave,valor
	bool ok;
	for(int i=0;i<5;i++){
		ok = abb_guardar(arbol6,claves[i],valores[i]);
	}
	print_test("Se insertaron 5 elementos",ok);
	print_test("Cantidad es 5", abb_cantidad(arbol6)==5);
	abb_iter_t* iter2 = abb_iter_in_crear(arbol6);
	print_test("Se creo un iter", iter2!=NULL);
	//Como itera INORDER, el actual es el mas chico de las claves.
	print_test("Ver actual es '1'", atoi(abb_iter_in_ver_actual(iter2))==atoi(claves[0]));
	print_test("Avanzo un lugar",abb_iter_in_avanzar(iter2));
	print_test("Iter no esta al final", !abb_iter_in_al_final(iter2));
	print_test("Ver actual es '2'", atoi(abb_iter_in_ver_actual(iter2))==atoi(claves[1]));
	abb_iter_in_avanzar(iter2);
	abb_iter_in_avanzar(iter2);
	abb_iter_in_avanzar(iter2);
	abb_iter_in_avanzar(iter2);
	print_test("Avanza el iterado, Esta al final", abb_iter_in_al_final(iter2));
	abb_iter_in_destruir(iter2);
	abb_destruir(arbol6);
	print_test("Arbol e iter destruidos",true);
	printf("\n");
}

void pruebas_iterar_volumen(size_t largo){
	//USO LA FUNCION DE LAS PRUEBAS DE LA CATEDRA DEL HASH
	fputs("### INICIO DE PRUEBAS CON ITER VOLUMEN ###\n",stdout);
	abb_t* arbol7 = abb_crear(strcmp,free);
	const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);
	unsigned* valores[largo];
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        for(size_t j=0; j<largo_clave-1; j++)
        {
            int x = 'A' + (rand() % 26);
            claves[i][j] = (char) x;
        }
        claves[i][largo_clave-1] = '\0';
        *valores[i] = i;
        ok = abb_guardar(arbol7, claves[i], valores[i]);
        if (!ok) break;
    }
    print_test("Se insertaron muchos elementos", ok);
	print_test("Cantidad es largo", abb_cantidad(arbol7)==largo);
	abb_iter_t* iter3 = abb_iter_in_crear(arbol7);
	print_test("Se creo un iterador", iter3 != NULL);
	bool avance;
	for(size_t j= 0; j<largo;j++){
		avance = abb_iter_in_avanzar(iter3);
	}
	print_test("Iter avanzo 'largo' veces", avance);
	print_test("Iter esta al final", abb_iter_in_al_final(iter3));
	abb_iter_in_destruir(iter3);
	free(claves);
	abb_destruir(arbol7);

}

void pruebas_abb_alumno(){
	/*Ejecuta todas las funciones*/
	prueba_crear_arbol_vacio();
	prueba_abb_null();
	prueba_algunos_elementos();
	prueba_muchos_elementos(5000);
	pruebas_iter_abb_vacio();
	pruebas_iter_algunos_elementos();
	pruebas_iterar_volumen(5000);
}