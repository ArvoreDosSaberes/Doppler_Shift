[![CI](https://github.com/ArvoreDosSaberes/Doppler_Shift/actions/workflows/ci.yml/badge.svg)](https://github.com/ArvoreDosSaberes/Doppler_Shift/actions/workflows/ci.yml)
![visitors](https://visitor-badge.laobi.icu/badge?page_id=ArvoreDosSaberes.Doppler_Shift)
[![License: CC BY-SA 4.0](https://img.shields.io/badge/License-CC_BY--SA_4.0-blue.svg)](https://creativecommons.org/licenses/by-sa/4.0/)
![Language: Portuguese](https://img.shields.io/badge/Language-Portuguese-brightgreen.svg)
[![Language-C](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![CMake](https://img.shields.io/badge/build-CMake-informational.svg)](https://cmake.org/)
[![Raylib](https://img.shields.io/badge/graphics-raylib-2ea44f.svg)](https://www.raylib.com/)
[![Issues](https://img.shields.io/github/issues/ArvoreDosSaberes/Doppler_Shift.svg)](https://github.com/ArvoreDosSaberes/Doppler_Shift/issues)
[![Stars](https://img.shields.io/github/stars/ArvoreDosSaberes/Doppler_Shift.svg)](https://github.com/ArvoreDosSaberes/Doppler_Shift/stargazers)
[![Forks](https://img.shields.io/github/forks/ArvoreDosSaberes/Doppler_Shift.svg)](https://github.com/ArvoreDosSaberes/Doppler_Shift/network/members)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](https://makeapullrequest.com)
[![Watchers](https://img.shields.io/github/watchers/ArvoreDosSaberes/Doppler_Shift)](https://github.com/ArvoreDosSaberes/Doppler_Shift/watchers)
[![Last Commit](https://img.shields.io/github/last-commit/ArvoreDosSaberes/Doppler_Shift)](https://github.com/ArvoreDosSaberes/Doppler_Shift/commits)
[![Contributors](https://img.shields.io/github/contributors/ArvoreDosSaberes/Doppler_Shift)](https://github.com/ArvoreDosSaberes/Doppler_Shift/graphs/contributors)

# Doppler_Shift — Simulador 3D do Efeito Doppler (Raylib)

Este projeto demonstra o efeito Doppler em 3D usando a biblioteca gráfica Raylib.
Permite variar ângulo, velocidade, distância e alternar entre fonte móvel e receptor móvel.

## Requisitos
- CMake >= 3.16
- Compilador C (GCC/Clang/MSVC)
- Internet para baixar a dependência `raylib` automaticamente via `FetchContent` (ou `raylib` instalado no sistema)

## Compilação

Em Linux/macOS/Windows (PowerShell), a sequência padrão:

```bash
# Na pasta do projeto Doppler_Shift/
mkdir -p build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

O executável será gerado em `build/doppler_shift` (ou `build/Release/doppler_shift.exe` no Windows/MSVC).

## Execução

```bash
./build/doppler_shift
```

## Controles
- Setas Esquerda/Direita: ajustam ângulo (graus)
- Setas Cima/Baixo: ajustam velocidade (m/s)
- PageUp/PageDown: ajustam distância (m)
- [ e ]: ajustam frequência base (Hz)
- S: alterna entre “fonte móvel” e “receptor móvel”
- H: mostra/oculta ajuda
- Mouse (botão direito pressionado): orbitar câmera
- Scroll: zoom (campo de visão)

## Conceito (resumo)
- Velocidade do som (aprox.): 343 m/s (ar a 20°C)
- Componente radial da velocidade: `v_rad = v * cos(ângulo)`
- Fonte móvel: `f' = f0 * c / (c - v_s,rad)`
- Receptor móvel: `f' = f0 * (c + v_r) / c`

Para uma explicação detalhada e exemplos de uso, consulte `TUTORIAL.md`.

## Documentação (Doxygen)

Este projeto inclui um `Doxyfile` e um alvo CMake `doc` (se o Doxygen estiver instalado ao configurar o projeto).

Gerar documentação HTML:

```bash
# Reconfigure para detectar o Doxygen (se instalou depois)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Gerar docs
cmake --build build --target doc -j

# Abra em:
xdg-open docs/html/index.html  # Linux
```

O Doxygen processa:
- `README.md` (usado como página inicial)
- `TUTORIAL.md`
- Arquivos-fonte em `src/`
