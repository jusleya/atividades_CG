//Autor: Jusley Arley de Oliveira Tavares
//Última modificação: 06/03/2017

//"Padrões"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//Classes
#include "Leao.h"
#include "Zebra.h"
#include "Tipo_planta.h"
//Para geometria
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
//Modelos
#include "carregadorObj.cpp"

//Para compilar:
//g++ -o main main.cpp Leao.cpp Zebra.cpp Tipo_planta.cpp -lGL -lGLU -lglut
//Para executar:
// ./main <arquivo de entrada>


using namespace std;

//Variáveis para arquivo
FILE* input;

//variáveis para geometria
float angle=0.0f, deltaMove=0.0f, deltaAngle=0.0f;
float lx=0.0f,lz=-1.0f; //Onde a câmera "vê"
float x=2.0f, z=3.0f, y=0.0f; //Posição da câmera na cena
float rotCameraX=0.0f, rotCameraY=0.0f; //para rotação do mouse
int posicaoMouseX, posicaoMouseY; //posição inicial do mouse

//Mundo:
vector<vector<int> > mundo;
//Ilha:
int margemx=0, margemz=0;
float porcI=0; //porcI: porcentagem da ilha (valor dado no arquivo)
//Onde começa e termina a ilha dentro da matriz mundo:
int xi=0, xf=0, zi=0, zf=0;
//Para os lagos:
int ilhaT=0, qntL=0; //ilhaT: Tamanho total da ilha (área), qntL: quantidade de lagos que terá
float porcL=0; //porcL: porcentagem do lago (valor dado no arquivo)
vector<int> xl; //Posições dos lagos em x
vector<int> zl; //Posições dos lagos em z
//Cena:
float px=0.0f, py=0.0f, pz=0.0f; //Posições da cena (valores x, y e z dado no arquivo)

//Animais e Plantas
vector <Leao *> leos; //Vetor de leões
vector <Zebra *> zes; //Vetor de zebras
vector <Tipo_planta *> planta; //Vetor de plantas
//nleoes: quantidade de leoes, nzebras: quantidade de zebras, nplantas: quantidade de plantas
int nleoes=0, nzebras=0, nplantas=0;

//Modelos 3D dos objetos
Modelo* obj, *obj1, *obj2, *obj3;

//Inicializa os parâmetros e define a cor de fundo da janela
void init(void){
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);
}

//Ajuda a mover pra frente ou pra trás
void computePos(float deltaMove){
	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

//Ajuda a virar pra esquerda ou direita
void computeDir(float deltaAngle){
	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}

//Desenha cena
void cena(){
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
		glColor3ub (39, 191, 255);
		glBegin(GL_QUADS);
			glVertex3f (0.0, 0.0, 0.0);
			glVertex3f (px, 0.0, 0.0);
			glVertex3f (px, 0.0, pz);
			glVertex3f (0.0, 0.0, pz);
		glEnd();
	glPopAttrib();
	glPopMatrix();
}

//Desenhar a ilha
void ilha(){
	glClear(GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
		glColor3ub(0, 128, 0);
		glBegin(GL_QUADS);
			glVertex3f(xi, 0.0, zf);
			glVertex3f(xf, 0.0, zf);
			glVertex3f(xf, 0.0, zi);
			glVertex3f(xi, 0.0, zi);
		glEnd();
	glPopAttrib();
	glPopMatrix();
}

//Desenhar lagos
void flagos(){
	glClear(GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		glColor3ub(0, 0, 255);
		glBegin(GL_QUADS);
			glVertex3f(0.0, 0.0, 0.0);
			glVertex3f(1, 0.0, 0.0);
			glVertex3f(1, 0.0, 1);
			glVertex3f(0.0, 0.0, 1);
		glEnd();
		glPopMatrix();
}

//Desenhar leão
void fleao(){
	glEnable(GL_TEXTURE_2D);
	glRotatef(90, -1, 0, 0);
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
		obj1->desenhar();
	glPopAttrib();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//Desenhar zebra
void fzebra(){
	glEnable(GL_TEXTURE_2D);
	glRotatef(90, -1, 0, 0);
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
		obj2->desenhar();
	glPopAttrib();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//Desenhar planta
void fplanta(){
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
		obj3->desenhar();
	glPopAttrib();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//Para movimentar o leão (com bug)
void movimento(int i){
	int x=leos[i]->get_posX();
	int z=leos[i]->get_posZ();
	if(leos[i]->get_dir()==0){
		if(mundo[x+1][z]==1){
			leos[i]->set_posX(leos[i]->get_posX()+0.02);
		}
		else{
			leos[i]->set_posX(leos[0]->get_posX()-0.02);
			leos[i]->set_dir(rand()%3);
		}
	}
	 if(leos[i]->get_dir()==1){
		if(mundo[x-1][z]==1){
			leos[i]->set_posX(leos[0]->get_posX()-0.02);
		}
		else{
			leos[i]->set_posX(leos[i]->get_posX()+0.02);
			leos[i]->set_dir(rand()%3);
		}
	}
	if(leos[i]->get_dir()==2){
		if(mundo[x][z+1]==1){
			leos[i]->set_posZ(leos[0]->get_posZ()+0.02);
		}
		else{
			leos[i]->set_posZ(leos[i]->get_posZ()-0.02);
			leos[i]->set_dir(rand()%3);
		}
	}
	 if(leos[i]->get_dir()==3){
		if(mundo[x][z-1]==1){
			leos[i]->set_posZ(leos[i]->get_posZ()-0.02);
		}
		else{
			leos[i]->set_posZ(leos[0]->get_posZ()+0.02);
			leos[i]->set_dir(rand()%3);
		}
	}
}

//Para movimentar o zebra (com bug e bug quando coloca pra executar junto do movimento do leão)
void movimentoz(int i){
	int x=zes[i]->get_posXz();
	int z=zes[i]->get_posZz();
	if(zes[i]->get_dirz()==0){
		if(mundo[x+1][z]!=1){
			zes[i]->set_posZz(zes[i]->get_posZz()-0.02);
		}
	}
	else  if(zes[i]->get_dirz()==2){
		if(mundo[x-1][z]!=1){
			zes[i]->set_posZz(zes[0]->get_posZz()+0.02);
		}
	}
	else if(zes[i]->get_dirz()==1){
		if(mundo[x][z+1]!=1){
			zes[i]->set_posXz(zes[0]->get_posXz()-0.02);
		}
	}
	else if(zes[i]->get_dirz()==3){
		if(mundo[x][z-1]!=1){
			zes[i]->set_posXz(zes[i]->get_posXz()+0.02);
		}
	}
}

//Adiciona as coisas na cena
void display(void){
	if(deltaMove) computePos(deltaMove);
	if(deltaAngle) computeDir(deltaAngle);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, 10.0f, z, x+lx, 1.0f, z+lz, 0.0f, 1.0f, 0.0f);
	glRotatef(rotCameraX, 1.0f, 0.0f, 0.0f);
	glRotatef(rotCameraY, 0.0f, 1.0f, 0.0f);
	glTranslatef(-x, -y, -z);
	cena(); //Adiciona a cena (o rio)
	ilha(); //Adiciona a ilha na cena
	//Adiciona os lagos na ilha
	for(int i=0; i<qntL; i++){
		glPushMatrix();
		glPushAttrib(GL_CURRENT_BIT);
			glTranslatef(xl[i],0,zl[i]); //Translada para as posições que foram passadas em xl e zl
			glScalef(0.45,0.45,0.45);
			flagos();
		glPopAttrib();
		glPopMatrix();
	}
	//Fim dos lagos
	//Adicionando os leões na ilha
	for(int i=0;i<nleoes;i++){
		glPushMatrix();
		glPushAttrib(GL_CURRENT_BIT);
			glTranslatef(leos[i]->get_posX(), 0,leos[i]->get_posZ()); //Translada para as posições que foram passadas
			glScalef(0.2, 0.2, 0.2);
			fleao();
			//Fim das restrições
			movimento(i); //Função da movimentação
		glPopAttrib();
		glPopMatrix();
	}
	//Fim dos leões
	//Adicionando os zebras na ilha
	for(int i=0;i<nzebras;i++){
		glPushMatrix();
		glPushAttrib(GL_CURRENT_BIT);
			glTranslatef(zes[i]->get_posXz(), 0,zes[i]->get_posZz()); //Translada para as posições que foram passadas
			glScalef(0.2, 0.2, 0.2);
			fzebra();
		glPopAttrib();
		glPopMatrix();
	}
	//Fim dos zebras
	//Adicionando os plantas na ilha
	for(int i=0;i<nplantas;i++){
		glPushMatrix();
		glPushAttrib(GL_CURRENT_BIT);
			glTranslatef(planta[i]->get_posXp(), 0,planta[i]->get_posZp()); //Translada para as posições que foram passadas
			glScalef(0.05, 0.05, 0.05);
			fplanta();
		glPopAttrib();
		glPopMatrix();
	}
	//Fim dos plantas
	glutSwapBuffers();
}

//Função para o que a câmera vê
void reshape(int w, int h){
	if(h==0) h=1;
	float raio=w*1.0/h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, raio, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

//Como mover com o uso do teclado
void specialKeys(int key, int xx, int yy){
	switch(key){
		case GLUT_KEY_LEFT: deltaAngle=-0.01f; break;
		case GLUT_KEY_RIGHT: deltaAngle=0.01f; break;
		case GLUT_KEY_UP: deltaMove=0.5f; break;
		case GLUT_KEY_DOWN: deltaMove=-0.5f; break;
	}
}

void releaseKey(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT: deltaAngle=0.0f; break;
		case GLUT_KEY_UP:
		case GLUT_KEY_DOWN: deltaMove=0; break;
	}
}

//Como mover com o mouse
void olharAoRedor(int x, int y){
	rotCameraY += 0.15*(x-posicaoMouseX);
	rotCameraX += 0.15*(y-posicaoMouseY);
	posicaoMouseY = y;
	posicaoMouseX = x;
}

void atualizarMouse(int x, int y){
	posicaoMouseY = y;
	posicaoMouseX = x;
}

int main(int argc, char* argv[]){
	printf("#ARGS = %i\n", argc);
	printf("PROGRAMA = %s\n", argv[0]);
	printf("ARG1 = %s\n", argv[1]);
	
	//Inicializando o arquivo:
	input = fopen(argv[1], "r");
	//fim de inicialização
	
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
	//fim da manipulação de arquivo

	//Manipulação em cada linha (ou seja, em l)
	int mt[6][4]; //mt: salva os valores dado
	//linha 0 é pra cena
	//linha 1 é para a ilha
	//linha 2 é para a lagos
	//linha 3 é para a leoes
	//linha 4 é para a zebras
	//linha 5 é para a plantas
	int n=0, m=0;
	//Preenche mt:
	for(int h=0; h<6; h++){
		for(int t=0; t<6; t++) mt[h][t]=-1;
	}
	//Começa a tratar l:
	for(int i=0; i<j; i++){
		string lin; //lin: ajuda na manipulação recebendo cada linha de l (uma por vez), não consegui tratar usando só l
		lin=l[i];
		int k=0, aux2=lin.size();
		while(k<aux2+1){
			//se for c || C || i || I (cena || ilha) e pula 3 para pegar os valores da cena
			if(lin[k]==67 || lin[k]==99 || lin[k]==73 || lin[k]==105) k=k+3;
			//se for l || L (lagos || leoes) e pula 4 para pegar os valores dos leoes e lagos
			if(lin[k]==76 || lin[k]==108) k=k+4;
			//se for z || Z e pula 5 para pegar os valores das zebras
			if(lin[k]==90 || lin[k]==122) k=k+5;
			//se for p || P e pula 6 para pegar os valores das plantas
			if(lin[k]==80 || lin[k]==112) k=k+6;
			//Pega os números e salva, cada um em uma linha da matriz
			if(lin[k]>47 && lin[k]<58){
				if(lin[k-1]>47 && lin[k-1]<58){ //Caso seja dois números seguidos
					mt[n][m-1]=(mt[n][m-1]*10)+(lin[k]-48), m++;
				}
				else mt[n][m]=(lin[k]-48), m++;
			}
			k++;
		}
		n++;
		m=0;
	}
	//fim da manipulação em l
	
	//atribuição de valores nas variáveis e criação de matrizes:
	px=mt[0][0];
	py=mt[0][1];
	pz=mt[0][3];
	porcI=mt[1][0];
	porcL=mt[2][0];
		
	//criar matriz mundo
	//0: Mar
	//1: Ilha
	//2: Lagos
	//3: Leão
	//4: Zebra
	//5: Planta
	for(int i=0; i<px; i++){
		vector<int> mundo1;
		for(int j=0; j<pz;j++){
			mundo1.push_back(0);
		}
		mundo.push_back(mundo1);
	}
	//Fim da matriz mundo
	
	//Ilha
	margemx=(int) (px*porcI/100);
	margemz=(int) (pz*porcI/100);
	xi=mundo.size()/2-margemx/2; //linhas
	xf=mundo.size()/2+margemx/2;
	zi=mundo[0].size()/2-margemz/2; //colunas
	zf=mundo[0].size()/2+margemz/2;
	for(int i=xi; i<xf; i++){
		for(int j=zi; j<zf;j++){
			mundo[i][j]=1;
		}
	}
	//Fim da Ilha
	
	//Lagos
	ilhaT= xf*zf;
	qntL=(ilhaT*(porcL/100));
	if(qntL%10!=0) qntL=qntL+1;
	int posLx=0, posLz=0, e=0;
	srand(time(NULL)); //Para que toda vez que o programa execute, seja diferente
	while(e<=qntL){
		posLx=rand()%(xf-1); //gerar valor aleatório pra x
		posLz=rand()%(zf-1); //gerar valor aleatório pra z
		if(mundo[posLx][posLz]==1){ //confere se o valor que foi sorteado é ilha (1) ou não (0)
			mundo[posLx][posLz]=2;
			xl.push_back(posLx); //adiciona as posições de x
			zl.push_back(posLz); //adiciona as posições de z
			e++;
		}
	}
	//Fim dos Lagos
	
	//adicionando os n primeiros leões
	int leo=0;
	nleoes=mt[3][0];
	while(leo<mt[3][0]){
		int pLx=0, pLz=0;
		Leao *a=new Leao();
		a->set_perda(mt[3][1]);
		a->set_peso(30);
		pLx=rand()%(xf-1);
		pLz=rand()%(zf-1);
		if(mundo[pLx][pLz]==1){
			mundo[pLx][pLz]=3;
			a->set_posX(pLx);
			a->set_posY(1);
			a->set_posZ(pLz);
			a->set_dir(0);
			leos.push_back(a);
			leo++;
		}
	}
	//fim de adicionar leões
	
	//adicionando os n primeiras zebras
	int ze=0;
	nzebras=mt[4][0];
	while(ze<mt[4][0]){
		int pZx=0, pZz=0;
		Zebra *z=new Zebra();
		z->set_perdaz(mt[3][1]);
		z->set_pesoz(10);
		pZx=rand()%(xf-1);
		pZz=rand()%(zf-1);
		if(mundo[pZx][pZz]==1){
			mundo[pZx][pZz]=4;
			z->set_posXz(pZx);
			z->set_posYz(1);
			z->set_posZz(pZz);
			z->set_dirz(0);
			zes.push_back(z);
			ze++;
		}
	}
	//fim de adicionar zebras
	
	//adicionando os n primeiras plantas
	int plant=0;
	nplantas=mt[5][0];
	while(plant<mt[5][0]){
		int pPx=0, pPz=0;
		Tipo_planta *p=new Tipo_planta();
		p->set_cresce(mt[5][1]);
		p->set_tamanho(10);
		pPx=rand()%(xf-1);
		pPz=rand()%(zf-1);
		if(mundo[pPx][pPz]==1){
			mundo[pPx][pPz]=5;
			p->set_posXp(pPx);
			p->set_posYp(1);
			p->set_posZp(pPz);
			planta.push_back(p);
			plant++;
		}
	}
	//fim de adicionar plantas
	
	//Imprimir matriz mundo
	for(int i=0; i<px; i++){
		for(int j=0; j<pz;j++){
			cout<<mundo[i][j]<<" ";
		}
		cout<<endl;
	}
	//Fim da impressão
	//fim da atribuição de valores na matriz mundo
	
	//execução da geometria:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	//Criar os modelos com as texturas
	obj = Modelo::carregarObj((char*)"OBJs/lago.obj");
	obj1 = Modelo::carregarObj((char*)"OBJs/Lion.obj",(char*)"leao.bmp");
	obj2 = Modelo::carregarObj((char*)"OBJs/male.obj",(char*)"male.bmp");
	obj3 = Modelo::carregarObj((char*)"OBJs/Basic Tree 2.obj",(char*)"folhas.bmp");
	//Fim da criação dos modelos
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc(atualizarMouse);
	glutMotionFunc(olharAoRedor);
	glutIdleFunc(display);
	glutSpecialFunc(specialKeys);
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(releaseKey);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	//Deleta os modelos criados
	delete obj;
	delete obj1;
	delete obj2;
	delete obj3;
	//Fim da execução da geometria
	return 0;
	//FIM!
}
