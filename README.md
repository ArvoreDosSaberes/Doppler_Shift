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
