#include <avr/io.h>
#include <util/delay.h>
#include "lcd_routines.h"
#include "lcd_routines.c"
#include "adc_routines.h"
#include "adc_routines.c"
#include "motor_valve_routines.c"


//Button Pins
#define STARTPin	PA2
#define STOPPin		PA3
#define RESETPin	PA4
#define ButtonPins	PINA
#define ButtonsPort PORTA
#define ButtonsDDR	DDRA

void buttons_init (void)
{
	ButtonsDDR |= _BV(STARTPin) | _BV(STOPPin) | _BV(RESETPin);
	//ButtonsPort |= _BV(STARTPin) | _BV(STOPPin) | _BV(RESETPin);
}

//States
#define MachineResetted			0
#define MachineInflating1		1
#define MachineStopped			2
#define MachineInflating2		3
#define MachineDeflating1		4
#define MachineDeflating2		5
#define RunComplete				6

//Tests
#define IsSTARTpressed			0
#define IsSTOPpressed			1
#define IsRESETpressed			2
#define IsMaxPrReached			3
#define IsAllData				4				

//Actions
#define DoNothing1 				0
#define StartInflatingAndDisp 	1
#define DeflateFastAndDisp 		2
#define ResetMachine 			3
#define KeepInflatingAndDisp 	4
#define DeflateSlowAndDisp		5
#define KeepDeflatingAndDisp	6
#define	DispDataAndDeflateFast	7


//SubStates
#define MeasurementNotStarted	0
#define SysMeasure				1
#define SysMeasure1				2
#define SysMeasure2				3
#define SysMeasure3				4
#define SysMeasure4				5
#define MeanMeasure				6
#define MeanMeasure1			7
#define MeanMeasure2			8
#define MeanMeasure3			9
#define MeanMeasure4			10
#define HRmeasure				11
#define HRmeasure1				12
#define HRmeasure2				13
#define HRmeasure3				14
#define HRmeasure4				15
#define DiasMeasure				16
#define DiasMeasure1			17
#define DiasMeasure2			18
#define DiasMeasure3			19
#define DiasMeasure4			20
#define MeasurementEnded		21


//SubTests
#define dummytest				0
#define IsACsigLTACmin			1
#define IsACsigGTACmin			2
#define IsACsigLTACmax			3
#define IsACampGTEQSysPrAmp		4
#define IsSysTrialGTEQ4			5
#define IsACampGTEQMeanPrAmp	6
#define IsMeanTrialGTEQ4		7
#define IsACsigGTbaseline1		8
#define IsACsigGTbaseline2		9
#define IsACsigLTbaseline1		10
#define IsACsigLTbaseline2		11
#define IsHRtrialGTEQ4			12
#define IsACampLTEQDiasPrAmp	13
#define	IsDiasTrialGTEQ4		14

//SubActions
#define dummyaction				0
#define DoNothing2				1
#define MakeACminEQACsig		2
#define MakeACmaxEQACsig		3
#define ResetACmin				4
#define IncSysTrial				5
#define NextSysTrial			6
#define SysStoreAndResetACmin	7
#define IncMeanTrial			8
#define NextMeanTrial			9
#define MeanStoreAndResetACmin	10
#define WaitAndIncHRtrialData	11
#define IncHRtrial				12
#define	NextHRtrial				13
#define HRStore					14
#define IncDiasTrial			15
#define NextDiasTrial			16
#define DiasStore				17

//Setpoints
#define BaseLine				128
#define MaxPr					75
#define SysPrAmpLL				32// >0.6V
#define SysPrAmpHL				51// 1.0V
#define MeanPrAmpLL				82// 1.6V
#define MeanPrAmpHL				153// 3.0V
#define DiasPrAmpLL				51 // 1.0V
#define DiasPrAmpHL				82 // 1.6V
#define HRprescalar				15

//Global varaibles
unsigned char State 			= MachineResetted;
unsigned char Test 				= IsSTARTpressed;
unsigned char Action			= DoNothing1;
unsigned char SubState 			= MeasurementNotStarted;
unsigned char SubTest			= dummytest;
unsigned char SubAction			= dummyaction;

unsigned char DCsig 			= 0;
unsigned char ACsig 			= 0;

unsigned char ACmin				= BaseLine;
unsigned char ACmax				= 0;
unsigned char SysTrial			= 0;
unsigned char SysPrData			= 0;
unsigned char MeanTrial			= 0;
unsigned char MeanPrData		= 0;
unsigned char HRtrial			= 0;
unsigned char HRtrialData[]		= {0,0,0,0};
unsigned char HRData			= 0;
unsigned char DiasTrial			= 0;
unsigned char DiasPrData		= 0;

unsigned char StartMsg [] 		= {"Press START"};
unsigned char InflatingMsg [] 	= {"Inflating "};
unsigned char DeflateSlowMsg []	= {"Deflating "};
unsigned char DeflateFastMsg []	= {"Deflating!"};
unsigned char FinalMsg []		= {"Sys/MAP/Dia/BPM"};

int main (void)
{

	//Initialize the machine
	unsigned char temp = 0;
	buttons_init();
	lcd_init();
	adc_init();
	for(unsigned char i = 0;i<sizeof(StartMsg)-1;i++) display_char(StartMsg[i]); 
	
	while(1)
	{
	//State switch case
	switch (State)
	{	
		case MachineResetted:	
			Test 	= IsSTARTpressed;
			break;

		case MachineInflating1:	
			Test 	= IsSTOPpressed;
			break;

		case MachineStopped	:
			Test 	= IsRESETpressed;
			break;
			
		case  MachineInflating2:	
			Test 	= IsMaxPrReached;
			break;
		
		case MachineDeflating1 :
			Test 	= IsSTOPpressed;
			break;
		
		case MachineDeflating2 :
			Test	= IsAllData;
			break;
		
		case RunComplete :			
			Test	= IsRESETpressed;
			break;
	}
	
	switch (Test)
	{
		case IsSTARTpressed	:
			if ((ButtonPins & _BV(STARTPin)) == 0) //true case
			{
				State	= MachineInflating1;
				Action	= StartInflatingAndDisp;
			}
			else
			{
				State	= MachineResetted;
				Action	= DoNothing1;
			}				
			break;
		
		case IsSTOPpressed:
			if ((ButtonPins & _BV(STOPPin)) == 0) //true case
			{
				State	= MachineStopped;
				Action	= DeflateFastAndDisp;
			}
			else
			{
					
				if (State == MachineInflating1) State = MachineInflating2;
				else State = MachineDeflating2;
				Action	= DoNothing1;
			}				
			break;
		
		case IsRESETpressed	:
			if ((ButtonPins & _BV(RESETPin)) == 0) //true case
			{
				State	= MachineResetted;
				Action	= ResetMachine;
			}
			else
			{
				if (State == RunComplete) State	= RunComplete;
				if (State == MachineStopped) State = MachineStopped;
				Action	= DoNothing1;
			}				
			break;
			
		case IsMaxPrReached	:
			DCsig = adc_read_dc(); 
			if (DCsig >= MaxPr)		
			{
				State	= MachineDeflating1;
				Action	= DeflateSlowAndDisp;
			}
			else
			{
				State	= MachineInflating1;
				Action	= KeepInflatingAndDisp;
			}
			break;
			
		case IsAllData :
			if (SubState == MeasurementEnded)		//Progress Case
			{
				State 	= RunComplete;
				Action 	= DispDataAndDeflateFast;
			}
			else
			{
				State 	= MachineDeflating1;
				Action 	= KeepDeflatingAndDisp;	
			}
			break;
	}
	
	switch (Action)
	{
		case DoNothing1 :
			break;
			
		case StartInflatingAndDisp :
			lcd_clear ();
			lcd_home ();
			for(unsigned char i = 0;i<sizeof(InflatingMsg)-1;i++) display_char(InflatingMsg[i]);
			inflate_cuff ();
			break;
			
		case DeflateFastAndDisp :
			lcd_clear ();
			lcd_home ();
			for(unsigned char i = 0;i<sizeof(DeflateFastMsg)-1;i++) display_char(DeflateFastMsg[i]);
			deflate_fast ();
			break;
		
		case ResetMachine :
			lcd_clear ();
			lcd_home ();
			for(unsigned char i = 0;i<sizeof(StartMsg)-1;i++) display_char(StartMsg[i]); 
			State 			= MachineResetted;
			Test 			= IsSTARTpressed;
			Action			= DoNothing1;
			SubState 		= MeasurementNotStarted;
			SubTest			= dummytest;
			SubAction		= dummyaction;

			DCsig 			= 0;
			ACsig 			= 0;

			ACmin			= BaseLine;
			ACmax			= 0;
			SysTrial		= 0;
			SysPrData		= 0;
			MeanTrial		= 0;
			MeanPrData		= 0;
			HRtrial			= 0;
			for(unsigned char i = 0;i<4;i++) HRtrialData[i]	= 0;
			HRData			= 0;
			DiasTrial		= 0;
			DiasPrData		= 0;
			break;
		
		case KeepInflatingAndDisp :
			lcd_home ();
			for(unsigned char i = 0;i<sizeof(InflatingMsg)-1;i++) display_char(InflatingMsg[i]);
			display_byte ((DCsig*2 + DCsig/10));
			break;
		
		case DeflateSlowAndDisp :
			lcd_clear ();
			lcd_home ();
			for(unsigned char i = 0;i<sizeof(DeflateSlowMsg)-1;i++) display_char(DeflateSlowMsg[i]);
			deflate_slow ();
			SubState = SysMeasure;
			break;
			
		case KeepDeflatingAndDisp :
			lcd_home ();
			for (unsigned char i = 0;i<sizeof(DeflateSlowMsg)-1;i++) display_char(DeflateSlowMsg[i]);
			DCsig = adc_read_dc ();
			display_byte ((DCsig*2 + DCsig/10));
			break;
			
		case DispDataAndDeflateFast :
			deflate_fast ();
			lcd_clear ();
			lcd_home ();
			for (unsigned char i = 0;i<sizeof(FinalMsg)-1;i++) display_char(FinalMsg[i]);
			move_to (0,1);
			display_byte ((SysPrData*2 + SysPrData/10));
			display_char ('/');
			display_byte ((MeanPrData*2 + MeanPrData/10));
			display_char ('/');
			display_byte ((DiasPrData*2 + DiasPrData/10));
			display_char ('/');
			for (unsigned char i = 0;i<4;i++) HRData += (HRtrialData [i])/4;
			display_byte (((60/HRprescalar)*(100/HRData)*10));
		
			break;
	}
	
	//if (SubState == MeasurementNotStarted || SubState == MeasurementEnded) continue;
	
	switch (SubState)
	{
		case MeasurementNotStarted : 
			break;
		
		case SysMeasure	:
			SubTest	= IsACsigLTACmin;
			break;
		
		case SysMeasure1 :
			SubTest	= IsACsigGTACmin;
			break;
		
		case SysMeasure2 :
			SubTest	= IsACsigLTACmax;
			break;
		
		case SysMeasure3 :
			SubTest	= IsACampGTEQSysPrAmp;
			break;
		
		case SysMeasure4 :
			SubTest	= IsSysTrialGTEQ4;
			break;
		
		case MeanMeasure	:
			SubTest	= IsACsigLTACmin;
			break;
		
		case MeanMeasure1 :
			SubTest	= IsACsigGTACmin;
			break;
		
		case MeanMeasure2 :
			SubTest	= IsACsigLTACmax;
			break;
		
		case MeanMeasure3 :
			SubTest	= IsACampGTEQMeanPrAmp;
			break;
		
		case MeanMeasure4 :
			SubTest	= IsMeanTrialGTEQ4;
			break;
		
		case HRmeasure : 
			SubTest	= IsACsigGTbaseline1;
			break;
		
		case HRmeasure1 : 
			SubTest	= IsACsigLTbaseline1;
			break;
		
		case HRmeasure2 : 
			SubTest	= IsACsigGTbaseline2;
			break; 
		
		case HRmeasure3 : 
			SubTest	= IsACsigLTbaseline2;
			break; 	
		
		case HRmeasure4 : 
			SubTest	= IsHRtrialGTEQ4;
			break;	
		
		case DiasMeasure :
			SubTest	= IsACsigLTACmin;
			break;
		
		case DiasMeasure1 :
			SubTest	= IsACsigGTACmin;
			break;
		
		case DiasMeasure2 :
			SubTest	= IsACsigLTACmax;
			break;
		
		case DiasMeasure3 :
			SubTest	= IsACampLTEQDiasPrAmp;
			break;
		
		case DiasMeasure4 :
			SubTest	= IsDiasTrialGTEQ4;
			break;		
		
		case MeasurementEnded :
			break;
	}
	
	switch (SubTest)
	{
		case dummytest : 
			break;
			
		case IsACsigLTACmin:
			ACsig = adc_read_ac ();
			if (ACsig < ACmin)
			{
				if (SubState == SysMeasure) SubState	= SysMeasure1;
				if (SubState == DiasMeasure) SubState	= DiasMeasure1;
				if (SubState == MeanMeasure) SubState	= MeanMeasure1;
				SubAction	= MakeACminEQACsig;
			}
			else
			{
				if (SubState == SysMeasure) SubState	= SysMeasure;
				if (SubState == DiasMeasure) SubState	= DiasMeasure;
				if (SubState == MeanMeasure) SubState	= MeanMeasure;
				SubAction	= DoNothing2;
			}
			break;
			
		case IsACsigGTACmin:
			ACsig = adc_read_ac ();
			if (ACsig > ACmin)
			{
				if (SubState == SysMeasure1) SubState	= SysMeasure2;
				if (SubState == DiasMeasure1) SubState	= DiasMeasure2;
				if (SubState == MeanMeasure1) SubState	= MeanMeasure2;
				SubAction	= MakeACmaxEQACsig;
			}
			else
			{
				if (SubState == SysMeasure1) SubState	= SysMeasure1;
				if (SubState == DiasMeasure1) SubState	= DiasMeasure1;
				if (SubState == MeanMeasure1) SubState	= MeanMeasure1;
				SubAction	= MakeACminEQACsig;
			}
			break;
			
		case IsACsigLTACmax:
			ACsig = adc_read_ac ();
			if (ACsig < ACmax)
			{
				if (SubState == SysMeasure2) SubState	= SysMeasure3;
				if (SubState == DiasMeasure2) SubState	= DiasMeasure3;
				if (SubState == MeanMeasure2) SubState	= MeanMeasure3;
				SubAction	= DoNothing2;
			}
			else
			{
				if (SubState == SysMeasure2) SubState	= SysMeasure2;
				if (SubState == DiasMeasure2) SubState	= DiasMeasure2;
				if (SubState == MeanMeasure2) SubState	= MeanMeasure2;
				SubAction	= MakeACmaxEQACsig;
			}
			break;
			
		case IsACampGTEQSysPrAmp:
			if (((ACmax - ACmin) >= SysPrAmpLL) && ((ACmax - ACmin) <= SysPrAmpHL))
			{
				SubState	= SysMeasure4;
				SubAction	= IncSysTrial;
			}
			else
			{
				SubState	= SysMeasure;
				SubAction	= ResetACmin;
			}
			break;
			
		case IsSysTrialGTEQ4 :
			if (SysTrial >= 4) 
			{
				SubState	= MeanMeasure;
				SubAction	= SysStoreAndResetACmin;
			}
			else
			{
				SubState	= SysMeasure;
				SubAction	= NextSysTrial;
			}
			break;	
		
		case IsACampGTEQMeanPrAmp:
			if (((ACmax - ACmin) >= MeanPrAmpLL) && ((ACmax - ACmin) <= MeanPrAmpHL))
			{
				SubState	= MeanMeasure4;
				SubAction	= IncMeanTrial;
			}
			else
			{
				SubState	= MeanMeasure;
				SubAction	= ResetACmin;
			}
			break;
			
		case IsMeanTrialGTEQ4 :
			if (MeanTrial >= 4) 
			{
				SubState	= HRmeasure;
				SubAction	= MeanStoreAndResetACmin;
			}
			else
			{
				SubState	= MeanMeasure;
				SubAction	= NextMeanTrial;
			}
			break;	
		
		case IsACsigGTbaseline1 :
			ACsig = adc_read_ac(); 
			if (ACsig > BaseLine)
			{
				SubState	= HRmeasure1;
				SubAction	= DoNothing2;
			}
			else
			{
				SubState	= HRmeasure;
				SubAction	= DoNothing2;
			}
			break;
		
		case IsACsigLTbaseline1 : 
			ACsig = adc_read_ac(); 
			if (ACsig < BaseLine)
			{
				SubState	= HRmeasure2;
				SubAction	= DoNothing2;
			}
			else
			{
				SubState	= HRmeasure1;
				SubAction	= DoNothing2;
			}
			break;
		
		case IsACsigGTbaseline2 : 
			ACsig = adc_read_ac(); 
			if (ACsig > BaseLine)
			{
				SubState	= HRmeasure3;
				SubAction	= DoNothing2;
			}
			else
			{
				SubState	= HRmeasure2;
				SubAction	= WaitAndIncHRtrialData;
			}
			break;

		case IsACsigLTbaseline2 : 
			ACsig = adc_read_ac(); 
			if (ACsig < BaseLine)
			{
				SubState	= HRmeasure4;
				SubAction	= IncHRtrial;
			}
			else
			{
				SubState	= HRmeasure3;
				SubAction	= WaitAndIncHRtrialData;
			}
			break;

		case IsHRtrialGTEQ4 : 
			if (HRtrial >= 4)	//will hopefully never go above 4
			{
				SubState	= DiasMeasure;
				SubAction	= HRStore;
			}
			else
			{
				SubState	= HRmeasure2;	//should this be HRmeasure?
				SubAction	= NextHRtrial;
			}
			break;
			
		case IsACampLTEQDiasPrAmp:
			if (((ACmax - ACmin) >= DiasPrAmpLL) && ((ACmax - ACmin) <= DiasPrAmpHL))
			{
				SubState	= DiasMeasure4;
				SubAction	= IncDiasTrial;
			}
			else
			{
				SubState	= DiasMeasure;
				SubAction	= ResetACmin;
			}
			break;		
		
		case IsDiasTrialGTEQ4 : 
			if (DiasTrial >= 4)
			{
				SubState	= MeasurementEnded;
				SubAction	= DiasStore;
			}
			else
			{
				SubState	= DiasMeasure;
				SubAction	= NextDiasTrial;
			}
			break;
	}
	
	switch (SubAction)
	{
		case dummyaction :
			break;
				
		case DoNothing2 :
			break;
		
		case MakeACminEQACsig:
			ACmin	= ACsig;
			break;

		case MakeACmaxEQACsig:
			ACmax	= ACsig;
			break;
			
		case ResetACmin :		
			ACmin	= BaseLine;
			break;
			
		case IncSysTrial :
			move_to (2,1);
			display_char('S');
			SysTrial ++;
			break;

		case NextSysTrial :
			ACmin	= BaseLine;
			break;

		case SysStoreAndResetACmin :	
			SysPrData	= adc_read_dc ();
			ACmin		= BaseLine;
			break;
				
		case IncMeanTrial :
			move_to (2,1);
			display_char('M');
			MeanTrial ++;
			break;

		case NextMeanTrial :
			ACmin	= BaseLine;
			break;

		case MeanStoreAndResetACmin :	
			MeanPrData	= adc_read_dc ();
			ACmin		= BaseLine;
			break;

		case WaitAndIncHRtrialData :
			move_to (2,1);
			display_char('H');
			display_byte (HRtrial);
			_delay_ms (10);
			HRtrialData [HRtrial]++;
			break;
		
		case IncHRtrial :
			HRtrial ++;
			break;
		
		case NextHRtrial :
			break;
		
		case HRStore :
			break;
		
		case IncDiasTrial :
			move_to (2,1);
			display_char('D');
			DiasTrial ++;
			break;

		case NextDiasTrial :
			ACmin	= BaseLine;
			break;
		
		case DiasStore :
			DiasPrData	= adc_read_dc ();
			break;
		
	}	
	};
	return (0);
}