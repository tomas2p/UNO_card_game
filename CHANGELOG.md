# Changelog

Todos los cambios relevantes de este proyecto se documentan en este archivo.

El formato sigue [Keep a Changelog](https://keepachangelog.com/es-ES/1.1.0/)
y el proyecto usa [Versionado Semántico](https://semver.org/lang/es/).

## [Unreleased]

### Añadido

- Licencia GPL-3.0-or-later.
- Enlace al portfolio y firma `@tomas2p` en el logo del README.
- Demo GIF del juego en el README.

## [1.0.0] - 2026-10-26

Primera versión publicada: juego completo para un jugador contra la IA.

### Añadido

- Baraja UNO completa de 112 cartas y barajado.
- Motor de juego con turnos, sentido de juego y color actual.
- Reversa (bloqueo con dos jugadores), bloqueo, +2 y +4.
- Reciclado del mazo de descarte cuando se agota.
- Sistema de gritar UNO++ con penalización de dos cartas.
- Regla oficial del +4: solo se puede jugar si no tienes el color actual.
- Jugadores IA que priorizan cartas no comodín y eligen el color mayoritario.
- Versión embebida en el binario y flags `--version` y `--help`.
- Integración continua con gcc y clang, tests y artefacto del binario.
- Pruebas de baraja, reglas, jugador, motor y partidas simuladas.

### Cambiado

- Modelo de carta con `enum class` y tablas estáticas (de 648 B a 8 B).
- Una única representación de la baraja, sin vectores de color duplicados.
- Formato de cartas compartido entre la mano y el descarte.
- `CardPack` nace vacío y expone `FullDeck()`.
- `Player` se limita a gestionar su mano; las reglas viven en `Game`.
- Entrada validada con `AskInt` y limpieza de pantalla con secuencias ANSI.
- Textos y recursos localizados por secciones con nombre.

### Corregido

- Índices fuera de rango al jugar o robar cartas.
- Fugas de memoria y uso de VLA para la lista de jugadores.
- Doble inversión de turnos al jugar una reversa.
- `PressKey` bloqueado al llegar a EOF.
- Selector de color recursivo y con salida inválida.
- `CardPack::size()` desincronizado del contenido real.
- Macros ANSI que colisionaban con las cabeceras estándar.

### Eliminado

- `system("clear")` y los macros ANSI globales.
- Código muerto: `game_online.cc`, `EventOptions` y textos sin uso.
- Binario `game_uno++` versionado en git.
