# Extensibilidade do Simulador ns-3 Aplicada a Redes Veiculares

Modernização do módulo de roteamento geográfico GPSR (*Greedy Perimeter Stateless
Routing*) para uma versão atual do ns-3, com avaliação comparativa em cenário de VANET.

Trabalho de Conclusão de Curso — Bacharelado em Tecnologia da Informação, Universidade
Federal Rural do Semi-Árido (UFERSA), Centro Multidisciplinar de Pau dos Ferros.

**Autor:** Maycon Soares Maia

**Orientador:** Prof. Dr. Francisco Carlos Gurgel da Silva Segundo

---

## Resumo

O ns-3 não possui, em seu núcleo oficial, suporte a protocolos de roteamento geográfico —
recomendados na literatura para Redes Veiculares Ad Hoc (VANETs) por lidarem melhor com
cenários de alta mobilidade do que protocolos topológicos como AODV e OLSR. A única
implementação conhecida do GPSR para o ns-3 é não-oficial e está desatualizada, incompatível
com o sistema de *build* (CMake) e a API das versões atuais do simulador.

Este projeto moderniza essa implementação e avalia seu desempenho em um cenário de VANET com
mobilidade nativa do ns-3, comparando-a aos protocolos AODV e OLSR por meio do módulo
FlowMonitor (*throughput*, *delay*, PDR e *overhead* de roteamento).

## Estrutura do repositório

| Caminho | Conteúdo |
|---|---|
| `monografia/` | Fonte LaTeX completo do TCC |
| `gpsr-module/` | Módulo GPSR modernizado (pronto para `ns-3/src/gpsr`) |
| `legacy-reference/` | Código original, preservado para comparação |
| `scenarios/` | Experimento VANET e insumos do SUMO (rede, rotas, mobility trace) |
| `analysis/` | Scripts de leitura e plotagem dos dados do FlowMonitor |
| `results/` | Dados brutos e processados dos experimentos (gerado, fora do Git) |
| `docs/` | Diário de bordo e logs de diagnóstico |

## Como compilar o módulo

O módulo modernizado é um pacote CMake nativo do ns-3. Basta copiá-lo para dentro da
árvore `src/` do simulador:

```bash
# 1. Clonar o ns-3 (versão alvo: ns-3.39 ou superior, que já usa CMake)
git clone https://github.com/nsnam/ns-3-dev-git.git
cd ns-3-dev-git

# 2. Copiar o módulo modernizado para dentro do ns-3
cp -r /caminho/para/TCC-GPSR-ns-3/gpsr-module src/gpsr

# 3. Habilitar o módulo e compilar
./ns3 configure --enable-modules=gpsr --enable-examples --enable-tests
./ns3 build
```

O `gpsr-module/CMakeLists.txt` já declara a biblioteca (`build_lib` com `LIBNAME gpsr`)
e as dependências: `core`, `network`, `internet`, `wifi`, `mobility` e `applications`.

## Como rodar os experimentos

O experimento VANET está em [`scenarios/vanet-test.cc`](scenarios/vanet-test.cc). Ele lê a
mobilidade de `scenarios/trace.tcl` (gerado pelo SUMO) e aceita o protocolo por linha de
comando:

```bash
# Cenário com 300 veículos em mobilidade realista do SUMO
./ns3 run scenarios/vanet-test.cc --routing=GPSR
./ns3 run scenarios/vanet-test.cc --routing=AODV
./ns3 run scenarios/vanet-test.cc --routing=OLSR
```

Cada execução serializa as métricas do FlowMonitor em `gpsr-sumo-results.xml`.

### Artefatos do SUMO

Os arquivos de entrada versionados em `scenarios/` são:

| Arquivo | Papel |
|---|---|
| `mapa.net.xml` | Rede viária gerada pelo `netgenerate` do SUMO (412 arestas) |
| `rotas.rou.xml` | 300 definições `<trip>` (rotas dinâmicas) |
| `routes.rou.xml` | 300 rotas `<vehicle>` fixas (gera `trace.tcl` a partir delas) |

Já `scenarios/trace.tcl` (2,2 MB) e `scenarios/trace.xml` (5,9 MB) são **saídas** do SUMO
e estão no `.gitignore` por serem pesadas e regeráveis:

```bash
sumo -c scenarios/routes.rou.xml \
     --net-file scenarios/mapa.net.xml \
     --output.tcldump-output scenarios/trace.tcl
```

## Como gerar os gráficos e análises

```bash
cd analysis
pip install pandas matplotlib

# 1. Extrai as métricas do FlowMonitor para CSV
python extrair_metricas.py

# 2. Plota PDR e atraso médio do cenário corrente
python gerar_graficos.py

# 3. Compara AODV × OLSR × GPSR (espera os três CSV em analysis/)
python gerar_comparativo.py
```

Os gráficos são gravados em PDF com tipografia serifada, prontos para inclusion direta
na monografia via `\includegraphics`.

## Licença

Este módulo é derivado de código do ns-3 e da implementação original de roteamento
geográfico referenciada em `legacy-reference/`. Distribuído sob a **GPLv2**, a mesma
licença do ns-3 — ver [`LICENSE`](LICENSE).

## Como citar

aer [`CITATION.cff`](CITATION.cff).
