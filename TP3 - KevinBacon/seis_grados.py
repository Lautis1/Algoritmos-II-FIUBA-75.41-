# IMPLEMENTACION DE FUNCIONES PARA USO EN TP3 #

from collections import deque
from collections import defaultdict
from itertools import combinations
from grafo import *
import random
import csv

ESTADO_VISITADO = True
ESTADO_NO_VISITADO = False
GRADO_CONSIDERADO_PARA_POPULARIDAD = 2
FACTOR_CANT_CAMINATAS = 50
FACTOR_LARGO_CAMINATA = 5


###################################################################################
#                                    FUNCIONES                                    #
###################################################################################


def grafo_crear(nombre_archivo):
    """
    Crea un grafo de conexiones de actores a partir de un archivo de datos.

    PRE: Recibe el nombre de un archivo separado por comas que contenga de lineas:
        actor,pelicula,pelicula,pelicula
        que equivalen a: vertice,arista,arista,arista
    POST: Devuelve un grafo creado a partir de estos datos.
    """

    grafo = Grafo()
    dicc_pelis = defaultdict(dict)  # Para ahorrarse el chequeo de ver si el dic tiene cierto elemento antes de agregar.
    with open(nombre_archivo) as actores:
        lector = csv.reader(actores)
        for linea in lector:
            grafo.agregar_vertice(linea[0], linea[1:])
            for peli in linea[1:]:
                dicc_pelis[peli][linea[0]] = {}
    for pelicula in dicc_pelis:
        for actor1, actor2 in combinations(dicc_pelis[pelicula], 2):
            grafo.agregar_arista(actor1, actor2, pelicula)
    return grafo


def camino(grafo, origen, llegada):
    """
    Devuelve el camino entre un actor de origen y uno de llegada.

    PRE: Recibe el grafo, un actor de origen y un actor de llegada.
    POST: Devuelve una lista ordenada de cadenas (peliculas) para llegar desde
    el origen hasta el final. None en caso de que no halla camino desde el actor origen hasta la llegada.
    Una lista vacia en caso de que el actor de llegada sea igual al actor de origen.
    """
    if not actor_existe(grafo, origen) or not actor_existe(grafo, llegada):
        return False
    padresV, _ = buscar_grados_y_padres(grafo, origen, llegada, -1)
    if llegada not in padresV:
        return []
    recorrid_min = [llegada]
    v_actual = padresV[llegada]
    while v_actual is not None:
        recorrid_min.append(v_actual)
        v_actual = padresV[v_actual]
    # Revierto la lista de recorrido
    recorrido = list(reversed(recorrid_min))
    lista_de_tuplas = []
    for i in range(len(recorrido) - 1):
        arista_pelicula = grafo.info_arista(recorrido[i], recorrido[i + 1])[0]
        # Como puede que haya mas de una pelicula, agrego la primera
        lista_de_tuplas.append((recorrido[i], recorrido[i + 1], arista_pelicula))
    return lista_de_tuplas


def actores_a_distancia(grafo, origen, n):
    """
    Devuelve los actores a distancia n del recibido.

    PRE: Recibe el grafo, el actor de origen y el numero deseado.
    POST: Devuelve la lista de cadenas (actores) a n pasos del recibido.
    """

    if not actor_existe(grafo, origen):
        return False
    _, dicc_orden = buscar_grados_y_padres(grafo, origen, None, n)
    act_n_distantes = []
    for vertice in dicc_orden:
        if dicc_orden[vertice] == n:
            act_n_distantes.append(vertice)
    return sorted(act_n_distantes)  # Tiene que imprimirlos en orden alfabetico


def popularidad(grafo, actor):
    """
    Calcula la popularidad del actor recibido.

    PRE: Recibe el grafo y un actor de origen
    POST: Devuelve un entero que simboliza la popularidad: todos los adyacentes
        de los adyacentes del actor, multiplicado por su cantidad de peliculas
    """

    if not actor_existe(grafo, actor):
        return False
    cantidad_de_actores_grado_dos = len(actores_a_distancia(grafo, actor, GRADO_CONSIDERADO_PARA_POPULARIDAD))
    return cantidad_de_actores_grado_dos * len((grafo.obtener_info_vertice(actor)))


def similares(grafo, origen, n):
    """
    Calcula los n actores mas similares al actor de origen y los devuelve en una
    lista ordenada de mayor similitud a menor.

    PRE: Recibe el grafo, el actor de origen, y el n deseado
    POST: Devuelve una lista de los n actores no adyacentes mas similares al
        pedido. La lista no debe contener al actor de origen.
    """
    if not actor_existe(grafo, origen):
        return False

    cantidad_de_caminatas = int(len(grafo.obtener_vertices()) / FACTOR_CANT_CAMINATAS)
    largo_caminata = int(cantidad_de_caminatas / FACTOR_LARGO_CAMINATA)
    actores_similares = defaultdict(int)

    for i in range(cantidad_de_caminatas):
        actor_actual = origen
        for j in range(largo_caminata):
            actor_actual = random.choice(list(grafo.obtener_adyacentes(actor_actual)))
            actores_similares[actor_actual] += 1

    # Saco el actor de origen de el dict de actores_similares
    if origen in actores_similares:
        del actores_similares[origen]
    for adyacente in grafo.obtener_adyacentes(origen):
        if adyacente in actores_similares:
            del actores_similares[adyacente]
    # Ordeno en base a la cantidad de veces que la random_walk paso por cada actor.
    lista_similares = sorted(actores_similares, key=actores_similares.get, reverse=True)

    return lista_similares[0:n]


# FUNCIONES AUXILIARES

def buscar_grados_y_padres(grafo, origen, destino, grado_buscado):
    """ Pre: Recibe un grafo, un vertice de origen , y opcionalmente puede recibir un destino, si no se
    esta buscando un vertice especifico se le pasa None como destino. Tambien opcionalmente puede
    recibir un grado buscado, en ese caso el algoritmo seguira buscando vertices hasta que encuentre
    un vertice con grado mayor al buscado. Si no se desea buscar un grado en especial la
    funcion recibe -1 como grado_buscado. Recorre el grafo con BFS para buscar el camino minimo entre dos vertices.
    Post: Devuelve un diccionario de vertices con sus correspondientes padres como valor
    y otro diccionario con vertices como clave y sus grados como valores."""

    cola = deque()
    vertices_visitados = {}
    padre_vertices = {}
    grado_vertices = {}
    cola.append(origen)
    vertices_visitados[origen] = ESTADO_VISITADO
    padre_vertices[origen] = None
    grado_vertices[origen] = 0
    while len(cola) > 0:
        v_actual = cola.popleft()
        if v_actual == destino or (grado_vertices[v_actual] >= grado_buscado != -1):
            break
        for v_ady in grafo.obtener_adyacentes(v_actual):
            if v_ady not in vertices_visitados:
                vertices_visitados[v_ady] = ESTADO_VISITADO
                padre_vertices[v_ady] = v_actual
                grado_vertices[v_ady] = grado_vertices[v_actual] + 1
                cola.append(v_ady)
    return padre_vertices, grado_vertices


def actor_existe(grafo, actor_buscado):
    """Devuelve True o False segun si el actor buscado esta en el grafo"""
    return actor_buscado in grafo


def cantidad_de_actores(grafo):
    """Devuelve la cantidad de actores presentes en el dataset"""
    return len(grafo)


def cantidad_de_peliculas(grafo):
    """Devuelve la cantidad de peliculas presentes en el dataset"""
    peliculas = set()
    for actor in grafo:
        peliculas_actor = grafo.obtener_info_vertice(actor)
        for pelicula in peliculas_actor:
            peliculas.add(pelicula)
    return len(peliculas)
