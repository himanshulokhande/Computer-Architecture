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
	int prediction=2;		 //1=to be taken, 0=wont be taken, 2=no prediction
	int pc,pc1,pc2;
	int index;
	int pht[1024]; //pattern history table
	int lhr[128]; //local history table
	int ghr= 0x0;
	
	char address[12]; //from file
	int op;
	
	int bits= argument_to_int(&argv[2]);
	char type = *argv[1]; //1= one level branch, 2= 2 level global, 3= 2 level Gshare, 4= 2 level local
	int temp=bits;
	int max_confidence=1;
	while(bits !=0){
		max_confidence *= 2;
		--bits;
	}
	max_confidence--;
	lowerThreshold=max_confidence/2;
	upperThreshold=lowerThreshold+1;

	//setting pht to 0
	for(int i=0; i<1024;i++){
		pht[i]=0;
	}
	//setting lhr to 0
	for(int i=0; i<128;i++){
		lhr[i]=0;
	}
	
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
		if(type=='1'){
			index=pc1;
		}if(type=='2'){
			index=ghr;
		}if(type=='3'){
			index=pc1 ^ ghr;
		}if(type=='4'){
			pc2 = pc & 0x1FC; 
			pc2= pc2 >> 2;
			index=lhr[pc2];
		}

		//checking count and predicting
		if(pht[index]<=max_confidence && pht[index]>upperThreshold){
			prediction=1;
		}
		if(pht[index]>=0 && pht[index]<lowerThreshold){
			prediction=0;
		}
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
			if(pht[index]!=max_confidence){
					pht[index]++;			
				}
		}else{
			if(pht[index]!=0){
				pht[index]--;
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
		if(op=='T' && type=='4'){
			lhr[pc2]=lhr[pc2]<<1;
			lhr[pc2]=lhr[pc2]|0x001;
			lhr[pc2]=lhr[pc2]&0x3FF;
		}
		if(op=='N' && type=='4'){
			lhr[pc2]=lhr[pc2]<<1;
			lhr[pc2]=lhr[pc2] & 0x3FF;
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

