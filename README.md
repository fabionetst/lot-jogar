# 📊 lot-jogar

> ⚠️ **Aviso:** Este projeto está em fase inicial de desenvolvimento (**pré-v1.0**). Funcionalidades podem mudar e melhorias ainda estão sendo implementadas.

Um aplicativo de linha de comando (CLI) escrito em **C++** para Linux que lê a planilha oficial de resultados da **Lotofácil** (baixada manualmente do site da Caixa Econômica Federal), analisa o histórico até o **penúltimo concurso**, compara com as dezenas do **último sorteio** e exibe o gráfico de frequência no terminal.

## 🚀 Funcionalidades

- **Leitura de Planilhas Excel (`.xlsx`):** Utiliza a biblioteca leve e rápida `xlsxio` para processar o histórico de sorteios sem depender de ferramentas pesadas.
- **Análise Histórica vs. Último Sorteio:** Contabiliza as ocorrências até o penúltimo concurso e destaca com `*` as dezenas que foram sorteadas no último concurso registrado na planilha.
- **Filtro por Intervalo de Concursos:** Permite restringir a contagem aos últimos `N` concursos anteriores ao último sorteio.
- **Contagem, Porcentagem e Ordenação:** Calcula o percentual de aparição de cada dezena (`01` a `25`) e ordena automaticamente da mais sorteada para a menos sorteada.
- **Visualização no Terminal:** Gera um gráfico de barras em texto (`=`) diretamente no terminal Linux.

## 📁 Estrutura do Projeto

```text
lot-jogar/
├── .vscode/
│   └── tasks.json     # Tarefa de build configurada para o VS Code
├── build/             # Binário compilado (main)
├── data/
│   └── lotofacil.xlsx # Planilha de resultados da Caixa
└── src/
    └── main.cpp       # Código-fonte principal
```

## 🛠️ Pré-requisitos

Para compilar e executar este projeto no Linux, você precisará de:

- O compilador **g++** instalado em `/usr/bin/g++`.
- A biblioteca **[xlsxio](https://github.com)** instalada no sistema (junto com suas dependências, como `expat` e `minizip`/`libzip`).
- *(Opcional)* **Visual Studio Code** com a extensão C/C++ para utilizar a tarefa de build configurada em `.vscode/tasks.json`.

### Instalando dependências no Ubuntu/Debian (Exemplo)
```bash
sudo apt update
sudo apt install g++ libexpat1-dev libminizip-dev
# Nota: Caso a libxlsxio-dev não esteja nos repositórios da sua distro, 
# compile-a a partir do repositório oficial no GitHub.
```

## 📦 Como Compilar

1. Clone este repositório:
   ```bash
   git clone https://github.com
   cd lot-jogar
   ```

2. **Via VS Code (Recomendado):**
   - Abra a pasta `lot-jogar` no Visual Studio Code.
   - Abra o arquivo `src/main.cpp` no editor.
   - Pressione `Ctrl + Shift + B` para executar a tarefa **"Compilar lot-jogar"**.
   - O executável `main` será gerado dentro da pasta `build/`.

3. **Via Terminal (Manual):**
   Caso prefira compilar diretamente pelo terminal a partir da raiz do projeto:
   ```bash
   mkdir -p build
   g++ -fdiagnostics-color=always -g src/main.cpp -o build/main -lxlsxio_read
   ```

## 💻 Como Usar

1. Acesse o [site oficial das Loterias Caixa](https://caixa.gov.br) e baixe a planilha de resultados da **Lotofácil** em formato Excel (`.xlsx`).
2. Renomeie o arquivo baixado para `lotofacil.xlsx` e coloque-o dentro da pasta `data/` (`data/lotofacil.xlsx`).
3. Entre na pasta `build/` e execute o programa passando a opção `--grafico`:

   - **Analisar todo o histórico (até o penúltimo concurso):**
     ```bash
     cd build
     ./main --grafico
     ```

   - **Analisar apenas os últimos `N` concursos (ex: últimos 100 antes do último sorteio):**
     ```bash
     cd build
     ./main --grafico 100
     ```

### Exemplo de Saída no Terminal

```text
 * 03 ============================== 1910 (60.63%)
   01 =============================  1890 (60.00%)
 * 25 =============================  1875 (59.52%)
...
   02 ============================   1845 (58.57%)
```
*(O asterisco `*` à esquerda indica que a dezena saiu no último concurso da planilha).*

## 🗺️ Roadmap (Próximos Passos até a v1.0)

- [x] Leitura da planilha `data/lotofacil.xlsx` via `xlsxio`.
- [x] Contagem até o penúltimo concurso e comparação (`*`) com o último sorteio.
- [x] Filtro por quantidade de concursos via linha de comando (`--grafico [N]`).
- [x] Ordenação automática pela contagem de sorteios.
- [ ] Adicionar flag `--help` e mensagens de erro descritivas no terminal.
- [ ] Permitir escolher outros tipos de ordenação (ex: por ordem numérica da dezena).
- [ ] Permitir passar o caminho do arquivo `.xlsx` via argumento no terminal.

## 🤝 Contribuição

Como o projeto ainda não atingiu a versão `1.0`, sugestões, relatos de bugs e *Pull Requests* são muito bem-vindos! Sinta-se à vontade para abrir uma *issue* em [fabionetst/lot-jogar](https://github.com).

## 📝 Licença

Este projeto está sob a licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.
