/*
* 
*   Author:  Eliel Marcos Romancini  (13/08/2019) 
* 
*   Filter Low Pass IIR
*   Order: 10ª
*   Freq. Sample: 1200Hz
*   Freq. Cut: 70Hz
*   Implementação: sub-sistemas de segunda ordem, na forma direta II.
*
*   Primeiro sub-sistema:
*   w1[n] = x[n] - a11 w1[n - 1] - a21 w1[n - 2]
*   y1[n] = g1 (b01 w1[n] + b11 w1[n - 1] + b21 w1[n - 2])  
* 
*   Segundo sub-sistema: 
*   w2[n] = y1[n] - a12 w2[n - 1] - a22 w2[n - 2]
*   y2[n] = g2 (b02 w2[n] + b12 w2[n - 1] + b22 w2[n - 2])
*
*   Terceiro sub-sistema:
*   w3[n] = y2[n]...
*   y3[n] = g3 (b03 w3[n]...  
*           :
*           :
*           :
*          
*/


#include <math.h>
#include <stdio.h>

#define f0          60
#define fs          1200
#define dc_level    512
#define amplitude   200

uint64_t n = 0;
int out = 0;
double filter_out = 0;
int sample = 0;
uint32_t t = 0;

// Coeficientes dos filtros
// Fs = 1200Hz;  Fc = 100Hz
double Coef[5][6] = {{1,2,1,1,-1.7680424510392703,0.89382981996845645},
                    {1,2,1,1,-1.605889608187006,0.72014061414324182},
                    {1,2,1,1,-1.489671523527865,0.59565419459051838},
                    {1,2,1,1,-1.4152575415932973,0.51594603038470987},
                    {1,2,1,1,-1.3790411698621838,0.47715304511722995}};
// Constantes do ganho dos filtros
double Gain[5]    = {0.031446842232296525,
                     0.028562751489058898,
                     0.026495667765663325,
                     0.025172122197853121,
                     0.024527968813761531};

int j=0;
int Ordem=10;                       // Ordem do filtro total
double w[3][5] = {{0,0,0,0,0},      // matriz de saidas atrasadas
                  {0,0,0,0,0},
                  {0,0,0,0,0}};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(500000);

}

int sample0 = 0;

void loop() {
  // Gerador de sinal (Entrada)
  n++; sample0 = dc_level + amplitude * sin(2*M_PI*f0*n/fs);
  sample = sample0 + amplitude * sin(2*M_PI*200*n/fs);

  filter_out = sample;

  // Filtro
  for(j=0;j<ceil(Ordem/2);j++)
  {
      w[2][j] = w[1][j];
      w[1][j] = w[0][j];

      w[0][j] = (Coef[j][3]*filter_out) - (Coef[j][4]*w[1][j]) - (Coef[j][5]*w[2][j]);

      filter_out = Gain[j] * ((Coef[j][0]*w[0][j]) + (Coef[j][1]*w[1][j]) + (Coef[j][2]*w[2][j]));
  }

  // Saída
  out = filter_out;

  printf("%d ",sample+512);
  printf("%d\n",out-512);

  delay(10);
  
}
