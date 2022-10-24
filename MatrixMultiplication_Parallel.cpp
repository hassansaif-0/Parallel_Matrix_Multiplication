
//--------------------------------LIBRARIES TO BE USED----------------------------------------------------------------------------
#include<iostream>
using namespace std;
#include<string>
#include<ctime>
#include<pthread.h>

//-------------------------------DECLARARTION OF GLOBAL VARIABLES TO USE IN THREADS----------------------------------------------
float **matA;
float **matB;
float **result1;
float **result2;
float **result3;
int mat1rows=1024;
int mat1cols=1024;
int mat2rows=1024;
int mat2cols=1024;

//-----------------------------A STRUCTURE VARIABLE TO HOLD DATA FOR A THREAD IN PARALLE PROCESSING------------------------------
struct data
{

int start;
int end;
int cols;

};



//------------------------------------Part i serial multiplication of----------------------------------------------------------------------

void serial()
{

for(int i = 0; i <mat1rows; ++i)
{
        for(int j = 0; j < mat2cols; ++j)
        {
        
        result1[i][j]=0;
        
            for(int k = 0; k < mat1cols; ++k)
            {
            
                result1[i][j] += matA[i][k] * matB[k][j];
                
            }
            
            }
            
}

}


//----------------------------------------PART II PARALLEL MULTIPLICATION BY DIVIDING MATRICES BLOCK WISE--------------------------------------

void *parallel(void *arg)
{

data *ptr=new data;
ptr=(data*)arg;
//GETTING THREAD SPECIFIC DATA
int s=ptr->start;
int r=ptr->end;
int col=ptr->cols;

for(int i = s; i < r; ++i)
{

        for(int j = 0; j < col; ++j)
        {
        
        result2[i][j]=0;
            for(int k = 0; k < col; ++k)
            {
                result2[i][j] += matA[i][k] * matB[k][j];
               
            }
	}
}



pthread_exit(NULL);

}





void *parallelcyclic(void *arg)
{

data *ptr=new data;
ptr=(data*)arg;

int s=ptr->start;
int r=ptr->end;
int col=ptr->cols;


for(int i = s; i < r; ++i)
{

        for(int j = 0; j < col; ++j)
        {
        
        result3[i][j]=0;
            for(int k = 0; k < col; ++k)
            {
                result3[i][j] += matA[i][k] * matB[k][j];
               
            }
}
}

pthread_exit(NULL);

}





//-----------------------------------A CHECKER FUNCTION TO EVALUATE MATRICES COMPUTED BY VARIOUS METHODS------------------------------------

bool checker()
{
for(int i=0;i<mat1rows;i++)
{
for(int j=0;j<mat1cols;j++)
{
if((result1[i][j]!=result2[i][j])||(result1[i][j]!=result3[i][j]))
{
return false;
}
}

}
return true;
}















int main(int argc,char **argv)
{

//DYNAMICALLY ALLOCATING MEMORIES

matA=new float *[mat1rows];
matB=new float* [mat1rows];

result1=new float* [mat1rows];
result2=new float* [mat1rows];
result3=new float* [mat1rows];
for(int i=0;i<mat1rows;i++)
{
matA[i]=new float[mat1cols];
matB[i]=new float[mat2cols];
result1[i]=new float[mat1cols];
result2[i]=new float[mat2cols];
result3[i]=new float[mat2cols];

}

//SEEDING THE TIME FOR RANDOM NUMBER GENERATION
srand(time(0));

//FILLING UP FIRST MATRIX

for(int i=0;i<mat1rows;i++)
{
for(int j=0;j<mat1cols;j++)
{

matA[i][j]=rand()%10+1;

}
}

//FILLING UP SECOND MATRIX

for(int i=0;i<mat2rows;i++)
{
for(int j=0;j<mat2cols;j++)
{

matB[i][j]=rand()%10+1;

}
}

//--------------------------task i-------------------------------------------------------------------------------
cout<<"Executing Serial Version"<<endl;
serial();


//RECEIVING COMAND LINE ARGUMENTS

int input=atoi(argv[1]);
//DECLARING ARRAY OF TIDS
 pthread_t *tid=new pthread_t[input];  
 
int blocks=mat1rows/input;
cout<<"Blocks"<<blocks<<endl;

cout<<"\n\nExecuting Parallel Version...............\n\n"<<endl;
//---------------------------------------------------------------task2 i-------------------------------------------------------------
int start=0,end;
   for(int i = 0; i < input; i++) {
     
     cout<<"Thread::"<<i+1<<"executing"<<endl;
     
         data *obj =new data ;
        obj->start=start;
        obj->end=start+blocks;
        obj->cols=mat2cols;
         
             //Thread ID
         pthread_attr_t attr; //Set of thread attributes
         
         //Get the default attributes
         pthread_attr_init(&attr);
         
         //Create the thread
         pthread_create(&tid[i],&attr,parallel,obj);
         
         
         //Make sure the parent waits for all thread to complete
       
    //     count++;
    start=start+blocks;
     
   }


 for(int i=0;i<input;i++)
 {
  pthread_join(tid[i], NULL);
 }





//-------------------------------------------task2 ii-------------------------------------------------------------------



cout<<"\n\nExecuting Cyclic Version...............\n\n"<<endl;

int starter=0;

int tester=blocks/input;



for(int iter=0;iter<tester;iter++)
{

for(int i = 0; i < input; i++) {
     
     cout<<"Thread::"<<i+1<<"executing"<<endl;
     
         data *obj =new data ;
        obj->start=starter;
        obj->end=starter+input;
        obj->cols=mat2cols;
         
             //Thread ID
         pthread_attr_t attr; //Set of thread attributes
         
         //Get the default attributes
         pthread_attr_init(&attr);
         
         //Create the thread
         
         pthread_create(&tid[i],&attr,parallelcyclic,obj);
          pthread_join(tid[i], NULL);
         if(i==input)
         {
         i=0;
         }
         
      
    starter=starter+input;
   
     
   }


}











input=checker();
if(input)
cout<<"\n\n SAME RESULT PRODUCED BY ALL METHODS\n\n"<<endl;
else
cout<<"ERROR INCONSISTENT RESULT GENERATED\n\n"<<endl;

pthread_exit(NULL);




}

