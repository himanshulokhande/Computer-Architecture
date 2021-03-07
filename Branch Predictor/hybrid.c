#include <stdio.h>
#include <math.h>
#include <string.h>

//function declarations
int argument_to_int(char *arg[]){
	int result= atoi(*arg);
	return result;
}

int main(int argc, char *argv[])
{
	FILE *pfin;
	char *mode = "r";
	long int i = 0; //counter to know the number of operations
	
	int phit = 0;
	int pmiss = 0;
	int noprediction=0;
	int upperThreshold,lowerThreshold=0;
	int prediction_l,prediction_g,prediction=2;		 //1=to be taken, 0=wont be taken, 2=no prediction
	int pc,pc1;
	int index1,index2;
	int gsharecount,localcount;
	int pht1[1024],pht2[1024]; //pattern history table
	int lhr[1024]; //local history table
	int ghr= 0x0;
	
	char address[12]; //from file
	int op;
	
	int bits= argument_to_int(&argv[1]);
	//char type = *argv[1]; //1= one level branch, 2= 2 level global, 3= 2 level Gshare, 4= 2 level local
	int temp=bits;
	int max_confidence=1;
	while(bits !=0){
		max_confidence *= 2;
		--bits;
	};
	max_confidence--;
	lowerThreshold=max_confidence/2;
	upperThreshold=lowerThreshold+1;

	//setting pht to 0
	for(int i=0; i<1024;i++){
		pht1[i]=0;
		pht2[i]=0;
		lhr[i]=0;
	}
	//setting lhr to 0
	//for(int i=0; i<128;i++){
	//	lhr[i]=0;
	//}
	
	//opening file
	pfin = fopen("trace.din",mode);
	if (pfin == NULL) {
		printf("Can't open input file\n");
		return(0);
	}
	while (fscanf(pfin, "%s %s",address,&op) != EOF){
		//printf("%s %s\n",address,&op); 
		sscanf(address,"%X",&pc);
		pc1 = pc & 0xFFC; 
		pc1 = pc1 >> 2;
		index1=pc1 ^ ghr;
		index2=lhr[pc1];
		
		//checking count and predicting
		if(pht1[index1]<=max_confidence && pht1[index1]>upperThreshold){
			prediction_g=1;
		}
		if(pht1[index1]>=0 && pht1[index1]<lowerThreshold){
			prediction_g=0;
		}

		if(pht2[index2]<=max_confidence && pht2[index2]>upperThreshold){
			prediction_l=1;
		}
		if(pht2[index2]>=0 && pht2[index2]<lowerThreshold){
			prediction_l=0;
		}

		//selector
		if(prediction_g==prediction_l){
			prediction=prediction_g;
		}
		if(prediction_g == 2 && prediction_l != 2){
			prediction=prediction_l;
			
		}
		if(prediction_l == 2 && prediction_g != 2){
			prediction=prediction_g;
			
		}
		else{
			if(localcount < gsharecount)
			{ 
				prediction = prediction_g; 
			}
			else{ 
				prediction = prediction_l; 
			}
		}
		
		//updating counters for correctness
		if((prediction_l==1 && op=='T') || (prediction_l==0 && op=='N')){
			localcount++;
		}

		if((prediction_g==1 && op=='T') || (prediction_g==0 && op=='N')){
			gsharecount++;
		}

		//prediction
		if((prediction==1 && op=='T') || (prediction==0 && op=='N')){
			phit++;
		}
		if((prediction==0 && op=='T') || (prediction==1 && op=='N')){
			pmiss++;
		}
		if(prediction==2){
			noprediction++;
		}
		//update counter
		if(op=='T'){
			if(pht1[index1]!=max_confidence){
					pht1[index1]++;			
				}
			if(pht2[index2]!=max_confidence){
					pht2[index2]++;			
				}
		}else{
			if(pht1[index1]!=0){
				pht1[index1]--;
			}
			if(pht2[index2]!=0){
				pht2[index2]--;
			}
		}
		//update ghr
		if(op=='T'){
			ghr=ghr<<1;
			ghr=ghr|0x001;
			ghr=ghr&0x3FF;
		}else{
			ghr=ghr<<1;
			ghr=ghr&0x3FF;
		}
		
		//update lhr
		if(op=='T'){
			lhr[pc1]=lhr[pc1]<<1;
			lhr[pc1]=lhr[pc1]|0x001;
			lhr[pc1]=lhr[pc1]&0x3FF;
		}
		if(op=='N'){
			lhr[pc1]=lhr[pc1]<<1;
			lhr[pc1]=lhr[pc1] & 0x3FF;
		}

		prediction=2;
		i++;
	}
	printf("hits:%d miss:%d \n",phit,pmiss);
	int total_predictions=phit+pmiss;
	printf("total predictions: %d \n",total_predictions);
	printf("no predictions: %d \n",noprediction);
	printf("total instructions: %ld \n",i);
	fclose(pfin);
	return 0;
}

