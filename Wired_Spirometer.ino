
const int analogInPin = A1;
int inputVolt=0; 
int value=0;

float volt = 0;

float pressure_psi = 0;
float pressure_pa = 0;
float massFlow = 0; 
float volFlow = 0;
float volume = 0; 
float total=0;
float PEF=0;
float FVC1=0;

int i=0;
int j=0;
int val=0;
float minimum=0;
float FVC=0;
int z1=0;
float ztotal=0;
float PEFavg=0;
float FVC1total=0;
float FVC1avg=0;
float FVCtotal=0;
float FVCavg=0;

//Constants

float vs = 5 ;
float rho = 1.225; 
float area= 0.0007065 ; 
float dt = 0;
int Index =0;
int arr[100];
float PEFarr[]={0,0,0,0,0,0,0,0,0,0};
float volFlow_arr[]={0,0,0,0,0};
float PEF_arr[100];
float FVC1arr[100];
float FVCarr[100];
int starttime=0;
int currenttime=0;
int timediff=0;
int previousMillis=0;
int currentMillis=0;
int interval=0;

int disp1=0;
int disp2=0;
int disp3=0;
int temp=0;
int larr[10];
float laverage=0;
int lIndex=0;
float ltotal=0;
float overallavg=0;
int count=0;
float average=0;
float sum=0;

void setup(){
Serial.begin(9600);

//Initialization

    for(int z=0;z<100;z++){
    arr[z] = 0;
    if(z<10)
    larr[z]=0;
    PEF_arr[z]=0;
    FVCarr[z]=0;
    FVC1arr[z]=0;
    }
}

void loop() 
{
  
  if(count<4500){ 
  inputVolt = analogRead(analogInPin); 
  volt = inputVolt*(vs/1023.0);
  pressure_psi = (15/2)*(volt);
  pressure_pa = pressure_psi*6894.75729;
  
  overallavg = (overallavg*count+pressure_pa)/(count+1);
  if(average<=(overallavg+200))
  {
      total = total - arr[Index];
      arr[Index]=pressure_pa;
      total = total + arr[Index];
      Index = Index + 1;

      if (Index >= 100)
        Index = 0 ;
        
      if(count<100)
        average = total/(count+1);
      
   }
   
    pressure_pa=pressure_pa-average;
    ltotal = ltotal - larr[lIndex];
    larr[lIndex]=pressure_pa;
    ltotal = ltotal + larr[lIndex];
    lIndex = lIndex+1;
    if (lIndex >= 10)
    lIndex = 0 ;
    laverage = ltotal/10;
   
 
    if(laverage<150)
    laverage = 0; 
        
    massFlow = 1000*sqrt(((laverage)*2*rho)/((1/(pow(area,2)))));
    volFlow = massFlow/rho;
    
    int currentMillis = millis();
    dt = currentMillis - previousMillis; 
    previousMillis = currentMillis; 
    
    minimum =PEFarr[0];
    for (i=1 ;i<10 ;i++){
    if (minimum > PEFarr[i]){
        minimum=PEFarr[i];
        val=i;
       
      }
    }
    if(volFlow>PEFarr[val]){
    PEFarr[val]=volFlow;
  
      }
      sum=0;
    for(j=0 ; j<10 ; j++)
    sum=sum+PEFarr[j];
    PEF=sum*6;
                                                          //value of PEF in litre per min
    if(laverage>0)
    volume=volFlow*dt/1000 +volume;
    FVC=volume;                                               //net volume (Vital capacity in litre per min)

   if((laverage>0)&&(starttime==0))
   starttime=millis();
   if((starttime>0)&&(laverage>0)){
   currenttime=millis();
   timediff = currenttime-starttime;

   if((laverage>0)&&(timediff<1000)){
     FVC1=volFlow*dt/1000+FVC1;                                             // Expiratory volume in 1 sec in litre per min
    }
    }


FVC1total = FVC1total - FVC1arr[z1];
FVC1arr[z1]=FVC1;
FVC1total = FVC1total + FVC1arr[z1];

FVC1avg=FVC1total/100;
int fvc1=FVC1*100;
int fvc1avg=FVC1avg*100;

if((fvc1==fvc1avg)&&(disp2==0)&&(fvc1avg!=0))
{
 Serial.print("FVC1 :");
 Serial.println(FVC1*0.039000530);
 disp2=1;
}
 
ztotal = ztotal - PEF_arr[z1];
PEF_arr[z1]=PEF;
ztotal = ztotal + PEF_arr[z1];
PEFavg = ztotal/100;

int pef=PEF*10;
int pefavg=PEFavg*10;
if((pef==pefavg)&&(disp1==0)&&(pefavg!=0))
{ 
  Serial.print("PEF :");
  Serial.println(0.1814*PEF+61.045);
  //Serial.println(PEF_cal*0.5154-216);
  disp1=1;
}

FVCtotal = FVCtotal-FVCarr[z1];
FVCarr[z1]=FVC;
FVCtotal = FVCtotal+FVCarr[z1];
FVCavg=FVCtotal/100;


int fvc=FVC*100;
int fvcavg=FVCavg*100;

if((fvc==fvcavg)&&(disp3==0)&&(fvcavg!=0))
{
    Serial.print("FVC :"); 
    Serial.println(0.04050384024*FVC);
    disp3=1;
    }

  z1 = z1 + 1;
  if (z1 >= 100) {
  z1= 0 ;
}

 if((count==4499)&&((disp1==0)||(disp2==0)||(disp3==0)))
{ 
  Serial.println("Error!");
}

count++;
}
} 