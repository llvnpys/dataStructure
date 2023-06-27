#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

int gcd(int a, int b){
  int largeNum;
  int smallNum;

  if(a< b){
    largeNum = a;
    smallNum = b;
  }
  else{
    largeNum = b;
    smallNum = a;
  }
  while(smallNum != 0){
    int tmp = largeNum % smallNum;
    largeNum = smallNum;
    smallNum = tmp;
  }
  return largeNum;
}

int lcm(int a, int b){
  return (a * b) / gcd(a, b);
}

int main(int argc, char **argv){
  fin = fopen(argv[1], "r");
  fout = fopen(argv[2], "w");

  int a, b;
  int gcd_value;
  int lcm_value;

  while(fscanf(fin, "%d%d", &a, &b)!= EOF){
    gcd_value = gcd(a,b);
    fprintf(fout, "gcd value:%d ", gcd_value);
    lcm_value = lcm(a,b);
    fprintf(fout, "lcm value:%d\n", lcm_value);
  }

  fclose(fin);
  fclose(fout);
}
