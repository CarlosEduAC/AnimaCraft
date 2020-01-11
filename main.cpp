#include <GL/glew.h>
#include <GL/glu.h>
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>
#include "stdio.h"
#include "PIG.h"
#include <math.h>

#define LADO 0.1

//Um bloco corresponde a uma parte de um membro composto (Braço, Perna e Tronco)
typedef struct{
    int x,y,z;
}Bloco;
//A perna é composto por 2 membros. Superior e Inferior.
typedef struct{
    Bloco me[2];
}Perna;
//O Braço é composto por 2 membros. Superior e Inferior.
typedef struct{
    Bloco me[2];
}Braco;
//O tronco é composto por 2 membros. Superior e Inferior.
typedef struct{
    Bloco me[2];
}Tronco;
//Um Corpo é composta por duas Perna, dois Braço,Tronco e Cabeça. X, Y e Z dão a posição real dos blocos em relação ao pivô
typedef struct{
    int x,y,z;
    Bloco cabeca;
    Braco braco[2];
    Perna perna[2];
    Tronco tronco;
}Corpo;
//Variáveis referentes a rotação e translação do Corpo e da Câmera
float angHor,angVer,angperna,angperna2,angperna3,angperna4,angbraco1,angbraco2,angbraco3,angcabeca,angcabeca2,angtronco,angtronco2,eyex,eyey,eyez,flagcontrole,cena;
//Tipo usado para organização dos modos
enum numTelas{menu,livre,programado,matrix,sair};
//Inicializa as variáveis de rotação
void InicializaAngulos (){
    angHor=0;angVer=0;angperna=0;angperna2=0;angperna3=0;angperna4=0;angbraco1=0;angbraco2=0;angbraco3=0;
    angcabeca=0;angcabeca2=0;angtronco=0;angtronco2=0;eyex = 0;eyey = 0;eyez = 2;cena=0;
}
//Responsável por inicializar o Corpo
Corpo CriaCorpo(){
    Corpo corpo;
    corpo.x = -1;corpo.y = 0;corpo.z = 0;
    corpo.cabeca.x = +2;corpo.cabeca.y = +3;corpo.cabeca.z = 0;
    corpo.tronco.me[0].x = 0;corpo.tronco.me[0].y = 0;corpo.tronco.me[0].z = 0;
    corpo.tronco.me[1].x = 0;corpo.tronco.me[1].y = -1;corpo.tronco.me[1].z = 0;
    corpo.perna[0].me[0].x = -1;corpo.perna[0].me[0].y = -1;corpo.perna[0].me[0].z = 0;
    corpo.perna[0].me[1].x = -1;corpo.perna[0].me[1].y = -1;corpo.perna[0].me[1].z = 0;
    corpo.perna[1].me[0].x = +1;corpo.perna[1].me[0].y = -1;corpo.perna[1].me[0].z = 0;
    corpo.perna[1].me[1].x = +1;corpo.perna[1].me[1].y = -1;corpo.perna[1].me[1].z = 0;
    corpo.braco[0].me[0].x = -2;corpo.braco[0].me[0].y = -1;corpo.braco[0].me[0].z = 0;
    corpo.braco[0].me[1].x = -2;corpo.braco[0].me[1].y = -1;corpo.braco[0].me[1].z = 0;
    corpo.braco[1].me[0].x = +2;corpo.braco[1].me[0].y = -1;corpo.braco[1].me[0].z = 0;
    corpo.braco[1].me[1].x = +2;corpo.braco[1].me[1].y = -1;corpo.braco[1].me[1].z = 0;
    return corpo;
}
//Desenha e rotaciona os Braços
void DesenhaBraco(Corpo corpo){
    int flag=0;
    glColor3f(1,1,1);
    for (int i=0;i<2;i++){
            glPushMatrix();
            glRotatef(angbraco1,1,0,0);
            glTranslatef((corpo.tronco.me[0].x+corpo.braco[i].me[0].x)*LADO,(corpo.tronco.me[0].y+corpo.braco[i].me[0].y+angbraco3)*2*LADO,-(corpo.tronco.me[0].z+corpo.braco[i].me[0].z)*LADO);
            glBegin(GL_QUADS);
            //Parte de cima do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();
            //Parte de baixo do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,0);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(0,LADO*2,0);
            glEnd();

            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,0);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();

            glRotatef(angbraco2,1,0,0);
            glTranslatef((corpo.braco[i].me[0].x-corpo.braco[i].me[1].x)*LADO,(corpo.braco[i].me[0].y+corpo.braco[i].me[1].y)*LADO,-(corpo.braco[i].me[0].z+corpo.braco[i].me[1].z)*LADO);
            glBegin(GL_QUADS);
            //Parte de cima do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();
            //Parte de baixo do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,0);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(0,LADO*2,0);
            glEnd();

            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,0);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();
            glPopMatrix();
    }
}
//Desenha e rotaciona as Pernas
void DesenhaPerna(Corpo corpo){
    glColor3f(1,1,1);
    for (int i=0;i<2;i++){
            glPushMatrix();
            glRotatef(angperna,1,0,0);
            glTranslatef((corpo.tronco.me[0].x+corpo.perna[i].me[0].x)*LADO,(corpo.tronco.me[0].y+corpo.perna[i].me[0].y)*2*LADO,-(corpo.tronco.me[0].z+corpo.perna[i].me[0].z)*LADO);
            glBegin(GL_QUADS);
            //Parte de cima do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();
            //Parte de baixo do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,0);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(0,LADO*2,0);
            glEnd();

            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,0);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();

            glRotatef(angperna2,1,0,0);
            glTranslatef((corpo.perna[i].me[0].x-corpo.perna[i].me[1].x)*LADO,(corpo.perna[i].me[0].y+corpo.perna[i].me[1].y)*LADO,-(corpo.perna[i].me[0].z+corpo.perna[i].me[1].z)*LADO);
            glBegin(GL_QUADS);
            //Parte de cima do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();
            //Parte de baixo do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,0);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(0,LADO*2,0);
            glEnd();

            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,0);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();

            glPopMatrix();
    }
}
//Desenha e rotaciona a Cabeça
void DesenhaCabeca(Corpo corpo){
    glColor3f(1,1,1);
    glRotatef(angcabeca,1,0,0);
    glRotatef(angcabeca2,0,1,0);
    glTranslatef((corpo.x+corpo.cabeca.x)*LADO,(corpo.y+corpo.cabeca.y)*LADO+LADO,-(corpo.z+corpo.cabeca.z)*LADO);
    glBegin(GL_QUADS);
    //Parte de cima do cubo
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,-LADO);
    glVertex3f(LADO,0,-LADO);
    glVertex3f(LADO,(2*LADO)-0.1,-LADO);
    glVertex3f(0,(2*LADO)-0.1,-LADO);
    glEnd();
    //Parte de baixo do cubo
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,0);
    glVertex3f(LADO,0,0);
    glVertex3f(LADO,(2*LADO)-0.1,0);
    glVertex3f(0,(2*LADO)-0.1,0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,-LADO);
    glVertex3f(LADO,0,0);
    glVertex3f(LADO,0,-LADO);
    glVertex3f(LADO,(2*LADO)-0.1,0);
    glVertex3f(LADO,(2*LADO)-0.1,-LADO);
    glVertex3f(0,(2*LADO)-0.1,0);
    glVertex3f(0,(2*LADO)-0.1,-LADO);
    glEnd();
}
//Desenha e rotaciona o Tronco Superior
void DesenhaTronco(Corpo corpo){
    glColor3f(1,1,1);
    glPushMatrix();
    glRotatef(angtronco,1,0,0);
    glRotatef(angtronco2,0,1,0);
    glTranslatef((corpo.x+corpo.tronco.me[0].x)*LADO,(corpo.y+corpo.tronco.me[0].y)*LADO,-(corpo.z+corpo.tronco.me[0].z)*LADO);
    glBegin(GL_QUADS);
    //Parte de cima do cubo
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,-LADO);
    glVertex3f(3*LADO,0,-LADO);
    glVertex3f(3*LADO,4*LADO,-LADO);
    glVertex3f(0,4*LADO,-LADO);
    glEnd();
    //Parte de baixo do cubo
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,0);
    glVertex3f(3*LADO,0,0);
    glVertex3f(3*LADO,4*LADO,0);
    glVertex3f(0,4*LADO,0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,-LADO);
    glVertex3f(3*LADO,0,0);
    glVertex3f(3*LADO,0,-LADO);
    glVertex3f(3*LADO,4*LADO,0);
    glVertex3f(3*LADO,4*LADO,-LADO);
    glVertex3f(0,4*LADO,0);
    glVertex3f(0,4*LADO,-LADO);
    glEnd();

    DesenhaCabeca(corpo);
    DesenhaBraco(corpo);
    glPopMatrix();
}
//Desenha o Tronco Inferior
void DesenhaTronco2(Corpo corpo){
    glColor3f(1,1,1);
    glPushMatrix();
    glTranslatef((corpo.x+corpo.tronco.me[1].x)*LADO,(corpo.y+corpo.tronco.me[1].y)*LADO,-(corpo.z+corpo.tronco.me[1].z)*LADO);
    glBegin(GL_QUADS);
    //Parte de cima do cubo
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,-LADO);
    glVertex3f(3*LADO,0,-LADO);
    glVertex3f(3*LADO,LADO,-LADO);
    glVertex3f(0,LADO,-LADO);
    glEnd();
    //Parte de baixo do cubo
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,0);
    glVertex3f(3*LADO,0,0);
    glVertex3f(3*LADO,LADO,0);
    glVertex3f(0,LADO,0);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(0,0,-LADO);
    glVertex3f(3*LADO,0,0);
    glVertex3f(3*LADO,0,-LADO);
    glVertex3f(3*LADO,LADO,0);
    glVertex3f(3*LADO,LADO,-LADO);
    glVertex3f(0,LADO,0);
    glVertex3f(0,LADO,-LADO);
    glEnd();
    glPopMatrix();
}
//Chama as funções para desenhar o Corpo
void DesenhaCorpo(Corpo corpo){
    DesenhaTronco(corpo);
    DesenhaTronco2(corpo);
    DesenhaPerna(corpo);
}
//Desenha e rotaciona as Pernas, porém pernas com ângulo diferente
void DesenhaPernaProgramada(Corpo corpo){
    glColor3f(1,1,1);
    for (int i=0;i<2;i++){
        if(i==0){
            glPushMatrix();
            glRotatef(angperna,1,0,0);
            glTranslatef((corpo.tronco.me[0].x+corpo.perna[i].me[0].x)*LADO,(corpo.tronco.me[0].y+corpo.perna[i].me[0].y)*2*LADO,-(corpo.tronco.me[0].z+corpo.perna[i].me[0].z)*LADO);
            glBegin(GL_QUADS);
            //Parte de cima do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();
            //Parte de baixo do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,0);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(0,LADO*2,0);
            glEnd();

            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,0);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();

            glRotatef(angperna2,1,0,0);
            glTranslatef((corpo.perna[i].me[0].x-corpo.perna[i].me[1].x)*LADO,(corpo.perna[i].me[0].y+corpo.perna[i].me[1].y)*LADO,-(corpo.perna[i].me[0].z+corpo.perna[i].me[1].z)*LADO);
            glBegin(GL_QUADS);
            //Parte de cima do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();
            //Parte de baixo do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,0);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(0,LADO*2,0);
            glEnd();

            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,0);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();

            glPopMatrix();
        }
        else{
            glPushMatrix();
            glRotatef(angperna3,1,0,0);
            glTranslatef((corpo.tronco.me[0].x+corpo.perna[i].me[0].x)*LADO,(corpo.tronco.me[0].y+corpo.perna[i].me[0].y)*2*LADO,-(corpo.tronco.me[0].z+corpo.perna[i].me[0].z)*LADO);
            glBegin(GL_QUADS);
            //Parte de cima do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();
            //Parte de baixo do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,0);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(0,LADO*2,0);
            glEnd();

            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,0);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();

            glRotatef(angperna4,1,0,0);
            glTranslatef((corpo.perna[i].me[0].x-corpo.perna[i].me[1].x)*LADO,(corpo.perna[i].me[0].y+corpo.perna[i].me[1].y)*LADO,-(corpo.perna[i].me[0].z+corpo.perna[i].me[1].z)*LADO);
            glBegin(GL_QUADS);
            //Parte de cima do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();
            //Parte de baixo do cubo
            glBegin(GL_LINE_LOOP);
            glVertex3f(0,0,0);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(0,LADO*2,0);
            glEnd();

            glBegin(GL_LINES);
            glVertex3f(0,0,0);
            glVertex3f(0,0,-LADO);
            glVertex3f(LADO,0,0);
            glVertex3f(LADO,0,-LADO);
            glVertex3f(LADO,LADO*2,0);
            glVertex3f(LADO,LADO*2,-LADO);
            glVertex3f(0,LADO*2,0);
            glVertex3f(0,LADO*2,-LADO);
            glEnd();

            glPopMatrix();
        }
    }
}
//Chama as funções para desenhar o Corpo no modo Programado e Matrix (Animação 1)
void DesenhaCorpoProgramado1(Corpo corpo,int timer,int tipo){
    if(tipo == 0){
        if(TempoDecorrido(timer)>0.15 && angHor > -360.0)
            angHor -= 1;
        else if(TempoDecorrido(timer)>7.70 && angcabeca2 <= 0 && angcabeca2 > -10.0)
            angcabeca2 -= 0.3;
        else if(TempoDecorrido(timer)>8.40 && angperna <= 0 && angperna > -40.0){
            angcabeca2 = 0.05;
            angperna -= 0.5;
            angperna2 += 0.3;
            angtronco2 += 0.2;
        }
        else if (TempoDecorrido(timer)>9.80 && angperna >= -40.0 && angperna < 0){
            angperna = 0.05;
            angperna2 = 0;
            angtronco2 = 0;
            angperna3 = 0;
        }
        else if (TempoDecorrido(timer)>9.85 && angperna3 <= 0 && angperna3 > -40.0){
            angperna3 -= 0.5;
            angperna4 += 0.3;
            angtronco2 -= 0.2;
        }
        else if (TempoDecorrido(timer)>11.20 && angperna3 >= -40.0 && angperna3 < 0){
            angperna3 = 0.05;
            angperna4 = 0;
            angperna = 0;
            angtronco2 = 0;
        }
    }
    else{
        if(TempoDecorrido(timer)>0.15 && angHor > -360.0)
            angHor -= 0.3;
        else if(TempoDecorrido(timer)>20.15 && angcabeca2 <= 0 && angcabeca2 > -10.0)
            angcabeca2 -= 0.01;
        else if(TempoDecorrido(timer)>36.80 && angperna <= 0 && angperna > -40.0){
            angcabeca2 = 0.05;
            angperna -= 0.05;
            angperna2 += 0.03;
            angtronco2 += 0.02;
            printf("%.2f - %.2f\n",TempoDecorrido(timer),angperna);
        }
        else if (TempoDecorrido(timer)>50.15 && angperna >= -40.0 && angperna < 0){
            angperna = 0.05;
            angperna2 = 0;
            angtronco2 = 0;
            angperna3 = 0;
        }
        else if (TempoDecorrido(timer)>50.20 && angperna3 <= 0 && angperna3 > -40.0){
            angperna3 -= 0.05;
            angperna4 += 0.03;
            angtronco2 -= 0.02;
            printf("%.2f \n",TempoDecorrido(timer));
        }
        else if (TempoDecorrido(timer)>63.60 && angperna3 >= -40.0 && angperna3 < 0){
            angperna3 = 0.05;
            angperna4 = 0;
            angperna = 0;
            angtronco2 = 0;
        }
    }
    DesenhaTronco(corpo);
    DesenhaTronco2(corpo);
    DesenhaPernaProgramada(corpo);
}
//Chama as funções para desenhar o Corpo no modo Programado e Matrix (Animação 2)
void DesenhaCorpoProgramado2(Corpo corpo,int timer,int tipo){
    if(tipo == 0){
        if(TempoDecorrido(timer)>0.15 && angbraco1 > -30.0 && angbraco2 > -120.0){
            angbraco1 -= 0.5;
            angbraco2 -= 2;
            angtronco -= 0.2;
        }
        else if (TempoDecorrido(timer)>2.20 && angbraco2 >= -120.0 && angperna > -90.0){
            angbraco1 = 0;
            if(angtronco < 20.0)
                angtronco += 1;
            angperna -= 2;
            angperna2 += 2;
        }
        else if(TempoDecorrido(timer)>3.10 && angVer < 360.0){
            angVer +=1;
        }
        else if(TempoDecorrido(timer)>9.30 && angbraco1 > -90.0){
            angperna = -360;
            angperna2 = -360;
            angbraco2 = -360;
            angtronco = 0;
            angbraco1 -= 2;
        }
    }
    else{
        if(TempoDecorrido(timer)>0.15 && angbraco1 > -30.0 && angbraco2 > -120.0){
            angbraco1 -= 0.1;
            angbraco2 -= 0.6;
            angtronco -= 0.05;
        }
        else if (TempoDecorrido(timer)>3.50 && angperna > -90.0){
            angbraco1 = 0;
            if(angtronco < 20.0)
                angtronco += 0.3;
            angperna -= 0.6;
            angperna2 += 0.6;
        }
        else if(TempoDecorrido(timer)>6.00 && angVer < 360.0){
            angVer +=0.1;
            printf("%.2f \n",TempoDecorrido(timer));
        }
        else if(TempoDecorrido(timer)>66.00 && angbraco1 > -90.0){
            angperna = -360;
            angperna2 = -360;
            angbraco2 = -360;
            angtronco = 0;
            angbraco1 -= 0.6;
        }
    }
    DesenhaTronco(corpo);
    DesenhaTronco2(corpo);
    DesenhaPerna(corpo);
}
//Chama as funções para desenhar o Corpo no modo Programado e Matrix (Animação 3)
void DesenhaCorpoProgramado3(Corpo corpo,int timer,int tipo){
    if(tipo == 0){
        if(TempoDecorrido(timer)>0.15 && angHor > -90.0 && TempoDecorrido(timer)<1.60)
            angHor -= 1;
        else if (TempoDecorrido(timer)>1.60 && angperna3 < 35.0 && TempoDecorrido(timer)<2.22){
            angperna3 += 1;
            if (angperna4 < 20)
                angperna4 += 0.5;
        }
        else if (TempoDecorrido(timer)>2.22 && angperna3 > -20.0 && TempoDecorrido(timer)<3.30){
            angperna3 -= 1;
            if(angperna3 < 0 && angperna4 > 0)
                angperna4 -= 1;
        }
        else if (TempoDecorrido(timer)>3.20 && angHor != 0){
            if(angHor != 0)
                angHor += 1;
            if(angperna3 != 0)
                angperna3 += 1;
        }
        else if (TempoDecorrido(timer)> 4.70){
            if(angbraco3 == 0)
                angbraco3 = 2;
        }
    }
    else{
        if(TempoDecorrido(timer) > 0.15 && angHor > -90.0 && TempoDecorrido(timer) < 5.15)
            angHor -= 0.3;
        else if (TempoDecorrido(timer) > 5.15 && angperna3 < 35.0 && TempoDecorrido(timer) < 7.10){
            angperna3 += 0.3;
            if (angperna4 < 20)
                angperna4 += 0.1;
        }
        else if (TempoDecorrido(timer) > 7.10 && angperna3 > -20.0 && TempoDecorrido(timer) < 10.20){
            angperna3 -= 0.3;
            if(angperna3 < 0 && angperna4 > 0)
                angperna4 -= 0.3;
        }
        else if (TempoDecorrido(timer)>10.20 && angHor < 0){
            if(angHor < 0)
                angHor += 0.3;
            if(angperna3 < 0)
                angperna3 += 0.1;
        }
        else if (TempoDecorrido(timer)> 15.25){
            if(angbraco3 == 0)
                angbraco3 = 2;
        }
    }
    DesenhaTronco(corpo);
    DesenhaTronco2(corpo);
    DesenhaPernaProgramada(corpo);
}
//Mostra qual placa de video do PC e versão do OpenGL
void ShowVersion(){
    const GLubyte* renderer = glGetString(GL_RENDERER); // Placa de Video
    const GLubyte* version = glGetString(GL_VERSION); // Versão do OpenGL
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
}
//Seta o modo de exibição para 2D
void Setup2D(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, LARG_TELA, ALT_TELA, 0, -1, 1);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
//Seta o modo de exibição para 3D
void Setup3D(float eyex,float eyey,float eyez,float angHor,float angVer){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0,1.0,0.1,500.0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-eyex,-eyey,-eyez);
    glRotatef(angHor,0,1,0);
    glRotatef(angVer, cos((angHor)*M_PI/180.0), 0.0,sin((angHor)*M_PI/180.0));
}
//Principal
int main( int argc, char* args[] ){
    PIG_Evento evento;
    PIG_Teclado meuTeclado = GetTeclado();;

    CriaJogo("AniMacraft");
    glewInit();
    int timer = CriaTimer();
    Corpo corpo;
    ShowVersion();

    while (JogoRodando()!=0){
        evento = GetEvento();
        if(GetEstadoJogo()==menu){
            if (evento.tipoEvento==EVENTO_TECLADO&&evento.teclado.acao==TECLA_PRESSIONADA){
                if (evento.teclado.tecla==TECLA_ENTER){
                    flagcontrole=1;
                    InicializaAngulos();
                    SetEstadoJogo(livre);
                }else if (evento.teclado.tecla==TECLA_BARRAESPACO){
                    flagcontrole=1;
                    InicializaAngulos();
                    SetEstadoJogo(programado);
                }else if (evento.teclado.tecla==TECLA_BACKSPACE){
                    flagcontrole=1;
                    InicializaAngulos();
                    SetEstadoJogo(matrix);
                }else if (evento.teclado.tecla==TECLA_ESC){
                    SetEstadoJogo(sair);
                }
            }
        }
        if(GetEstadoJogo()==livre){
            if (evento.tipoEvento==EVENTO_TECLADO&&evento.teclado.acao==TECLA_PRESSIONADA){
                if (evento.teclado.tecla==TECLA_ESQUERDA){
                    angHor -= 1;
                }else if(evento.teclado.tecla==TECLA_DIREITA){
                    angHor += 1;
                }else if(evento.teclado.tecla==TECLA_CIMA){
                    eyex += sin((angHor)*M_PI/180.0)*0.1f;
                    eyez -= cos((angHor)*M_PI/180.0)*0.1f;
                }else if(evento.teclado.tecla==TECLA_BAIXO){
                    eyex -= sin((angHor)*M_PI/180.0)*0.1f;
                    eyez += cos((angHor)*M_PI/180.0)*0.1f;
                }else if(evento.teclado.tecla==TECLA_w){
                    if(angperna>-90.0 && angperna <= 15.0)
                        angperna -= 1;
                }else if(evento.teclado.tecla==TECLA_q){
                    if(angperna>=-90.0 && angperna < 15.0)
                        angperna += 1;
                }else if(evento.teclado.tecla==TECLA_s){
                    if(angperna2>=0 && angperna2 < 100.0)
                        angperna2 += 1;
                }else if(evento.teclado.tecla==TECLA_a){
                    if(angperna2>0 && angperna2 <= 100.0)
                        angperna2 -= 1;
                }else if(evento.teclado.tecla==TECLA_x){
                    angperna3 += 1;
                }else if(evento.teclado.tecla==TECLA_z){
                    angperna3 -= 1;
                }else if(evento.teclado.tecla==TECLA_r){
                    if(angbraco1 > -185.0 && angbraco1 <= 30.0)
                        angbraco1-= 1;
                }else if(evento.teclado.tecla==TECLA_e){
                    if(angbraco1 >= -185.0 && angbraco1 < 30.0)
                        angbraco1 += 1;
                }else if(evento.teclado.tecla==TECLA_f){
                    if(angbraco2 < 0 && angbraco2 >= -165.0)
                        angbraco2 += 1;
                }else if(evento.teclado.tecla==TECLA_d){
                    if(angbraco2 <= 0 && angbraco2 > -165.0)
                        angbraco2 -= 1;
                }else if(evento.teclado.tecla==TECLA_v){
                    angbraco3 -= 1;
                }else if(evento.teclado.tecla==TECLA_c){
                    angbraco3 -= 1;
                }else if(evento.teclado.tecla==TECLA_y){
                    if(angcabeca >= -4.0 && angcabeca < 4.0)
                        angcabeca += 1;
                }else if(evento.teclado.tecla==TECLA_t){
                    if(angcabeca > -4.0 && angcabeca <= 4.0)
                        angcabeca -= 1;
                }else if(evento.teclado.tecla==TECLA_h){
                    if(angcabeca2 >= -10.0 && angcabeca2 < 10.0)
                        angcabeca2 += 1;
                }else if(evento.teclado.tecla==TECLA_g){
                    if(angcabeca2 > -10.0 && angcabeca2 <= 10.0)
                        angcabeca2 -= 1;
                }else if(evento.teclado.tecla==TECLA_n){
                    if(angtronco >= -10.0 && angtronco < 105.0)
                        angtronco += 1;
                }else if(evento.teclado.tecla==TECLA_b){
                    if(angtronco > -10.0 && angtronco <= 105.0)
                        angtronco -= 1;
                }else if(evento.teclado.tecla==TECLA_i){
                    if(angtronco2 >= -10.0 && angtronco2 < 10.0)
                        angtronco2 += 1;
                }else if(evento.teclado.tecla==TECLA_u){
                    if(angtronco2 > -10.0 && angtronco2 <= 10.0)
                        angtronco2 -= 1;
                }else if (evento.teclado.tecla==TECLA_BARRAESPACO){
                    flagcontrole=1;
                    InicializaAngulos();
                    SetEstadoJogo(programado);
                }else if (evento.teclado.tecla==TECLA_BACKSPACE){
                    flagcontrole=1;
                    InicializaAngulos();
                    SetEstadoJogo(matrix);
                }else if (evento.teclado.tecla==TECLA_ESC){
                    SetEstadoJogo(sair);
                }
            }
        }
        if(GetEstadoJogo()==programado){
            if (evento.tipoEvento==EVENTO_TECLADO&&evento.teclado.acao==TECLA_PRESSIONADA){
                if (evento.teclado.tecla==TECLA_ESQUERDA){
                    angHor -= 1;
                }else if(evento.teclado.tecla==TECLA_DIREITA){
                    angHor += 1;
                }else if(evento.teclado.tecla==TECLA_CIMA){
                    eyex += sin((angHor)*M_PI/180.0)*0.1f;
                    eyez -= cos((angHor)*M_PI/180.0)*0.1f;
                }else if(evento.teclado.tecla==TECLA_BAIXO){
                    eyex -= sin((angHor)*M_PI/180.0)*0.1f;
                    eyez += cos((angHor)*M_PI/180.0)*0.1f;
                }else if (evento.teclado.tecla==TECLA_1){
                    flagcontrole=1;
                    InicializaAngulos();
                    timer = CriaTimer();
                    cena=1;
                }else if (evento.teclado.tecla==TECLA_2){
                    flagcontrole=1;
                    InicializaAngulos();
                    timer = CriaTimer();
                    cena=2;
                }else if (evento.teclado.tecla==TECLA_3){
                    flagcontrole=1;
                    InicializaAngulos();
                    timer = CriaTimer();
                    cena=3;
                }else if (evento.teclado.tecla==TECLA_ENTER){
                    flagcontrole=1;
                    InicializaAngulos();
                    SetEstadoJogo(livre);
                }else if (evento.teclado.tecla==TECLA_BACKSPACE){
                    flagcontrole=1;
                    InicializaAngulos();
                    SetEstadoJogo(matrix);
                }else if (evento.teclado.tecla==TECLA_ESC){
                    SetEstadoJogo(sair);
                }
            }
        }
        if(GetEstadoJogo()==matrix){
            if (evento.tipoEvento==EVENTO_TECLADO&&evento.teclado.acao==TECLA_PRESSIONADA){
                if (evento.teclado.tecla==TECLA_ESQUERDA){
                    angHor -= 1;
                }else if(evento.teclado.tecla==TECLA_DIREITA){
                    angHor += 1;
                }else if(evento.teclado.tecla==TECLA_CIMA){
                    eyex += sin((angHor)*M_PI/180.0)*0.1f;
                    eyez -= cos((angHor)*M_PI/180.0)*0.1f;
                }else if(evento.teclado.tecla==TECLA_BAIXO){
                    eyex -= sin((angHor)*M_PI/180.0)*0.1f;
                    eyez += cos((angHor)*M_PI/180.0)*0.1f;
                }else if (evento.teclado.tecla==TECLA_1){
                    flagcontrole=1;
                    InicializaAngulos();
                    timer = CriaTimer();
                    cena=1;
                }else if (evento.teclado.tecla==TECLA_2){
                    flagcontrole=1;
                    InicializaAngulos();
                    timer = CriaTimer();
                    cena=2;
                }else if (evento.teclado.tecla==TECLA_3){
                    flagcontrole=1;
                    InicializaAngulos();
                    timer = CriaTimer();
                    cena=3;
                }else if (evento.teclado.tecla==TECLA_ENTER){
                    flagcontrole=1;
                    InicializaAngulos();
                    SetEstadoJogo(livre);
                }else if (evento.teclado.tecla==TECLA_BARRAESPACO){
                    flagcontrole=1;
                    InicializaAngulos();
                    SetEstadoJogo(programado);
                }else if (evento.teclado.tecla==TECLA_ESC){
                    SetEstadoJogo(sair);
                }
            }
        }
        IniciaDesenho();

        if(GetEstadoJogo()==menu){
            Setup2D();
        }
        if(GetEstadoJogo()==livre){
            Setup3D(eyex,eyey,eyez,angHor,angVer);
            if(flagcontrole){
                corpo = CriaCorpo();
                flagcontrole = 0;
            }
            DesenhaCorpo(corpo);
        }
        if(GetEstadoJogo()==programado){
            Setup3D(eyex,eyey,eyez,angHor,angVer);
            if(flagcontrole){
                corpo = CriaCorpo();
                flagcontrole = 0;
            }
            if(cena == 1)
                DesenhaCorpoProgramado1(corpo,timer,0);
            else if (cena == 2)
                DesenhaCorpoProgramado2(corpo,timer,0);
            else if (cena == 3)
                DesenhaCorpoProgramado3(corpo,timer,0);
            else
                DesenhaCorpo(corpo);
        }
        if(GetEstadoJogo()==matrix){
            Setup3D(eyex,eyey,eyez,angHor,angVer);
            if(flagcontrole){
                corpo = CriaCorpo();
                flagcontrole = 0;
            }
            if(cena == 1)
                DesenhaCorpoProgramado1(corpo,timer,1);
            else if (cena == 2)
                DesenhaCorpoProgramado2(corpo,timer,1);
            else if (cena == 3)
                DesenhaCorpoProgramado3(corpo,timer,1);
            else
                DesenhaCorpo(corpo);
        }
        if(GetEstadoJogo()==sair){
            goto end;
        }

        EncerraDesenho();
    }
    end: system("cls");
    FinalizaJogo();
    return 0;
}
