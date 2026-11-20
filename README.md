## 🗺️ Roadmap (Próximos Passos até a v1.0)

# 📊 lot-jogar

> ⚠️ **Aviso:** Este projeto está em fase inicial de desenvolvimento (**pré-v1.0**). Funcionalidades podem mudar e melhorias ainda estão sendo implementadas.

Um aplicativo de linha de comando (CLI) modular escrito em **C++17** orientado a objetos para Linux. O programa lê a planilha oficial de resultados da **Lotofácil** (baixada manualmente do site da Caixa Econômica Federal), analisa o histórico de sorteios, compara as dezenas mais frequentes com o último concurso, realiza simulações retrospectivas (*backtesting*), gera palpites com análise de equilíbrio e compara múltiplas estratégias de jogo diretamente no terminal.

## 🚀 Funcionalidades

- **Arquitetura Modular (POO):** Código dividido em classes com responsabilidades isoladas (`Concurso`, `LeitorPlanilha`, `AnalisadorLotofacil` e `AplicacaoCLI`).
- **Leitura de Planilhas Excel (`.xlsx`):** Utiliza a biblioteca leve e rápida `xlsxio` para processar o histórico de sorteios sem depender de ferramentas pesadas, garantindo ordem cronológica automática.
- **Gráfico Comparativo (`--grafico`):** Contabiliza a frequência de cada dezena (`01` a `25`) até o penúltimo concurso, destaca com `*` as dezenas sorteadas no último concurso e exibe um resumo de quantas das 15 dezenas do topo acertaram.
- **Simulação Histórica (`--simular`):** Testa retrospectivamente quantos pontos (de 11 a 15 acertos) o grupo das 15 dezenas mais frequentes teria feito ao longo da história da Lotofácil (com histórico acumulado ou janela móvel de `N` concursos via `--simular <N>`).
- **Gerador e Conferidor de Palpites (`--palpite`):** Seleciona as 15 dezenas mais frequentes até o último concurso cadastrado (ou nos últimos `N` sorteios via `--palpite <N>`), ordena em formato de volante (`01` a `25`), exibe indicadores de equilíbrio (pares/ímpares, soma total e repetidas do concurso anterior) e informa quantas vezes esse palpite específico já foi premiado em todo o histórico.
- **Análise Comparativa de Estratégias (`--analisar`):** Avalia 5 estratégias distintas (*Quentes*, *Frias*, *Atrasadas*, *Repetição do Anterior* e *Equilibrada Par/Ímpar*), gerando 5 jogos por estratégia ao longo do histórico e exibindo o ranking comparativo junto com os 5 palpites da estratégia campeã para o próximo concurso.
- **Ajuda e Detecção Automática (`--help`):** Localiza automaticamente a planilha na pasta `data/` e fornece menu de instruções no terminal.

## 📁 Estrutura do Projeto

```text
lot-jogar/
├── .vscode/
│   ├── c_cpp_properties.json  # Configuração de IntelliSense e padrão C++17
│   ├── launch.json            # Configuração de execução e debug (GDB)
│   └── tasks.json             # Tarefa de build configurada para compilar src/*.cpp
├── build/                     # Binário compilado (main)
├── data/
│   └── lotofacil.xlsx         # Planilha de resultados da Caixa (ignorada no Git)
├── src/
│   ├── Concurso.hpp           # Declaração da classe de dados do sorteio
│   ├── Concurso.cpp           # Implementação da classe Concurso
│   ├── LeitorPlanilha.hpp     # Declaração da leitura via xlsxio
│   ├── LeitorPlanilha.cpp     # Implementação da leitura e validação do .xlsx
│   ├── AnalisadorLotofacil.hpp# Declaração dos cálculos, gráfico, simulação, palpite e análise
│   ├── AnalisadorLotofacil.cpp# Implementação das análises estatísticas e estratégias
│   ├── AplicacaoCLI.hpp       # Declaração da interface de linha de comando
│   ├── AplicacaoCLI.cpp       # Implementação do parser de argumentos e fluxo CLI
│   └── main.cpp               # Ponto de entrada da aplicação
├── .gitignore                 # Arquivos ignorados pelo controle de versão
├── LICENSE                    # Licença MIT
└── README.md                  # Documentação do projeto
```

## 🛠️ Pré-requisitos

Para compilar e executar este projeto no Linux, você precisará de:

- O compilador **g++** (com suporte a **C++17**) instalado em `/usr/bin/g++`.
- A biblioteca **[xlsxio](https://github.com)** instalada no sistema (junto com suas dependências, como `expat` e `minizip`/`libzip`).
- *(Opcional)* **Visual Studio Code** com a extensão C/C++ para utilizar as tarefas de build e debug configuradas em `.vscode/`.

### Instalando dependências no Ubuntu/Debian (Exemplo)
```bash
sudo apt update
sudo apt install g++ gdb libexpat1-dev libminizip-dev
# Nota: Caso a libxlsxio-dev não esteja nos repositórios da sua distro, 
# compile-a a partir do repositório oficial: https://github.com
```

## 📦 Como Compilar

1. Clone este repositório:
   ```bash
   git clone https://github.com
   cd lot-jogar
   ```

2. **Via VS Code (Recomendado):**
   - Abra a pasta `lot-jogar` no Visual Studio Code.
   - Pressione `Ctrl + Shift + B` a partir de qualquer arquivo aberto para executar a tarefa **"Compilar lot-jogar"** (ou pressione `F5` para compilar e executar diretamente com os parâmetros definidos em `.vscode/launch.json`).
   - O executável `main` será gerado dentro da pasta `build/`.

3. **Via Terminal (Manual):**
   Caso prefira compilar diretamente pelo terminal a partir da raiz do projeto:
   ```bash
   mkdir -p build
   g++ -std=c++17 -fdiagnostics-color=always -g src/*.cpp -o build/main -lxlsxio_read
   ```

## 💻 Como Usar

1. Acesse o [site oficial das Loterias Caixa](https://caixa.gov.br) e baixe a planilha de resultados da **Lotofácil** em formato Excel (`.xlsx`).
2. Renomeie o arquivo baixado para `lotofacil.xlsx` e coloque-o dentro da pasta `data/` (`data/lotofacil.xlsx`).
3. Execute o programa a partir da raiz do projeto (ou de dentro da pasta `build/`) utilizando uma das opções abaixo:

   - **Exibir o gráfico completo (até o penúltimo concurso vs. último):**
     ```bash
     ./build/main --grafico
     ```

   - **Exibir o gráfico dos últimos `N` concursos antes do último sorteio:**
     ```bash
     ./build/main --grafico 100
     ```

   - **Simular o desempenho das 15 mais frequentes em todo o histórico:**
     ```bash
     ./build/main --simular
     ```

   - **Simular usando janela móvel dos últimos `N` concursos:**
     ```bash
     ./build/main --simular 50
     ```

   - **Gerar palpite com análise de equilíbrio e desempenho histórico:**
     ```bash
     ./build/main --palpite
     ./build/main --palpite 50
     ```

   - **Comparar 5 estratégias diferentes (5 jogos cada) e obter os palpites da campeã:**
     ```bash
     ./build/main --analisar
     ./build/main --analisar 50
     ```

   - **Exibir a ajuda no terminal:**
     ```bash
     ./build/main --help
     ```

### Exemplos de Saída no Terminal

#### Opção `--grafico`
```text
Historico analisado : Concurso 1 ao 3149 (3149 concursos)
Comparado com       : Concurso 3150 (* = dezena sorteada no ultimo concurso)

 * 03 ============================== 1910 (60.65%)
   01 =============================  1890 (60.02%)
 * 25 =============================  1875 (59.54%)
...
   02 ============================   1845 (58.59%)

Resumo: 10 das 15 dezenas mais frequentes sairam no concurso 3150.
```

#### Opção `--simular 50`
```text
=== SIMULACAO HISTORICA (TOP 15 MAIS FREQUENTES) ===
Base de calculo   : Ultimos 50 concursos moveis
Concursos testados: 3100 (Concurso 51 ao 3150)

15 acertos:    0 vezes (0.00%)
14 acertos:    2 vezes (0.06%)
13 acertos:   45 vezes (1.45%)
12 acertos:  210 vezes (6.77%)
11 acertos:  480 vezes (15.48%)

Total de concursos com premiacao (11 a 15 pts): 737 (23.77%)
```

#### Opção `--palpite 50`
```text
=== SUGESTAO DE PALPITE (TOP 15 MAIS FREQUENTES) ===
Base de calculo : Concurso 3101 ao 3150 (50 concursos)

Dezenas sugeridas:
01 - 02 - 03 - 04 - 05 - 07 - 10 - 11 - 13 - 14 - 18 - 20 - 22 - 24 - 25

Analise do volante:
- Pares / Impares       : 8 pares e 7 impares
- Soma das dezenas      : 194
- Repetidas do anterior : 9 dezenas do concurso 3150

Desempenho deste palpite fixo em todo o historico (3150 concursos):
- 15 acertos:    0 vezes (0.00%)
- 14 acertos:    1 vezes (0.03%)
- 13 acertos:   38 vezes (1.21%)
- 12 acertos:  195 vezes (6.19%)
- 11 acertos:  462 vezes (14.67%)
- Total premiado (11 a 15 pts): 696 vezes (22.10%)
```

#### Opção `--analisar 50`
```text
=== ANALISE COMPARATIVA DE ESTRATEGIAS (5 PALPITES POR ESTRATEGIA) ===
Base de calculo   : Janela movel dos ultimos 50 concursos
Concursos testados: 3100 (Concurso 51 ao 3150)

Estrategia                  | Media Pts | Premiados (11+) | 11pts | 12pts | 13pts | 14pts | 15pts
----------------------------+-----------+-------------------+-------+-------+-------+-------+------
4. Repeticao (9 Ant + 6)    |     10.28 |   1142 (36.84%) |   780 |   295 |    62 |     5 |     0
5. Equilibrada (8 Imp/7Par) |     10.21 |   1110 (35.81%) |   765 |   281 |    59 |     5 |     0
1. Quentes (Top Frequencia) |     10.18 |   1095 (35.32%) |   754 |   279 |    58 |     4 |     0
3. Atrasadas (Mais Tempo)   |     10.05 |   1030 (33.23%) |   715 |   260 |    52 |     3 |     0
2. Frias (Menos Sorteadas)  |      9.98 |    995 (32.10%) |   690 |   252 |    50 |     3 |     0

>>> ESTRATEGIA CAMPEA: 4. Repeticao (9 Ant + 6)
5 Palpites gerados para o proximo concurso (apos o 3150):

Jogo 1: 01 - 03 - 04 - 05 - 07 - 09 - 10 - 11 - 13 - 14 - 17 - 18 - 20 - 22 - 25
...
```

## 🗺️ Roadmap (Próximos Passos até a v1.0)

- [x] Leitura da planilha `data/lotofacil.xlsx` via `xlsxio`.
- [x] Contagem até o penúltimo concurso e comparação (`*`) com o último sorteio.
- [x] Resumo de acertos das 15 dezenas mais frequentes no último concurso.
- [x] Filtro por quantidade de concursos via linha de comando (`--grafico <N>`).
- [x] Simulação histórica retrospectiva (`--simular` e `--simular <N>`).
- [x] Gerador de palpites com análise de equilíbrio e conferência histórica (`--palpite` e `--palpite <N>`).
- [x] Comparador de 5 estratégias distintas com geração de 5 jogos (`--analisar` e `--analisar <N>`).
- [x] Opção `--help` e detecção automática do caminho `data/lotofacil.xlsx`.
- [x] Modularizar o código em classes e múltiplos arquivos (`.hpp` / `.cpp`).
- [ ] Exportar palpites e relatórios para arquivo `.txt` ou `.csv`.
- [ ] Adicionar conferidor manual de jogos (`--conferir <dezenas>`).
- [ ] Permitir passar o caminho do arquivo `.xlsx` via argumento no terminal.
- [ ] Criar `Makefile` ou `CMakeLists.txt` para facilitar a compilação fora do VS Code.
