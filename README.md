# ppt-bimestre-2

Un juego interactivo por consola que permite jugar Piedra-Papel-Tijera en dos modalidades: **clásica** y **modo secreto** (Piedra, Papel, Tijera, Lagarto, Spock). Incluye sistema de guardado de estadísticas, historial de jugadores y marcadores persistentes.

---

##  Tabla de Contenidos
- [Características](#-características)
- [Requisitos](#-requisitos)
- [Cómo Jugar](#-cómo-jugar)
- [Modos de Juego](#-modos-de-juego)
- [Estructura del Código](#-estructura-del-código)
- [Archivos Generados](#-archivos-generados)
- [Comandos del Menú](#-comandos-del-menú)
- [Explicación del Código](#-explicación-del-código)
- [Contribución](#-contribución)

---

##  Características

- **Dos modos de juego**: Clásico (3 opciones) y Secreto (5 opciones)
- **Dos tipos de partida**: 
  - Jugador vs CPU
  - Jugador 1 vs Jugador 2 (local)
- **Sistema de persistencia**: Guarda automáticamente las estadísticas
- **Historial de jugadores**: Top 10 con mejores victorias
- **Interfaz por consola**: Menú intuitivo y fácil de usar
- **Validación de entrada**: Protegido contra entradas inválidas
- **Paso por referencia**: Modifica objetos originales sin copias innecesarias
- **Archivos de texto**: Almacenamiento simple y legible

---

##  Requisitos

- **Compilador C++11 o superior** (GCC, Clang, MSVC)
- **Sistema operativo**: Windows, Linux o macOS
- **Permisos de escritura** en el directorio de ejecución (para archivos de guardado)

---

Cómo Jugar
Inicia el programa y los jugadores existentes se cargarán automáticamente.

1. Si es la primera vez, ingresa los nombres de los jugadores.

2. Elige una opción del menú (0-7).

3. Sigue las instrucciones en pantalla para cada modo de juego.

4, Las estadísticas se guardan automáticamente después de cada partida.
 Modos de Juego
Modo Clásico (Opciones 1 y 2)
Opciones: 1 = Piedra, 2 = Papel, 3 = Tijera

Reglas:

Piedra vence a Tijera

Tijera vence a Papel

Papel vence a Piedra

Modo Secreto (Opciones 3 y 4)
Opciones: 1 = Piedra, 2 = Papel, 3 = Tijera, 4 = Lagarto, 5 = Spock

Reglas (basado en The Big Bang Theory):

Tijera vence a Papel y Lagarto

Papel vence a Piedra y Spock

Piedra vence a Tijera y Lagarto

Lagarto vence a Spock y Papel

Spock vence a Tijera y Piedra

Estructura del Código

```text
├── jugador (struct) # Datos del jugador actual
│   ├── nombre
│   ├── eleccion
│   ├── victoriasCPU
│   ├── derrotasCPU
│   ├── empatesCPU
│   ├── victoriasVS
│   ├── derrotasVS
│   └── empatesVS
│
├── historialJugador (struct) # Datos para el historial
│   ├── nombre
│   ├── victoriasCPU
│   ├── derrotasCPU
│   ├── empatesCPU
│   ├── victoriasVS
│   ├── derrotasVS
│   ├── empatesVS
│   └── totalPartidas
│
├── Funciones de utilidad
│   ├── limpiarBuffer()       # Limpia el buffer de entrada
│   ├── obtenerEntradaValida()# Valida entrada numérica
│   └── imprimirMenu()        # Muestra el menú principal
│
├── Funciones de juego
│   ├── jugarContraCPU()      # Modo vs CPU
│   └── jugarVS()             # Modo vs Jugador
│
├── Funciones de evaluación
│   ├── evaluarNormal()       # Lógica clásica (3 opciones)
│   └── evaluarSecreto()      # Lógica extendida (5 opciones)
│
├── Funciones de persistencia
│   ├── guardarMarcadores()   # Guarda en "Juego actual.txt"
│   ├── cargarMarcadores()    # Carga desde archivo
│   ├── guardarEnHistorial()  # Actualiza historial global
│   ├── guardarJugadoresEnHistorial()
│   └── cargarHistorial()     # Carga historial general
│
├── Funciones de gestión
│   ├── resetearEstadisticas()# Reinicia estadísticas de sesión
│   ├── cambiarJugadores()    # Cambia jugadores activos
│   └── mostrarHistorial()    # Muestra el top 10
│
└── main()                    # Bucle principal del juego
```
##  Archivos Generados

El programa crea y administra automáticamente los siguientes archivos en formato CSV para guardar la información:

| Archivo | Descripción |
| :--- | :--- |
| **`Juego actual.txt`** | Estadísticas de la sesión activa (mantiene la persistencia entre ejecuciones). |
| **`historial.txt`** | Top 10 jugadores con mejores victorias acumuladas de por vida. |

### Estructura de guardado (`Juego actual.txt` / `historial.txt`)
```csv
Jugador,Vic_CPU,Der_CPU,Emp_CPU,Vic_VS,Der_VS,Emp_VS
Juan,5,3,2,4,1,0
María,3,4,1,2,2,1
```
Opción	Descripción
0	Salir (guarda automáticamente)
1	Jugar vs CPU (modo clásico)
2	Jugar J1 vs J2 (modo clásico)
3	Jugar vs CPU (modo secreto)
4	Jugar J1 vs J2 (modo secreto)
5	Ver marcadores de la sesión actual
6	Cambiar jugadores (guarda los anteriores en historial)
7	Ver historial de todos los jugadores (Top 10)

Explicación del Código

1. Estructuras de Datos
cpp
typedef struct {
    string nombre;
    int eleccion;
    int victoriasCPU = 0;
    int derrotasCPU = 0;
    int empatesCPU = 0;
    // ... más estadísticas
} jugador;
Propósito: Almacena toda la información de un jugador durante la sesión actual.

2. Validación de Entrada
cpp
int obtenerEntradaValida(const string& mensaje, int min, int max) {
    int opcion;
    cout << mensaje;
    cin >> opcion;
    while (cin.fail() || opcion < min || opcion > max) {
        limpiarBuffer();
        cout << "Entrada invalida. Ingrese un numero valido (" << min << "-" << max << "): ";
        cin >> opcion;
    }
    return opcion;
}
Propósito: Asegura que el usuario ingrese solo números dentro del rango permitido.

3. Evaluación de Jugadas
cpp
int evaluarNormal(int j1, int j2) {
    if (j1 == j2) return 0;  // Empate
    if (j1 + 1 == j2 || j1 - 2 == j2) return 2;  // Gana J2
    return 1;  // Gana J1
}
Propósito: Determina el ganador en el modo clásico sin usar condicionales complejos.

4. Paso por Referencia
cpp
void jugarContraCPU(jugador& j1, jugador& j2, bool secreto) {
    // ...
    actual.victoriasCPU++;  // Modifica el objeto original
    // ...
}
Propósito: Las estadísticas se actualizan directamente en los objetos originales, no en copias.

5. Persistencia de Datos
cpp
void guardarMarcadores(jugador& j1, jugador& j2) {
    string path = "Juego actual.txt";
    ofstream file(path);
    // Escribe los datos en formato CSV
}
Propósito: Guarda las estadísticas en un archivo de texto para cargarlas en futuras ejecuciones
