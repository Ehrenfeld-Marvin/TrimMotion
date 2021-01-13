/*Foam::scalar*/ void Foam::solidBodyMotionFunctions::TrimMotion::Amplitude(scalar* Amplitude_Value, scalar* phi_Value) const
{
/*++++++++++++++++++++++++++++	VARIABLEN ZURÜCKSETZEN		+++++++++++++++++++++++++++*/

	scalar time=time_.value();
	scalar delta_t=time_.deltaT().value();
	
	Time_Intervall_Value = (NuOfOsc*2*3.141592653)/omega_PID;
	Ramp_Time = Time_Intervall_Value/4;

	float Force_X_Value=0;
	float Force_Y_Value=0;
	float Force_Z_Value=0;
	
	int Average_Divider=0;
	
    	Force_Average_X = 0;							
	Force_Average_Z = 0;
	Force_Average_Y = 0;
	
	
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	

	if(time > Read_Time_Intervall)
	{

		#include "ReadFile.H"					//TRIM_FORCES LESEN

      		if(Average_Divider==0) Average_Divider=1;		//If Bedingung wird bei t=0 trotzdem aufgerugen, Divider wäre hier =0

      		Force_Average_X /= (Average_Divider);			//Mittelwerte von n-Cycles bestimmen
		Force_Average_Y /= (Average_Divider);
     		Force_Average_Z /= (Average_Divider);
     		
     		PatchName.find("1");
     		
/*#################	AMPLITUDE - PID	##############################*/     		

		amplitude_average = sqrt(sqr(Force_Average_X)+sqr(Force_Average_Y));
		amplitude_target = Target_Force;
    		amplitude_y = PID_REGLER
     		(
     			amplitude_target, 
     			amplitude_average, 
     			Amplitude_K_P, 
     			Amplitude_K_I, 
     			Amplitude_K_D,
			&e_t_SUMME_AMP,
     			&IST_alt_AMP,
     			&e_t_alt_AMP,
     			&P_AMP,
			&I_AMP,
			&D_AMP,
			&IST_AMP,
			&e_t_AMP
     		);							//PID-REGLER
     		if(amplitude_y<=-50) amplitude_y = -50;		//MAXIMALER ANSTELLWINKEL
     		
     		if(PatchName=="_Blade1")
     		{
     			writePIDdata
			(
				"/Amplitude-PID-Data",
				amplitude_target,			
     				Amplitude_K_P, 
     				Amplitude_K_I, 
     				Amplitude_K_D,
				IST_AMP,
				e_t_SUMME_AMP,
				e_t_AMP,
				amplitude_y,
				P_AMP,
				I_AMP,
				D_AMP,
				Force_Average_X,
				Force_Average_Y,
				Force_Average_Z,
				time
			);
			
			Back_up_Data_PID
			(
				"/Amplitude-BackUp-Data",
				time,
				IST_alt_AMP,
				e_t_SUMME_AMP,
				amplitude_y
			);
		}
		
/*#############################################################################*/    

/*++++++++++++++++++++++++++	PHASE - PID	+++++++++++++++++++++++++++++++++*/  	
		deg_to_rad=(angle_F_X*2*pi)/360;
     		phi_target = Target_Force*sin(deg_to_rad)*(-1);
		phi_y = PID_REGLER
		(
			phi_target, 
			Force_Average_X, 
			Phi_K_P, 
			Phi_K_I, 
			Phi_K_D,
			&e_t_SUMME_PHI,
     			&IST_alt_PHI,
     			&e_t_alt_PHI,
     			&P_PHI,
			&I_PHI,
			&D_PHI,
			&IST_PHI,
			&e_t_PHI
		);
		
		if(PatchName=="_Blade1")
		{
		 
			writePIDdata
			(
				"/Phi-PID-Data",
				phi_target,			
     				Phi_K_P, 
     				Phi_K_I, 
     				Phi_K_D,
				IST_PHI,
				e_t_SUMME_PHI,
				e_t_PHI,
				phi_y,
				P_PHI,
				I_PHI,
				D_PHI,
				Force_Average_X,
				Force_Average_Y,
				Force_Average_Z,
				time
			);
			
			Back_up_Data_PID
			(
				"/Phi-BackUp-Data",
				time,
				IST_alt_PHI,
				e_t_SUMME_PHI,
				phi_y
			);
		}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/		

		Ramp_change=true;					//Rampe aktivieren
		Ramp_Time_Counter=Ramp_Time;				//Zeit für Rampe zurücksetzen
		
		Read_Time_Intervall += Time_Intervall_Value; 		//Intervall auf nächsten Cycle erhöhen
		

//		return amplitude_;
	}
	

	
	if(Ramp_change && Ramp_Time_Counter>0)			//Amplitude wird linear geändert (Rampenfunktion)
	{
			
	
		delta_amplitude 	= amplitude_y 	- amplitude_ramp;
		delta_phi 		= phi_y 	- phi_ramp;
		
		amplitude_ramp 	= amplitude_0 	+ (delta_t * delta_amplitude) 	/ Ramp_Time_Counter;
		phi_ramp 		= phi_0 	+ (delta_t * delta_phi) 		/ Ramp_Time_Counter;
		
		Ramp_Time_Counter -= delta_t;
		
		amplitude_0 	= amplitude_ramp;
		phi_0 		= phi_ramp;


		if(Ramp_Time_Counter<0) 
		{
			Ramp_change = false;
			amplitude_ramp 	= amplitude_y;
			phi_ramp 		= phi_y;
		}
		

			
	}
	
	*Amplitude_Value 	= amplitude_ramp;
	*phi_Value 		= phi_ramp;



		
/*	
	cout 	<<"\n\n\nAMPLITUDE:\n\n"
		<<"amplitude_target: " << amplitude_target << "\n"
		<<"Force_Average_Y: " << Force_Average_Y << "\n"  
		<<"Amplitude_K_P: " << Amplitude_K_P << "\n"  
		<<"Amplitude_K_I: " << Amplitude_K_I << "\n"  
		<<"Amplitude_K_D: " << Amplitude_K_D << "\n" 
		<<"P_AMP: " << P_AMP << "\n" 
		<<"I_AMP: " << I_AMP << "\n" 
		<<"D_AMP: " << D_AMP << "\n" 
		<<"IST_AMP: " << IST_AMP << "\n"
		<<"e_t_AMP: " << e_t_AMP << "\n"
		<<"e_t_SUMME_AMP: " << e_t_SUMME_AMP << "\n"
		<<"delta T: " << delta_t << "\n"
		<<"amplitude_ramp: " << amplitude_ramp << "\n"
		<<"*Amplitude_Value: " << *Amplitude_Value << "\n"
		<<"amplitude_y: " << amplitude_y << "\n"
		<<"\n\n\nPHASE:\n\n"
		<<"phi_target: " << phi_target << "\n"
		<<"Force_Average_X: " << Force_Average_X << "\n"  
		<<"Phi_K_P: " << Phi_K_P << "\n"  
		<<"Phi_K_I: " << Phi_K_I << "\n"  
		<<"Phi_K_D: " << Phi_K_D << "\n" 
		<<"P_PHI: " << P_PHI << "\n" 
		<<"I_PHI: " << I_PHI << "\n" 
		<<"D_PHI: " << D_PHI << "\n" 
		<<"IST_PHI: " << IST_PHI << "\n"
		<<"e_t_SUMME_PHI: " << e_t_SUMME_PHI << "\n"
		<<"e_t_PHI: " << e_t_PHI << "\n"
		<<"phi_ramp: " << phi_ramp << "\n"
		<<"*phi_Value: " << *phi_Value << "\n"
		<<"phi_y: " << phi_y << "\n\n\n";
*/		

	

		
//	return PID_Values;
}

