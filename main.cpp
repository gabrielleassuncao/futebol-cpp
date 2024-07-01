//nome: Gabrielle do Carmo Assunção

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

float alpha = 0;
float beta = 0;
float delta = 1;

typedef struct { // define as coordenadas x, y e z
    float x;
    float y;
    float z;
} ponto;

ponto bola = {0.0f, 0.0f, 0.0f}; //define as coordenadas inciais do ponto da bola

float deslocaBolax = 0.01f; // fatores de translação
float deslocaBolay = 0.001f;
float deslocaBolaz = 0.0f;

float limDirx = 7.0f; // limita a bolinha para voltar à posição inicial quando sai da tela
float limEsqx = -7.0f;

bool espacoPressionado = false; //variavel utilizada para verificar se o espaco foi pressionado, ou nao
int chutesIncorretos = 0; //define os chutes incorretos
int gols = 0; //define os gols
char errado[3] = "0"; //armazena os chutes errados para a montagem do placar
char certo[3] = "0"; //armazena os chutes certos para a montagem do placar
char info[14]; //variavel que armazena se voce acertou ou errou

void init() {
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST); // Teste de profundidade
    glMatrixMode(GL_PROJECTION); // Define a matriz como a atual de projeção
    glLoadIdentity(); // Redefine a matriz para seu estado inicial
    glOrtho(-10, 10, -10, 10, -10, 10); // Define a projeção ortográfica
}

void desenhaPlacar(float x, float y, char text[]) { //funcao que desenha o placar na tela
    char *c;
    glRasterPos3f(x, y, 0);

    for (c = text; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
}
    void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o buffer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(30, 1, 0, 0); // Aplica a rotação

    glLineWidth(3.0); // Define a espessura das linhas do campo
    glColor3f(1, 1, 1); // Define a cor das linhas do campo

    // Placar
    desenhaPlacar(-9.5, 10, "Gols: ");
    desenhaPlacar(-6.5, 10, certo);
    desenhaPlacar(-9.5, 9, "Erros: ");
    desenhaPlacar(-6.5, 9, errado);
    desenhaPlacar(-1.5, -10, info);

    // Gol
    glBegin(GL_LINE_STRIP);
    glVertex3d(-3, 0, -5);
    glVertex3d(-3, 2.8, -5);
    glVertex3d(3, 2.8, -5);
    glVertex3d(3, 0, -5);
    glEnd();

    // Linha do Gol
    glBegin(GL_LINE_STRIP);
    glVertex3d(-10, 0, -5);
    glVertex3d(10, 0, -5);
    glEnd();

    // Marcação da Bola
    glBegin(GL_POINTS);
    glVertex3d(0, 0, 0);
    glEnd();

    glPushMatrix(); // Salva a matriz de modelo-visão atual
    glTranslatef(bola.x, bola.y, bola.z); // Translação
    // Desenho da esfera
    glColor3f(1.0f, 1.0f, 1.0f); // Cor da esfera
    glutWireSphere(0.5, 20, 10); // Define o raio e as dimensões da esfera
    glPopMatrix(); // Restaura a matriz

    // Desenha o campo
    glPushMatrix();
    glTranslatef(0, -1, 0); // Posiciona o campo corretamente
    glScalef(30, 0.1f, 6.9); // Define as dimensões do campo
    glColor3f(0.133, 0.545, 0.133);
    glutSolidCube(2.0f); // Tamanho do cubo
    glPopMatrix();

    glutSwapBuffers();
}

void moveBola(int value) { //funcao utilizada para fazer a movimentacao da bola e verificar se foi gol ou nao
    bola.x += deslocaBolax;
    bola.y += deslocaBolay;

    if (bola.x > limDirx || bola.x < limEsqx) { //funcao que reseta a posicao da bola e faz ela se mover da direita para a esquerda
        bola.x = 0.0f;
        bola.y = 0.0f;
        deslocaBolax = -deslocaBolax;
    }

    if (espacoPressionado) { //verifica se o espaco foi pressionado, se sim verifica se foi gol ou nao de acordo com as coordenadas definidas do gol,
                            //armazenando-as na variavel info que posteriormente vai nos mostrar se foi gol ou nao na tela
        bola.z += deslocaBolaz;
        if (bola.z <= -5.0) {
            espacoPressionado = false;
            if (bola.x >= -1.5 && bola.x <= 1.5 && bola.y <= 2.8 && bola.y >= 0) {
                gols++;
                snprintf(certo, sizeof(certo), "%d", gols);
                strcpy(info, "Gol!");
            } else {
                chutesIncorretos++;
                snprintf(errado, sizeof(errado), "%d", chutesIncorretos);
                strcpy(info, "Errou!");
            }
            bola.z = 0.0f;
        } else {
            deslocaBolaz = -0.1f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(5, moveBola, 0);
}

void keyboard(unsigned char key, int x, int y) { //funcao que define a usabilidade das teclas

    if (key == ' ') //tentativa de fazer o gol
        espacoPressionado = true;
    if (key == 27) // Sai do programa
        exit(0);
    if (key == 'c') // Executa o trabalho passado
        system("\"C:\\Users\\gabiii\\Documents\\futebol2\\bin\\Debug\\meunomeAlteracoes.exe\"");
    if (key == 'r') { //reseta o placar
        gols = 0;
        snprintf(certo, sizeof(certo), "%d", gols);
        chutesIncorretos = 0;
        snprintf(errado, sizeof(errado), "%d", chutesIncorretos);
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Futebol");
    init();
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutTimerFunc(5, moveBola, 0);
    glutMainLoop();

    return 0;
}
