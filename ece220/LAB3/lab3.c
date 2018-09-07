#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//note: use M_PI for the value of PI
int main(){
	int n;
	int omega1;
	int omega2;
	int i;
	double x=-999; //will be overwritten in the loop
	double f=-999;	//will be overwritten in the loop
	printf("Enter the values of n, omega1, and omega2\n");//prompt
	scanf("%d%d%d",&n,&omega1,&omega2); //user inputs values
	for(i = 0; i < 10; i++){
		x = (i*M_PI)/n;
		f = sin(omega1*x) + (0.5*sin(omega2*x));
		printf("(%f, %f)\n",x,f);

	}
	return 0;

}
