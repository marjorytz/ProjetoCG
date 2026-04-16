#include <iostream>
#include <vector>

#include "PPM.hpp"

using namespace std;

void exerc3()
{
	PPM img;

	criar(&img, 300, 300, RGB(255, 255, 0));
	gravar(&img, "saida-exerc3.ppm");
	destruir(&img);
}

void exerc4()
{
	PPM img;

	criar(&img, 300, 300, RGB(255, 255, 0));
	setLinha(&img, 150, RGB(255, 0, 0));
	gravar(&img, "saida-exerc4.ppm");
	destruir(&img);
}

void exerc6()
{
	PPM img;

	criar(&img, 300, 300, RGB(255, 255, 0));

	int x0 = 100, y0 = 10, x1 = 225, y1 = 164;
	setBordaRGB(&img, x0, y0, x1, y1, RGB(255, 0, 0));

	gravar(&img, "saida-exerc6.ppm");
	destruir(&img);
}

void exerc7()
{
	PPM img;

	criar(&img, 500, 500, RGB(0, 0, 0));

	corAleatoria(&img);

	gravar(&img, "saida-exerc7.ppm");
	destruir(&img);
}

void exerc8()
{
	PPM imgE, imgS;

	ler(&imgE, "numeros.ppm");
	criar(&imgS, imgE.larg, imgE.alt, RGB(0, 0, 0));

	int x0 = 195, y0 = 310, x1 = 380, y1 = 570;

	setRecorteRGB(&imgE, &imgS, x0, y0, x1, y1);

	gravar(&imgS, "saida-exerc8.ppm");
	destruir(&imgE);
	destruir(&imgS);
}

void exerc9()
{
	PPM imgE;
	PGM imgS;

	ler(&imgE, "spider.ppm");
	criarPGM(&imgS, imgE.larg, imgE.alt, 0);

	convertPPMtoPGM(&imgE, &imgS);

	gravarPGM(&imgS, "saida-exerc9.pgm");
	destruir(&imgE);
	destruirPGM(&imgS);
}

void exerc10()
{
	PPM imgE, imgS;

	ler(&imgE, "numeros.ppm");
	criar(&imgS, imgE.larg, imgE.alt, RGB(0, 0, 0));

	setFlipHorizontal(&imgE, &imgS);

	gravar(&imgS, "saida-exerc10.ppm");
	destruir(&imgE);
	destruir(&imgS);
}

void exerc11()
{
	PPM img;

	ler(&img, "numeros-exerc11.ppm");

	int x0 = 391, y0 = 327, x1 = 574, y1 = 575;
	setBordaEspessuraRGB(&img, x0, y0, x1, y1, 10, RGB(255, 0, 0));

	gravar(&img, "numeros-exerc11.ppm");

	destruir(&img);
}

// QUESTÃO 12: Mosaico de Imagens PPM
void exerc12()
{
	cout << "\n--- EXERCICIO 12: MOSAICO DE IMAGENS ---" << endl;

	// Tenta abrir o arquivo de texto
	ifstream arquivo("imagens.txt");
	if (!arquivo.is_open())
	{
		cout << "Erro: Nao foi possivel abrir o arquivo 'imagens.txt'." << endl;
		cout << "Certifique-se de que ele existe na mesma pasta do programa." << endl;
		return;
	}

	// Lê todos os nomes de imagens e guarda no vetor
	vector<string> listaImagens;
	string nome;
	while (arquivo >> nome)
	{
		listaImagens.push_back(nome);
	}
	arquivo.close();

	int N = listaImagens.size();
	if (N == 0)
	{
		cout << "O arquivo 'imagens.txt' esta vazio." << endl;
		return;
	}

	// Lê a primeira imagem apenas para descobrir a largura (L) e altura (A)
	PPM imgTeste;
	if (!ler(&imgTeste, listaImagens[0]))
	{
		cout << "Erro ao carregar a primeira imagem: " << listaImagens[0] << endl;
		return;
	}

	int L = imgTeste.larg;
	int A = imgTeste.alt;
	destruir(&imgTeste);

	// Cria a imagem de saída gigante
	PPM imgS;
	criar(&imgS, L * N, A, RGB(0, 0, 0));

	// Percorre a lista colando cada imagem no mosaico
	for (int i = 0; i < N; i++)
	{
		PPM imgAtual;
		if (ler(&imgAtual, listaImagens[i]))
		{
			int offsetX = i * L; // Deslocamento para colocar a imagem lado a lado

			// Copia os pixels da imagem atual para o quadro correto na imagem de saída
			for (int y = 0; y < A; y++)
			{
				for (int x = 0; x < L; x++)
				{
					RGB corPixel = getPixel(&imgAtual, x, y);
					setPixel(&imgS, x + offsetX, y, corPixel);
				}
			}
			destruir(&imgAtual); // Limpa a imagem atual para não lotar a memória RAM
		}
		else
		{
			cout << "Aviso: Pulando imagem com erro -> " << listaImagens[i] << endl;
		}
	}

	// Grava o resultado final
	if (gravar(&imgS, "saida-exerc12.ppm"))
	{
		cout << "Mosaico com " << N << " imagens gerado com sucesso! Salvo como 'saida-exerc12.ppm'." << endl;
	}

	destruir(&imgS);
}

void exerc13()
{
	PPM imgE, imgS;

	ler(&imgE, "numeros.ppm");
	criar(&imgS, imgE.larg, imgE.alt, RGB(255, 255, 255));

	inverterRGB(&imgE, &imgS);

	gravar(&imgS, "saida-exerc13.ppm");
	destruir(&imgE);
	destruir(&imgS);
}

void exerc14()
{
	PPM imgE, imgS;

	ler(&imgE, "spider.ppm");

	// Cria a imagem de saída com o mesmo tamanho
	criar(&imgS, imgE.larg, imgE.alt, RGB(0, 0, 0));

	// Chama a função para aplicar o efeito
	quantizarCores(&imgE, &imgS);

	// Salva no disco
	if (gravar(&imgS, "saida-exerc14.ppm"))
	{
		cout << "Sucesso! Imagem quantizada salva como 'saida-exerc14.ppm'." << endl;
	}

	// Limpa a memória
	destruir(&imgE);
	destruir(&imgS);
}

void exerc15()
{
	PPM img;
	int x0 = 200, y0 = 180, x1 = 180, y1 = 160;

	criar(&img, 500, 500, RGB(255, 255, 0));

	RGB corL = RGB(0, 0, 255);

	DDALine(&img, 0, 0, img.larg - 1, img.alt - 1, corL);
	DDALine(&img, 0, img.alt - 1, img.larg - 1, 0, corL);

	gravar(&img, "saida-exerc15.ppm");
	destruir(&img);
}

int main(void)
{

	/*
	//#Exemplo1#: criando uma imagem e gravando no disco
	PPM img1;
	criar(&img1, 5, 7, RGB(255, 255, 0));
	gravar(&img1, "exemplo1.ppm");
	destruir(&img1);

	//#Exemplo2#: imprimindo os atributos da imagem
	PPM img2;
	cout << "#Exemplo 2#\n";
	imprimir(&img2);
	criar(&img2, 700, 500, RGB(255, 255, 0));
	imprimir(&img2);
	gravar(&img2, "exemplo2.ppm");
	destruir(&img2);
	imprimir(&img2);

	//#Exemplo3#: ler imagem, getPixel, setPixel
	PPM img3;
	RGB corP;
	cout << "\n\n#Exemplo 3#\n";
	ler(&img3, "exemploPPM.ppm");

	//ler o valor do pixel de coordenadas (x=1, y=1)
	corP = getPixel(&img3, 0,0);
	cout << "A cor do pixel(0,0): [" << corP << "] " << endl;

	setPixel(&img3, 1, 0, RGB(0, 128, 128)); //alterar a cor do pixel(1, 0) para RGB(0, 128, 128)
	setPixel(&img3, 2, 0, RGB(255, 0, 255)); //alterar a cor do pixel(2, 0) para rosa
	setPixel(&img3, 2, 4, RGB(128, 0, 128)); //alterar a cor do pixel(2, 4) para roxo
	gravar(&img3, "exemplo3.ppm"); //gravar o resultado no disco
	destruir(&img3);
	*/

	// exerc3();
	// exerc4();
	// exerc6();
	// exerc7();
	// exerc8();
	// exerc9();
	// exerc10();
	// exerc11();
	// exerc12();
	// exerc13();
	// exerc14();
	// exerc15();

	// cout << "Pressione uma tecla para encerrar o programa.\n";
	// getchar();
	return EXIT_SUCCESS;
}