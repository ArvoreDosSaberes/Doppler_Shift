# TUTORIAL — Simulador 3D do Efeito Doppler

Este tutorial apresenta o conceito do efeito Doppler, como o simulador representa o fenômeno e um passo a passo para explorar diferentes cenários.

## 1. Conceito

- O efeito Doppler descreve a variação da frequência (ou comprimento de onda) observada quando há movimento relativo entre fonte e receptor.
- Em meios acústicos (som no ar), a velocidade de propagação (c) ≈ 343 m/s a 20°C.
- A componente radial da velocidade (alinhada à linha de visada) é a que produz o desvio de frequência:
  - v_rad = v * cos(ângulo)
- Fórmulas usadas no simulador:
  - Fonte em movimento (receptor fixo):
    - f' = f0 * c / (c - v_s,rad)
  - Receptor em movimento (fonte fixa):
    - f' = f0 * (c + v_r) / c

Observações: sinais e convenções importam. Neste simulador, o ângulo 0° coloca a velocidade no sentido da linha fonte→receptor.

## 2. Interface do simulador

- Cena 3D com fonte (esfera laranja) e receptor (esfera azul) alinhados no eixo X.
- Linha de visada (LOS) em branco entre fonte e receptor.
- Vetor de velocidade (seta) sobre o elemento que está se movendo (fonte ou receptor).
- Um arco amarelo indica o ângulo entre a LOS e o vetor de velocidade.
- HUD com os parâmetros atuais e frequência observada calculada.
- Opcionalmente, miniaturas de imagens PNG (se estiverem na pasta) no canto direito.

## 3. Controles

- Setas Esquerda/Direita: ajustar ângulo (°)
- Setas Cima/Baixo: ajustar velocidade (m/s)
- PageUp/PageDown: ajustar distância (m)
- [ e ]: ajustar frequência base (Hz)
- S: alternar entre “fonte móvel” e “receptor móvel”
- H: alternar ajuda/explicações
- Espaço: alternar miniaturas de PNGs
- Mouse (botão direito): orbitar a câmera
- Scroll: zoom (campo de visão)

Dicas:
- Comece com ângulo = 0° para máxima componente radial.
- Aproxime a velocidade do som (sem ultrapassá-la) para observar desvios acentuados.
- Explore ângulos próximos a 90° para observar como a componente radial tende a zero e a frequência observada volta ao valor base.

## 4. Experimentos sugeridos

1) Aproximação direta:
- Modo: Fonte móvel
- Ângulo: 0°
- Velocidade: 30 m/s
- Distância: 5 m
- Observe f' > f0 (aproximação aumenta a frequência observada)

2) Afastamento direto:
- Modo: Fonte móvel
- Ângulo: 180° (velocidade oposta à LOS)
- Velocidade: 30 m/s
- Observe f' < f0

3) Movimento transversal:
- Ângulo: 90°
- Qualquer velocidade moderada (20–40 m/s)
- Observe f' ≈ f0 (componente radial ≈ 0)

4) Receptor móvel:
- Pressione S para alternar para receptor móvel
- Repita (1)–(3) e compare as diferenças das fórmulas

## 5. Interpretação dos resultados

- A magnitude do desvio depende da componente radial e da relação v/c.
- Em velocidades subsônicas usuais, f' varia suavemente com o ângulo.
- Próximo de c, a fórmula para fonte móvel tem assimetria mais perceptível (evite ultrapassar c no simulador por realismo). O código faz um clamp numérico para evitar divisão por zero.

## 6. Como estender

- Gráficos de espectro/áudio: usar um gerador de seno com `f'` e tocar via áudio para ouvir o efeito.
- Trajetória temporal: animar as posições e calcular f'(t).
- Múltiplas fontes/receptores: visualizar interferências e somatório.
- Exportação: salvar parâmetros e resultados em CSV.

Boa exploração!
