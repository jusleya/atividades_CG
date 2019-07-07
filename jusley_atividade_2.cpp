//Jusley Arley de Oliveira Tavares
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace std;

struct p {
	float r;
	float g;
	float b;
};

int main () {
	//Pontos
	vector<float> p1, p2;
	//valores da visualizacao em x e y
	float tx=0, ty=0;
	//resolucao
	int resx=0, resy=0;
	//fov: em graus, n: near, rad_fov: fov em radianos
	float fov=0, n=0, rad_fov=0;
	//valores de delta
	float deltax=0, deltay=0;
	
	//funcao d:
	vector<int> op, d;
	vector<float> e;
	float a=0, b=0, c=0;
	float t1=0, t2=0, r=0, y=0;
	
	printf("Informe o valor da fov:\n");
	scanf("%f", &fov);
	rad_fov = ((fov * 3.14)/180);
	
	printf("Informe o valor do near:\n");
	scanf("%f", &n);
	
	printf("Informe o valor para resx e resy:\n");
	scanf("%d %d", &resx, &resy);
	
	ty= tan(fov/2)*(2*n);
	tx= (ty*resx)/resy;
	
	float aux_1=0, aux_2=0;
	aux_1= -(tx/2);
	aux_2= -(ty/2);
	p1.push_back(aux_1);
	p1.push_back(aux_2);
	p1.push_back(n);
	
	deltax= ((p1[0]-p2[0])/resx);
	deltay= ((p1[1]-p2[1])/resy);
	
	int o[3];
	o[0]= p1[0]+0+(-deltax/2);
	o[1]= p1[1]+(-deltay/2)+0;
	o[2]= p1[2]+0+0;
	
	//imagem
	struct p p[resx][resy];
	for(int i=0; i<resx; i++) {
		for(int j=0; j<resy; j++) {
			p[i][j].r = o[0] + (i+1)*(0) + deltax;
			p[i][j].g = o[1] + (i+1)*(deltay) + 0;
			p[i][j].b = o[2] + (i+1)*(0) + 0;
		}
	}
	
	//funcao d:
	for(int i=0; i<3; i++) op.push_back(0), e.push_back(0);
	printf("Raio: \n");
	scanf("%f", &r);
	// d vai receber os valores de pij. faca um for pra pecorrer pij e salve em d,
	// depois veja se t1 e t2 sao valores validos para o hist, se for, pinta de branco, senao preto
	a=(d[0]*d[0])+(d[1]*d[1])+(d[2]*d[2]);
	b=(d[0]*(e[0]-op[0]))+(d[1]*(e[1]-op[1]))+(d[2]*(e[2]-op[2]));
	b=2*b;
	c=((e[0]-op[0])*(e[0]-op[0]))+((e[1]-op[1])*(e[1]-op[1]))+((e[2]-op[2])*(e[2]-op[2]));
	c=c-(r*r);
	
	y=(b*b)-(4*a*c);
	
	t1=(((-b)+sqrt(y))/(2*a));
	t2=(((-b)-sqrt(y))/(2*a));
	printf("Valor de t': %.2f, t'': %.2f.\n", t1, t2);
	
	return 0;
}
