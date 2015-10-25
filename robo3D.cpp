/*
 * Robo_3D.cpp
 *
 * Copyright 2014 Rafael Andrade <rafaelsfg@hotmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 * Programa que simula um robô em 3D
 */

//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//  Pressione:                                                                          //
//            'o' para girar o ombro no sentido horário                                 //
//            'O' para girar o ombro no sentido anti-horário                            //
//            'b' para girar o braço no sentido horário                                 //
//            'B' para girar o braço no sentido anti-horário                            //
//            'd' para fechar os dedos                                                  //
//            'D' para abrir os dedos                                                   //
//            'a' para girar a munheca no sentido horário                               //
//            'A' para girar a munheca no sentido anti-horário                          //
//            'm' para girar a mão no sentido anti-horário                              //
//            'M' para girar a mão no sentido horário                                   //
//            'p' para girar o Anti-braço em torno do eixo Z                            //
//            'P' para girar o Anti-braço em torno do eixo Z                            //
//            "Page Up" Aumenta o Zoom                                                  //
//            "Page Down" Diminui o Zomm                                                //
//            Clicando e movimentando o Mouse, muda a vizualização do espaço            //
//            Setas "Direita" ou "Esquerda" do Teclado giram o robô no plano            //
//            Setas "Para Cima" ou "Para Baixo" movimentam o robô no plano              //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define pi 3.1416   //  Define a constante Pi com o valor 3.1416

int obsteta=180, obsfi=70,  //  Angulos de visualização do observador
    teta=-70,              //  Angulo de rotação do Ombro
    fi=80,                 //  Angulo de rotação do Braço
    alfa=0,                //  Angulo de rotação da mão
    beta=0,                //  Angulo de rotação da munheca
    lambda=-70,            //  Angulo de rotação do anti-braço em torno do eixo Z
    gama=0,                //  Angulo de rotação do robô
    d=6,                   //  Distância entre os dedos
    Xo=0, Yo=0,            //  Posição inicial do mouse
    c=0,                   //  Condição do agarro da mão
    camera=0;              //  Câmara do robô (0 = desligada, 1 = ligada)

float xo = -30, yo = 50, zo = 40,          //  Posição do objeto no espaço
      t = 0, f = 0, l = 0, a = 0, b = 0, g = 0;  //  Angulos de inclinação do objeto

float posx0 = 0, posy0,  //  Posição inicial do robô no plano
      posx = 0, posy = 0;  //  Posição atual do Robô no plano

//   Função para desenhar paralelepípedos
void paralelo(float l, float c, float a, float posx, float posy, float posz)
{
    glBegin(GL_POLYGON);    // Face posterior
        glNormal3i(0, 0, 1);
        glVertex3f(posx + l, posy + c, posz + a);   glVertex3f(posx - l, posy + c, posz + a);
        glVertex3f(posx - l, posy, posz + a);       glVertex3f(posx + l, posy, posz + a);
    glEnd();

    glBegin(GL_POLYGON);    // Face frontal
        glNormal3i(0, 0, -1);
        glVertex3f(posx + l, posy + c, posz - a);   glVertex3f(posx + l, posy, posz - a);
        glVertex3f(posx - l, posy, posz - a);       glVertex3f(posx - l, posy + c, posz - a);
    glEnd();

    glBegin(GL_POLYGON);    // Face lateral esquerda
        glNormal3i(-1, 0, 0);
        glVertex3f(posx - l, posy + c, posz + a);   glVertex3f(posx - l, posy + c, posz - a);
        glVertex3f(posx - l, posy, posz - a);       glVertex3f(posx - l, posy, posz + a);
    glEnd();

    glBegin(GL_POLYGON);    // Face lateral direita
        glNormal3i(1, 0, 0);
        glVertex3f(posx + l, posy + c, posz + a);   glVertex3f(posx + l, posy, posz + a);
        glVertex3f(posx + l, posy, posz - a);       glVertex3f(posx + l, posy + c, posz - a);
    glEnd();

    glBegin(GL_POLYGON);    // Face superior
        glNormal3i(0, 1, 0);
        glVertex3f(posx - l, posy + c, posz - a);   glVertex3f(posx - l, posy + c, posz + a);
        glVertex3f(posx + l, posy + c, posz + a);   glVertex3f(posx + l, posy + c, posz - a);
    glEnd();

    glBegin(GL_POLYGON);    // Face inferior
        glNormal3i(0, -1, 0);
        glVertex3f(posx + l, posy, posz - a);       glVertex3f(posx - l, posy, posz - a);
        glVertex3f(posx - l, posy, posz + a);       glVertex3f(posx + l, posy, posz + a);
    glEnd();
}

//   Função para desenhar o túneo
void Tuneo(int t)
{
    glColor3f(0.8, 0.0, 0.0);

    glBegin(GL_QUADS); //  Parede direita
        glVertex3i(200, 200, 200);    glVertex3i(200, t, 200);
        glVertex3i(200, t, -200);     glVertex3i(200, 200, -200);
    glEnd();

    glBegin(GL_QUADS); //  Parede esquerda
        glVertex3i(-200, 200, 200);   glVertex3i(-200, t, 200);
        glVertex3i(-200, t, -200);    glVertex3i(-200, 200, -200);
    glEnd();

    glColor3f(0.0, 0.9, 0.0);

    glBegin(GL_QUADS); //  Piso
        glVertex3i(200, 200, -200);   glVertex3i(200, t, -200);
        glVertex3i(-200, t, -200);    glVertex3i(-200, 200, -200);
    glEnd();

    glBegin(GL_QUADS); //  Teto
        glVertex3i(200, 200, 200);    glVertex3i(200, t, 200);
        glVertex3i(-200, t, 200);     glVertex3i(-200, 200, 200);
    glEnd();
}

// Chamada para fazer o desenho do Anti-braço
void Antibraco(void)
{
    glRotatef(lambda, 0, 0, 1);  //  Gira o Anti-braço em torno do eixo Z
    glRotatef(teta, 1, 0, 0);    //  Gira o Anti-braço
    glColor3i(0, 0, 1);
    glutSolidSphere(10, 20, 20); //  Desenha o Ombro
    glColor3i(1, 0, 0);
    paralelo(5, 30, 5, 0, 9, 0);      //  Desenha o Anti-braço

    glTranslatef(0.0, 45.0, 0.0);
    glRotatef(fi, 1, 0, 0);      //  Gira o Braço
    glColor3f(0, 0, 0.7);
    glutSolidSphere(6, 20, 20);  //  Desenha o Cotovelo
    glColor3f(1, 0, 0);
    paralelo(5, 35, 5, 0, 5, 0);      //  Desenha o Braço

    glTranslatef(0, 43, 0);
    glColor3f(0, 0, 0.5);
    glutSolidSphere(5, 20, 20); //  Desenha a munheca
    glRotatef(beta, 1, 0, 0);   //  Gira a munheca
    glRotatef(alfa, 0, 1, 0);   //  Gira a mão
    glColor3f(0, 0.8, 0);
    paralelo(10, 10, 5, 0, 4, 0);    //  Desenha a Mão
    paralelo(2, 10, 4, d, 14, 0);    //  Desenha o 1º Dedo
    paralelo(2, 10, 4, -d, 14, 0);   //  Desenha o 2º Dedo

}

//  Função que faz a mão agarrar objetos
void Agarrar(void)
{
    float x = -30, y = 50, z = 40;

    glColor3f(1, 0, 0.5);

    //   Calcula as coordenadas da mão
    x = posx - 19 * sin(pi * gama / 180) - (45 * cos(pi * teta / 180) + 43 * cos(pi * (teta + fi) / 180) +
        18 * cos(pi * (teta + fi + beta) / 180)) * cos(pi * (90 - gama - lambda) / 180);

    y = posy + 19 * cos(pi * gama / 180) + (45 * cos(pi * teta / 180) + 43 * cos(pi * (teta + fi) / 180) +
        18 * cos(pi * (teta + fi + beta) / 180)) * sin(pi * (90 - lambda - gama) / 180);

    z = 45 * sin(pi * teta / 180) + 43 * sin(pi * (teta + fi) / 180) + 18 * sin(pi * (teta + fi + beta) / 180);

    if(xo > (x - 6) && xo < (x + 6) && zo < (z + 6) && zo > (z - 6) && yo >(y - 6) && yo <(y + 6) && d == 5)
    {
        xo = x;
        yo = y;
        zo = z;
        l = lambda;
        g = gama;
        f = fi;
        b = beta;
        t = teta;
        a = alfa;
        c = 1;
    }
    else
        c = 0;
}

//  Função para desenhar o cubo exterior
void Linhas(void)
{
    int dim = 200; //  Dimensão do cubo Exterior

    glColor3f(0, 0, 0);
    glBegin(GL_LINES);   //  Desenha os Eixos X, Y e Z
        glColor3f(1, 0, 0);
        glVertex3d(-40, 0, 0);      glVertex3d(40, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3d(0, -40, 0);      glVertex3d(0, 40, 0);
        glColor3f(0, 0, 1);
        glVertex3d(0, 0, -40);      glVertex3d(0, 0, 40);
    glEnd();

    glBegin(GL_LINES);  //  Desenha as linhas do cubo Exterior
        glVertex3d(dim, dim, dim);      glVertex3d(dim, -dim, dim);
        glVertex3d(dim, -dim, dim);     glVertex3d(dim, -dim, -dim);
        glVertex3d(dim, -dim, -dim);    glVertex3d(dim, dim, -dim);
        glVertex3d(-dim, -dim, -dim);   glVertex3d(-dim, dim, -dim);
        glVertex3d(-dim, -dim, -dim);   glVertex3d(-dim, -dim, dim);
        glVertex3d(-dim, dim, dim);     glVertex3d(-dim, -dim, dim);
        glVertex3d(dim, -dim, dim);     glVertex3d(-dim, -dim, dim);
        glVertex3d(-dim, dim, -dim);    glVertex3d(-dim, dim, dim);
        glVertex3d(dim, -dim, -dim);    glVertex3d(-dim, -dim, -dim);
        glVertex3d(-dim, dim, -dim);    glVertex3d(dim, dim, -dim);
        glVertex3d(dim, dim, -dim);     glVertex3d(dim, dim, dim);
        glVertex3d(dim, dim, dim);      glVertex3d(-dim, dim, dim);
   glEnd();

}

//  Função para desenhar o cenário
void Cenario(void)
{
    glRotatef(-90, 0, 0, 1);
    Tuneo(800);

    glTranslatef(0, 1000, 0);
    glRotatef(90, 0, 0, 1);
    Tuneo(1600);

    glRotatef(180, 0, 0, 1);
    Tuneo(800);

    glRotatef(90, 0, 0, 1);
    Tuneo(1600);

    glTranslatef(0, 1600, 0);
    glColor3f(0, 0, 1);
    glBegin(GL_QUADS);
       glVertex3d(500, 0, -200);   glVertex3d(500, 800, -200);
       glVertex3d(-500, 800, -200); glVertex3d(-500, 0, -200);
    glEnd();
}

GLfloat angle=60;
GLfloat fAspect;

//  Função para desenhar o Robô
void Robo(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutFullScreen ( );

    glPushMatrix();
    glTranslatef(posx, posy, 0);
    glRotatef(gama, 0, 0, 1);
    glColor3f(0.2, 0.7, 0.6);
    paralelo(6,10,5,0, -5, 15);       //  Desenha a Camara
    glColor3f(0.8, 0, 0.6);
    paralelo(10,20,110,0,-10,-100);   //  Desenha o Pedestal
    glColor3f(0.2, 0.5, 0.8);
    paralelo(30, 60, 60, 0, -30, -200);  //  Desenha a Base
    glTranslatef(0, 19, 0);
    Antibraco();                      //  Desenha o Anti-braço
    Agarrar();                        //  Chama a função para segurar objetos
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(xo, yo, zo);   //  Desloca o objeto para as coordenadas da mão
    glRotatef(l+g, 0, 0, 1);    //  Rotaciona o objeto em torno do eixo Z
    glRotatef(f+b+t, 1, 0, 0);  //  Rotaciona o objeto em torno do eixo X
    glRotatef(a, 0, 1, 0);      //  Rotaciona o objeto em torno do eixo Y
    paralelo(3,6,3,0,0,0);      //  Desenha o Objeto
    glPopMatrix();

    Linhas();     //  Desenha as linhas do cubo exterior

    glPushMatrix();
    Cenario();       //  Desenha o senário
    glPopMatrix();

    glutSwapBuffers();
}

//   Função para iluminar o ambiente
void Iluminacao(void)
{
    GLfloat luzAmbiente[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat luzDifusa[4] = {0.7, 0.7, 0.7, 1.0};       // "cor"
    GLfloat luzEspecular[4] = {0.5, 0.5, 0.5, 0.5};     // "brilho"
    GLfloat posicaoLuz[4] = {0.0, 50.0, 50.0, 1.0};

    // Capacidade de brilho do material
    GLfloat especularidade[4] = {1.0, 0.5, 1.0, 1.0};
    GLint especMaterial = 60;

    // Especifica a cor de fundo da janela
    glClearColor(1, 1, 1, 1);

    // Habilita o modelo de tonalização de Gouraud
    glShadeModel(GL_SMOOTH);

    // Define a refletância do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);

    // Define a concentração do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

    // Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

    // Habilita a definição da cor do material
    // a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);

    //Habilita o uso de iluminação
    glEnable(GL_LIGHTING);

    // Habilita a luz de número 0
    glEnable(GL_LIGHT0);

    // Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);
}

// Função usada para especificar o volume de visualização
void Visualizacao(void)
{
    float obsx, obsy, obsz;  //  Coordenadas do observador
    float obsx0 = posx, obsy0 = posy, obsz0 = 0;
    float a, d = 200;             //  Distância do observador à origem
    int r1 = 15, r2 = 190;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle,fAspect, 0.1, 8000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //  Calcula a posição do Observador
    if(camera % 2 == 0)
    {
        if(posx == 0)
            a = 0;
        else
            a = atan(posy / posx);

        obsx = posx + d * sin(pi * obsfi / 180) * cos(pi * (a - obsteta) / 180);
        obsy = posy + d * sin(pi * obsfi / 180) * sin(pi * (a - obsteta) / 180);
        obsz = d * cos(pi * obsfi / 180);
    }
    else
    {
        obsx0 = posx0 + (r1 + r2) * sin(pi * obsfi / 180) * cos(pi * (obsteta + gama) / 180);
        obsy0 = posy0 + (r1 + r2) * sin(pi * obsfi / 180) * sin(pi * (obsteta + gama) / 180);
        obsz0 = 10 + (r1 + r2) * cos(pi * obsfi / 180);
        obsx = posx + r1 * sin(pi * obsfi / 180) * cos(pi * (obsteta + gama) / 180);
        obsy = posy + r1 * sin(pi * obsfi / 180) * sin(pi * (obsteta + gama) / 180);
        obsz = 20 + r1 * cos(pi * obsfi / 180);
    }

    gluLookAt(obsx, obsy, obsz, obsx0, obsy0, obsz0, 0, 0, 1);

}

//  Redimensiona a Janela
void Ajustedimensao(GLsizei w, GLsizei h)
{
    if (h == 0)
        h = 1;

    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    Visualizacao();
}

// Gerencia eventos do mouse
void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
              if (state == GLUT_DOWN); //  Eventos quando o botão esquerdo é pressionado

    if (button == GLUT_RIGHT_BUTTON)
              if (state == GLUT_DOWN); //  Eventos quando o botão direito é pressionado

    Visualizacao();
    glutPostRedisplay();
}

//  Função que Desenha Texto no Espaço
void DesenhaTexto(char *string)
{
    glColor3f(0, 0.5, 1);
    glRasterPos3f(20, 20, 20);  //  Posição x,y,z do texto
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *string++);

}

//  Função para mudar a visualização do espaço pelo movimento do mouse
void MoveMouse(int x, int y)
{
    int p = 0,q = 0;  //  Velocidade de rotação da visualização
    int l;    //  Controla o limite da câmera

    if(camera % 2 == 0)
        l = 179;
    else
        l = 125;

    if(x > Xo)
        p = 1;

    if(x < Xo)
        p = -1;

    if(obsfi != 1)
        if(y > Yo)
            q = -1;

    if(obsfi != l)
        if(y < Yo)
            q = 1;

    obsteta = obsteta + p;
    obsfi = obsfi + q;

    Xo = x;
    Yo = y;

    Visualizacao();
    glutPostRedisplay();
}

//  Função para mudar a posição do robô no plano através do teclado
void tecladoespecial(int key, int x, int y)
{
    int p = 6;//  Tamanho do passo do robô

    if(key == GLUT_KEY_LEFT)
        gama = gama + 3;  //  Gira o robô para Esquerda

    if(key == GLUT_KEY_RIGHT)
        gama = gama - 3; //  Gira o robô para direita

    if(key == GLUT_KEY_UP)
    {  //  Movimenta o robô para frente
        posx = posx0 + p * cos(pi * gama / 180);
        posy = posy0 + p * sin(pi * gama / 180);
        posx0 = posx;
        posy0 = posy;
    }

    if(key == GLUT_KEY_DOWN)
    {  //  Movimenta o robô para trás
        posx = posx0 - p * cos(pi * gama / 180);
        posy = posy0 - p * sin(pi * gama / 180);
        posx0 = posx;
        posy0 = posy;
    }

    if(key == GLUT_KEY_PAGE_UP)
        if (angle >= 10)
            angle -= 5;  //  Aumenta o Zoom

    if(key == GLUT_KEY_PAGE_DOWN)
        if (angle <= 130)
            angle += 5;  //  Diminui o Zoom

    if(key == GLUT_KEY_F1)
    {
        camera++;   //  Liga e desliga a câmera do robô
        if(camera % 2 == 0)
        {
            obsteta = 0;
            obsfi = 70;
        }
        else
        {
            obsteta = 0;
            obsfi = 90;
        }
    }

    Visualizacao();
    glutPostRedisplay();
}

//  Função para movimentar o Braço
void teclado(unsigned char key, int a, int b)
{
    switch (key)
    {
        case 27:
            exit(0);    //  Sai do programa quando pressionado "ESC"
            break;

        case 'o':
            if(teta != 90)
                teta++;    //  Limita o angulo de rotação máxima do Ombro
            break;

        case 'O':
            if(teta != -180)
                teta--;  //  Limita o angulo de rotação mínima do Ombro
            break;

        case 'b':
            if(fi != 110)
                fi++;       //  Limita o angulo de rotação máxima do braço
            break;
        case 'B':
            if(fi != 0)
                fi--;         //  Limita o angulo de rotação mínima do braço
            break;

        case 'd':
            if(d != 2 && c!=1)
                d--;          //  Limita a distância mínima dos dedos
            break;
        case 'D':
            if(d != 6)
                d++;          //  Limita a distância máxima dos dedos
            break;

        case 'm':
            alfa++;          //  Faz girar a mão no sentido anti-horário
            break;

        case 'M':
            alfa--;          //  Faz girar a mão no sentido horário
            break;

        case 'a':
            if(beta != 90)
                beta++;  //  Limita o angulo máximo de rotação da munheca
            break;

        case 'A':
            if(beta != -90)
                beta--;  //  Limita o angulo mínimo de rotação da munheca
            break;

        case 'p':
            if(lambda != 90)
                lambda++;
            break;

        case 'P':
            if(lambda != -90)
                lambda--;
            break;
    }

    glutPostRedisplay();
}

// Programa Principal
int main(int argc, char*argv[])
{
    glutInit (&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(150, 50);
    glutCreateWindow("Robô 3D");
    glutDisplayFunc(Robo);
    glutReshapeFunc(Ajustedimensao);
    glutMouseFunc(Mouse);
    glutMotionFunc(MoveMouse);
    glutSpecialFunc(tecladoespecial);
    glutKeyboardFunc(teclado);
    Iluminacao();
    glutMainLoop();
}
