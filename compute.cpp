#include<stdio.h>
#include<math.h>

#define MAX_TRANSITION_NUMBER 100
#define MAX_WAVELENGTH_NUMBER 20000
#define PIE 3.14159f
#define LN2 log(2.0f)

typedef struct
{
    double* x;
    double* y;
    int count;
}DataPairs;

typedef struct
{
    double min;
    double max;
}ValueRange;

bool readDataFromFile(const char* filename, double* param1, double* param2, int record_num)
{
    //Read raw data from file to preallocated arrays
    FILE *fp;
    fp = fopen(filename,"r");
    if (!fp)
        return false;
    
    for(int i=0; i<record_num; i++)
        fscanf(fp, "%lf %lf", param1 + i, param2 + i);

    return true;
}

bool writeResultToFile(const char* filename, double* param1, double* param2, int record_num)
{
    //Write arrays to file
    FILE *fp;
    fp = fopen(filename,"w");
    if (!fp)
        return false;
    
    for(int i=0; i<record_num; i++)
        fprintf(fp, "%lf %lf\n", param1[i], param2[i]);

    return true;
}

double calc_transition(double wavelen, double intensity, DataPairs* transition_data, ValueRange* fwhm_range)
{
    double e_array[MAX_TRANSITION_NUMBER], fw_array[MAX_TRANSITION_NUMBER];
    double fwhm[MAX_TRANSITION_NUMBER];
    double temp, result;
    int transition_count = 0;
    int i;
    
    for(i=0; i<transition_data->count; i++)
    {
        //Calculate then FWHM when df/ds=0
        temp = 2.0f * sqrt(2.0f * LN2) * (wavelen -  transition_data->x[i]);
        if (temp < 0) temp = -temp;
        
        //Save the transition if it satisfy FWHM criteria
        if (temp >= fwhm_range->min && temp <= fwhm_range->max)
        {
            e_array[transition_count] = transition_data->x[i];
            fw_array[transition_count] = transition_data->y[i];
            fwhm[transition_count] = temp;
            transition_count++;
        }
    }
    if (transition_count < 1)
        return 0;
    
    
    //Calculate Sc basing on selected transitions 
    result = 0;
    for(i=0; i<transition_count; i++){
        temp = 2.0f * (wavelen - e_array[i]) * sqrt(LN2) / fwhm[i];
        result= result + 2.0f * sqrt(LN2/PIE) * (fw_array[i] / fwhm[i]) * exp(-temp*temp); 
    }
    return intensity / result;
}

void fit_gaussian(const char* filename, DataPairs* transition_data, ValueRange* fwhm_range, DataPairs* fit_result)
{
    FILE* fp = fopen(filename, "r");
    if (!fp) return;
    if (transition_data->count < 1) return;
    
    fit_result->x = (double*)malloc(sizeof(double) * MAX_WAVELENGTH_NUMBER);
    fit_result->y = (double*)malloc(sizeof(double) * MAX_WAVELENGTH_NUMBER);
    
    double wavelen, intensity;
    int count;
    while(!feof(fp))
    {
        fscanf(fp, "%lf %lf", &wavelen, &intensity);
        
        fit_result->x[count] = wavelen;
        fit_result->y[count] = calc_transition(wavelen, intensity, transition_data, fwhm_range);        
        count++;
    }
    fit_result->count = count;
}


int main(){
    const char* dataFile_Transition = "data.txt";
    const char* dataFile_Absorption = "data2.txt";
    const char* resultFile_Sc = "result.txt";
    const int transition_num=40;
    double e_array[MAX_TRANSITION_NUMBER], fw_array[MAX_TRANSITION_NUMBER];
    DataPairs transition_data, fit_Sc;
    ValueRange fwhm_range;
    
    //Read parameters
    printf("Please input the range of FWHM:\n");
    scanf("%lf %lf", &(fwhm_range.min), &(fwhm_range.max));
    
    //Get transition data from file
    readDataFromFile(dataFile_Transition,
                     e_array,
                     fw_array,
                     transition_num);
    transition_data.x = e_array;
    transition_data.y = fw_array;
    transition_data.count = transition_num;
    
    //Fit experiement data
    fit_gaussian(dataFile_Absorption,
                 &transition_data,
                 &fwhm_range,
                 &fit_Sc);
    
    writeResultToFile(resultFile_Sc, fit_Sc.x, fit_Sc.y, fit_Sc.count);
    
    printf("finish\n");
    return 0;
}
 
