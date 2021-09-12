// VowelRecognition.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include<math.h>
#define p 12 
#define N 320
#define pi 3.14
static int correct_predicted=0;//This variable count number of test vowel pedicted correctly
void Dc_shift(){//Defination to find average Dc shift in Silence signal 
FILE *fp1,*fp2,*fp3;
int count=0;
long double ch,sum=0,avg;
fp1 = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101038_u_20.txt","r");
while(fscanf(fp1,"%Lf",&ch)==1 && count<2*N){
	sum = sum +ch;
	count++;
  }
avg = sum/count;
printf("avg Amplitude=%Lf",avg);
fp3 = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101038_u_20.txt","r");
fp2 = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_u_DcShift_20.txt","w");
while(fscanf(fp3,"%Lf",&ch)==1 ){
	if(ch >= 0)
	fprintf(fp2,"%Lf\n",ch - avg);
	else{
		fprintf(fp2,"%Lf\n",ch + avg);
	}
 }
return ;
}
void normalization(){//Defination to Normalize the Signal on scale of  10,000  Amplitude 
long double ch,max =0;
FILE *fp1,*fp2,*fp3;
fp1 = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_u_DcShift_20.txt","r");
fp2 = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_u_normalize_20.txt","w");
while(fscanf(fp1,"%Lf",&ch)==1){//finding max absolute amplitude of Signal
	if(abs(ch)>max)
		 max = abs(ch);
}
printf("\nMax Amplitude:%Lf",max);
fp3 = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_u_DcShift_20.txt","r");
while(fscanf(fp3,"%Lf",&ch)==1){
	fprintf(fp2,"%Lf\n",(10000/max)*ch);//Multiplying each Amplitude of Signal with constant
 }
}
void find_Ri(long double steady_frame[],long double Ri[],int start_index){//Defination to  Find Ri values of each frame of Signal
//taking Arguments/Input Steady Frame and Start index of Steady Frame
long double sum =0;
int count =0,i=0;
for(int k = 0 ;k<=p ;k++){
    for(int j = start_index ;j <(N+start_index) - k; j++){
       if(j+k < (N+start_index)-k){
            sum = sum + steady_frame[j]*steady_frame[j +k];
       }
    }
Ri[k] = sum;
sum =0;
 }
 return;
}
void find_Ai(long double Ri[],long double Ai[][p+1]){//Defination to find Ai value of each Frame of Signal,Which is taking input Ri values
long double Ei[p+1],Ki[p+1],temp;
int i,j;
Ei[0]= Ri[0];
for(i = 1 ;i<=p;i++){
        temp =0;
    for(j = 1; j<=i-1;j++){
        temp = temp + Ai[j][i-1]*Ri[i-j];
    }
    Ki[i] = (Ri[i]-temp)/Ei[i-1];
    Ai[i][i]= Ki[i];
    for(j = 1;j<=i-1;j++){
        Ai[j][i] = Ai[j][i-1]-Ki[i]*Ai[i-j][i-1];
    }
    Ei[i]= (1-Ki[i]*Ki[i])*Ei[i-1];
 }
return;
}
void find_Ci(long double Ci[],long double Ai[][p+1]){//Defination to find Ci Value of each Frame,which is taking input Ai Values
	long double sum =0;
	for(int i =1;i<=p;i++){
		for(int j =1; j<=i-1 ; j++){
			sum = sum +((long double)j/i)*Ci[j]*Ai[i-j][p];
		}
		Ci[i] = Ai[i][p] + sum;
		sum =0;
	}
}			
int input_steady_frame(long double steady_frame[],int count){//Defination to take input as a Steady Frame, which is taking input as amplitude count Value from where steady frame start
FILE *fp1;
long double ch;
int k=0,i=0;
fp1 = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_e_normalize_12.txt","r");
while(fscanf(fp1,"%Lf",&ch)==1){
	k++;
	if(k >= count && i<5*N){
getch();
		steady_frame[i++] = ch;
	}
	if(i>=5*N)
		return 1;
 }
}
void find_steady_frame(long double steady_frame[]){//Defination to find Steady frame, which is taking input as Normalize signal
long double ch,current_ste,prev_ste=0,sum=0,start_marker,Ri[p+1],Ai[p+1][p+1],Ci[p+1],Wm;
FILE *fp1,*fp2;
int count =0,i=0,z=0,x=0;
fp1 = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_e_normalize_12.txt","r");//reading normalized signal of Vowel
fp2 = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_Ci_e_test_+Ai_12.txt","w");//opening file to write capstral coefficient for a particular Normalized signal
while(fscanf(fp1,"%Lf",&ch)==1){
	sum = sum +ch*ch;
	count++;
	i++;
	if(i == N){
		current_ste = sum/N;//caculating STE for the frame
		if(current_ste>5*prev_ste){//comparing crrent frame STE with previous frame STE for Finding Marker for Steady Frame
			start_marker = ch;
			z =input_steady_frame(steady_frame,count);//calling Method to get Input as Steady Frame by passing argument as start marker
				if(z ==1){//finding Ri,Ai,and Ci for each  Steady frame 
					for(int i=0;i<5;i++){
					find_Ri(steady_frame,Ri,i*N);//calling method to find Ri Values of each steady frame
					printf("\n");
					find_Ai(Ri,Ai);//calling Method to find Ai values by passing argument as Ri values
					//for(int i=1;i<=p;i++){
				    // Ai[i][p] = -Ai[i][p];
					//}
					
					Ci[0] = log10(pow(Ri[0],2));//finding Ci[0] value of each Steady frame
					find_Ci(Ci,Ai);//calling method to find Ci Values  by passing Argument as Ai values
					for(int i =1 ;i<p;i++){
						Wm = 1+(p/2)*sin(pi*i/p);
						fprintf(fp2,"%Lf\t",Ci[i]*Wm);//mutiplying each Ci values by raised Sine window and writing raised sine value into text File
					 }
					fprintf(fp2,"%Lf\n",Ci[p]*(1+(p/2)*sin(pi*p/12)));
			
					}
				  // return ;
				}
			 return;
		   }
		else {
			prev_ste = current_ste;//if currnet frame STE value not crosses the threshold STE Value then we are Reinitialize the temprory Variable for the Evaluation of Next Frame
		    i=0;
			sum =0;
		}
	}

 }
	return;
}
void find_Avg_Ci(){//Defination to find Average Ci values with respect to the frames
	long double raised_Ci[50][12],ch,avg=0,sum=0;
	FILE *fp1,*fp2;
	int i,j;
	i=j=0;
	fp1 = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_Ci_u_+Ai.txt","r");//reading ci's text file for finding the Average Ci of reference file
	fp2 = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_Ci_u_reference_+Ai.txt","w");//opening file to write Calculated Average Ci Values
	while(fscanf(fp1,"%Lf",&ch)==1){//transfering Ci's value into the 2-D array to find Average Ci values
		if(i<50 && j<12){
           raised_Ci[i][j++] = ch; 
		}
		else{
			if(++i<50){
			j=0;
			raised_Ci[i][j++] = ch; 
			}
			else 
				break;
		}
	}
	/*printf("\n");
	for(int i =0;i<50;i++){
		for(int j =0;j<12;j++){
			printf("%Lf\t",raised_Ci[i][j]);
		}
		printf("\n");
	}*/
	for(int i=0;i<5;i++){//finding the Average Ci
		for(int j=0;j<12;j++){
			for(int k =i;k<50;k= k+5){
			sum = sum + raised_Ci[k][j];
		}
		avg =sum/10;
		fprintf(fp2,"%Lf\t",avg);
		sum=0;
	}
	fprintf(fp2,"\n");
 }
}
void cal_tokhura_distence( long double ref_Ci[][p],long double test_Ci[][p],long double avg_tokhura_dist[],long double weight[],int index){//Defination of method to calculate Tokura Distence Between reference file and test file based on Ci values
	long double sum =0,Avg_dist[5],S=0;
	for(int i =0;i<5;i++){
		for(int j =0;j<12;j++){
			sum = sum + weight[j]*pow((test_Ci[i][j] - ref_Ci[i][j]),2);
		}
		Avg_dist[i] = sum;
		sum=0;
	}
	for (int i =0;i<5;i++){
		S= S+ Avg_dist[i];
	}
	avg_tokhura_dist[index] = S/5;
	return;

}
void vowel_recognise(long double weight[],char test_file[],char vowel[],int test_vowel_index){//Defination of Method to recognise vowel by passing Argument as a test file and Tokhura Weights
 FILE *fp_a,*fp_e,*fp_i,*fp_o,*fp_u,*fp_test;
 long double ref_Ci[5][12],test_Ci[5][12],ch_a,ch_e,ch_i,ch_o,ch_u,ch_t,avg_tokhura_dist[5];
 int i=0 ,j=0,min;
 fp_a= fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_Ci_a_reference_+Ai.txt","r");//reading reference file of vowel "a"
 fp_e = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_Ci_e_reference_+Ai.txt","r");//reading reference file of vowel "e"
 fp_i = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_Ci_i_reference_+Ai.txt","r");//reading reference file of vowel "i"
 fp_o = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_Ci_o_reference_+Ai.txt","r");//reading reference file of vowel "o"
 fp_u = fopen("D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_Ci_u_reference_+Ai.txt","r");//reading reference file of vowel "u"
 fp_test = fopen(test_file,"r");//Reading test file
 while(fscanf(fp_test,"%Lf",&ch_t)==1){//coping test file Ci value into 2-D Array
		//printf("%Lf\t",ch);
		if(i<5 && j<12){
           test_Ci[i][j++] = ch_t; 
		}
		else{
			if(++i<5){
			j=0;
			test_Ci[i][j++] = ch_t; 
			}
			else 
				break;
		}
}
 for(int k=0;k<5;k++){//Reading each reference File and transfering Ci values into 2-D Array
	 if(k==0){//reading Vowel "a"
		 i=j=0;
		 while(fscanf(fp_a,"%Lf",&ch_a)==1){
		//printf("%Lf\t",ch);
		if(i<5 && j<12){
           ref_Ci[i][j++] = ch_a; 
		}
		else{
			if(++i<5){
			j=0;
			ref_Ci[i][j++] = ch_a; 
			}
			else 
				break;
		}
	  }
      cal_tokhura_distence(ref_Ci,test_Ci,avg_tokhura_dist,weight,k);//Calling Method To find Tokhura Distence between Reference file And Test File
	 }
	 if(k==1){//Reading Vowel "e"
		 i= j=0;
		 while(fscanf(fp_e,"%Lf",&ch_e)==1){
		//printf("%Lf\t",ch);
		if(i<5 && j<12){
           ref_Ci[i][j++] = ch_e; 
		}
		else{
			if(++i<5){
			j=0;
			ref_Ci[i][j++] = ch_e; 
			}
			else 
				break;
		}
	}
    
    cal_tokhura_distence(ref_Ci,test_Ci,avg_tokhura_dist,weight,k);
  }
  if(k==2){//Reading Vowel "i"
	     i= j=0;
		 while(fscanf(fp_i,"%Lf",&ch_i)==1){
		//printf("%Lf\t",ch);
		if(i<5 && j<12){
           ref_Ci[i][j++] = ch_i; 
		}
		else{
			if(++i<5){
			j=0;
			ref_Ci[i][j++] = ch_i; 
			}
			else 
				break;
		}
	}
    
    cal_tokhura_distence(ref_Ci,test_Ci,avg_tokhura_dist,weight,k);
  }
  if(k==3){//reading Vowel "o"
	     i= j=0;
		 while(fscanf(fp_o,"%Lf",&ch_o)==1){
		//printf("%Lf\t",ch);
		if(i<5 && j<12){
           ref_Ci[i][j++] = ch_o; 
		}
		else{
			if(++i<5){
			j=0;
			ref_Ci[i][j++] = ch_o; 
			}
			else 
				break;
		}
	}
    
    cal_tokhura_distence(ref_Ci,test_Ci,avg_tokhura_dist,weight,k);
  }
  if(k==4){//reading Vowel "u"
	     i= j=0;
		 while(fscanf(fp_u,"%Lf",&ch_u)==1){
		//printf("%Lf\t",ch);
		if(i<5 && j<12){
           ref_Ci[i][j++] = ch_u; 
		}
		else{
			if(++i<5){
			j=0;
			ref_Ci[i][j++] = ch_u; 
			}
			else 
				break;
		}
	}
    
    cal_tokhura_distence(ref_Ci,test_Ci,avg_tokhura_dist,weight,k);
  }

}
 for(int i=0;i<5;i++){
	 printf("\ntokhura distence %c to %c :%Lf",vowel[test_vowel_index],vowel[i],avg_tokhura_dist[i]);//printing Tokhura Distence between test Vowel and reference Vowels
 }
 min = 0;
 for(int i = 1;i<5;i++){//Calculating index of  Minimum Tokhura Distence
	 if(avg_tokhura_dist[i]<avg_tokhura_dist[min]){
		 min = i;
	 }
 }
switch(min){//printing predicted Vowel with respect to Minimum Tokhura Distence
 case 0:
	 printf("\npridicted vowel:a");
	 if(vowel[test_vowel_index]=='a')//incrementing correct_predicted value if the vowel is correctly Recognise
		 correct_predicted++;
	 break;
 case 1:
	 printf("\npridicted vowel:e");
	 if(vowel[test_vowel_index]=='e')
		 correct_predicted++;
	 break;
case 2:
	printf("\npridicted vowel:i");
	if(vowel[test_vowel_index]=='i')
		 correct_predicted++;
	 break;
case 3:
	printf("\npridicted vowel:o");
	if(vowel[test_vowel_index]=='o')
		 correct_predicted++;
	 break;
case 4:
	printf("\npridicted vowel:u");
	if(vowel[test_vowel_index]=='u')
		 correct_predicted++;
	 break;
 default:
	 printf("\nthis is not a vowel");
 }
 return;
}
int _tmain(int argc, _TCHAR* argv[]){
long double Ri[p+1],Ci[p+1],Ai[p+1][p+1],steady_frame[N*5];
char Test_vowel[5] = {'a' ,'e' ,'i' ,'o','u'};
char buf[200];//buffer used to store test file Address
int Num_test_file = 50;
/*----------------------- Methods to Call for Training----------------------------*/
//Dc_shift();
//normalization();
//find_steady_frame(steady_frame);
//find_Avg_Ci();

/*----------------------End of Training Method--------------------------------------*/
/*----------------------Mehods to Call For Testing----------------------------------*/
long double Tokhura_weight[12] = {1.0, 3.0, 7.0, 13.0, 19.0, 22.0, 25.0, 33.0, 42.0, 50.0, 56.0, 61.0};
for(int i=0;i<5;i++){
printf("\n**************** Test vowel: %c *****************",Test_vowel[i]);
for(int j =11;j<=20;j++){
     sprintf(buf,"D:\\Speech_processing\\recording\\vowel_recording_01\\recordings\\204101015_Ci_%c_test_+Ai_%d.txt",Test_vowel[i],j);/*this Method Concatenate String with Number(int),Example:
 "Folder_Path\\204101015_Ci_" + vowel[i]+ "_test_+Ai_" +test_file_number+".txt"*/
vowel_recognise(Tokhura_weight,buf,Test_vowel,i);//calling of Method for Vowel Recognise by passing Test file(buf),test_Vowel,current_test_vowel_index(i)
 }
}
printf("\n-----------------------------------------------------------");
printf("\nTotal Test Files :%d",Num_test_file);
printf("\nNo of Correct Predicted %d:",correct_predicted);//printing number of test file correctly indentify
printf("\nAccuracy(in percentage):%f",(float)(correct_predicted*100)/Num_test_file);//finding Accuracy
/*----------------------End of Testing Method---------------------------------*/
getch();
	return 0;
}



