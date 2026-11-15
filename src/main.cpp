#include <iostream>
//#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <xlsxio_read.h>

/**
 * Lê as dezenas do arquivo xlsx e conta as vezes que cada dezenas foram sorteadas.
 * @param quantidadeContagem Informa quantos concursos anteriores contar, se não informar, conta todos os concursos.
 * @return Retorna 'true' se sucesso ou 'false' se erro.
 */
bool grafico(int quantidadeContagem=0)
{
    std::vector<std::string> celulas;
    std::vector<std::vector<std::string>> linhas; //-> Informações do arquivo xlsx são armazenadas aqui.

    std::vector<int> quantidadeDezenas(26,0); //-> A quantidade das dezenas contadas são armazenadas aqui.

    std::vector<int>ultimoConcurso; //-> As dezenas do último concurso são armazenadas aqui;

    std::pair<int,int> quantidade;
    std::vector<std::pair<int,int>> quantidades; //-> A quantidade e as dezenas são armazenadas aqui para 
                                                 //   ordenação da quantidade maior para a menor.

    std::string caminhoArquivo = "../data/lotofacil.xlsx";

    char *valorCelula;  //-> Ponteiro para armazenar as celulas lidas.

    //-> Abre o arquivo xlsx.
    xlsxioreader leitor = xlsxioread_open(caminhoArquivo.c_str());
    if (leitor == nullptr)
        return false;

    //-> Seleciona a planilha a ser lida, 'nullptr' escolha a primeira.
    xlsxioreadersheet planilha = xlsxioread_sheet_open(leitor, nullptr, XLSXIOREAD_SKIP_NONE);

    while(xlsxioread_sheet_next_row(planilha)) {//-> Lê até a última linha da planilha selecionada 
                                                //   anteriormente.

        celulas.clear();

        //-> Lê as células da linha.
        while((valorCelula = xlsxioread_sheet_next_cell(planilha)) != nullptr) {

            if(valorCelula != "")
                celulas.push_back(valorCelula);
            else
                celulas.push_back(" ");
            
            xlsxioread_free(valorCelula); //-> Libera memória alocada para gravar conteúdo da célula.
        }
        linhas.push_back(celulas);
    }

    xlsxioread_sheet_close(planilha);
    xlsxioread_close(leitor);

    int totalLinhas = linhas.size(), ultimoSorteio = totalLinhas -2,inicioContagem;

    //-> Obtém último sorteio gravado no vetor.
    try {

        for(int c = 2; c <= 16; c++)
            ultimoConcurso.push_back(std::stoi(linhas[totalLinhas -1][c]));
    }
    catch(...) {

        return false;
    }

    //-> Calcula de onde começar a ler os concursos.
    if(quantidadeContagem <= 0) {

        inicioContagem = 1;
        quantidadeContagem = totalLinhas;
    }
    else
        inicioContagem = totalLinhas -quantidadeContagem;

    //-> Conta dezenas.
    try {
        for(int l = inicioContagem; l <= ultimoSorteio; l++)
            for(int c = 2; c <= 16; c++)
                quantidadeDezenas[std::stoi(linhas[l][c])]++;
    }
    catch(...) {

        return false;
    }

    //-> Ordena dezenas e indíces na ordem decrescente de quantidades.
    for(int dez = 1; dez <= 25; dez++) {

        quantidade.first  = quantidadeDezenas[dez];
        quantidade.second = dez;

        quantidades.push_back(quantidade);
    }
    std::sort(quantidades.rbegin(),quantidades.rend());

    /* Calcula a porcentagem de repetições das dezenas conforme a quantidade de concursos escolhido
       e apresenta na tela.
    */
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