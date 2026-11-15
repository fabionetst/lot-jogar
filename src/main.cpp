#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <xlsxio_read.h>

/**
 * Lê uma célula específica (linha e coluna) de um arquivo .xlsx.
 * 
 * @param caminho_arquivo Caminho para o arquivo .xlsx
 * @param nome_aba Nome da aba (passe "" para abrir a primeira aba automaticamente)
 * @param linha_alvo Número da linha (começando em 1)
 * @param coluna_alvo Número da coluna (começando em 1, onde A=1, B=2, C=3...)
 * @return O conteúdo da célula como std::string (ou "" se estiver vazia/não encontrada)
 */
std::string ler_celula(const std::string &caminho_arquivo, const std::string &nome_aba, size_t linha_alvo, 
                        size_t coluna_alvo) {
    // 1. Abre o arquivo Excel
    xlsxioreader leitor = xlsxioread_open(caminho_arquivo.c_str());
    if (leitor == nullptr) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << caminho_arquivo << "'." << std::endl;
        return "";
    }

    // Define o nome da aba (nullptr abre a primeira aba disponível)
    const char* aba = nome_aba.empty() ? nullptr : nome_aba.c_str();

    // 2. Abre a planilha usando XLSXIOREAD_SKIP_NONE para manter os índices reais de linhas e colunas
    xlsxioreadersheet planilha = xlsxioread_sheet_open(leitor, aba, XLSXIOREAD_SKIP_NONE);
    if (planilha == nullptr) {
        std::cerr << "Erro: Nao foi possivel abrir a aba solicitada." << std::endl;
        xlsxioread_close(leitor);
        return "";
    }

    std::string resultado = "";
    size_t linha_atual = 0;
    bool encontrado = false;

    // 3. Percorre as linhas sequencialmente
    while (xlsxioread_sheet_next_row(planilha)) {
        linha_atual++;

        // Otimização: Se já passou da linha desejada, encerra a busca imediatamente
        if (linha_atual > linha_alvo) {
            break;
        }

        size_t coluna_atual = 0;
        char* valor_celula = nullptr;

        // 4. Percorre as colunas da linha atual
        while ((valor_celula = xlsxioread_sheet_next_cell(planilha)) != nullptr) {
            coluna_atual++;

            if (linha_atual == linha_alvo && coluna_atual == coluna_alvo) {
                resultado = std::string(valor_celula);
                encontrado = true;
            }

            // Libera a memória alocada pelo xlsxio para esta célula
            xlsxioread_free(valor_celula);

            if (encontrado) {
                break; // Sai do loop de colunas
            }
        }

        if (encontrado) {
            break; // Sai do loop de linhas
        }
    }

    // 5. Fecha os recursos abertos
    xlsxioread_sheet_close(planilha);
    xlsxioread_close(leitor);

    return resultado;
}
/**
 * Lê uma célula específica (linha e coluna) de um arquivo .xlsx.
 * 
 * @param caminho_arquivo Caminho para o arquivo .xlsx
 * @param nome_aba Nome da aba (passe "" para abrir a primeira aba automaticamente)
 * @param linha_alvo Número da linha (começando em 1)
 * @param coluna_alvo Número da coluna (começando em 1, onde A=1, B=2, C=3...)
 * @return O conteúdo da célula como std::string (ou "" se estiver vazia/não encontrada)
 */
std::string ler_celula(xlsxioreader leitor,const char *aba,size_t linha_alvo, size_t coluna_alvo)
{
    // 2. Abre a planilha usando XLSXIOREAD_SKIP_NONE para manter os índices reais de linhas e colunas
    xlsxioreadersheet planilha = xlsxioread_sheet_open(leitor, aba, XLSXIOREAD_SKIP_NONE);
    if (planilha == nullptr) {
        std::cerr << "Erro: Nao foi possivel abrir a aba solicitada." << std::endl;
        xlsxioread_close(leitor);
        return "";
    }

    std::string resultado = "";
    size_t linha_atual = 0;
    bool encontrado = false;

    // 3. Percorre as linhas sequencialmente
    while (xlsxioread_sheet_next_row(planilha)) {
        linha_atual++;

        // Otimização: Se já passou da linha desejada, encerra a busca imediatamente
        if (linha_atual > linha_alvo) {
            break;
        }

        size_t coluna_atual = 0;
        char* valor_celula = nullptr;

        // 4. Percorre as colunas da linha atual
        while ((valor_celula = xlsxioread_sheet_next_cell(planilha)) != nullptr) {
            coluna_atual++;

            if (linha_atual == linha_alvo && coluna_atual == coluna_alvo) {
                resultado = std::string(valor_celula);
                encontrado = true;
            }

            // Libera a memória alocada pelo xlsxio para esta célula
            xlsxioread_free(valor_celula);

            if (encontrado) {
                break; // Sai do loop de colunas
            }
        }

        if (encontrado) {
            break; // Sai do loop de linhas
        }
    }

    // 5. Fecha os recursos abertos
    xlsxioread_sheet_close(planilha);
    xlsxioread_close(leitor);

    return resultado;
}

bool grafico(int quantidadeContagem=0)
{
    std::vector<std::string> celulas;
    std::vector<std::vector<std::string>> linhas;
    std::vector<std::pair<int,int>> quantidades;
    std::vector<int> quantidadeDezenas(26,0),ultimoConcurso;

    std::pair<int,int> quantidade;

    std::string caminhoArquivo = "../data/lotofacil.xlsx";

    char *valorCelula;

    xlsxioreader leitor = xlsxioread_open(caminhoArquivo.c_str());
    if (leitor == nullptr)
        return false;

    xlsxioreadersheet planilha = xlsxioread_sheet_open(leitor, nullptr, XLSXIOREAD_SKIP_NONE);

    while(xlsxioread_sheet_next_row(planilha)) {

        celulas.clear();
        while((valorCelula = xlsxioread_sheet_next_cell(planilha)) != nullptr) {

            if(valorCelula != "")
                celulas.push_back(valorCelula);
            else
                celulas.push_back(" ");
            
            xlsxioread_free(valorCelula);
        }
        linhas.push_back(celulas);
    }

    xlsxioread_sheet_close(planilha);
    xlsxioread_close(leitor);

    int totalLinhas = linhas.size(), ultimoSorteio = totalLinhas -2,inicioContagem;

    try {

        for(int c = 2; c <= 16; c++)
            ultimoConcurso.push_back(std::stoi(linhas[totalLinhas -1][c]));
    }
    catch(...) {

        return false;
    }

    if(quantidadeContagem <= 0) {

        inicioContagem = 1;
        quantidadeContagem = totalLinhas;
    }
    else
        inicioContagem = totalLinhas -quantidadeContagem;

    try {
        for(int l = inicioContagem; l <= ultimoSorteio; l++)
            for(int c = 2; c <= 16; c++)
                quantidadeDezenas[std::stoi(linhas[l][c])]++;
    }
    catch(...) {

        return false;
    }

    for(int dez = 1; dez <= 25; dez++) {

        quantidade.first  = quantidadeDezenas[dez];
        quantidade.second = dez;

        quantidades.push_back(quantidade);
    }
    std::sort(quantidades.rbegin(),quantidades.rend());

    float porCento;
    for(auto const &q: quantidades) {

        porCento = (float) q.first /quantidadeContagem;
        if(std::find(ultimoConcurso.begin(),ultimoConcurso.end(),q.second) != ultimoConcurso.end())
            std::cout<< " * ";
        else
            std::cout<< "   ";

        std::cout<< std::setw(2)<< std::setfill('0')
                 << q.second<< ' '<< std::string(50 *porCento,'.')<< ' '
                 <<std::fixed<< std::setprecision(2)
                 << (porCento *100)<< '%'<< std::endl;
    }

    return true;
}

int main(int argc,char *argv[])
{
    std::string args[2];

    switch(argc) {

        case 2:
            args[0] = argv[1];
            if(args[0] == "--grafico")
                grafico();
        break;
        case 3:
            args[0] = argv[1];
            args[1] = argv[2];
    
            if(args[0] == "--grafico" and !args[1].empty()) {
    
                    // O cast para unsigned char evita comportamento indefinido com acentos
                    if(std::all_of(args[1].begin(), args[1].end(), [](unsigned char c) {
                        return std::isdigit(c);
                    })) {

                    grafico(std::stoi(args[1]));
                }
            }
        break;
    }

    return 0;
}