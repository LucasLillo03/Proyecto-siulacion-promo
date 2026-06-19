# PowerDEVS - Simulacion de Bomba de Infusion

Simulacion de un sistema de bomba de infusion utilizando PowerDEVS. El modelo simula el flujo de medicacion, sensores de fin de bolsa, confirmacion de enfermero, ordenes medicas, modulo de alarmas y actuadores, en un entorno de eventos discretos.
Los parametros seran cargados desde el directorio /escenarios donde estaran dispuestos varios archivos de codigo scilab para ser cargados en el workspace de scilab-powerDEVS. Los distintos DEVS atomicos accederan a los valores de los parametros a traves de la funcion getScilabVar (esta decision de implementacion fue tomada con el objetivo de facilitar la simulacion de distintos escenarios).

## Estructura del directorio

```
mysources/
├── README.md                      # Este archivo
├── cambios_especificacion.md      # Documentacion de cambios en la especificacion
├── constantes.h                   # Constantes globales de la simulacion
├── random_utils.h                 # Funciones de utilidad para generacion aleatoria
├── tipos.h                        # Definiciones de tipos compartidos
├── set_semilla.cpp / .h           # Inicializacion de semilla aleatoria
├── gen_orden_medica.cpp / .h      # Generador de ordenes medicas
├── gen_alarmas.cpp / .h           # Generador de alarmas
├── gen_alarmacritica.cpp / .h     # Generador de alarmas criticas
├── sen_flujo.cpp / .h             # Sensor de flujo
├── sen_fin_bolsa.cpp / .h         # Sensor de fin de bolsa
├── sen_confirmacion_enfermero.cpp / .h  # Sensor de confirmacion de enfermero
├── actuador_bomba.cpp / .h        # Actuador de bomba
├── accionador_bolsa.cpp / .h      # Accionador de bolsa
├── modulo_alarmas.cpp / .h        # Modulo de alarmas
├── outputs/                       # Archivos generados por la simulacion
├── visualizer/                    # App Electron para visualizacion de graficos
│   ├── package.json
│   ├── main.js
│   ├── preload.js
│   └── src/
│       ├── index.html
│       ├── renderer.js
│       └── styles.css
├── Model 01.pdm                   # Modelo PowerDEVS
├── Model 01.pds                   # Configuracion del modelo
└── Model 01.stm                   # Maquina de estados
```

## Fuentes de simulacion

| Archivo | Descripcion |
|---------|-------------|
| `constantes.h` | Define las constantes de configuracion (capacidad de bolsa, umbrales, tiempos, etc.) |
| `tipos.h` | Tipos de datos compartidos entre los distintos componentes |
| `random_utils.h` | Funciones auxiliares para generacion de numeros aleatorios |
| `set_semilla` | Inicializa la semilla del generador aleatorio desde Scilab |
| `gen_orden_medica` | Genera ordenes medicas con caudal recetado y periodicidad configurable |
| `gen_alarmas` | Genera eventos de alarma general |
| `gen_alarmacritica` | Genera eventos de alarma critica |
| `sen_flujo` | Sensor que mide el flujo actual de medicacion |
| `sen_fin_bolsa` | Sensor que detecta cuando la bolsa se vacia |
| `sen_confirmacion_enfermero` | Sensor que recibe la confirmacion del enfermero |
| `actuador_bomba` | Actuador que controla la bomba de infusion |
| `accionador_bolsa` | Accionador que repone la bolsa de medicacion |
| `modulo_alarmas` | Modulo logico que procesa y emite alarmas |

## Outputs de simulacion

## Visualizer (App Electron) (TO DO: hacer script que levante todo desde la raiz)

Aplicacion de escritorio para visualizar graficamente los CSVs generados por la simulacion.

### Requisitos

- Node.js >= 18
- npm

### Instalacion

```bash
cd visualizer
npm install
```

### Ejecucion

```bash
cd visualizer
npm start            # modo normal (requiere sandbox configurado)
npm run start:dev    # modo sin sandbox (alternativa para Linux)
```

> En Linux puede aparecer el error `The SUID sandbox helper binary was found, but is not configured correctly`.
> Usar `npm run start:dev` para evitarlo, o configurar el sandbox con:
> `sudo chown root node_modules/electron/dist/chrome-sandbox && sudo chmod 4755 node_modules/electron/dist/chrome-sandbox`

### Uso

1. Al abrir la app, se cargan automaticamente los archivos `.csv` del directorio `outputs/`
2. Seleccionar los archivos a visualizar
3. Plotly renderiza un grafico interactivo

### Arquitectura

```
main.js (Electron main process)
  ├── IPC handler: get-csv-list (escanea outputs/)
  ├── IPC handler: read-csv    (lee archivo)
  └── Crea BrowserWindow con preload.js

preload.js
  └── contextBridge expone window.api.readCSV() y window.api.getCSVList()

src/index.html
  ├── Plotly.js (CDN) para graficos interactivos
  ├── PapaParse (CDN) para parseo de CSV
  └── renderer.js (logica de negocio)
```

### Tecnologias

- **Electron** - Entorno de escritorio
- **Plotly.js** - Graficos interactivos (zoom, pan, exportacion)
- **PapaParse** - Parseo de archivos CSV
- **contextBridge** - Comunicacion segura entre procesos
