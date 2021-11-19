#include <math.h>
double cubicData[4] = {0, 0, 0, 0}; 
double cubicFunc();
void Elimination(double arr[][4], int pivot, int target);


void loop(){
  updataData(value);
  estimatedValue = cubicFunc();
}

void updateData(double newData){
  for (int i=0; i<3; i++){
    cubicData[i] = cubicData[i+1];
  }
  cubicData[3] = newData;
}

double cubicFunc(void){
  double matrix[3][4];
  for(int i=0; i<3; i++){
    matrix[i][0] = cubicData[i+1] - cubicData[0];
    for(int j=1; j<4; j++) matrix[i][j] = pow(i+1, j); //pow - #include <math.h> 
  }
  for(int i=0; i<3; i++){  
    for(int j=0; j<3; j++){
      if(i==j) continue;
      Elimination(matrix, i, j);
    }
  }
  
  double result = 0;
  for(int i=0; i<3; i++) result += matrix[i][0]*(pow(4, i+1));
  
  result += cubicData[0];
  
  return result;
}

void Elimination(double arr[][4], int pivot, int target){
  double multiple = arr[target][pivot+1];
  double divide = arr[pivot][pivot+1];
  for(int i=0; i<4; i++) {
      arr[pivot][i] /= divide;
      arr[target][i] -= arr[pivot][i]*multiple;
  }
}
