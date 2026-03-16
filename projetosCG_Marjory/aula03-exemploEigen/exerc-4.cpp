#include <iostream>
#include <cmath>
#include "PGM.hpp"

using namespace std;

#include <Eigen/Dense>
using namespace Eigen;

#define _PI 3.14159265358979323846

float degree2rad(float ang)
{
    float r = ang * (float)(_PI / 180.0);

    return r;
}

Matrix3f getTranslacao(float tx, float ty)
{
    Matrix3f Tans = Matrix3f::Identity();

    Tans(0, 2) = tx;
    Tans(1, 2) = ty;

    return Tans;
}

Matrix3f getRotacao(float angulo)
{
    float rad = degree2rad(angulo);

    float s = sin(rad);
    float c = cos(rad);

    Matrix3f Rot = Matrix3f::Identity();

    Rot(0, 0) = c;
    Rot(0, 1) = -s;
    Rot(1, 0) = s;
    Rot(1, 1) = c;

    return Rot;
}

Matrix3f getEscala(float sx, float sy)
{
    Matrix3f Esc = Matrix3f::Identity();

    Esc(0, 0) = sx;
    Esc(1, 1) = sy;

    return Esc;
}

Matrix3f getCisalhamentoHor(float a)
{
    Matrix3f Cis = Matrix3f::Identity();

    Cis(0, 1) = a;

    return Cis;
}

Matrix3f getCisalhamentoVer(float b)
{
    Matrix3f Cis = Matrix3f::Identity();

    Cis(1, 0) = b;

    return Cis;
}

Matrix3f getReflexaoEixoX()
{
    Matrix3f Ref = Matrix3f::Identity();

    Ref(1, 1) = -1;

    return Ref;
}

Matrix3f getReflexaoEixoY()
{
    Matrix3f Ref = Matrix3f::Identity();

    Ref(0, 0) = -1;

    return Ref;
}

void transf2DInv(PGM *imgE, PGM *imgS, Matrix3f Minv)
{
    if (imgE->larg != imgS->larg || imgE->alt != imgS->alt)
        return;

    // Percorrer a imagem de saída
    for (size_t yS = 0; yS < imgS->alt; yS++)
    {
        for (size_t xS = 0; xS < imgS->larg; xS++)
        {
            Vector3f coordEntrada = Minv * Vector3f(xS, yS, 1);

            float xE = round(coordEntrada.x());
            float yE = round(coordEntrada.y());

            if (coordValida(imgE, xE, yE))
                setPixel(imgS, xS, yS, getPixel(imgE, xE, yE));
        }
    }
}

int main(void)
{
    setlocale(LC_ALL, "Portuguese");

    PGM imgE;
    string nomeArquivo;

    cout << "Digite o nome da imagem de entrada (ex: entrada.pgm): ";
    cin >> nomeArquivo;

    if (!ler(&imgE, nomeArquivo))
    {
        cout << "Falha ao carregar a imagem de entrada." << endl;
        return EXIT_FAILURE;
    }

    float cx = imgE.larg / 2.0f;
    float cy = imgE.alt / 2.0f;
    Vector2f pontoCentro(cx, cy);

    Matrix3f Tinv = getTranslacao(-pontoCentro.x(), -pontoCentro.y());
    Matrix3f T = getTranslacao(pontoCentro.x(), pontoCentro.y());

    int menu = 0;
    int contSaida = 1;

    Matrix3f MatAcumulada = Matrix3f::Identity();

    do
    {
        cout << "\n--- MENU DE TRANSFORMACOES ---" << endl;
        cout << "1- Translacao" << endl;
        cout << "2- Rotacao" << endl;
        cout << "3- Escala" << endl;
        cout << "4- Cisalhamento Horizontal" << endl;
        cout << "5- Cisalhamento Vertical" << endl;
        cout << "6- Reflexao em torno do eixo x" << endl;
        cout << "7- Reflexao em torno do eixo y" << endl;
        cout << "0- Sair" << endl;
        cout << "Opcao: ";
        cin >> menu;

        Matrix3f MatAtual = Matrix3f::Identity();
        bool aplicarTransformacao = false;

        switch (menu)
        {
        case 1:
        {
            float tx, ty;
            cout << "Informe Tx: ";
            cin >> tx;
            cout << "Informe Ty: ";
            cin >> ty;
            MatAtual = getTranslacao(tx, ty);
            aplicarTransformacao = true;
            break;
        }
        case 2:
        {
            float ang;
            cout << "Informe o angulo em graus: ";
            cin >> ang;
            MatAtual = getRotacao(ang);
            aplicarTransformacao = true;
            break;
        }
        case 3:
        {
            float sx, sy;
            cout << "Informe Sx: ";
            cin >> sx;
            cout << "Informe Sy: ";
            cin >> sy;
            MatAtual = getEscala(sx, sy);
            aplicarTransformacao = true;
            break;
        }
        case 4:
        {
            float shx;
            cout << "Informe Shx (Cisalhamento Hor.): ";
            cin >> shx;
            MatAtual = getCisalhamentoHor(shx);
            aplicarTransformacao = true;
            break;
        }
        case 5:
        {
            float shy;
            cout << "Informe Shy (Cisalhamento Ver.): ";
            cin >> shy;
            MatAtual = getCisalhamentoVer(shy);
            aplicarTransformacao = true;
            break;
        }
        case 6:
        {
            MatAtual = getReflexaoEixoX();
            aplicarTransformacao = true;
            break;
        }
        case 7:
        {
            MatAtual = getReflexaoEixoY();
            aplicarTransformacao = true;
            break;
        }
        case 0:
            cout << "Encerrando programa..." << endl;
            break;
        default:
            cout << "Opcao invalida. Tente novamente." << endl;
        }

        if (aplicarTransformacao)
        {
            MatAcumulada = MatAtual * MatAcumulada;

            Matrix3f Mglobal = T * MatAcumulada * Tinv;

            Matrix3f Minv = Mglobal.inverse();

            PGM imgS;
            criar(&imgS, imgE.larg, imgE.alt, 0);

            transf2DInv(&imgE, &imgS, Minv);

            string nomeDaSaida = "saida_" + to_string(contSaida) + ".pgm";
            if (gravar(&imgS, nomeDaSaida))
            {
                cout << "Imagem gravada como '" << nomeDaSaida << "'." << endl;
                contSaida++;
            }

            destruir(&imgS);
        }

    } while (menu != 0);

    destruir(&imgE);
    return EXIT_SUCCESS;
}