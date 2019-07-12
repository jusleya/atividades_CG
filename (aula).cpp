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

int main(int argc, char* argv[]) {
	printf("ARG1 = %s\n", argv[1]);
	FILE* input =fopen(argv[1], "r");
	FILE* output = fopen("imagem.ppm", "w");
	
	//Manipulação de arquivo
	int j=0; //j: contador das linhas
	char* g; //g: salva cada linha do arquivo
	char linha[100]; //linha[i]: cada posição recebe uma linha do arquivo por caracter
	vector <string> l(6); //l: recebe cada posição de linha e salva em uma string (um vector de string que salva as linhas)
	while(!feof(input)){
		g=fgets(linha, 100, input);
		if(g) l[j]=linha, j++;
	}
	fclose(input);
	
	int mt[3][4]; //mt: salva os valores dado
	// linha 1: dados da esfera
	// linha 2: dados da janela de visualizacao
	// linha 3: posicao da fonte de luz
	
	int nn=0, m=0;
	
	//Começa a tratar l:
	for(int i=0; i<j; i++){
		string lin; //lin: ajuda na manipulação recebendo cada linha de l (uma por vez), não consegui tratar usando só l
		lin=l[i];
		int k=0, aux2=lin.size();
		while(k<aux2+1){
			if(lin[k]>47 && lin[k]<58){
				if(lin[k-1]>47 && lin[k-1]<58){ //Caso seja dois números seguidos
					mt[nn][m-1]=(mt[nn][m-1]*10)+(lin[k]-48);
				}
				else mt[nn][m]=(lin[k]-48), m++;
			}
			if(lin[k-1]==45) mt[nn][m-1]=(mt[nn][m-1]*-1);
			k++;
		}
		nn++;
		m=0;
	}
	//fim da manipulação em l
	for(int i=0; i<3; i++) {
		for(int j=0; j<4; j++) {
			cout << mt[i][j] << " ";
		}
		cout << "\n";
	}
	
	fprintf(output, "P3\n");
	
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
	
	n=mt[1][0];
	fov=mt[1][1];
	resx=mt[1][2];
	resy=mt[1][3];
	
	rad_fov = ((fov * 3.14)/180);
	
	fprintf(output, "%d %d\n", resx, resy);
	fprintf(output, "255\n");
	
	ty= tan(rad_fov/2)*(2*n);
	tx= (ty*resx)/resy;
	
	float aux_1=0, aux_2=0;
	aux_1= -(tx/2);
	aux_2= -(ty/2);
	p1.push_back(aux_1);
	p1.push_back(aux_2);
	p1.push_back(-n);
	p2.push_back(-aux_1);
	p2.push_back(-aux_2);
	p2.push_back(-n);
	
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
			cout << "("<< p[i][j].r << ", " << p[i][j].g << ", " << p[i][j].b << ")";
		}
		cout << "\n";
	}
	//funcao d:
	vector<int> op;
	vector<float> t1, t2;
	float a=0, b=0, c=0;
	float r=0, y=0;
	int d[3], e[3];
	
	//funcao d:
	for(int i=0; i<3; i++) op.push_back(0);
	e[0]=mt[0][0];
	e[1]=mt[0][1];
	e[2]=mt[0][2];
	r=mt[0][3];
	
	struct p img[resx][resy];
	
	for(int i=0; i<resx; i++){
		for(int j=0; j<resy; j++){
			float aux1=0, aux2=0;
			d[0]=p[i][j].r;
			d[1]=p[i][j].g;
			d[2]=p[i][j].b;
			
			a=(d[0]*d[0])+(d[1]*d[1])+(d[2]*d[2]);
			b=(d[0]*(e[0]-op[0]))+(d[1]*(e[1]-op[1]))+(d[2]*(e[2]-op[2]));
			b=2*b;
			c=((e[0]-op[0])*(e[0]-op[0]))+((e[1]-op[1])*(e[1]-op[1]))+((e[2]-op[2])*(e[2]-op[2]));
			c=c-(r*r);
			//Calculo delta
			y=(b*b)-(4*a*c);
			if(y<0) {
				img[i][j].r=0;
				img[i][j].g=0;
				img[i][j].b=255;
			}
			else {
				aux1=(((-b)+sqrt(y))/(2*a));
				aux2=(((-b)-sqrt(y))/(2*a));
				//t1.push_back(aux1);
				//t2.push_back(aux2);
				if(aux1>=1 || aux2>=1){
					img[i][j].r=0;
					img[i][j].g=255;
					img[i][j].b=0;
					//printf("branco");
				}
			}
		}
	}
	
	for(int i=0; i<resx; i++){
		for(int j=0; j<resy; j++){
			fprintf(output, "%.0f %.0f %.0f     ", img[i][j].r, img[i][j].g, img[i][j].b);
		}
		fprintf(output, "\n");
	}

	fclose(output);
	return 0;
}
