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

Ver [`docs/estrutura-repositorio.md`](docs/estrutura-repositorio.md) para a descrição
completa de cada pasta. Resumo:

| Pasta | Conteúdo |
|---|---|
| `monografia/` | Fonte LaTeX completo do TCC |
| `gpsr-module/` | Módulo GPSR modernizado (pronto para `ns-3/src/gpsr`) |
| `legacy-reference/` | Código original, preservado para comparação |
| `scenarios/` | Scripts do cenário experimental de VANET |
| `analysis/` | Scripts de leitura e plotagem dos dados do FlowMonitor |
| `results/` | Dados brutos e processados dos experimentos |
| `docs/` | Diário de bordo, logs de diagnóstico, documentação de ambiente |

## Como compilar o módulo

Guia completo em [`docs/ambiente/INSTALL.md`](docs/ambiente/INSTALL.md). Resumo:

```bash
# 1. Clonar o ns-3 (versão alvo: ver INSTALL.md)
# 2. Copiar o módulo modernizado para dentro do ns-3
cp -r gpsr-module ns-3/src/gpsr

# 3. Configurar e compilar
cd ns-3
./ns3 configure --enable-examples --enable-tests
./ns3 build
```

## Como rodar os experimentos

```bash
cd scenarios
./run-all-experiments.sh
```

Isso executa as 30 repetições por cenário e protocolo descritas na Seção 4.7 da
Metodologia, salvando os XMLs do FlowMonitor em `results/raw/`.

## Como gerar os gráficos e análises

```bash
cd analysis
pip install -r requirements.txt
python parse_flowmonitor.py
python plot_metricas.py
```

## Licença

Este módulo é derivado de código do ns-3 e da implementação original de roteamento
geográfico referenciada em `legacy-reference/README.md`. Distribuído sob a **GPLv2**, a
mesma licença do ns-3 — ver [`LICENSE`](LICENSE).

## Como citar

Ver [`CITATION.cff`](CITATION.cff).