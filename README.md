# Ray Tracing em C++

Repostório focado no meu aprendizado de Ray Tracing utilizando o C++ e os conhecimentos adquiridos na literatura (os links dos livros estão disponíveis na seção Créditos).

Sobre os arquivos:

- BuleUtah: o [Bule de Utah](https://pt.wikipedia.org/wiki/Bule_de_Utah) ou bule de Newell é um tradicional modelo 3D usado para testes e também uma piada interna no ramo da computação gráfica.

- CornellBox: é uma [Cornell Box](https://en.wikipedia.org/wiki/Cornell_box), uma das formas da computação gráfica para testar a renderização de objetos "3D".

- Esfera: é uma esfera, um objeto 3D leve de renderizar e bom para fazer testes de textura, material, foco ou fonte de luz, e etc.

Também será implementado métodos de paralelismo para tornar a renderização mais eficiente, também foi implementado um suporte para renderizar no Terminal, e logo irei organizar melhor as pastas do conteúdo do projeto. Foi feito no Linux.

## Instalação

Instalação no Linux para as ferramentas do C e C++ (Debian). 
Também será instalado a biblioteca [stb](https://github.com/nothings/stb), que é usada para pegar imagens para as texturas dos objeto:

```
sudo apt install cmake make gcc g++ libstb-dev
```

Instalação no Windows para as ferramentas do C e C++. Para o [stb](https://github.com/nothings/stb) nesse caso, faça clone do repositório, coloque em uma pasta chamada external e vá em Texture e arrume o diretório da chamada do arquivo:

No Windows use o MinGW e instale o gcc, g++, cmake e make, o link do site:

```
https://www.mingw-w64.org/
```

Após instalar as ferramentas necessárias, fazer os seguintes passos:

Fazer o clone do projeto:
```
git clone https://github.com/lucasfturos/RayTracing.git && cd RayTracing/
```
E criar o arquivo de build 
```
cmake -B build
cd build/
make
```

## Como Usar



## Créditos

Eu recomendo a leitura dos livros e acesse o repositório [RayTracing](https://github.com/RayTracing/raytracing.github.io) para ter acesso a todo o conteúdo (livro, código fonte e etc.) sobre essa prática da computação gráfica.

Aqui o link direto para os livros:
- [_RayTracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html);
- [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html);
- [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html).

O Suporte para renderizar as imagens no terminal foi feito utilizando como base o vídeo [3D ASCII with Colors](https://youtu.be/M-sIyi9VMSU) do canal [Tsoding Daily](https://www.youtube.com/@TsodingDaily).

