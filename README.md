# Ray Tracing em C++

Repostório focado no meu aprendizado de Ray Tracing utilizando o C++ e os conhecimentos adquiridos na literatura (os links dos livros estão disponíveis na seção Créditos).

Sobre os arquivos:
- CornellBox é uma [Cornell Box](https://en.wikipedia.org/wiki/Cornell_box), uma das formas da computação gráfica para testar a renderização de objetos "3D". Foi a própria linguagem de progrmação C++ para fazer o plot do objeto, a técnica utilizada é o RayTracing.

- Esfera é uma esfera feita utilizando a própria linguagem de progrmação C++ para fazer o plot do objeto, a técnica utilizada é o RayTracing.

Também será implementado métodos de paralelismo para tornar a renderização mais eficiente, também foi implementado um suporte para renderizar no Terminal, e logo irei organizar melhor as pastas do conteúdo do projeto. Foi feito no Linux.

## Instalação e como usar

Instalação no Linux para as ferramentas do C e C++ (Debian):

```
sudo apt install cmake make gcc g++
```

Instalação no Windows para as ferramentas do C e C++:

No Windows use o MinGW e instale o gcc, g++, cmake e make, o link do site:

```
https://www.mingw-w64.org/
```

Após instalar as ferramentas necessárias, fazer os seguintes passos:

Fazer o clone do projeto:
```
git clone https://github.com/lucasfturos/RayTracing.git && cd RayTracing/
```
E criar o arquivo de build, dando como exemplo o projeto Esfera/ 
```
cmake -B build
cd build/
make
./EsferaRayTracing >> output.ppm
eog output.ppm
```

O comando eog é um programa de visualização de imagens do Gnome, caso não tenha use um visualizador de imagens compatível com o formato .ppm.

## Créditos

Eu recomendo a leitura dos livros e acesse o repositório [RayTracing](https://github.com/RayTracing/raytracing.github.io) para ter acesso a todo o conteúdo (livro, código fonte e etc.) sobre essa prática da computação gráfica.

Aqui o link direto para os livros:
- [_RayTracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html);
- [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html);
- [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html).

O Suporte para renderizar as imagens no terminal foi feito utilizando como base o vídeo [3D ASCII with Colors](https://youtu.be/M-sIyi9VMSU) do canal [Tsoding Daily](https://www.youtube.com/@TsodingDaily).

