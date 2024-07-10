# Ray Tracing em C++

Repostório focado no meu aprendizado de Ray Tracing utilizando o C++ e os conhecimentos adquiridos na literatura (os links dos livros estão disponíveis na seção Créditos).

Também tem a versão em Rust na pasta `rust-version`.

Sobre os arquivos:

-   BuleUtah: o [Bule de Utah](https://pt.wikipedia.org/wiki/Bule_de_Utah) ou bule de Newell é um tradicional modelo 3D usado para testes e também uma piada interna no ramo da computação gráfica.

-   CornellBox: é uma [Cornell Box](https://en.wikipedia.org/wiki/Cornell_box), uma das formas da computação gráfica para testar a renderização de objetos "3D".

-   Esfera: é uma esfera, um objeto 3D leve de renderizar e bom para fazer testes de textura, material, foco ou fonte de luz, e etc.

Também será implementado métodos de paralelismo para tornar a renderização mais eficiente, também foi implementado um suporte para renderizar no Terminal, e logo irei organizar melhor as pastas do conteúdo do projeto. Foi feito no Linux.

## Instalação e Execução

### Requisitos

-   CMake
-   Make
-   GCC/G++(ou MinGW no Windows)
-   Biblioteca STB
-   SDL2
-   Vulkan

### Instruções

1. Clone o repositório e vá para o diretório do projeto:

```
git clone https://github.com/lucasfturos/RayTracing.git && cd RayTracing/
```

2. Crie o diretório de build e compile o projeto:

```
cmake -B build
cd build/
make
```

### Como Usar

O sistema tem as seguintes opções de objetos:

-   Obj: Bule de Utah
-   Simple: Esfera
-   Room: CornellBox

E as seguintes formas de visualização:

-   SDL: Janela de visualização
-   PPM: Renderiza em uma imagem
-   Term: Renderiza no terminal

Para usar é só usar esse dessa forma:

```
./src/Ray Objeto Visualizar
```

Exemplos:

```
./src/Ray Simple SDL
./src/Ray Room PPM > out.ppm
./src/Ray Obj Term
```

## Créditos

1.  [_RayTracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html);
2.  [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html);
3.  [_Ray Tracing: The Rest of Your Life_](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html).
4.  [3D ASCII with Colors](https://youtu.be/M-sIyi9VMSU)
