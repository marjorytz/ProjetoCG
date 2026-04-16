#ifndef PGM_H_INCLUDED
#define PGM_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
using namespace std;

struct PGM
{
    string tipo;
    int larg;
    int alt;
    int vmax;
    unsigned char *pixels; // unsigned char -> 0 a 255
    PGM()                  // construtor -> inicializar variaveis
    {
        pixels = nullptr;
        larg = 0;
        alt = 0;
        vmax = 255;
        tipo = "";
    }
};

void destruir(PGM *pgm)
{
    if (pgm->pixels)
        delete pgm->pixels;
    pgm->pixels = nullptr;
    pgm->larg = 0;
    pgm->alt = 0;
    pgm->tipo = "";
    pgm->vmax = 255;
}

void imprimir(PGM *pgm)
{
    cout << "\nPGM\n";
    cout << "Tipo: " << pgm->tipo << endl;
    cout << "Dimensao: " << pgm->larg << "x" << pgm->alt << endl;
    cout << "vMax: " << pgm->vmax << endl;
    if (pgm->pixels) // verifica se o vetor de pixels está alocado
        cout << "Vetor de pixels: " << &(pgm->pixels) << endl;
    else
        cout << "Vetor de pixels: NULL\n";
}

// void criar(PGM *pgm, int largura, int altura)
// {
//     if (pgm->pixels)
//         delete pgm->pixels;

//     int tamanho = largura * altura;

//     pgm->tipo = "P2";
//     pgm->larg = largura;
//     pgm->alt = altura;
//     pgm->vmax = 255;
//     pgm->pixels = new unsigned char[tamanho];

//     // definir a cor preta para todos os pixels
//     for (int i = 0; i < tamanho; i++)
//         pgm->pixels[i] = 0;
// }

/// QUESTÃO 4
void criar(PGM *pgm, int largura, int altura, unsigned char corFundo)
{
    if (pgm->pixels)
        delete pgm->pixels;

    int tamanho = largura * altura;

    pgm->tipo = "P2";
    pgm->larg = largura;
    pgm->alt = altura;
    pgm->vmax = 255;
    pgm->pixels = new unsigned char[tamanho];

    // definir a cor informada (preto/cinza/branco) para todos os pixels
    for (int i = 0; i < tamanho; i++)
        pgm->pixels[i] = corFundo;
}

bool gravar(PGM *pgm, string caminho)
{
    if (!pgm->pixels)
        return false;

    ofstream arq;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PGM: endereco do arquivo invalido\n";
        return false;
    }

    arq << pgm->tipo << endl;

    arq << pgm->larg << " " << pgm->alt << endl;

    arq << 255 << endl; // valor máximo de cor fixo em 255

    int tam = pgm->larg * pgm->alt;
    for (int i = 0; i < tam; i++)
    {
        arq << (int)pgm->pixels[i] << endl;
        arq.flush();
    }

    arq.close();
    return true;
}

string lerLinhaArquivo(std::ifstream &arq)
{
    string linha = "", dadoLido = "";
    while (!arq.eof())
    {
        linha = "";
        std::getline(arq, linha);

        // Remove \r do final da linha (caso exista)
        if (!linha.empty() && linha.back() == '\r')
            linha.pop_back();

        if (linha.size() > 0 && linha[0] != '#')
            return linha;
    }
    return linha;
}

bool ler(PGM *pgm, string caminho)
{
    ifstream arq;
    string dado;
    stringstream str;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PGM: endereco do arquivo invalido\n";
        return false;
    }

    // ler o tipo
    dado = lerLinhaArquivo(arq);
    if (dado == "P2")
        pgm->tipo = dado;
    else
    {
        cout << "PGM: erro ao ler o tipo da imagem\n";
        return false;
    }

    // ler largura e altura
    dado = lerLinhaArquivo(arq);
    str = stringstream(dado);
    if (!(str >> pgm->larg))
    {
        cout << "PGM: erro ao ler a largura\n";
        return false;
    }
    if (!(str >> pgm->alt))
    {
        cout << "PGM: erro ao ler a largura\n";
        return false;
    }

    // ler vmax
    dado = lerLinhaArquivo(arq);
    str = stringstream(dado);
    if (!(str >> pgm->vmax))
    {
        cout << "PGM: erro ao ler vmax\n";
        return false;
    }

    if (pgm->pixels)
        delete pgm->pixels;

    int tam = pgm->larg * pgm->alt;

    pgm->pixels = new unsigned char[tam];

    int i = 0, p;
    while (!arq.eof())
    {

        if (arq >> p)
        {
            if (i < tam)
                pgm->pixels[i] = (unsigned char)p;
            i++;
        }
        else
            break;
    }

    if (i != tam)
    {
        cout << "PGM: erro ao ler os pixels\n";
        return false;
    }
    arq.close();
    return true;
}

// Retorna pixeis de determinada região
unsigned char getPixel(PGM *pgm, int x, int y)
{
    if (!pgm->pixels)
        return 0;

    return pgm->pixels[y * pgm->larg + x];
}

// Modifica os pixeis de determinada região
void setPixel(PGM *pgm, int x, int y, unsigned char cor)
{
    if (!pgm->pixels)
        return;

    pgm->pixels[y * pgm->larg + x] = cor;
}

void DDALine(PGM *img, int x0, int y0, int x1, int y1, unsigned char corL)
{

    // Calculate dx and dy
    int dx = x1 - x0;
    int dy = y1 - y0;

    int step;

    // If dx > dy we will take step as dx
    // else we will take step as dy to draw the complete
    // line
    if (abs(dx) > abs(dy))
        step = abs(dx);
    else
        step = abs(dy);

    // Calculate x-increment and y-increment for each step
    float x_incr = (float)dx / step;
    float y_incr = (float)dy / step;

    // Take the initial points as x and y
    float x = x0;
    float y = y0;

    for (int i = 0; i < step; i++)
    {
        setPixel(img, x, y, corL);
        // putpixel(round(x), round(y), WHITE);
        // cout << round(x) << " " << round(y) << "\n";
        x += x_incr;
        y += y_incr;
        // delay(10);
    }
}

/// QUESTÃO 5
// Preencher determinada linha de uma Imagem
void setLinha(PGM *pgm, int linha, unsigned char cor)
{
    for (int i = 0; i < pgm->larg; i++)
    {
        if (!pgm->pixels)
            return;

        pgm->pixels[linha * pgm->larg + i] = cor;
    }
}

/// QUESTÃO 6
// Verifica se a coordenada é válida
bool coordValida(PGM *pgm, int x, int y)
{
    if (!pgm->pixels)
        return false;

    return (x >= 0 && x < pgm->larg &&
            y >= 0 && y < pgm->alt);
}

/// QUESTÃO 7
// Preencher uma região de uma imagem
void setFill(PGM *pgm, int x0, int y0, int x1, int y1, unsigned char cor)
{
    if (!pgm->pixels)
        return;

    for (int y = y0; y <= y1; y++)
    {
        for (int x = x0; x <= x1; x++)
        {
            if (coordValida(pgm, x, y))
                pgm->pixels[y * pgm->larg + x] = cor; // da pra usar setPixel também
        }
    }
}

void setRecorte(PGM *pgmE, PGM *pgmS, int x0, int y0, int x1, int y1)
{
    if (!pgmE->pixels) // verificar se a coordenada é valida (coordValida)
        return;

    for (int x = x0; x <= x1; x++)
    {
        for (int y = y0; y <= y1; y++)
        {
            unsigned char c = getPixel(pgmE, x, y);
            setPixel(pgmS, x, y, c);
        }
    }
}

// QUESTÃO 9
// Método para inverter a imagem verticalmente (flip)
void setFlipVertical(PGM *pgm, PGM *pgmS)
{
    // Verificar se o ponteiro é válido
    if (!pgm->pixels)
        return;

    // Varre a imagem original de baixo para cima
    // e copia pixel por pixel para a nova imagem
    for (int y = 0; y < pgm->alt; y++)
    {
        for (int x = 0; x < pgm->larg; x++)
        {
            unsigned char c = getPixel(pgm, x, pgm->alt - 1 - y);
            setPixel(pgmS, x, y, c);
        }
    }
}

// QUESTÃO 11
// Desenhar uma borda em uma imagem PGM
void setBorda(PGM *pgm, int espessura, unsigned char cor)
{
    if (!pgm->pixels)
        return;

    int e = espessura;

    for (int y = 0; y < pgm->alt; y++)
    {
        for (int x = 0; x < pgm->larg; x++)
        {
            if (y < e || y >= (pgm->alt - e) || x < e || x >= (pgm->larg - e))
            {
                setPixel(pgm, x, y, cor);
            }
        }
    }
}

// QUESTÃO 13
// Combinar duas imagens PGM
void combinarPGM(PGM *img1, PGM *img2, PGM *imgS, int tipoCombinacao)
{
    // Verifica se os ponteiros são válidos
    if (!img1->pixels || !img2->pixels || !imgS->pixels)
        return;

    // Varre todos os pixels das imagens
    for (int y = 0; y < img1->alt; y++)
    {
        for (int x = 0; x < img1->larg; x++)
        {
            // Pega o valor da cor em ambas as imagens na mesma coordenada
            unsigned char c1 = getPixel(img1, x, y);
            unsigned char c2 = getPixel(img2, x, y);
            unsigned char corFinal = 0;

            // Aplica a regra matemática de acordo com a opção escolhida no menu
            if (tipoCombinacao == 1) // Média
            {
                corFinal = (c1 + c2) / 2;
            }
            else if (tipoCombinacao == 2) // Maior (max)
            {
                corFinal = (c1 > c2) ? c1 : c2;
            }
            else if (tipoCombinacao == 3) // Menor (min)
            {
                corFinal = (c1 < c2) ? c1 : c2;
            }

            // Grava a cor calculada na imagem de saída
            setPixel(imgS, x, y, corFinal);
        }
    }
}

// QUESTÃO 14
// Desenhar uma borda em uma região de uma imagem PGM
void setBordaCoord(PGM *pgm, int x0, int y0, int x1, int y1, unsigned char cor)
{
    if (!pgm->pixels)
        return;

    // Linha para o Topo e a Base
    for (int x = x0; x <= x1; x++)
    {
        setPixel(pgm, x, y0, cor); // Topo
        setPixel(pgm, x, y1, cor); // Base
    }
    // Linha para Lados
    for (int y = y0; y <= y1; y++)
    {
        setPixel(pgm, x0, y, cor); // Esquerda
        setPixel(pgm, x1, y, cor); // Direita
    }
}

// QUESTÃO 15
// Converter os tons de cinza de uma imagem PGM
void convertTons(PGM *pgm, PGM *pgmS)
{
    if (!pgm->pixels)
        return;

    for (int y = 0; y < pgm->alt; y++)
    {
        for (int x = 0; x < pgm->larg; x++)
        {
            if (getPixel(pgm, x, y) > 128)
                setPixel(pgmS, x, y, 255);
            else if (getPixel(pgm, x, y) <= 128)
                setPixel(pgmS, x, y, 0);
        }
    }
}

// QUESTÃO 7 - Revisão
// Função para remover buracos usando a Mediana
void preencherBuracoMediana(PGM *imgE, PGM *imgS)
{
    // Verifica se as imagens foram carregadas corretamente
    if (!imgE->pixels || !imgS->pixels) return;

    for (int y = 0; y < imgE->alt; y++)
    {
        for (int x = 0; x < imgE->larg; x++)
        {
            unsigned char corAtual = getPixel(imgE, x, y);

            // Se o pixel NÃO for um buraco, apenas copia a cor original e vai para o próximo
            if (corAtual != 0)
            {
                setPixel(imgS, x, y, corAtual);
                continue;
            }
            
            vector<unsigned char> vizinhos;

            // Percorre a grade 3x3 ao redor do pixel P(x,y)
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    // Ignora o próprio pixel central P(x,y)
                    if (dx == 0 && dy == 0) continue;

                    int nx = x + dx;
                    int ny = y + dy;

                    // Regra: Verifica se a coordenada é válida (dentro da imagem)
                    if (coordValida(imgE, nx, ny))
                    {
                        unsigned char corVizinho = getPixel(imgE, nx, ny);
                        
                        // Regra: A intensidade da cor deve ser diferente de zero
                        if (corVizinho != 0)
                        {
                            vizinhos.push_back(corVizinho);
                        }
                    }
                }
            }

            // Regra: Cálculo da Mediana
            if (!vizinhos.empty())
            {
                // Ordena os valores em ordem crescente
                sort(vizinhos.begin(), vizinhos.end());
                
                int tam = vizinhos.size();
                unsigned char mediana = 0;

                if (tam % 2 != 0) {
                    // Ímpar: Pega o elemento central exato
                    mediana = vizinhos[tam / 2];
                } else {
                    // Par: Média aritmética dos dois elementos centrais
                    mediana = (vizinhos[(tam / 2) - 1] + vizinhos[tam / 2]) / 2;
                }

                // Tapa o buraco na imagem de saída com a mediana calculada
                setPixel(imgS, x, y, mediana);
            }
            else
            {
                // se TODOS os vizinhos ao redor também forem 0, continua 0
                setPixel(imgS, x, y, 0);
            }
        }
    }
}

#endif