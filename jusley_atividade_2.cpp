//Jusley Arley de Oliveira Tavares
//As duas questoes estao aqui, eh so comentar uma e descomentar a outra

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace std;

int main () {
	// Questao 1
	vector<int> o, d;
	vector<float> e;
	int x=0;
	float a=0, b=0, c=0;
	float t1=0, t2=0, r=0, y=0;
	for(int i=0; i<3; i++) o.push_back(0), e.push_back(0);
	printf("Informe os valores da direcao do raio: \n");
	for(int i=0; i<3; i++){
		scanf("%d", &x);
		d.push_back(x);
	}
	
	printf("Raio: \n");
	scanf("%f", &r);
	
	a=(d[0]*d[0])+(d[1]*d[1])+(d[2]*d[2]);
	b=(d[0]*(e[0]-o[0]))+(d[1]*(e[1]-o[1]))+(d[2]*(e[2]-o[2]));
	b=2*b;
	c=((e[0]-o[0])*(e[0]-o[0]))+((e[1]-o[1])*(e[1]-o[1]))+((e[2]-o[2])*(e[2]-o[2]));
	c=c-(r*r);
	
	y=(b*b)-(4*a*c);
	
	t1=(((-b)+sqrt(y))/(2*a));
	t2=(((-b)-sqrt(y))/(2*a));
	printf("Valor de t': %.2f, t'': %.2f.\n", t1, t2);

	
	//Questao 2
	/*int nx=0, ny=0, cont=0;
	float n=0;
	printf("Informe os valores para nx e ny, respectivamente:\n");
	scanf("%d %d", &nx, &ny);
	printf("Informe o valor de n:\n");
	scanf("%f", &n);
	int Mx[nx][nx], My[ny][ny];
			
	for(int j=0; j<nx; j++){
		for(int i=0; i<nx; i++)
			Mx[i][j]=i;
		cont++;
	}
	
	for(int i=0; i<ny; i++)
		for(int j=0; j<ny; j++)
			My[i][j]=j;

	printf("Matriz:\n");
	for(int i=0; i<nx; i++){
		for(int j=0; j<nx; j++){
			printf("(%d,%d) ", Mx[i][j], My[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
	float x=0, y=0;
	cont=cont/2;
	if(cont==1)
		x=-0.5, y=0.5;
	else{
		x=((-1)*cont)-0.5;
		y=cont+0.5;
	}
	
	printf("Os pontos que compoem o centro dos pixels sao: (%.2f, %.2f, %.2f)", x, y, -n);*/
	
	return 0;
}
