#include<stdio.h>
#include<math.h>

int main(){
    FILE *fp1,*fp2;
    int i,j;
    double a,b=0,result=0,temp; 
    double e_array[100], fw_array[100];
    
    printf("please input a \n");
    scanf("%lf",&a);
    
    fp1=fopen("data.txt","r");
    fp2=fopen("result.txt","w");
    for(i=0; i<40; i++)
    {
        fscanf(fp1,"%lf %lf",&e_array[i],&fw_array[i]);
        //printf("\n%lf,%lf,", e_array[i], fw_array[i]);
    }
    
    for(j=0;j<70;j++){ 
    	result=0;
		for(i=0;i<40;i++){
            temp=2.0f*(b-e_array[i])*sqrt(log(2.0f))/a;
            result= result + 2.0f*sqrt(log(2.0f)/3.14159f)*(fw_array[i]/a)*exp(-temp*temp); 
        }
        fprintf(fp2,"%lf\n",result);
        b+=0.1f;
	}
    printf("finish");
    fclose(fp1);
    fclose(fp2);

}
 
