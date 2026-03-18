#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include "PPM.hpp"

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

// Ler a imagem de entrada e criar a imagem de saída antes de chamar a função
// Mapeamento Direto
void transf2D(PPM *imgE, PPM *imgS, Matrix3f M, int offset)
{
    // Percorrer a imagem de entrada
    for (int yE = 0; yE < imgE->alt; yE++)
    {
        for (int xE = 0; xE < imgE->larg; xE++)
        {
            Vector3f coordSaida = M * Vector3f(xE, yE, 1);

            int xS = round(coordSaida.x());
            int yS = round(coordSaida.y());

            if (xS >= 0 && xS < imgE->larg && yS >= 0 && yS < imgE->alt)
            {
                setPixel(imgS, xS + offset, yS, getPixel(imgE, xE, yE));
            }
        }
    }
}

// Mapeamento Inverso
void transf2DInv(PPM *imgE, PPM *imgS, Matrix3f Minv, int offset)
{
    // Percorre o tamanho de 1 segmento (que tem o tamanho da imgE)
    for (int yS = 0; yS < imgE->alt; yS++)
    {
        for (int xS = 0; xS < imgE->larg; xS++)
        {
            Vector3f coordEntrada = Minv * Vector3f(xS, yS, 1);

            int xE = std::round(coordEntrada.x());
            int yE = std::round(coordEntrada.y());

            if (coordValida(imgE, xE, yE))
            {
                setPixel(imgS, xS + offset, yS, getPixel(imgE, xE, yE));
            }
        }
    }
}

int main(void)
{
    setlocale(LC_ALL, "Portuguese");

    string cmd, imgENome, imgSNome, mapType;
    int ntrans = 0;

    cin >> cmd >> ntrans;
    cin >> cmd >> imgENome;
    cin >> cmd >> imgSNome;
    cin >> cmd >> mapType;

    PPM imgE;
    if (!ler(&imgE, imgENome))
    {
        cout << "Falha ao carregar a imagem de entrada!" << endl;
        return EXIT_FAILURE;
    }

    PPM imgS;
    int largTotal = imgE.larg * ntrans;
    criar(&imgS, largTotal, imgE.alt, RGB(0, 0, 0));

    float cx = imgE.larg / 2.0f;
    float cy = imgE.alt / 2.0f;
    Vector2f pontoCentro(cx, cy);

    Matrix3f Tinv = getTranslacao(-pontoCentro.x(), -pontoCentro.y());
    Matrix3f T = getTranslacao(pontoCentro.x(), pontoCentro.y());

    Matrix3f MatAcumulada = Matrix3f::Identity();

    for (int i = 0; i < ntrans; i++)
    {
        string t_type;

        cout << "[T] Translacao" << endl;
        cout << "[R] Rotacao" << endl;
        cout << "[S] Escala" << endl;
        cout << "[CE] Cisalhamento Horizontal" << endl;
        cout << "[RE] Reflexao em torno do eixo x" << endl;
        cout << "Tansformacao: ";
        cin >> t_type;

        Matrix3f MatrizAtual = Matrix3f::Identity();

        if (t_type == "R")
        {
            float a;
            cin >> a;
            MatrizAtual = getRotacao(a);
        }
        else if (t_type == "S")
        {
            float sx, sy;
            cin >> sx >> sy;
            MatrizAtual = getEscala(sx, sy);
        }
        else if (t_type == "T")
        {
            float tx, ty;
            cin >> tx >> ty;
            MatrizAtual = getTranslacao(tx, ty);
        }
        else if (t_type == "RE")
        {
            string eixo;
            cin >> eixo;
            if (eixo == "X")
                MatrizAtual = getReflexaoEixoX();
            else
                MatrizAtual = getReflexaoEixoY();
        }
        else if (t_type == "CI")
        {
            string eixo;
            float val;
            cin >> eixo >> val;
            if (eixo == "H")
                MatrizAtual = getCisalhamentoHor(val);
            else
                MatrizAtual = getCisalhamentoVer(val);
        }
        else
        {
            cout << "Transformacao invalida" << endl;
            return EXIT_FAILURE;
        }

        MatAcumulada = MatrizAtual * MatAcumulada;

        Matrix3f Mglobal = T * MatAcumulada * Tinv;

        int offset = i * imgE.larg;

        if (mapType == "DIR")
        {
            transf2D(&imgE, &imgS, Mglobal, offset);
        }
        else if (mapType == "INV")
        {
            Matrix3f Minv = Mglobal.inverse();
            transf2DInv(&imgE, &imgS, Minv, offset);
        }
    }

    if (gravar(&imgS, imgSNome))
    {
        cout << "Imagem salva como '" << imgSNome << "'" << endl;
    }

    destruir(&imgE);
    destruir(&imgS);

    return EXIT_SUCCESS;
}