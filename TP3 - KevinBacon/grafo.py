##IMPLEMENTACION DE LA CLASE GRAFO.
#EMPIEZO COMO DICCIONARIO DE DICCIONARIOS, SI PREFERIS OTRA COSA MODIFICAMOS

class Grafo:

	"""Inicializacion de la clase Grafo"""
	def __init__(self):
		"""Constructor de un grafo vacio. Crea un diccionario de vertices"""
		self.vertices = {}

	def agregar_vertice(self, nombre_vertice):
		"""Recibe el grafo y un vertice. Si el vertice no se encuentra en el grafo, 
		lo agrega."""
		if nombre_vertice in self.vertices:
			return
		self.vertices[nombre_vertice] = {}

	def eliminar_vertice(self, vertice_a_borrar):
		"""Borra el vertice 'vertice_a_borrar' del grafo.
		Ademas, hay que eliminar las aristas que tuviesen al vertice borrado
		como origen o destino"""
		if not vertice_a_borrar in self.vertices:
			return
		for vertice_ady in self.vertices:
			self.vertices[vertice_ady].pop(vertice_a_borrar)
		self.vertices.pop(vertice_a_borrar)

	def agregar_arista(self, vertice_origen, vertice_destino, nombre):
		"""Une los vertices recibidos mediante una arista a crear, cuyo nombre es
		el nombre recibido"""

		self.vertices[vertice_origen] = Arista(vertice_origen, vertice_destino,nombre)
		#Sin clase arista seria algo asi CREO
		self.vertices[vertice_origen][vertice_destino] = nombre
		self.vertices[vertice_destino][vertice_origen] = nombre


	def eliminar_arista(self, vertice_origen, vertice_destino):
		"""Borra la arista que une los dos vertices recibidos por parametro"""
		
		if not (vertice_origen or vertice_destino) in self.vertices:
			return
		self.vertices[vertice_origen].pop(vertice_destino)
		self.vertices[vertice_destino].pop(vertice_origen)

	def obtener_adyacentes(self, vertice):
		"""Devuelve una lista de todos los vertices adyacentes al vertice recibido por
		parametro."""

		lista_vertices_ady = []
		for ady in self.vertices[vertice]:
			lista_vertices_ady.append(ady)
		return lista_vertices_ady

	def chequear_adyacencia(self, vertice1, vertice2):
		"""Devuelve un booleano segun si los vertices dados son adyacentes o no"""

		return vertice1 in self.vertices[vertice2] and vertice2 in self.vertices[vertice1]

	def obtener_vertices(self):
		"""Devuelve una lista de todos los vertices presentes en el grafo"""
		return self.vertices.keys()

	def obtener_aristas(self, vertice1, vertice2):
		"""Devuelve la informacion que contiene la arista que une dos vertices"""
		return self.vertices[vertice1][vertice2] #sin usar clase arista
		#usando clase arista seria algo asi creo
		arista = Arista(vertice1,vertice2)
		return arista.nombre_arista

	def __str__(self):

		vertices = self.vertices
		return str(vertices)

#################################################################################3
class Arista: #Hice esta clase xq fede dijo que era recomendable

	def __init__(self, v_origen, v_destino, nombre = None):
		"""Constructor de una Arista que une a los vertices recibidos.
		Dicha arista tiene nombre."""
		self.v_origen = v_origen
		self.v_destino = v_destino
		self.nombre_arista = nombre

		#Uno los vertices
		self.vertices[v_origen][v_destino] = nombre_arista
		self.vertices[v_destino][v_origen] = nombre_arista

	def nombre_arista(self, arista):

		return self.arista.nombre_arista




