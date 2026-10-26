# UNO++ The Card Game

Juego de UNO para terminal escrito en C++17. Incluye baraja completa, turnos,
cartas especiales, IA y reciclado del mazo de descarte.

## Estado

- [x] Crear la baraja (112 cartas)
- [x] Ver la baraja
- [x] Barajar
- [x] Sistema de turnos
- [x] Cambio de color (comodín)
- [x] Cambio de sentido (reversa)
- [x] Pila de juego / descarte
- [x] Cartas especiales +2 y +4
- [x] Jugadores IA
- [ ] Gritar UNO
- [ ] Juego online

## Requisitos

- Compilador con C++17 (`g++` o `clang++`)
- `make`
- Terminal de al menos 80x30
- Ejecutar desde la raíz del proyecto (los textos viven en `resources/`)

## Compilar y ejecutar

```sh
make          # compila en build/ y genera ./game_uno++
make run      # compila si hace falta y arranca el juego
./game_uno++  # ejecución directa
make clean    # borra build/ y el binario
```

La compilación usa `-Wall -Wextra -Wpedantic`. El binario y `build/` están
ignorados por git.

## Tests

```sh
make test
```

Las pruebas cubren la composición de la baraja, las reglas de `CanPlay`, el
robo/descarte del jugador, el reparto inicial y partidas simuladas de 2 a 8
jugadores (siempre terminan y ninguna carta se pierde).

## Controles

En el menú principal se elige la opción con su número. Dentro de una partida
contra la IA:

- `1` poner una carta (se pide la posición, desde 1)
- `2` coger una carta
- `3` opciones de depuración (muestra todas las manos)
- `0` abandonar la partida

Al jugar un comodín se elige el color de la mesa.

## Reglas implementadas

- Se puede jugar una carta del mismo color o del mismo tipo; los comodines
  siempre se pueden jugar.
- La partida empieza con una carta de número.
- Reversa invierte el sentido (con dos jugadores actúa como bloqueo).
- Bloqueo salta al siguiente jugador.
- +2 y +4 hacen robar al siguiente jugador y le saltan el turno.
- Si el mazo se agota, se rebarajan todas las cartas del descarte menos la de
  arriba.
- La IA prioriza cartas no comodín y, si juega un comodín, elige el color que
  más tiene.
- Gana quien se queda sin cartas.

## Estructura

| Archivo | Responsabilidad |
| --- | --- |
| `src/card_class.*` | `Card`, `Color`, `Type` y formato de cartas |
| `src/card_pack_class.*` | baraja/mano, barajado y `CanPlay` |
| `src/player_class.*` | nombre, mano y operaciones de robo/descarte |
| `src/game_class.*` | motor: turnos, efectos, reciclado y ganador |
| `src/game_ia.cc` | interfaz de la partida contra la IA |
| `src/uno_main.cc` | menú principal |
| `src/uno_tools.*` | entrada validada, limpieza de pantalla y recursos |
| `src/ansi_colors.h` | constantes ANSI (`ansi::`) |
| `resources/` | logo y textos por secciones |
| `tests/uno_test.cc` | pruebas sin dependencias externas |

## Autor

Tomás Pino Pérez.
