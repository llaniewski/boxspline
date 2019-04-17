#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double cspline(double s[], int k, double x, int i) {
  if (k > 0) {
    if (s[0] == 0) {
      return cspline(s+1,k-1,x,i);
    } else {
      return (cspline(s+1,k-1,x+s[0]/2,i+1) - cspline(s+1,k-1,x-s[0]/2,i+1))/s[0];
    }
  } else {
    if (x < 0) return 0;
    if (i < 0) return 0;
    double ret = 1;
    for (; i>0; i--) ret = ret * x / i;
    return ret;
  }
}

int mod(int i, int k) {
  return ((i % k) + k) % k;
}

double cspline_tab(double s[], int k, double x, double dx, double val[], int n, int d) {
  int w = floor(x/dx);
  double ret = 0;
  if (d <= 0) ret += val[mod(w,n)];
  int i = w;
  for (i=w+1; i<100*n;i++) {
    double c = cspline(s,k,x-i*dx,-d);
    if (c == 0) break;
    ret = ret + c*(val[mod(i,n)] - val[mod(i-1,n)]);
  }
  printf("%d - ",i);
  for (i=w; i>-100*n;i--) {
    double c = cspline(s,k,i*dx-x,-d);
    if (d % 2 == 1) c = -c;
    if (c == 0) break;
    ret = ret + c*(val[mod(i-1,n)] - val[mod(i,n)]);
  }
  printf("%d\n",i);
  return ret;
}

double cspline2(double s[], int k, double x) {
  if (k > 0) {
    double a,b;
    a = cspline2(s+1,k-1,x+s[0]/2);
    b = cspline2(s+1,k-1,x-s[0]/2);
    return (a-b)*x/s[0] + (a+b)/2;
  } else {
    if (x < 0) return 0;
    return 1;
  }
}


int main(int argc, char *argv[]) {
  FILE * f = fopen("out.csv","w");
  double s[] = {0.1,0.1,0.1,0.1};
  const int n = 100;
  double v[n];
  for (int i=0; i<n; i++) v[i] = 1.0*rand()/RAND_MAX;
//  for (int i=0; i<n; i++) if (i > n/2) v[i] = 1.0; else v[i] = 0.0;
  
  for (int i=0; i<argc-1; i++) sscanf(argv[i+1],"%lf",&s[i]);
  
  fprintf(f,"x");
  for (int k=0; k<4; k++) fprintf(f,",y%d",k);
  for (int k=0; k<4; k++) fprintf(f,",v%d",k);
  for (int k=0; k<4; k++) fprintf(f,",d%d",k);
  fprintf(f,"\n");
  for (double x = -1; x < 1; x += 0.005) {
    fprintf(f,"%lg",x);
    for (int k=0; k<4; k++) fprintf(f,",%lg", cspline(s, k, x, 0));
    for (int k=0; k<4; k++) fprintf(f,",%lg", cspline_tab(s, k, x, 0.02,v, n, 0));
    for (int k=0; k<4; k++) fprintf(f,",%lg", cspline_tab(s, k, x, 0.02,v, n, 1));
//    for (int k=0; k<4; k++) fprintf(f,",%lg", cspline2(s, k, x));
    fprintf(f,"\n");
  }
  fclose(f);
}


