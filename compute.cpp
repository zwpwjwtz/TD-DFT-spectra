#include<stdio.h>
#include<math.h>

int main(){
    FILE *fp1,*fp2;
    int i,j,k;
    const int data_num=40, round_num=70;
    double a0, af, step, a, b, result, temp; 
    double e_array[100], fw_array[100], result_array[100][100];
    
    //Read parameters
    printf("please input a0, af, interval \n");
    scanf("%lf %lf %lf",&a0, &af, &step);
    
    //Read raw data from file
    fp1=fopen("data.txt","r");
    for(i=0; i<data_num; i++)
    {
        fscanf(fp1,"%lf %lf",&e_array[i],&fw_array[i]);
        //printf("\n%lf,%lf,", e_array[i], fw_array[i]);
    }

    
    //Calculate DTF module
    i=0;
    for(a=a0; a<af; a+=step)
    {
        b=0;
        for(j=0;j<round_num;j++){ 
            result=0;
            for(k=0;k<data_num;k++){
                temp=2.0f*(b-e_array[k])*sqrt(log(2.0f))/a;
                result= result + 2.0f*sqrt(log(2.0f)/3.14159f)*(fw_array[k]/a)*exp(-temp*temp); 
            }
            result_array[i][j]=result;
            b+=0.1f;
        }
        i++;
	}
	
    //Print file header
    fp2=fopen("result.txt","w");
    fprintf(fp2,"a\t");
    for(a=a0;a<af;a+=step)
        fprintf(fp2,"%lf\t",a);
    fprintf(fp2,"\n");
    
    //Print file body
    for(j=0; j<round_num; j++)
    {
        fprintf(fp2,"%d\t",j);
        for(k=0; k<i-1; k++)
            fprintf(fp2,"%lf\t",result_array[k][j]);
        fprintf(fp2,"\n");
    }
    
    printf("finish");
    fclose(fp1);
    fclose(fp2);

}
 
