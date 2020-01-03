/*compile with $g++ -o homo-lumo homo-lumo-x.y.c */
/*to use:   $homo-lumo siestalabel*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
int main(int argc, char *argv[])
{
FILE *outcar;
int i,j,k,s,nbands,nkpt,kh,kl;
char str[200]="",ch;
float efermi,a,b,gapup,gapdown;


if( argc > 1 )strcat(str,argv[1]);

strcat(str,".bands");

outcar = fopen(str,"r"); /* Arquivo ASCII, para leitura */
if(!outcar)
{
printf("Erro na abertura do arquivo %s\n",str);
exit(0);
}


fscanf(outcar,"%f",&efermi);      /*lê a energia de fermi*/

for(i=0;i<3;i++){
do
ch = getc(outcar);           /*vai pra proxima linha*/
while(ch!='\n');
}

fscanf(outcar,"%d",&nbands);      /*lê o numero de bandas*/
fscanf(outcar,"%d",&s);      /*lê se há spin*/
fscanf(outcar,"%d",&nkpt);      /*lê o numero de kpts*/

printf("fermi=%f bands=%d spin=%d nkpt=%d\n\n",efermi,nbands,s,nkpt);

/*-----------------------------no-spin----------------------------------------------------------------------------------------------------*/
if(s==1){

int sum=0,bndh[nkpt],bndl[nkpt];
float h[nkpt],l[nkpt],homo,lumo,directgapup[nkpt],directgapdown[nkpt];


for(i=0;i<nkpt;i++){
b=1000;
fscanf(outcar,"%f",&a);
for(j=0;j<nbands;j++){
fscanf(outcar,"%f",&a);
if(b<efermi && a>efermi){l[i]=a; h[i]=b; bndh[i]=j; bndl[i]=j+1; directgapup[i]=l[i]-h[i];}
b=a;
}}

printf("NKPT   HOMO      LUMO     LUMO-HOMO   bandHOMO  bandLUMO\n");

for(i=0;i<nkpt;i++){
printf("%3d %f %f   %f       %d        %d\n",i+1,h[i],l[i],directgapup[i],bndh[i],bndl[i]);

}


printf("\n");
printf("HOMO(k-point)        LUMO(k-point)        homoBAND     lumoBAND     direct gap(k-point)\n");
homo=h[0];
lumo=l[0];
kh=kl=0;

for(k=0;k<nkpt;k++){
if(homo<h[k]){homo=h[k];kh=k;}
if(lumo>l[k]){lumo=l[k];kl=k;}
}

int g;
gapup=20;
for(j=0;j<nkpt;j++){
if(directgapup[j]<gapup){gapup=directgapup[j];g=j;}
}





printf("%f(%d)         %9f(%d)            %d            %d           %9f(%d)\n\n",homo,kh+1,lumo,kl+1,bndh[kl],bndl[kh],gapup,g+1);

printf("LUMO-HOMO = %f (%d-%d)\n\n",lumo-homo,kl+1,kh+1);


for(k=0;k<nkpt-1;k++){if(bndh[k]!=bndh[k+1])sum++;}
if(sum==0)printf("SEMICONDUCTOR!\n");
else {printf("METAL!\n");}

}

/*------------------------with-spin-------------------------------------------------------------------------------------------------------*/
else{

int sum=0,bndhup[nkpt],bndlup[nkpt],bndhdown[nkpt],bndldown[nkpt],khup,klup;
float hup[nkpt],lup[nkpt],hdown[nkpt],ldown[nkpt],homo,lumo,homoup,lumoup,homodown,lumodown,directgapup[nkpt],directgapdown[nkpt];


for(i=0;i<nkpt;i++){
b=1000;
fscanf(outcar,"%f",&a);
for(j=0;j<nbands;j++){
fscanf(outcar,"%f",&a);
if(b<efermi && a>efermi){lup[i]=a; hup[i]=b; bndhup[i]=j; bndlup[i]=j+1; directgapup[i]=lup[i]-hup[i];}
b=a;
}

b=1000;

for(j=0;j<nbands;j++){
fscanf(outcar,"%f",&a);
if(b<efermi && a>efermi){ldown[i]=a; hdown[i]=b; bndhdown[i]=j; bndldown[i]=j+1; directgapdown[i]=ldown[i]-hdown[i];}
b=a;
}}

printf("NKPT  HOMOup    LUMOup   LUMOup-HOMOup bandHOMOup bandLUMOup  HOMOdown  LUMOdown LUMOdown-HOMOdown bandHOMOdown bandLUMOdown\n");

for(i=0;i<nkpt;i++){
printf("%3d %f %f     %f       %d         %d     %f %f     %f          %d           %d\n",i+1,hup[i],lup[i],directgapup[i],bndhup[i],bndlup[i],hdown[i],ldown[i],directgapdown[i],bndhdown[i],bndldown[i]);}


printf("\n");


homoup=hup[0];
lumoup=lup[0];
khup=klup=0;

for(k=0;k<nkpt;k++){
if(homoup<hup[k]){homoup=hup[k];khup=k;}
if(lumoup>lup[k]){lumoup=lup[k];klup=k;}
}

int gup;
gapup=20;
for(j=0;j<nkpt;j++){
if(directgapup[j]<gapup){gapup=directgapup[j];gup=j;}
}

homodown=hdown[0];
lumodown=ldown[0];
kh=kl=0;

for(k=0;k<nkpt;k++){
if(homodown<hdown[k]){homodown=hdown[k];kh=k;}
if(lumodown>ldown[k]){lumodown=ldown[k];kl=k;}
}

int gdown;
gapdown=20;
for(j=0;j<nkpt;j++){
if(directgapdown[j]<gapdown){gapdown=directgapdown[j];gdown=j;}
}

sprintf(str,"");
for(k=0;k<nkpt-1;k++){if(bndhup[k]!=bndhup[k+1])sum++;}
if(sum==0)strcat(str,"SEMICONDUCTOR!");
else {strcat(str,"METAL!");}

printf("HOMOup(k-point) LUMOup(k-point) bandHOMOup bandLUMOup direct-gap-up(k-point) indirect-gap-up(k-point)    type\n");
printf("%f(%d)    %9f(%d)         %d         %d       %9f(%d)         %9f(%d-%d)          %s\n\n",homoup,khup+1,lumoup,klup+1,bndhup[klup],bndlup[khup],gapup,gup+1,lumoup-homoup,klup+1,khup+1,str);

sprintf(str,""); 
sum=0;
for(k=0;k<nkpt-1;k++){if(bndhdown[k]!=bndhdown[k+1])sum++;}
if(sum==0)strcat(str,"SEMICONDUCTOR!");
else {strcat(str,"METAL!");}

printf("HOMOdown(k-point) LUMOdown(k-point) bandHOMOdown bandLUMOdown direct-gap-down(k-point) indirect-gap-down(k-point)    type\n");
printf("%f(%d)      %9f(%d)           %d           %d         %9f(%d)           %9f(%d-%d)         %s\n\n",homodown,kh+1,lumodown,kl+1,bndhdown[kl],bndldown[kh],gapdown,gdown+1,lumodown-homodown,kl+1,kh+1,str);





}






fclose(outcar);

printf("\n\n------------------------------------------------------\n");
printf("Homo-Lumo energy for SIESTA results\n");
printf("by F. Matusalem - 04/2019 - filipematus@gmail.com\n");
printf("Version 1.0 04/2019\n\n");
   
}
