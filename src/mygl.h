#ifndef _MYGL_H_
#define _MYGL_H_

#include "math.h"
#include "definitions.h"

typedef struct Color {
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
} Color;

typedef struct Pixel {
	int x;
	int y;
	Color color;
} Pixel;

typedef struct Line {
	Pixel pA;
	Pixel pB;
} Line;

typedef struct Triangle {
	Pixel vA;
	Pixel vB;
	Pixel vC;
} Triangle;

unsigned int getMemoryAdress(Pixel p) {
	return (p.x) * 4 + (p.y) * IMAGE_WIDTH * 4;
}

void putPixel(Pixel p) {
	unsigned int adress = getMemoryAdress(p);

	FBptr[adress] = p.color.R;
	FBptr[adress + 1] = p.color.G;
	FBptr[adress + 2] = p.color.B;
	FBptr[adress + 3] = p.color.A;
}

void drawLine(Line l) {

	// Se |DeltaY| < |DeltaX| -> permuta as coordenadas  de cada ponto
	int permuta = 0;
	if (abs(l.pB.y - l.pA.y) > abs(l.pB.x - l.pA.x)) {
		int aux;

		// permuta coordenadas do ponto de inicio da reta
		aux = l.pA.x;
		l.pA.x = l.pA.y;
		l.pA.y = aux;

		// permuta coordenadas do ponto de fim da reta
		aux = l.pB.x;
		l.pB.x = l.pB.y;
		l.pB.y = aux;

		permuta = 1; // variavel para saber se na hora de desenhar, precisa permutar

	}

	// se DeltaX < 0 inverte os pontos de inicio e fim da linha
	if (l.pB.x < l.pA.x) {
		Pixel a = l.pA;
		l.pA = l.pB;
		l.pB = a;
	}

	int dx = l.pB.x - l.pA.x;
	int dy = l.pB.y - l.pA.y;
	int addY = 1;

	// se DeltaY < 0 os valores de y e da variavel de decisão d serão decrementados
	if (dy < 0) {
		addY = -1;
		dy *= -1;
	}

	int d = 2 * dy - dx;
	int addE = 2 * dy;
	int addNE = 2 * (dy - dx);
	int x = l.pA.x;
	int y = l.pA.y;

	Pixel pixel = { x, y, l.pA.color };
	Pixel *p = &pixel;

	//desenha o primeiro pixel
	if (permuta) {
		Pixel permutado = { pixel.y, pixel.x, pixel.color };
		putPixel(permutado);
	} else {
		putPixel(pixel);
	}

	//Calculos para Interpolação de Cor
	float lineSize = dx;
	float addR = (l.pB.color.R - l.pA.color.R) / lineSize;
	float addG = (l.pB.color.G - l.pA.color.G) / lineSize;
	float addB = (l.pB.color.B - l.pA.color.B) / lineSize;
	float addA = (l.pB.color.A - l.pA.color.A) / lineSize;
	float R = l.pA.color.R;
	float G = l.pA.color.G;
	float B = l.pA.color.B;
	float A = l.pA.color.A;

	while (pixel.x < l.pB.x) {

		// Definição das cores para interpolação
		R += addR;
		G += addG;
		B += addB;
		A += addA;
		p->color.R = R;
		p->color.G = G;
		p->color.B = B;
		p->color.A = A;

		if (d <= 0) {
			d += addE;
			p->x++;
		} else {
			d += addNE;
			p->x++;
			p->y += addY;
		}

		//se as coordenadas foram permutadas, aqui elas permutadas novamente
		if (permuta) {
			Pixel permutado = { pixel.y, pixel.x, pixel.color };
			putPixel(permutado);
		} else {
			putPixel(pixel);
		}
	}
}

void drawTriangle(Triangle t) {

	Line line1 = { t.vA, t.vB };
	Line line2 = { t.vB, t.vC };
	Line line3 = { t.vC, t.vA };

	drawLine(line1);
	drawLine(line2);
	drawLine(line3);

}

/*Para uso do drawFilledTriangle
 *
 * Essa função é uma variação do algoritmo de braseham, ela faz os mesmos calculos para determinar
 * o proximo pixel de uma reta entre os pontos A e B, mas ao inves de desenhar o pixel, ela chama
 * o drawLine para desenhar uma reta de C para cada pixel da reta AB
 *
 */
void drawAllLines(Pixel p1, Pixel p2, Pixel p3) {

	// Se |DeltaY| < |DeltaX| -> permuta as coordenadas  de cada ponto
	int permuta = 0;
	if (abs(p2.y - p1.y) > abs(p2.x - p1.x)) {
		int aux;

		// permuta coordenadas do ponto de inicio da reta
		aux = p1.x;
		p1.x = p1.y;
		p1.y = aux;

		// permuta coordenadas do ponto de fim da reta
		aux = p2.x;
		p2.x = p2.y;
		p2.y = aux;

		permuta = 1; // variavel para saber se na hora de desenhar, precisa permutar

	}

	// se DeltaX < 0 inverte os pontos de inicio e fim da linha
	if (p2.x < p1.x) {
		Pixel a = p1;
		p1 = p2;
		p2 = a;
	}

	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	int addY = 1;

	// se DeltaY < 0 os valores de y e da variavel de decisão d serão decrementados
	if (dy < 0) {
		addY = -1;
		dy *= -1;
	}

	int d = 2 * dy - dx;
	int addE = 2 * dy;
	int addNE = 2 * (dy - dx);
	int x = p1.x;
	int y = p1.y;

	Pixel pixel = { x, y, p1.color };
	Pixel *p = &pixel;

	//Calculos para Interpolação de Cor
	float lineSize = dx;
	float addR = (p2.color.R - p1.color.R) / lineSize;
	float addG = (p2.color.G - p1.color.G) / lineSize;
	float addB = (p2.color.B - p1.color.B) / lineSize;
	float addA = (p2.color.A - p1.color.A) / lineSize;
	float R = p1.color.R;
	float G = p1.color.G;
	float B = p1.color.B;
	float A = p1.color.A;

	while (pixel.x < p2.x) {

		// Definição das cores para interpolação
		R += addR;
		G += addG;
		B += addB;
		A += addA;
		p->color.R = R;
		p->color.G = G;
		p->color.B = B;
		p->color.A = A;

		if (d <= 0) {
			d += addE;
			p->x++;
		} else {
			d += addNE;
			p->x++;
			p->y += addY;
		}

		//se as coordenadas foram permutadas, aqui elas permutadas novamente
		if (permuta) {
			Pixel permutado = { pixel.y, pixel.x, pixel.color };
			Line line = { p3, permutado };
			drawLine(line);
		} else {

			Line line = { p3, pixel };
			drawLine(line);
		}

	}

}

void drawFilledTriangle(Triangle t) {
	// Para cada pixel P da reta entre os vértices A e B, desenha uma reta que vai de C até P
	drawAllLines(t.vA, t.vB, t.vC);
	// Para cada pixel P da reta entre os vértices A e C, desenha uma reta que vai de B até P
	drawAllLines(t.vA, t.vC, t.vB);
	// Para cada pixel P da reta entre os vértices B e C, desenha uma reta que vai de A até P
	drawAllLines(t.vB, t.vC, t.vA);
}

void drawTriangles() {
	Pixel p1 = { 26, 235, { 255, 0, 0, 255 } };
	Pixel p2 = { 247, 233, { 0, 255, 0, 255 } };
	Pixel p3 = { 120, 58, { 0, 0, 255, 255 } };
	Triangle t1 = { p1, p2, p3 };
	drawFilledTriangle(t1);

	Pixel p4 = { 29, 308, { 248, 121, 126, 220 } };
	Pixel p5 = { 239, 392, { 7, 151, 194, 36 } };
	Pixel p6 = { 45, 458, { 159, 137, 97, 140 } };
	Triangle t2 = { p4, p5, p6 };
	drawFilledTriangle(t2);

	Pixel p7 = { 369, 456, { 125, 74, 62, 255 } };
	Pixel p8 = { 500, 330, { 192, 247, 224, 239 } };
	Pixel p9 = { 352, 300, { 103, 234, 29, 94 } };
	Triangle t3 = { p7, p8, p9 };
	drawFilledTriangle(t3);

	Pixel p10 = { 332, 57, { 228, 225, 223, 162 } };
	Pixel p11 = { 445, 292, { 137, 13, 157, 220 } };
	Pixel p12 = { 397, 68, { 105, 49, 76, 204 } };
	Triangle t4 = { p10, p11, p12 };
	drawFilledTriangle(t4);

	Pixel p13 = { 200, 100, { 193, 175, 255, 231 } };
	Pixel p14 = { 422, 294, { 118, 84, 223, 4 } };
	Pixel p15 = { 134, 13, { 14, 227, 61, 156 } };
	Triangle t5 = { p13, p14, p15 };
	drawFilledTriangle(t5);

}

void drawPolygon(Pixel vertices[], int qtVert) {
	int i;
	for (i = 0; i < qtVert - 1; i++) {
		Line l = { vertices[i], vertices[i + 1] };
		drawLine(l);
	}
	Line l = { vertices[qtVert - 1], vertices[0] };
	drawLine(l);
}

void drawFilledPolygon(Pixel vertices[], int qtVert, Color color) {

	int i, j, k;

	for (i = 0; i < qtVert; i++) {
		vertices[i].color = color;
	}

	for (i = 0; i < qtVert; i++) {
		for (j = i + 1; j < qtVert; j++) {
			for (k = j + 1; k < qtVert; k++) {
				Triangle t = { vertices[i], vertices[j], vertices[k] };
				drawFilledTriangle(t);
			}

		}
	}

}

void showPolygons() {

	Pixel p1 = { 97, 64, { 255, 0, 0, 255 } };
	Pixel p2 = { 204, 63, { 0, 255, 0, 255 } };
	Pixel p3 = { 270, 140, { 0, 0, 255, 255 } };

	Pixel p4 = { 252, 240, { 248, 121, 126, 220 } };
	Pixel p5 = { 137, 259, { 7, 151, 194, 36 } };
	Pixel p6 = { 47, 190, { 159, 137, 97, 140 } };

	Pixel v[] = { p1, p2, p3, p4, p5, p6 };
	Color white = { 255, 255, 255, 255 };
	drawFilledPolygon(v, 6, white);
}
void drawOctants() {
	Color branco = { 255, 255, 255, 255 };
	int i;

	for (i = 0; i < IMAGE_WIDTH; i++) {

		//desenha eixo x
		Pixel p = { i, IMAGE_HEIGHT / 2, branco };
		putPixel(p);

		//desenha eixo y
		p.x = IMAGE_WIDTH / 2;
		p.y = i;
		putPixel(p);

		//desenha diagonal principal
		p.x = i;
		p.y = i;
		putPixel(p);

		//desenha diagonal secundária
		p.x = 512 - i;
		p.y = i;
		putPixel(p);
	}

}

void drawOctantLines() {
	Pixel centro = { IMAGE_WIDTH / 2, IMAGE_HEIGHT / 2, { 255, 99, 71, 255 } };

	//desenha linha no primeiro octante
	Color cor1 = { 255, 20, 147, 255 };
	Pixel final1 = { IMAGE_WIDTH - 1, IMAGE_HEIGHT * 3 / 4, cor1 };
	Line linha1 = { centro, final1 };
	drawLine(linha1);

	//desenha linha no segundo octante
	Color cor2 = { 255, 255, 0, 255 };
	Pixel final2 = { IMAGE_WIDTH * 3 / 4, IMAGE_HEIGHT - 1, cor2 };
	Line linha2 = { centro, final2 };
	drawLine(linha2);

	//desenha linha no terceiro octante
	Color cor3 = { 152, 251, 152, 255 };
	Pixel final3 = { IMAGE_WIDTH / 4, IMAGE_HEIGHT - 1, cor3 };
	Line linha3 = { centro, final3 };
	drawLine(linha3);

	//desenha linha no quarto octante
	Color cor4 = { 0, 255, 0, 255 };
	Pixel final4 = { 0, IMAGE_HEIGHT * 3 / 4, cor4 };
	Line linha4 = { centro, final4 };
	drawLine(linha4);

	//desenha linha no quinto octante
	Color cor5 = { 216, 191, 216, 255 };
	Pixel final5 = { 0, IMAGE_HEIGHT / 4, cor5 };
	Line linha5 = { centro, final5 };
	drawLine(linha5);

	//desenha linha no sexto octante
	Color cor6 = { 0, 0, 255, 255 };
	Pixel final6 = { IMAGE_WIDTH / 4, 0, cor6 };
	Line linha6 = { centro, final6 };
	drawLine(linha6);

	//desenha linha no sétimo octante
	Color cor7 = { 173, 255, 47, 255 };
	Pixel final7 = { IMAGE_WIDTH * 3 / 4, 0, cor7 };
	Line linha7 = { centro, final7 };
	drawLine(linha7);

	//desenha linha no oitavo octante
	Color cor8 = { 255, 69, 0, 255 };
	Pixel final8 = { IMAGE_WIDTH - 1, IMAGE_HEIGHT / 4, cor8 };
	Line linha8 = { centro, final8 };
	drawLine(linha8);
}

#endif // _MYGL_H_

