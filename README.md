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
